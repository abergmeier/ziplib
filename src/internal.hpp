/*
 * inflate.hpp
 *
 *  Created on: Feb 9, 2013
 *      Author: andreas
 */

#ifndef _ZIPLIB_INFLATE_HPP_
#define _ZIPLIB_INFLATE_HPP_

#include "header.hpp"
#include "content.hpp"

namespace ziplib {
	enum class extra_id : std::uint16_t {
		ZIP64          = 0x0001, // Zip64 extended information extra field
		AV_INFO        = 0x0007,
		OS2            = 0x0009,
		NTFS           = 0x000a,
		OPENVMS        = 0x000c,
		UNIX           = 0x000d,
		PATCH          = 0x000f,
		PKCS_7         = 0x0014, //for X.509 Certificates
		X509_FILE      = 0x0015, // X.509 Certificate ID and Signature for individual file
		X509_CENTRAL   = 0x0016, // X.509 Certificate ID for Central Directory
		STRONG_ENCRYPT = 0x0017,
		RECORD_CTRL    = 0x0018, //Record Management Controls
		PKCS_7_LIST    = 0x0019, // PKCS#7 Encryption Recipient Certificate List
		IMB_ATTR       = 0x0065 // IBM S/390 (Z390), AS/400 (I400) attributes - uncompressed
	};

	namespace inflate {
		struct base_inflater {
			virtual ~base_inflater() noexcept = default;
			virtual void inflate();
		};

		struct internal {
			bool put( zip_stream& stream );
			template <typename It>
			std::tuple<It, zip_local_file_header*> find_header( It begin, It end );

			typedef std::unique_ptr<inflate::base_inflater> inflater_pointer;
			inflater_pointer create_inflater( compression_method );
			void inflate( const char* begin, const char* end );
			void reset();

			std::array<inflater_pointer, COMPRESSION_METHOD_COUNT> _inflaters;
			inflater_pointer _inflater;
		private:
			typedef std::uint64_t left_type;
			left_type _left;
			enum class stage {
				HEADER,
				CONTENT
			};

			ziplib::header      _header;
			content     _content;
			stage       _stage;
			struct {
				zip_data_descriptor ptr_begin;
				zip_data_descriptor* ptr;

				void reset() {
					ptr = &ptr_begin;
				}
			} _footer;
		};

		std::unique_ptr<base_inflater> create_inflater( compression_tag::stored    );
		std::unique_ptr<base_inflater> create_inflater( compression_tag::bzip2     );
		std::unique_ptr<base_inflater> create_inflater( compression_tag::deflate   );
		std::unique_ptr<base_inflater> create_inflater( compression_tag::deflate64 );
	} //namespace inflate
} //namespace ziplib

#endif // _ZIPLIB_INFLATE_HPP_

