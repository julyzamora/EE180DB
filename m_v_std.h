
/*---------------------------------------
Returns mean, std_dev, variance value of an
array

parameters:
  data: acceleration/gyro data
  data_length: data length
  r_value: 1 for mean, 2 for variance, 3 for std dev
---------------------------------------*/
#include <math.h>

double m_v_std(double data [], double data_length, int r_value){
  int i;
  double sum = 0;
  double sum1 = 0;
  for(i = 0; i < data_length; i++){
    sum = sum + data[i];
  }
  double mean = sum/data_length;
  for(i = 0; i < data_length; i++){
    sum1 = sum1 + pow((data[i] - mean),2);
  }
  double variance = sum1/data_length;
  double std_dev = sqrt(variance);
  switch(r_value){
    case 1:
      return mean;
      break;
    case 2:
      return variance;
      break;
    case 3:
      return std_dev;
      break;
    default:
      return 0;
      break;
    }
}
