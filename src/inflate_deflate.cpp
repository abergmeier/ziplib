/*
 * inflate_deflate.cpp
 *
 *  Created on: Feb 9, 2013
 *      Author: andreas
 */

#include "internal.hpp"

using namespace ziplib;
using namespace ziplib::inflate;

namespace {
	class inflater : public base_inflater {
	public:
		virtual void inflate() override;
	};

	class inflater64 : public inflater {
		virtual void inflate() override;
	};
}

namespace {

	enum class compression_strategy : std::uint8_t {
		NORMAL     = 0,
		MAX        = 1 << 1,
		FAST       = 1 << 2,
		SUPER_FAST = 1 << 1 + 1 << 2
	};

	constexpr std::uint8_t ZIP_COMPRESSION_MASK = static_cast<std::uint8_t>(compression_strategy::MAX)
	                                            + static_cast<std::uint8_t>(compression_strategy::FAST);

	compression_strategy strategy( const zip_local_file_header& header ) {
		return static_cast<compression_strategy>( *reinterpret_cast<const std::uint16_t*>(header.partial.flag) & ZIP_COMPRESSION_MASK );
	}
}

void
inflater::inflate() {
}

namespace {
	// The size will be in the corresponding 8 byte ZIP64 extended information
	// extra field
	const std::uint32_t USE_EXTENDED = 0xFFFFFFFF;
}

void
inflater64::inflate() {
}


