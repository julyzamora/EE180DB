/*---------------------------------------
Feature vector layout:
  columns 1 - 36:
      mean, variance, standard deviation of
          acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z
          s1 then s2 in that order
  columns 37 - 42
      zero_crossings rate of
          acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z
          in that order
  columns 43 - 48
      position displacement of
          acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z
          in that order
  columns 49 - 54
      angle displacement of
          acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z
          in that order
  columsn 55 - 60
      correlation coefficent of
          acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z
          in that order
  columns 60 - ???
      autocorrelation coeff
Next steps:
  use pca analysis to reduce feature set size



run as gcc -o feature_set feature_set.c -lm -lc -lliquid
---------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "get_formatted_motion_data.h"
#include "get_file_length.h"
#include "m_v_std.h"
#include "zero_crossings.h"
#include "displacement.h"
#define FEATURE_SET_SIZE 78

/*
can probably get signal length with file_length * sampling_rate
*/
#define SIGNAL_LENGTH 10

int main(){
  int i;
  //need to add motion_data_2[] because we have two sensors
  char motion_data[] = "./Motion_datasets/motion_data_output.csv";
  int file_length = get_file_length(motion_data);
  //remove row that include column labels
  double feature_vector[FEATURE_SET_SIZE];
  file_length = file_length - 1;
  /*
  ----------------------------------------
  initialize parameters
  ----------------------------------------
  */
  double acc_x_s1[file_length];
  double acc_y_s1[file_length];
  double acc_z_s1[file_length];
  double acc_x_s2[file_length];
  double acc_y_s2[file_length];
  double acc_z_s2[file_length];
  double gyro_x_s1[file_length];
  double gyro_y_s1[file_length];
  double gyro_z_s1[file_length];
  double gyro_x_s2[file_length];
  double gyro_y_s2[file_length];
  double gyro_z_s2[file_length];
  get_formatted_motion_data(motion_data, acc_x_s1, acc_y_s1, acc_z_s1,
                              gyro_x_s1, gyro_y_s1, gyro_z_s1);
  get_formatted_motion_data(motion_data, acc_x_s2, acc_y_s2, acc_z_s2,
                              gyro_x_s2, gyro_y_s2, gyro_z_s2);


  /*--------------------------------------------------

  Not all features will be used but is needed for feature
  vector to write to pca.csv for python prototyping,
    see: sklearn.decomposition.PCA
  -----------------------------------------------------*/

  /*--------------------------------------------------

    Columns 1 - 38
  -----------------------------------------------------*/
  feature_vector[1] = m_v_std(acc_x_s1, file_length, 1);
  feature_vector[2] = m_v_std(acc_x_s1, file_length, 2);
  feature_vector[3] = m_v_std(acc_x_s1, file_length, 3);
  feature_vector[4] = m_v_std(acc_y_s1, file_length, 1);
  feature_vector[5] = m_v_std(acc_y_s1, file_length, 2);
  feature_vector[6] = m_v_std(acc_y_s1, file_length, 3);
  feature_vector[7] = m_v_std(acc_z_s1, file_length, 1);
  feature_vector[8] = m_v_std(acc_z_s1, file_length, 2);
  feature_vector[9] = m_v_std(acc_z_s1, file_length, 3);
  feature_vector[10] = m_v_std(gyro_x_s1, file_length, 1);
  feature_vector[11] = m_v_std(gyro_x_s1, file_length, 2);
  feature_vector[12] = m_v_std(gyro_x_s1, file_length, 3);
  feature_vector[14] = m_v_std(gyro_y_s1, file_length, 1);
  feature_vector[15] = m_v_std(gyro_y_s1, file_length, 2);
  feature_vector[16] = m_v_std(gyro_y_s1, file_length, 3);
  feature_vector[17] = m_v_std(gyro_z_s1, file_length, 1);
  feature_vector[19] = m_v_std(gyro_z_s1, file_length, 2);
  feature_vector[20] = m_v_std(gyro_z_s1, file_length, 3);
  feature_vector[21] = m_v_std(acc_x_s2, file_length, 1);
  feature_vector[22] = m_v_std(acc_x_s2, file_length, 2);
  feature_vector[23] = m_v_std(acc_x_s2, file_length, 3);
  feature_vector[24] = m_v_std(acc_y_s2, file_length, 1);
  feature_vector[25] = m_v_std(acc_y_s2, file_length, 2);
  feature_vector[26] = m_v_std(acc_y_s2, file_length, 3);
  feature_vector[27] = m_v_std(acc_z_s2, file_length, 1);
  feature_vector[28] = m_v_std(acc_z_s2, file_length, 2);
  feature_vector[29] = m_v_std(acc_z_s2, file_length, 3);
  feature_vector[30] = m_v_std(gyro_x_s2, file_length, 1);
  feature_vector[31] = m_v_std(gyro_x_s2, file_length, 2);
  feature_vector[32] = m_v_std(gyro_x_s2, file_length, 3);
  feature_vector[33] = m_v_std(gyro_y_s2, file_length, 1);
  feature_vector[34] = m_v_std(gyro_y_s2, file_length, 2);
  feature_vector[35] = m_v_std(gyro_y_s2, file_length, 3);
  feature_vector[36] = m_v_std(gyro_z_s2, file_length, 1);
  feature_vector[37] = m_v_std(gyro_z_s2, file_length, 2);
  feature_vector[38] = m_v_std(gyro_z_s2, file_length, 3);

  /*--------------------------------------------------

    Columns 39 - 50
  -----------------------------------------------------*/
  feature_vector[39] = zero_crossings(acc_x_s1, file_length)/SIGNAL_LENGTH;
  feature_vector[40] = zero_crossings(acc_y_s1, file_length)/SIGNAL_LENGTH;
  feature_vector[41] = zero_crossings(acc_z_s1, file_length)/SIGNAL_LENGTH;
  feature_vector[42] = zero_crossings(gyro_x_s1, file_length)/SIGNAL_LENGTH;
  feature_vector[43] = zero_crossings(gyro_y_s1, file_length)/SIGNAL_LENGTH;
  feature_vector[44] = zero_crossings(gyro_z_s1, file_length)/SIGNAL_LENGTH;
  feature_vector[45] = zero_crossings(acc_x_s2, file_length)/SIGNAL_LENGTH;
  feature_vector[46] = zero_crossings(acc_y_s2, file_length)/SIGNAL_LENGTH;
  feature_vector[47] = zero_crossings(acc_z_s2, file_length)/SIGNAL_LENGTH;
  feature_vector[48] = zero_crossings(gyro_x_s2, file_length)/SIGNAL_LENGTH;
  feature_vector[49] = zero_crossings(gyro_y_s2, file_length)/SIGNAL_LENGTH;
  feature_vector[50] = zero_crossings(gyro_z_s2, file_length)/SIGNAL_LENGTH;

  /*--------------------------------------------------

    Columns 51 - 62
  -----------------------------------------------------*/
  feature_vector[51] = displacement(acc_x_s1, file_length);
  feature_vector[52] = displacement(acc_y_s1, file_length);
  feature_vector[53] = displacement(acc_z_s1, file_length);
  feature_vector[54] = displacement(gyro_x_s1, file_length);
  feature_vector[55] = displacement(gyro_y_s1, file_length);
  feature_vector[56] = displacement(gyro_z_s1, file_length);
  feature_vector[57] = displacement(acc_x_s2, file_length);
  feature_vector[58] = displacement(acc_y_s2, file_length);
  feature_vector[59] = displacement(acc_z_s2, file_length);
  feature_vector[60] = displacement(gyro_x_s2, file_length);
  feature_vector[61] = displacement(gyro_y_s2, file_length);
  feature_vector[62] = displacement(gyro_z_s2, file_length);




  return;

}
