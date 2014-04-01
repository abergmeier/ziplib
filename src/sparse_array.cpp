#include "sparse_array.hpp"

using namespace ziplib;

sparse_array::shared_value_type
sparse_array::erase_before( ptr_off offset ) {
	auto it = _end.find( offset );

	if( it == _end.end() )
		return nullptr;

	_end.erase( it );
	return it->second;
}

// FIXME: We currently don't handle overlapping inserts
const sparse_array::value_type&
sparse_array::insert( ptr_off offset, const char* begin, const char* end ) {
	{ // Try to find entry before
		auto sharedVector = erase_before( offset );

		if( sharedVector ) {
			// Append
			sharedVector->insert( sharedVector->end(), begin, end );
			_end.insert( std::make_pair(offset + std::distance(begin, end), sharedVector) );

			return *sharedVector;
		}
	}

	// Need to create new sparse entry
	shared_value_type beginVector = std::make_shared<value_type>( begin, end );
	auto end_offset = offset + std::distance(begin, end);
	_end.insert( std::make_pair(end_offset, std::move(beginVector)) );

	return *beginVector;
}
