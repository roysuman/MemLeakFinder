/*
 * =====================================================================================
 *
 *       Filename:  overload.cpp
 *
 *    Description:  overload malloc and free
 *
 *        Version:  1.0
 *        Created:  Friday 25 December 2015 07:32:35  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *   Organization:  OPEN SOURCE
 *
 * =====================================================================================
 */


#include<iostream>
#include <execinfo.h>
#ifndef  __USE_GNU
#define __USE_GNU
#endif
#include "info_lookup.h"
#include<dlfcn.h>
#define MAX_FRAME 128
#define PTHREAD 1
static bool is_loop_back = true;
#define __FUNCTION_DECLARE_(funcName, returnType, arguments)\
	        static returnType (*libc_##funcName) arguments
                
        
#define __LOAD__ADDR__( var ) do {\
	if ( ( libc_##var = dlsym ( RTLD_NEXT , #var) ) == NULL ){ \
		std::cerr<<"not able to load libc method #var"<<#var<<"Error "<<dlerror()<<std::endl;\
		exit ( -1 );    }       \
} while (0)

static suman_mem_leak_identifier::Lookup lookup_table;
static void init_application( void ) ;//__attribute__((constructor));
static void print_exit_stat( void) __attribute__((destructor));

#ifdef PTHREAD

static pthread_mutex_t lookup_lock = PTHREAD_MUTEX_INITIALIZER;
static void get_lock( ){
	        pthread_mutex_lock( &lookup_lock);
		        return;
}
static void free_lock(){
	        pthread_mutex_unlock( &lookup_lock);
		        return;
}
#else


static void get_lock(){}
static void free_lock(){}
#endif

__FUNCTION_DECLARE_( malloc,void *,(size_t));
__FUNCTION_DECLARE_( free , void, (void *));

static void init_application( void ){
	std::cout<<"INIT_PAPP"<<std::endl;
	libc_malloc = reinterpret_cast<void *(*)(size_t)>(dlsym(RTLD_NEXT,"malloc"));
	if ( libc_malloc == 0 )
		std::cerr<<"ERROR... Failed to get malloc "<<dlerror()<<std::endl;

	libc_free = reinterpret_cast< void(*)(void*)>(dlsym(RTLD_NEXT,"free"));
	if ( libc_free == 0 )
		std::cerr<<"ERROR... Failed to get free"<<dlerror()<<std::endl;
	
//	__LOAD__ADDR__( malloc );
//	__LOAD__ADDR__( free );
}
 
extern "C" void * malloc ( size_t size ){
#ifdef DEBUG
	std::cout<<__FUNCTION__<<std::endl;
#endif
	
	while ( libc_malloc == 0 )
		init_application();
	void *mem_ptr = libc_malloc( size );
	std::cout<<mem_ptr<<"::"<<is_loop_back<<std::endl;
	if (  is_loop_back ){

		pthread_t this_thread_id = pthread_self();
#ifdef DEBUG
		std::cout<<"ThreadID"<<this_thread<<std::endl;
#endif
		get_lock();
		is_loop_back = false;
		 void* this_frames[ MAX_FRAME ];
		size_t this_stack_size = backtrace(this_frames, MAX_FRAME);
		char** this_stack_trace = backtrace_symbols(this_frames, this_stack_size);
		suman_mem_leak_identifier::MemInfoStr mem_info( mem_ptr , size, this_stack_trace , this_stack_size ,this_thread_id);
		lookup_table.insert_stat( mem_info.address , mem_info);
#ifdef DEBUG
		mem_info.print();
#endif


		libc_free(this_stack_trace);
		is_loop_back = true;
		free_lock();
	}
	return mem_ptr;
}


extern "C" void free(void *mem_ptr){
	while ( libc_free == 0 )
		init_application();
	suman_mem_leak_identifier::address_type adr = reinterpret_cast < suman_mem_leak_identifier::address_type>(mem_ptr);
	libc_free( mem_ptr );
	if ( adr> 0 && is_loop_back ){
		get_lock();
		is_loop_back = false;

		lookup_table.remove_stat( adr );
		is_loop_back = true;
		free_lock();
	}
	return;
}

static void print_exit_stat( void ){
	
	lookup_table.print_stat( );
	
}


