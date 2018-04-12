/*---------------------------------------
Feature vector layout:
  columns 1 - 38
      mean, variance, standard deviation of
          acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z
          s1 then s2 in that order
  columns 39 - 50
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
  gather data
  write python script for pca analysis to reduce feature set size
  get frequency domain signal
  file cleanup/free memory
  implement timing for sensor data acquisition
  implement state machine
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
#include "correlation_coeff.h"
#include "fft.h"
#include "ccorr_coeff.h"
#include "normalize_data.h"
#include "xcorr.h"
#define FEATURE_SET_SIZE 164
/*
can probably get signal length with file_length * sampling_rate
*/
#define SIGNAL_LENGTH 10.0
#define BUFFERSIZE 200

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
  char execute_correlation_data [] = "./waveform_feature_xcorr ";
  char temp_correlation_file [] = "correlation_data.csv";
  char sampling_rate [] = " 20";
  char buffer[BUFFERSIZE];
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
  double freq_acc_x_s1[file_length];
  double freq_acc_y_s1[file_length];
  double freq_acc_z_s1[file_length];
  double freq_gyro_x_s1[file_length];
  double freq_gyro_y_s1[file_length];
  double freq_gyro_z_s1[file_length];
  double freq_acc_x_s2[file_length];
  double freq_acc_y_s2[file_length];
  double freq_acc_z_s2[file_length];
  double freq_gyro_x_s2[file_length];
  double freq_gyro_y_s2[file_length];
  double freq_gyro_z_s2[file_length];
  /*--------------------------------------------------

  Don't have to pass by reference for these functions
  because arrays are passed by pointers in c
  generate xcorrelation file

  2*file_length because we are doing correlation
  also this assumes that each signal is the same length!
  --------------------------------------------------*/
  double temp_corr_arr[2 * file_length];
  printf("Gathering sensor data...\n");
  get_formatted_motion_data(motion_data, acc_x_s1, acc_y_s1, acc_z_s1,
                              file_length, 1);
  get_formatted_motion_data(motion_data, gyro_x_s1, gyro_y_s1, gyro_z_s1, file_length, 2);

  get_formatted_motion_data(motion_data, acc_x_s2, acc_y_s2, acc_z_s2,
                                                          file_length, 1);
  get_formatted_motion_data(motion_data, gyro_x_s2, gyro_y_s2, gyro_z_s2, file_length, 2);
  // get_fft_data(acc_x_s1, file_length, freq_acc_x_s1);
  // get_fft_data(freq_acc_y_s1, file_length);
  // get_fft_data(freq_acc_z_s1, file_length);
  // get_fft_data(freq_gyro_x_s1, file_length);
  // get_fft_data(freq_gyro_y_s1, file_length);
  // get_fft_data(freq_gyro_z_s1, file_length);
  // get_fft_data(freq_acc_x_s2, file_length);
  // get_fft_data(freq_acc_y_s2, file_length);
  // get_fft_data(freq_acc_z_s2, file_length);
  // get_fft_data(freq_gyro_x_s2, file_length);
  // get_fft_data(freq_gyro_y_s2, file_length);
  // get_fft_data(freq_gyro_z_s2, file_length);


  /*
    Using min max scaler shifts data to lie between
    0 and 1 therefore we need to run zero crossings rate before
    we normalize the data
  */
  /*--------------------------------------------------

    Columns 39 - 50
  -----------------------------------------------------*/
  printf("Calculating zero crossings rate...\n");
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

  /*--------------------------------------------------

    Normalize data
  -----------------------------------------------------*/
  for(i = 0; i < file_length; i++){
    printf("%f, %f, %f, %f, %f, %f\n",acc_x_s1[i], acc_y_s1[i], acc_z_s1[i], gyro_x_s1[i], gyro_y_s1[i], gyro_z_s1[i]);
  }
  printf("normalizing dataset...\n");
  normalize(acc_x_s1, file_length);
  normalize(acc_y_s1, file_length);
  normalize(acc_z_s1, file_length);
  normalize(gyro_x_s1, file_length);
  normalize(gyro_y_s1, file_length);
  normalize(gyro_z_s1, file_length);
  normalize(acc_x_s2, file_length);
  normalize(acc_y_s2, file_length);
  normalize(acc_z_s2, file_length);
  normalize(gyro_x_s2, file_length);
  normalize(gyro_y_s2, file_length);
  normalize(gyro_z_s2, file_length);

  for(i = 0; i < file_length; i++){
    printf("%f, %f, %f, %f, %f, %f\n",acc_x_s1[i], acc_y_s1[i], acc_z_s1[i], gyro_x_s1[i], gyro_y_s1[i], gyro_z_s1[i]);
  }

  /*--------------------------------------------------

  Not all features will be used but is needed for feature
  vector to write to pca.csv for python prototyping,
    see: sklearn.decomposition.PCA
  -----------------------------------------------------*/

  /*--------------------------------------------------

    Columns 1 - 38
  -----------------------------------------------------*/
  printf("Calculating mean variance and standard deviation...\n");
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

  // /*--------------------------------------------------
  //
  //   Columns 39 - 50 see line 137
  // -----------------------------------------------------*/


  /*--------------------------------------------------

    Columns 51 - 62
  -----------------------------------------------------*/
  printf("Calculating position and angle displacement...\n");
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

  /*--------------------------------------------------

    Columns 63 - 86
  -----------------------------------------------------*/
  printf("Computing correlation coefficent...\n");
  feature_vector[63] = correlation_coeff(acc_x_s1, acc_y_s1, file_length);
  feature_vector[64] = correlation_coeff(acc_x_s1, acc_z_s1, file_length);
  feature_vector[65] = correlation_coeff(acc_y_s1, acc_z_s1, file_length);
  feature_vector[66] = correlation_coeff(acc_x_s2, acc_y_s2, file_length);
  feature_vector[67] = correlation_coeff(acc_x_s2, acc_z_s2, file_length);
  feature_vector[68] = correlation_coeff(acc_y_s2, acc_z_s2, file_length);
  feature_vector[69] = correlation_coeff(acc_x_s1, acc_x_s2, file_length);
  feature_vector[70] = correlation_coeff(acc_x_s1, acc_y_s2, file_length);
  feature_vector[71] = correlation_coeff(acc_x_s1, acc_z_s2, file_length);
  feature_vector[72] = correlation_coeff(acc_y_s1, acc_z_s2, file_length);
  feature_vector[73] = correlation_coeff(acc_y_s1, acc_y_s2, file_length);
  feature_vector[74] = correlation_coeff(acc_z_s1, acc_z_s2, file_length);
  feature_vector[75] = correlation_coeff(gyro_x_s1, gyro_y_s1, file_length);
  feature_vector[76] = correlation_coeff(gyro_x_s1, gyro_z_s1, file_length);
  feature_vector[77] = correlation_coeff(gyro_y_s1, gyro_z_s1, file_length);
  feature_vector[78] = correlation_coeff(gyro_x_s2, gyro_y_s2, file_length);
  feature_vector[79] = correlation_coeff(gyro_x_s2, gyro_z_s2, file_length);
  feature_vector[80] = correlation_coeff(gyro_y_s2, gyro_z_s2, file_length);
  feature_vector[81] = correlation_coeff(gyro_x_s1, gyro_x_s2, file_length);
  feature_vector[82] = correlation_coeff(gyro_x_s1, gyro_y_s2, file_length);
  feature_vector[83] = correlation_coeff(gyro_x_s1, gyro_z_s2, file_length);
  feature_vector[84] = correlation_coeff(gyro_y_s1, gyro_z_s2, file_length);
  feature_vector[85] = correlation_coeff(gyro_y_s1, gyro_y_s2, file_length);
  feature_vector[86] = correlation_coeff(gyro_z_s1, gyro_z_s2, file_length);

  /*--------------------------------------------------

    Columns 87 - 92
    Cross Correlation Coeff
    layout:
      x1x1,y1y1,z1z1,
      x1y1,x1z1,y1z1,
      x1x2,x1y2,x1z2,
      y1x2,y1y2,y1z2,
      z1x2,z1y2,z1z2,
      x2x2,y2y2,z2z2,
      x2y2,x2z2,y2z2
    correlation between acceleration sensors
  -----------------------------------------------------*/
  printf("Computing autocorrelation and cross-correlation coefficents...\n");
  feature_vector[87] = xcorr(acc_x_s1, acc_x_s1, temp_corr_arr, file_length);
  feature_vector[88] = xcorr(acc_y_s1, acc_y_s1, temp_corr_arr, file_length);
  feature_vector[89] = xcorr(acc_z_s1, acc_z_s1, temp_corr_arr, file_length);
  feature_vector[90] = xcorr(acc_x_s1, acc_y_s1, temp_corr_arr, file_length);
  feature_vector[91] = xcorr(acc_x_s1, acc_z_s1, temp_corr_arr, file_length);
  feature_vector[92] = xcorr(acc_y_s1, acc_z_s2, temp_corr_arr, file_length);
  feature_vector[93] = xcorr(acc_x_s1, acc_x_s2, temp_corr_arr, file_length);
  feature_vector[94] = xcorr(acc_x_s1, acc_y_s2, temp_corr_arr, file_length);
  feature_vector[95] = xcorr(acc_x_s1, acc_z_s2, temp_corr_arr, file_length);
  feature_vector[96] = xcorr(acc_y_s1, acc_x_s2, temp_corr_arr, file_length);
  feature_vector[97] = xcorr(acc_y_s1, acc_y_s2, temp_corr_arr, file_length);
  feature_vector[98] = xcorr(acc_y_s1, acc_z_s2, temp_corr_arr, file_length);
  feature_vector[99] = xcorr(acc_z_s1, acc_x_s2, temp_corr_arr, file_length);
  feature_vector[100] = xcorr(acc_z_s1, acc_y_s2, temp_corr_arr, file_length);
  feature_vector[101] = xcorr(acc_z_s1, acc_z_s2, temp_corr_arr, file_length);
  feature_vector[102] = xcorr(acc_x_s2, acc_x_s2, temp_corr_arr, file_length);
  feature_vector[103] = xcorr(acc_y_s2, acc_y_s2, temp_corr_arr, file_length);
  feature_vector[104] = xcorr(acc_z_s2, acc_z_s2, temp_corr_arr, file_length);
  feature_vector[105] = xcorr(acc_x_s2, acc_y_s2, temp_corr_arr, file_length);
  feature_vector[106] = xcorr(acc_x_s2, acc_z_s2, temp_corr_arr, file_length);
  feature_vector[107] = xcorr(acc_y_s2, acc_z_s2, temp_corr_arr, file_length);

  /*--------------------------------------------------

    Columns 87 - 92
    Cross Correlation Coeff
    layout:
      x1x1,y1y1,z1z1,
      x1y1,x1z1,y1z1,
      x1x2,x1y2,x1z2,
      y1x2,y1y2,y1z2,
      z1x2,z1y2,z1z2,
      x2x2,y2y2,z2z2,
      x2y2,x2z2,y2z2
    auto/cross correlation between gyro sensors
  -----------------------------------------------------*/
  feature_vector[108] = xcorr(gyro_x_s1, gyro_x_s1, temp_corr_arr, file_length);
  feature_vector[109] = xcorr(gyro_y_s1, gyro_y_s1, temp_corr_arr, file_length);
  feature_vector[110] = xcorr(gyro_z_s1, gyro_z_s1, temp_corr_arr, file_length);
  feature_vector[111] = xcorr(gyro_x_s1, gyro_y_s1, temp_corr_arr, file_length);
  feature_vector[112] = xcorr(gyro_x_s1, gyro_z_s1, temp_corr_arr, file_length);
  feature_vector[113] = xcorr(gyro_y_s1, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[114] = xcorr(gyro_x_s1, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[115] = xcorr(gyro_x_s1, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[116] = xcorr(gyro_x_s1, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[117] = xcorr(gyro_y_s1, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[118] = xcorr(gyro_y_s1, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[119] = xcorr(gyro_y_s1, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[120] = xcorr(gyro_z_s1, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[121] = xcorr(gyro_z_s1, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[122] = xcorr(gyro_z_s1, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[123] = xcorr(gyro_x_s2, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[124] = xcorr(gyro_y_s2, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[125] = xcorr(gyro_z_s2, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[126] = xcorr(gyro_x_s2, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[127] = xcorr(gyro_x_s2, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[128] = xcorr(gyro_y_s2, gyro_z_s2, temp_corr_arr, file_length);

  /*--------------------------------------------------

    Columns 87 - 92
    Cross Correlation Coeff
    layout:
      x1gx1, x1gy1, x1gz1
      y1gx1, y1gy1, g1gz1
      z1gx1, z1gy1, z1gz1
    cross correlation between sensor 1 accelration gyro sensor 1
    no need for auto and cross for individual sensors
    ...did previously
  -----------------------------------------------------*/
  feature_vector[129] = xcorr(acc_x_s1, gyro_x_s1, temp_corr_arr, file_length);
  feature_vector[130] = xcorr(acc_x_s1, gyro_y_s1, temp_corr_arr, file_length);
  feature_vector[131] = xcorr(acc_x_s1, gyro_z_s1, temp_corr_arr, file_length);
  feature_vector[132] = xcorr(acc_y_s1, gyro_x_s1, temp_corr_arr, file_length);
  feature_vector[133] = xcorr(acc_y_s1, gyro_y_s1, temp_corr_arr, file_length);
  feature_vector[134] = xcorr(acc_y_s1, gyro_z_s1, temp_corr_arr, file_length);
  feature_vector[135] = xcorr(acc_z_s1, gyro_x_s1, temp_corr_arr, file_length);
  feature_vector[136] = xcorr(acc_z_s1, gyro_y_s1, temp_corr_arr, file_length);
  feature_vector[137] = xcorr(acc_z_s1, gyro_z_s1, temp_corr_arr, file_length);
  /*--------------------------------------------------

    Columns 87 - 92
    Cross Correlation Coeff
    layout:
      x1gx2, x1gy2, x1gz2
      y1gx2, y1gy2, g1gz2
      z1gx2, z1gy2, z1gz2
    cross correlation between sensor 1 accelration gyro
    sensor 2
    no need for auto and cross for individual sensors
    ...did previously
  -----------------------------------------------------*/
  feature_vector[138] = xcorr(acc_x_s1, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[139] = xcorr(acc_x_s1, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[140] = xcorr(acc_x_s1, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[141] = xcorr(acc_y_s1, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[142] = xcorr(acc_y_s1, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[143] = xcorr(acc_y_s1, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[144] = xcorr(acc_z_s1, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[145] = xcorr(acc_z_s1, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[146] = xcorr(acc_z_s1, gyro_z_s2, temp_corr_arr, file_length);

  /*--------------------------------------------------

    Columns 87 - 92
    Cross Correlation Coeff
    layout:
      x1gx2, x1gy2, x1gz2
      y1gx2, y1gy2, g1gz2
      z1gx2, z1gy2, z1gz2
    cross correlation between sensor 2 accelration gyro
    sensor 1
    no need for auto and cross for individual sensors
    ...did previously
  -----------------------------------------------------*/
  feature_vector[147] = xcorr(acc_x_s2, gyro_x_s1, temp_corr_arr, file_length);
  feature_vector[148] = xcorr(acc_x_s2, gyro_y_s1, temp_corr_arr, file_length);
  feature_vector[149] = xcorr(acc_x_s2, gyro_z_s1, temp_corr_arr, file_length);
  feature_vector[150] = xcorr(acc_y_s2, gyro_x_s1, temp_corr_arr, file_length);
  feature_vector[151] = xcorr(acc_y_s2, gyro_y_s1, temp_corr_arr, file_length);
  feature_vector[152] = xcorr(acc_y_s2, gyro_z_s1, temp_corr_arr, file_length);
  feature_vector[153] = xcorr(acc_z_s2, gyro_x_s1, temp_corr_arr, file_length);
  feature_vector[154] = xcorr(acc_z_s2, gyro_y_s1, temp_corr_arr, file_length);
  feature_vector[155] = xcorr(acc_z_s2, gyro_z_s1, temp_corr_arr, file_length);
  /*--------------------------------------------------

    Columns 87 - 92
    Cross Correlation Coeff
    layout:
      x1gx2, x1gy2, x1gz2
      y1gx2, y1gy2, g1gz2
      z1gx2, z1gy2, z1gz2
    cross correlation between sensor 2 accelration gyro
    sensor 2
    no need for auto and cross for individual sensors
    ...did previously
  -----------------------------------------------------*/
  feature_vector[156] = xcorr(acc_x_s2, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[157] = xcorr(acc_x_s2, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[158] = xcorr(acc_x_s2, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[159] = xcorr(acc_y_s2, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[160] = xcorr(acc_y_s2, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[161] = xcorr(acc_y_s2, gyro_z_s2, temp_corr_arr, file_length);
  feature_vector[162] = xcorr(acc_z_s2, gyro_x_s2, temp_corr_arr, file_length);
  feature_vector[163] = xcorr(acc_z_s2, gyro_y_s2, temp_corr_arr, file_length);
  feature_vector[164] = xcorr(acc_z_s2, gyro_z_s2, temp_corr_arr, file_length);
  /*--------------------------------------------------

    End of time domain features
  -----------------------------------------------------*/
  for(i = 0; i < FEATURE_SET_SIZE; i++){
    printf("%f\n", feature_vector[i]);
  }

  return;

}
