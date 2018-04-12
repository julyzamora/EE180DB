/*
----------------------------------------
Computes cross correlation coefficent

Steps:
    use motion data for cross correlation
    extract values in cross correlation value
    discrete integration of values
    return sum
----------------------------------------
*/
double cc_coeff(double corr_data [], int corr_file_length){
  int i;
  double coeff = 0;
  for(i = 0; i < corr_file_length; i++){
    coeff = coeff + corr_data[i];
  }
  return coeff;

}
