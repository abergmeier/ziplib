/*
 * content.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: andreas
 */

#include "include.hpp"

namespace ziplib {
	struct content {
		bool read_full( zip_stream& stream );
		void reset();
		//void reset( const header& );
	private:

	};
}


