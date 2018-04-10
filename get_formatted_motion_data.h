/*---------------------------------------
Reads in a txt file and returns an array



Parameters:
  filename:
      Not relative need absolute path
  file_length:
      length of the file

  **** might also be used for cross correlation files

---------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <float.h>

void get_formatted_motion_data(char filename [], double acc_x [], double acc_y [],
                                double acc_z [], double gyro_x [], double gyro_y [], double gyro_z []){
  int i = 0;
  char * line = NULL;
  FILE * m_file;
  size_t len = 0;
  ssize_t read;
  bool skipfirst = false;
  int column = 0;
  int row = 0;
  double value = 0.0;
  m_file = fopen(filename,"r");
  if(m_file == NULL){
    printf("File not found...");
  }
  while ((read = getline(&line, &len, m_file)) != -1) {
    if(skipfirst == false){
      skipfirst = true;
      row = row + 1;
      column = column + 1;
      continue;
    }
    char * items = strtok(line,",");
    while(items != NULL){
      switch(column){
        case 2:
          acc_x[i] = atof(items);
          break;
        case 3:
          acc_y[i] = atof(items);
          break;
        case 4:
          acc_z[i] = atof(items);
          break;
        case 5:
          gyro_x[i] = atof(items);
          break;
        case 6:
          gyro_y[i] = atof(items);
          break;
        case 7:
          gyro_z[i] = atof(items);
          break;
        default:
          break;
      }
        items = strtok(NULL,",");
        column = column + 1;
    }
    row = row + 1;
    column = 1;
    i = i + 1;
  }
  fclose(m_file);
}
