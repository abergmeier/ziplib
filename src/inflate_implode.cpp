/*
 * inflate_implode.cpp
 *
 *  Created on: Feb 9, 2013
 *      Author: andreas
 */

#include <cstdint>
#include "internal.hpp"

using namespace ziplib;
using namespace ziplib::inflate;

namespace {
	// Indicates 8K sliding dictionary was used, instead of 4K
	const std::uint8_t ZIP_FLAG_8K_DICT        = 1 << 1;
	// Indicates 3 Shannon-Fano trees were used, instead of 2
	const std::uint8_t ZIP_FLAG_3_SHANNON_FANO = 1 << 2;
}

namespace {
	class inflater : public base_inflater {
	public:
		virtual void inflate() override;
	};
}

void
inflater::inflate() {

}

