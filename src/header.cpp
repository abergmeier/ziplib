/*
 * header.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: andreas
 */

#include <array>
#include <algorithm>
#include "header.hpp"

using namespace ziplib;

namespace {
	struct header_util {
		header_util( const zip_local_file_header& header );
		struct {
			char* data;
			std::uint32_t length;
		} filename;

		struct {
			std::uint32_t length;
		} extra;

		compression_method compression;
		decltype(header().flags()) flags;
		std::uint32_t compressed_size;
	private:
		const header::file_header_with_filename& _internal;
	};

	constexpr std::uint32_t _get_size( unsigned int element_count, const char* field ) {
		return 0; //TODO: IMPLEMENT!
	}

	bool isFilenameValid( zip_local_file_header& header ) {
		header_util biggerHeader( header );

		if( biggerHeader.filename.length == 0 )
			return true;

		const auto end = biggerHeader.filename.data + biggerHeader.filename.length;
		auto termIt = std::find( biggerHeader.filename.data, end, '\0' );

		if( termIt == end ) {
			termIt = end - 1;
			// Enforce proper termination
			*termIt = '\0';
			std::clog << "Filename was not terminated properly: " << biggerHeader.filename.data;
		}

		if( biggerHeader.filename.data[0] == '/' ) {
			std::clog << "Ignored filename starting with '/': " << biggerHeader.filename.data;
			return false;
		}

		decltype(biggerHeader.filename.data) it;
		do {
			// Find disallowed
			it = std::find( biggerHeader.filename.data, termIt, '\\' );
			*it = '/';
		} while( it != termIt );

		return true;
	}

	template <typename It>
	bool isHeaderFitting( It begin, It end, zip_local_file_header& header ) {
		header_util head( header );
		return std::distance(begin, end) >= ZIP_LOCAL_FILE_HEADER_BYTES + head.filename.length + header.partial.size_file_extra;
	}

	template <typename It>
	bool isHeaderFitting( It begin, It end ) {
		return std::distance(begin, end) >= ZIP_LOCAL_FILE_HEADER_BYTES;
	}
}

#define NELEMENTS(x) ( sizeof(x) / sizeof(x[0]) )
#define get_size(x) _get_size(NELEMENTS(x), x)

header_util::header_util( const zip_local_file_header& header ) :
	filename(), extra(), compression(), flags(), _internal( *reinterpret_cast<const header::file_header_with_filename*>( &header ) )
{
	filename.data   = _internal.filename;
	filename.length = get_size( _internal.header.partial.size_filename );

	extra.length = get_size( _internal.header.partial.size_file_extra );

	compression = static_cast<compression_method>( _internal.header.partial.compression_method );

	flags = *reinterpret_cast<const decltype(flags)*>( _internal.header.partial.flag );
	compressed_size = *reinterpret_cast<const decltype(compressed_size)*>( _internal.header.partial.compressed_size );
}

header::header() :
	_stage( stage::SEEKING ), _signatureCharactersMatch( 0 ), _buffer() {
}


void
header::reset() {
	_buffer.clear();
}

std::uint64_t
header::variable_size() {
	assert( _buffer.size() >= ZIP_LOCAL_FILE_HEADER_BYTES );
	header_util header( data()->header );
	return header.filename.length
	     + header.extra.length;
}

header::file_header_with_filename*
header::data() {
	return reinterpret_cast<file_header_with_filename*>( _buffer.data() );
}

const header::file_header_with_filename*
header::data() const {
	return reinterpret_cast<const file_header_with_filename*>( _buffer.data() );
}

compression_method
header::compression_method() const {
	header_util header( data()->header );
	return header.compression;
}

std::uint16_t
header::flags() const {
	header_util header( data()->header );
	return header.flags;
}

std::uint32_t
header::compressed_size() const {
	header_util header( data()->header );
	return header.compressed_size;
}

bool
header::read_dynamic( zip_stream& stream ) {
	const auto needs = variable_size() - _buffer.size();
	read_partial( stream, stream.next_in, stream.next_in + needs );

	return _buffer.size() == variable_size();
}

bool
header::read_fixed( zip_stream& stream ) {
	// Copy fixed part of header
	const auto needs = ZIP_LOCAL_FILE_HEADER_BYTES - _buffer.size();
	read_partial( stream, stream.next_in, stream.next_in + needs );

	return _buffer.size() == ZIP_LOCAL_FILE_HEADER_BYTES;
}

bool
header::find_header( zip_stream& stream ) {
	// Seeking for header (signature)

	typedef std::array<char, 4> signature_array;
	static signature_array LOCAL_FILE_HEADER_SIGNATURE{{ 0x50, 0x4B, 0x03, 0x04 }};

	const auto foundSignature = [&]() {
		return _signatureCharactersMatch == LOCAL_FILE_HEADER_SIGNATURE.size();
	};

	if( stream.avail_in > 0 )
		return foundSignature();

	const auto end = stream.next_in + stream.avail_in;

	do {
		const auto it = [&]() -> signature_array::const_iterator {
			if( _signatureCharactersMatch == 0 ) {
				// No match found yet
				const auto it = std::find( stream.next_in, end,
				                           LOCAL_FILE_HEADER_SIGNATURE[_signatureCharactersMatch] );

				if( it != end )
					++_signatureCharactersMatch;

				return it;
			} else {
				// Already matched some characters, so next have to follow
				const auto maxScanWidth = std::min( static_cast<signature_array::size_type>(stream.avail_in),
				                                    LOCAL_FILE_HEADER_SIGNATURE.size() );
				const auto scanEnd = stream.next_in + maxScanWidth;
				const auto scanSignatureBegin = LOCAL_FILE_HEADER_SIGNATURE.begin() +_signatureCharactersMatch;
				const auto scanSignatureEnd   = LOCAL_FILE_HEADER_SIGNATURE.begin() +_signatureCharactersMatch + maxScanWidth;
				const auto it = std::search( stream.next_in, scanEnd,
				                             scanSignatureBegin,
				                             scanSignatureEnd    );

				if( it == scanEnd )
					// Could not match any further characters
					_signatureCharactersMatch = 0;
				else
					_signatureCharactersMatch += maxScanWidth;

				return it;
			}
		}();

		{ // Save all bytes not used
			_unused_data.insert( stream.total_in, stream.next_in, it );
			const auto distance = std::distance( stream.next_in, it );
			stream.next_in  += distance;
			stream.avail_in -= distance;
			stream.total_in += distance;
		}
		// Continue, till we found a signature or cannot find anymore
	} while( !foundSignature()
	      && stream.next_in != end );

	return foundSignature();
}

bool
header::read_full( zip_stream& stream ) {
	// Try to read header fully

	if( _stage == stage::SEEKING && find_header(stream) )
		_stage = stage::FIXED_HEADER;

	if( _stage == stage::FIXED_HEADER && read_fixed(stream) )
		_stage = stage::DYNAMIC_HEADER;

	if( _stage == stage::DYNAMIC_HEADER && read_dynamic(stream) ) {
		_stage = stage::SEEKING;
		return true;
	}

	return false;
}

void
header::read_partial( zip_stream& stream, const char* begin, const char* end ) {
	assert( std::distance(begin, end) >= 0 );
	std::size_t distance = std::distance( begin, end );

	distance = std::min( distance , static_cast<std::size_t>(stream.avail_in) );
	_buffer.insert( _buffer.end(), begin, begin + distance );

	stream.next_in  += distance;
	stream.avail_in -= distance;
	stream.total_in += distance;
}
