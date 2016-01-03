/*
 * =====================================================================================
 *
 *       Filename:  a.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  Sunday 27 December 2015 10:19:43  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  SIGCONT (suman roy), email.suman.roy@gmail.com
 *   Organization:  OPEN SOURCE
 *
 * =====================================================================================
 */
#include<iostream>
#include<string>
#include<stdlib.h>
int main(){
	std::cout<<"SUMAN\n";
 	int *p = new int[10];
	for ( size_t loop = 0 ; loop< 10 ; ++loop)
		std::cout<<p[loop]<<std::endl;
	delete p;
	std::cout<<"END"<<std::endl;
	return 0;
}
