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

void get_formatted_motion_data(char filename [], double x [], double y [],
                                double z [], int file_length, int type){
  int i = 0;
  char * line = NULL;
  FILE * m_file;
  size_t len = 0;
  ssize_t read;
  bool skipfirst = false;
  int column = 0;
  int row = 0;
  double value = 0.0;
  double temp_x[file_length];
  double temp_y[file_length];
  double temp_z[file_length];
  double temp_gx[file_length];
  double temp_gy[file_length];
  double temp_gz[file_length];
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
          temp_x[i] = atof(items);
          break;
        case 3:
          temp_y[i] = atof(items);
          break;
        case 4:
          temp_z[i] = atof(items);
          break;
        case 5:
          temp_gx[i] = atof(items);
          break;
        case 6:
          temp_gy[i] = atof(items);
          break;
        case 7:
          temp_gz[i] = atof(items);
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
  if(type == 1){
    for(i = 0; i < file_length; i++){
      x[i] = temp_x[i];
      y[i] = temp_y[i];
      z[i] = temp_z[i];
    }
  }
  if(type == 2){
    for(i = 0; i < file_length; i++){
    x[i] = temp_gx[i];
    y[i] = temp_gy[i];
    z[i] = temp_gz[i];
    }
  }
  fclose(m_file);
}
