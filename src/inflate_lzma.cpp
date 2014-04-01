/*
 * inflatelzma.cpp
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
}

namespace {

	// Indicates whether an end-of-stream (EOS) marker is used,
	// instead of knowing the compressed data size
	const std::uint8_t ZIP_FLAG_EOS_MARKER_USED = 1 << 1;
}

void
inflater::inflate() {
}

