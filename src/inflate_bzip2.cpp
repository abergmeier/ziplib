/*
 * inflate_bzip2.cpp
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

std::unique_ptr<base_inflater>
create_inflater( compression_tag::bzip2 ) {
	return nullptr;
	//return std::make_unique<inflater>();
}
