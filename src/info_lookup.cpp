/*
 * =====================================================================================
 *
 *       Filename:  info_lookup.cpp
 *
 *    Description:  maintain all lookup info
 *
 *        Version:  1.0
 *        Created:  Friday 25 December 2015 08:23:55  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *   Organization:  OPEN SOURCE
 *
 * =====================================================================================
 */

#include "info_lookup.h"
using namespace suman_mem_leak_identifier;
void 
Lookup::print_stat( void ) {
	std::cout<<"Memory Leak\n";
	for ( auto &loop:lookup_table){//.begin() ; loop != this->lookup_table.end() ; ++ loop ){
		(loop.second).print();
	}
	return;
}

bool 
Lookup::insert_stat( suman_mem_leak_identifier::address_type address, MemInfoStr mem_info )  {
	/* the address should not be present into the HASHMAP */
	if ( !lookup_table.insert(std::make_pair(address, mem_info)).second )
		return false;
	return true;
}
bool 
Lookup::remove_stat( suman_mem_leak_identifier::address_type address)  {
	HASHMAP_ITR it;
	if ( ( it= lookup_table.find( address ) ) == lookup_table.end()){
		std::cerr<<"\nERROR .. Address "
			<<address<<"..Not present in lookup table"
			<<std::endl;
		return false;
	}
	lookup_table.erase( it );
	return true;
}
