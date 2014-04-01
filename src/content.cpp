/*
 * content.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: andreas
 */

#include "content.hpp"

using namespace ziplib;

void
content::reset() {
}

bool
content::read_full( zip_stream& stream ) {
	return false;
#if 0
	// After header we read content
	if( _left == UNKNOWN_SIZE ) {
		constexpr Bytef DESCRIPTOR_SIGNATURE[4] = 0x504B0708;
		// Try to find (optional) descriptor signature
		const auto end = stream.next_in + stream.avail_in;
		auto it = std::search( stream.next_in, end,
		                       begin(DESCRIPTOR_SIGNATURE), end(DESCRIPTOR_SIGNATURE) );

		if( it == end ) {
			// We do not know whether entry ends here. So copy to backbuffer.
			_backbuffer.insert( _backbuffer.end(), stream.next_in, end );
		} else {
			// File entry may end here

			// Skip signature
			auto distance = G_NSIZE( DESCRIPTOR_SIGNATURE );
			stream.next_in  += distance;
			stream.avail_in -= distance;
			stream.total_in += distance;

			distance = std::min( stream.avail_in, _footer.left );
			std::copy( stream.next_in, stream.next_in + distance, _footer.ptr );
			stream.next_in  += distance;
			stream.avail_in -= distance;
			stream.total_in += distance;
			_footer.ptr  += distance;
			_footer.left -= distance;
		}
	} else {
		// We have a known size
		const auto distance = std::min( stream.avail_in, _left );
		_inflater.inflate( stream.next_in, stream.next_in + distance );
	}

	return !_left;
	// Whether content is completely processed
#endif
}

