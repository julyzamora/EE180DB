/*
#--------------------------------------------------------#
  Normalizes datasets
#--------------------------------------------------------#
*/
double minimum = DBL_MAX;
double maximum = -DBL_MAX;
void normalize(double data[], int data_length){
  int i;
  for(i = 0; i < data_length; i++){
    if(data[i] < minimum){
      minimum = data[i];
    }
    if(data[i] > maximum){
      maximum = data[i];
    }
  }
  for(i = 0; i < data_length; i++){
    data[i] = (data[i] - minimum) / (maximum - minimum);
  }
  return;
}
