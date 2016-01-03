 /* 
 * =====================================================================================
 *
 *       Filename:  main_code.cpp
 *
 *    Description:  the main programm
 *
 *        Version:  1.0
 *        Created:  Thursday 23 April 2015 03:09:50  IST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Suman Roy (), 
 *   Organization:  SELF
 *
 * =====================================================================================
 */
#define _GNU_SOURCE 1
#include<stdbool.h>
#include<getopt.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<iostream>
#include<ctype.h>
#define VERSION "1.0"
#define MAX_PATH_LENGTH 256
#define LIB_NAME "mem_leak.so"
#define DEBUG 1
static void 
version( void){
	std::cout<<"Mem_Leak_Version "<<VERSION<<" Author SUMAN ROY"<<std::endl;
	return;
}
static void 
usage(void  ){

	std::cout<<"\n\t -v/-V Version\n"
		<<"\n\t -u/-U Usage\n"
		<<"\n\t -c/-C programm toexecute\n";
	version();
	exit(EXIT_FAILURE);
	return;
}

static void 
parse(char *line, char **argv){
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = NULL;//'\0';                 /* mark the end of argument list  */
}
void  
execute(char **argv){
#ifdef DEBUG
	std::cout<<argv[0]<<argv[1]<<argv<<std::endl;
//	fprintf(stdout , "argv [ 1 ] = %s\n ",argv [1 ] );
//	fprintf( stdout , "argv [ 2 ] = %s \n ", argv [ 2] );
/* 	fprintf( stdout , "argv [ 3 ] = %s \n ", argv [ 3] );
	fprintf( stdout , "argv [ 4 ] = %s \n ", argv [ 4] );
	fprintf( stdout , "argv [ 5 ] = %s \n ", argv [ 5] );*/

#endif
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
		  std::cerr<<"*** ERROR: exec failed"<<std::endl;
               exit(EXIT_FAILURE);
          }
}
int main( int argc , char** argv){
	
	int      opt;
	char     buff [ MAX_PATH_LENGTH];
	bool     cmd_flag = false;
	char     *command ;

	char     *argv2[64];              /* the command line argument      */

	cmd_flag = false;
	while (  ( opt = getopt ( argc , argv ,"uUvVc:C:") ) != -1 ){
			std::cout<<opt<<std::endl;
		switch ( opt ){
			case 'v':
			case 'V':
				version();
				break;
			case 'C':
			case 'c':
				command = optarg;
				cmd_flag = true;
				std::cout<<"COMMAND"<<command<<std::endl;
				break;
 			case '?':
				if (optopt == 'c' || optopt == 'C')
					std::cerr<<" requires an argument";
				else if (isprint (optopt))
					std::cerr<<"Unknown Option"<< optopt;
				usage();
				break;
			case 'u':
			case 'U':
			default:
				std::cout<<"DEFAULT"<<std::endl;
				usage();
		}
	}

	if (!cmd_flag) {
		std::cerr<<"Expected argument after options"<<std::endl;
		usage();
	}

	argc -= optind  ;
	argv +=optind  ;
	std::cout<<"Go parse "<<command<<std::endl;
	parse( command, argv2);
	//	return 1;
		const char *lib = LIB_NAME;
		strcat( buff , lib);// , sizeof( lib));
		setenv( "LD_PRELOAD",buff,1);
		std::cout<<*argv2<<std::endl;
		execute(argv2);
		//control will never return
		return 0;
}

