/*
 * ziplib.h

 *
 *  Created on: Feb 7, 2013
 *      Author: andreas
 */

#ifndef _ZIPLIB_ZIPLIB_H_
#define _ZIPLIB_ZIPLIB_H_

#include <zlib.h>
#include <stdint.h>

#define ZIPEXPORT ZEXPORT
#define ZIPEXTERN ZEXTERN

/* a type chosen by DEFINE */
#ifdef HAVE_64BIT_INT_CUSTOM
	typedef  64BIT_INT_CUSTOM_TYPE ZPOS64_T;
#else
	#ifdef HAS_STDINT_H
	#include "stdint.h"
		typedef uint64_t ZPOS64_T;
	#else

		/* Maximum unsigned 32-bit value used as placeholder for zip64 */
		#define MAXU32 0xffffffff

		#if defined(_MSC_VER) || defined(__BORLANDC__)
			typedef unsigned __int64 ZPOS64_T;
		#else
			typedef unsigned long long int ZPOS64_T;
		#endif
	#endif
#endif //HAVE_64BIT_INT_CUSTOM

#define ZIP_OK 0

/* tm_unz contain date/time info */
typedef struct tm_unz_s
{
	uInt tm_sec;            /* seconds after the minute - [0,59] */
	uInt tm_min;            /* minutes after the hour - [0,59] */
	uInt tm_hour;           /* hours since midnight - [0,23] */
	uInt tm_mday;           /* day of the month - [1,31] */
	uInt tm_mon;            /* months since January - [0,11] */
	uInt tm_year;           /* years - [1980..2044] */
} tm_unz;

enum zip_flags {
	ZIP_ENCRYPTED           = 1 << 0,
	// fields crc-32, compressed size and uncompressed size are set to zero in the local header.
	// The correct values are put in the data descriptor immediately following the compressed data.
	ZIP_FILE_SIZE_UNKNOWN   = 1 << 3,
	// File is compressed patched data
	ZIP_PATCH_DATA          = 1 << 5,
	// MUST set the version needed to extract value to at least
	// 50 and you MUST also set bit 0. If AES encryption is used, the version needed
	// to extract value MUST be at least 51.
	ZIP_STRONG_ENCRYPTION   = 1 << 6,
	// The filename and comment fields for this file MUST be encoded using UTF-8.
	ZIP_LANG_ENCODING       = 1 << 11,
	// Indicates selected data values in the Local Header are masked to
	// hide their actual values.
	ZIP_LOCAL_HEADER_MASKED = 1 << 13
};

#define ZIP_CENTRAL_FILE_INFO_HEADER 46
#define ZIP_CENTRAL_FILE_INFO_FOOTER 22

#define ZIP_LOCAL_FILE_HEADER_BYTES 30

typedef struct zip_data_descriptor_s {
	char crc32[4];
	char compressed_size[4];
	char uncompressed_size[4];
} zip_data_descriptor;

struct zip_file_header_partial_s {
	char version_needed[2];     // version needed to extract
	char flag[2];               // general purpose bit flag
	uint16_t compression_method; // compression method
	char modification_time[2];
	char modification_data[2];

	char crc[4];                // crc-32

	char compressed_size[4];    // compressed size

	char uncompressed_size[4];  // uncompressed size
	char size_filename[2];      // filename length
	char size_file_extra[2];    // extra field length
};

typedef struct zip_local_file_header_s {
	char signature[4];          // 0x504b0304 (little-endian number)

	struct zip_file_header_partial_s partial;

} zip_local_file_header;

typedef struct zip_file_info64_s
{
	uLong version;              /* version made by                 2 bytes */
	uLong version_needed;       /* version needed to extract       2 bytes */
	uLong flag;                 /* general purpose bit flag        2 bytes */
	uLong compression_method;   /* compression method              2 bytes */
	uLong dosDate;              /* last mod file date in Dos fmt   4 bytes */
	uLong crc;                  /* crc-32                          4 bytes */
	ZPOS64_T compressed_size;   /* compressed size                 8 bytes */
	ZPOS64_T uncompressed_size; /* uncompressed size               8 bytes */
	uLong size_filename;        /* filename length                 2 bytes */
	uLong size_file_extra;      /* extra field length              2 bytes */
	uLong size_file_comment;    /* file comment length             2 bytes */

	uLong disk_num_start;       /* disk number start               2 bytes */
	uLong internal_fa;          /* internal file attributes        2 bytes */
	uLong external_fa;          /* external file attributes        4 bytes */

	tm_unz tmu_date;
} zip_file_info64;

typedef struct zip_file_info_s
{
	char signature[4];          // 0x02014b50
	char version[2];            // version made by                 2 bytes */
	struct zip_file_header_partial_s partial;

	char size_file_comment[2];  /* file comment length             2 bytes */

	char disk_num_start[2];     /* disk number start               2 bytes */
	char internal_fa[2];        /* internal file attributes        2 bytes */
	char external_fa[4];        /* external file attributes        4 bytes */
	char local_header_offset[4];

	tm_unz tmu_date;
} zip_file_info;

typedef struct zip_stream_s {
	const char *next_in;  // next input byte
	uInt        avail_in; // number of bytes available at next_in
	uLong       total_in; // total number of input bytes read so far

	char* next_out;  // next output byte should be put there
	uInt  avail_out; // remaining free space at next_out
	uLong total_out; // total number of bytes output so far

	z_const char *msg;  /* last error message, NULL if no error */
	void*         state; /* not visible by applications */

} zip_stream;

typedef zip_stream* zip_streamp;

#include <ziplib/inflate.h>

#endif // _ZIPLIB_ZIPLIB_H_ */
