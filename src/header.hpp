/*
 * header.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: andreas
 */

#ifndef _ZIPLIB_HEADER_HPP_
#define _ZIPLIB_HEADER_HPP_

#include "include.hpp"
#include "sparse_array.hpp"

namespace ziplib {
	struct header {
		header();
		struct file_header_with_filename {
			zip_local_file_header header;
			char* const filename;
		};
		file_header_with_filename* data();
		const file_header_with_filename* data() const;

		std::uint16_t flags() const;
		ziplib::compression_method compression_method() const;
		std::uint32_t compressed_size() const;
		/**
		 * Read part of the header till end
		 * \returns whether header end was reached
		 */
		bool read_full( zip_stream& stream );
		void reset();
	private:
		enum class stage {
			SEEKING,
			FIXED_HEADER,
			DYNAMIC_HEADER
		};
		stage _stage;
		/**
		 * Signature characters that were successfully matched
		 */
		std::uint8_t _signatureCharactersMatch;
		bool read_fixed  ( zip_stream& stream );
		bool read_dynamic( zip_stream& stream );
		bool find_header ( zip_stream& stream );

		void read_partial( zip_stream& stream, const char* begin, const char* end );
		std::uint64_t variable_size();
		std::vector<char> _buffer;
		sparse_array _unused_data;
	};
}


#endif // _ZIPLIB_HEADER_HPP_
