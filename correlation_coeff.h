/*---------------------------------------
Calculates correlation coefficent

---------------------------------------*/

#include <math.h>

float correlation_coeff(double x [], double y [], int signal_length){
  int i;
  double x_sum = 0;
  double y_sum = 0;
  double xy_sum = 0;
  double sum_sq_x = 0;
  double sum_sq_y = 0;
  for(i = 0; i < signal_length; i++){
    x_sum = x_sum + x[i];
    y_sum = y_sum + y[i];
    xy_sum = xy_sum + x[i] + y[i];
    sum_sq_y = sum_sq_y + y[i]*y[i];
    sum_sq_x = sum_sq_x + x[i]*x[i];
  }
  float correlation_coeff = (float)(signal_length * xy_sum - x_sum * y_sum)
                              / sqrt((signal_length * sum_sq_x - x_sum*x_sum)
                                      *(signal_length * sum_sq_y - y_sum*y_sum));
  return correlation_coeff;
}
