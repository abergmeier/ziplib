/*
 * include.hpp
 *
 *  Created on: Apr 11, 2013
 *      Author: andreas
 */

#ifndef _ZIPLIB_INCLUDE_HPP_
#define _ZIPLIB_INCLUDE_HPP_

#include <vector>
#include <cstdint>
#include <memory>
#include <cassert>
#include <array>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <ziplib/ziplib.h>

namespace ziplib {
	enum class compression_method : std::uint16_t {
		STORED     = 0, // No compression
		SHRUNK     = 1,
		REDUCED_1  = 2, // with compression factor 1
		REDUCED_2  = 3, // with compression factor 2
		REDUCED_3  = 4, // with compression factor 3
		REDUCED_4  = 5, // with compression factor 4
		IMPLODED   = 6,
		TOKENIZING = 7,
		DEFLATE    = 8,
		DEFLATE64  = 9,
		OLD_TERSE  = 10, // PKWARE Data Compression Library imploding (old IBM TERSE)
		// RESERVED
		BZIP2      = 12,
		// RESERVED
		LZMA       = 14, // (EFS)
		// RESERVED
		// RESERVED
		// RESERVED
		NEW_TERSE  = 18, // IBM TERSE (new)
		PFS        = 19, // IBM LZ77 z Architecture
		WAVPACK    = 97,
		PPMD       = 98 // PPMd version I, Rev 1
	};

	namespace compression_tag {
		constexpr class stored     {} STORED    ;
		constexpr class shrunk     {} SHRUNK    ;
		constexpr class reduced_1  {} REDUCED_1 ;
		constexpr class reduced_2  {} REDUCED_2 ;
		constexpr class reduced_3  {} REDUCED_3 ;
		constexpr class reduced_4  {} REDUCED_4 ;
		constexpr class imploded   {} IMPLODED  ;
		constexpr class tokenizing {} TOKENIZING;
		constexpr class deflate    {} DEFLATE   ;
		constexpr class deflate64  {} DEFLATE64 ;
		constexpr class old_terse  {} OLD_TERSE ;
		constexpr class bzip2      {} BZIP2     ;
		constexpr class lzma       {} LZMA      ;
		constexpr class new_terse  {} NEW_TERSE ;
		constexpr class pfs        {} PFS       ;
		constexpr class wavpack    {} WAVPACK   ;
		constexpr class ppmd       {} PPMD      ;
	}

	constexpr std::uint16_t compression_method_index( compression_method method ) {
		return method == compression_method::WAVPACK ?
			20:
			method == compression_method::PPMD ?
				21:
				static_cast<std::uint16_t>( method );
	}

	constexpr auto COMPRESSION_METHOD_COUNT = compression_method_index( compression_method::PPMD ) + 1;

	enum class arch : std::uint8_t {
		DOS = 0,
		AMIGA = 1,
		OPENVMS = 2,
		UNIX = 3,
		VM_CMS = 4,
		ATARI_ST = 5,
		HPFS = 6,
		MAC = 7,
		ZSYSTEM = 8,
		CP_M = 9,
		NTFS = 10,
		MVS = 11,
		VSE = 12,
		ACORN = 13,
		VFAT = 14,
		ALT_MVS = 15,
		BEOS = 16,
		TANDEM = 17,
		OS400 = 18,
		OSX = 19
	};
} //namespace ziplib

#endif // _ZIPLIB_INCLUDE_HPP_
