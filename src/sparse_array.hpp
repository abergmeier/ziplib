/*
 * sparse_array.hpp
 *
 *  Created on: Apr 5, 2013
 *      Author: andreas
 */

#ifndef _ZIPLIB_SPARSE_ARRAY_HPP_
#define _ZIPLIB_SPARSE_ARRAY_HPP_

#include <cstdint>
#include <vector>
#include <memory>
#include <map>

namespace ziplib {
	struct sparse_array {
			typedef std::int64_t                ptr_off;
			typedef std::vector<char>           value_type;
			typedef std::shared_ptr<value_type> shared_value_type;
			/**
			 * Insert content between `begin` and `end` at position `offset`
			 */
			const value_type& insert( ptr_off offset, const char* begin, const char* end );
		private:
			typedef std::map<ptr_off, shared_value_type> shared_map_type;
			shared_map_type _end;

			/**
			 * Try to find element which ends right before `offset`
			 */
			shared_value_type erase_before( ptr_off offset );
		};
} // namespace ziplib

#endif // _ZIPLIB_SPARSE_ARRAY_HPP_
