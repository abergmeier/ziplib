
#include "internal.hpp"

using namespace ziplib;
using namespace ziplib::inflate;

namespace {
	// unz_file_info_interntal contain internal info about a file in zipfile
	typedef struct unz_file_info64_internal_s
	{
		zip_file_info file_info;
		char offset_curfile[8]; // relative offset of local header 8 bytes
	} unz_file_info64_internal;

	internal& intern( zip_stream& strm ) {
		return *static_cast<internal*>( strm.state );
	}

	constexpr typename internal::left_type UNKNOWN_SIZE = std::numeric_limits<typename internal::left_type>::max();
}

void
internal::reset() {
	_header.reset();
	_content.reset();
	_stage = stage::HEADER;

	if( _inflater ) {
		// Put inflater back
		auto index = compression_method_index( _header.compression_method() );
		std::swap( _inflater, _inflaters[index] );
	}

	_left = 0;
}

#define MAKE_INFLATER(x) ziplib::inflate::create_inflater(compression_tag::x)
#define HANDLE_CASE(x) case compression_method::x:\
	return MAKE_INFLATER(x);

std::unique_ptr<inflate::base_inflater>
internal::create_inflater( const compression_method method ) {
	switch( method ) {
	HANDLE_CASE(STORED)
#if 0
	case SHRUNK:
	case REDUCED_1:
	case REDUCED_2:
	case REDUCED_3:
	case REDUCED_4:
	case IMPLODED:
	case TOKENIZING:
#endif
	HANDLE_CASE(DEFLATE)
	HANDLE_CASE(DEFLATE64)
#if 0
	case OLD_TERSE:
	case BZIP2:
	case LZMA:
	case NEW_TERSE:
	case PFS:
	case WAVPACK:
	case PPMD:
#endif
	default:
		std::cerr << "Compression method '" << static_cast<std::uint16_t>(method) << "' not supported." << std::endl;
		throw std::runtime_error("BLAH");
		break;
	}
}

bool
internal::put( zip_stream& stream ) {

	if( !stream.avail_in )
		return true;

	if( _stage == stage::HEADER && _header.read_full( stream ) ) {
		const auto method = _header.compression_method();
		const auto method_index = compression_method_index( method );
		std::swap( _inflater, _inflaters[method_index] );
		if( _inflater == nullptr ) {
			// inflater not instantiated yet
			_inflater = create_inflater( method );
		}

		if( _header.flags() & ZIP_FILE_SIZE_UNKNOWN ) {
			_left = UNKNOWN_SIZE;
			std::clog << "File entry does not contain a size. Trying to process." << std::endl;
		} else
			_left = _header.compressed_size();

		_stage = stage::CONTENT;
	}

	if( _stage == stage::CONTENT && _content.read_full( stream )) {
		reset();
	}
}

void
internal::inflate( const char* begin, const char* end ) {
}

#if 0
// Returns possible new location to search for header
template <typename It>
std::tuple<It, zip_local_file_header*>
internal::find_header<It>( It begin, It end ) {

	auto distance = std::distance(begin, end);

	if( distance < 1 )
		return begin;

	--distance;
	// Set to last element
	auto last_match = begin + distance;

	std::uint8_t failure_percent;
	zip_local_file_header* header = nullptr;

	do {



		failure_percent = 0;
		last_match = begin;

		if( !isHeaderFitting( begin, end ) )
			break;

		header = reinterpret_cast<zip_local_file_header*>( begin );

		if( !isHeaderFitting(begin, end, *header) )
			break;

		if( !isFilenameValid( *header ) ) {
			failure_percent += 20;
		}

		begin += std::min( SIZE_N(header.signature), std::distance(begin, end) );
	} while( begin != end && !header );

	if( failure_percent >= 100 )
		last_match += std::min( SIZE_N(header.signature), std::distance(last_match, end) );

	return std::make_tuple( last_match, header );
}


/*
  Set the current file of the zipfile to the first file.
  return UNZ_OK if there is no problem
*/
extern int ZEXPORT unzGoToFirstFile (unzFile file)
{
    int err=UNZ_OK;
    unz64_s* s;
    if (file==NULL)
        return UNZ_PARAMERROR;
    s=(unz64_s*)file;
    s->pos_in_central_dir = s->offset_central_dir;
    s->num_file=0;
    err=unz64local_GetCurrentFileInfoInternal(file,&s->cur_file_info,
                                             &s->cur_file_info_internal,
                                             NULL,0,NULL,0,NULL,0);
    s->current_file_ok = (err == UNZ_OK);
    return err;
}

int ZEXPORT
zip_inflateInit( zip_streamp strm ) {
	assert( strm );

	// Set only fields written by us
	strm->total_in  = 0;
	strm->total_out = 0;
	strm->state     = new internal();
	return ZIP_OK;
}

int ZIPEXPORT
zip_inflate( zip_streamp strm, int ) {
	assert( strm );

	intern( *strm ).put( *strm );
	return ZIP_OK;
}

/*
  Set the current file of the zipfile to the next file.
  return UNZ_OK if there is no problem
  return UNZ_END_OF_LIST_OF_FILE if the actual file was the latest.
*/
extern int ZEXPORT unzGoToNextFile (unzFile  file)
{
    unz64_s* s;
    int err;

    if (file==NULL)
        return UNZ_PARAMERROR;
    s=(unz64_s*)file;
    if (!s->current_file_ok)
        return UNZ_END_OF_LIST_OF_FILE;
    if (s->gi.number_entry != 0xffff)    /* 2^16 files overflow hack */
      if (s->num_file+1==s->gi.number_entry)
        return UNZ_END_OF_LIST_OF_FILE;

    s->pos_in_central_dir += SIZECENTRALDIRITEM + s->cur_file_info.size_filename +
            s->cur_file_info.size_file_extra + s->cur_file_info.size_file_comment ;
    s->num_file++;
    err = unz64local_GetCurrentFileInfoInternal(file,&s->cur_file_info,
                                               &s->cur_file_info_internal,
                                               NULL,0,NULL,0,NULL,0);
    s->current_file_ok = (err == UNZ_OK);
    return err;
}
#endif


