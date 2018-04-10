#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>


#define BUFFERSIZE 100

int main(){
  char input[32];
  bool gesture_ready = false;
  char compile_Acquire_LowPass_Continuous[] = "gcc -o Acquire_LowPass_Continuous Acquire_LowPass_Continuous.c -lm -lc -lliquid";
  char execute_Acquire_LowPass_Continuous[] = "./Acquire_LowPass_Continuous 0.1 1";
  while(gesture_ready == false){
    if(gesture_ready == false){
      printf("Press enter when ready...");
      fgets(input,sizeof(input),stdin);
      if(input[0]=='\n') {
        //acquire data
        system(compile_Acquire_LowPass_Continuous);
        system(execute_Acquire_LowPass_Continuous);
        gesture_1_ready = true;
      }

    }

  }

}
