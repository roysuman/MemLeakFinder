/*
 * =====================================================================================
 *
 *       Filename:  info_lookup.h
 *
 *    Description:  structure and classes to store the information for lookup
 *
 *        Version:  1.0
 *        Created:  Friday 25 December 2015 07:51:50  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *   Organization:  OPEN SOURCE
 *
 * =====================================================================================
 */
#ifndef __info_lookup_h
#define __info_lookup_h
#include<iostream>
#include<map>
#include<vector>
#include<string>
namespace suman_mem_leak_identifier{
	typedef long long address_type;
	typedef std::vector<std::string>::iterator VEC_ITR;
	typedef struct MemInfo{
		address_type address;
		size_t size;
		std::vector<std::string> stack_trace;
		pthread_t caller_thread_id;
		MemInfo(void *sys_addr_, size_t size_,char** stack_trace_,size_t level_,pthread_t thread  ){
			address = reinterpret_cast< suman_mem_leak_identifier::address_type>(sys_addr_);
			size = size_;
			for ( size_t loop = 1 ; loop < level_ ; ++ loop){
				stack_trace.push_back(std::string(stack_trace_[loop]));
			}
			caller_thread_id = thread;

		}
		void print( void ){
			std::cout<<"---------------------------------------------\n";
			std::cout<<"Thread Id "<<caller_thread_id
				<<"\nMemorySize "<<size
				<<"\nStack Trace"<<std::endl;
			for ( std::vector<std::string>::iterator it= stack_trace.begin() ; it != stack_trace.end() ; ++it){
				std::cout<<*it<<std::endl;
			}
			std::cout<<"---------------------------------------------\n";
			
		}
	}MemInfoStr;
	typedef MemInfoStr* MemInfoStrPtr;
	typedef std::map<suman_mem_leak_identifier::address_type,MemInfoStr> HASHMAP;
	typedef HASHMAP::iterator HASHMAP_ITR;
		
	class Lookup{
		private:
			HASHMAP lookup_table;
		public:
			Lookup(){}
			virtual ~Lookup(){}
			void print_stat( void) ;
			bool insert_stat( suman_mem_leak_identifier::address_type , MemInfoStr) ;
			bool remove_stat( suman_mem_leak_identifier::address_type) ;

	};
}

#endif
