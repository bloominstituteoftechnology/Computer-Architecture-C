#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */

  int main(int argc, char *argv[]){
  	
	if(argc ==2){
  		struct cpu cpu;
  		cpu_init(&cpu);
  		cpu_load(&cpu, argv);
  		cpu_run(&cpu);
	}
	else {
                printf("Please provide a file name \n");
	}
  	return 0;
}	 


