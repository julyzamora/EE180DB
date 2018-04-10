//Calculates and updates zcs: the zero crossing rate of a signal


/*---------------------------------------
Parameters:
  double [] input:
    time series data (acceleration or gyro)
  int * zc:
    zero crossings associated with data
  int length:
    length of the signal in seconds
Returns:
  N/A updates pointer
---------------------------------------*/

int getsgn(int x){
  if(x < 0)
    return 0;
  else
    return 1;
}
int zero_crossings(double input [], int length){
  int i;
  int zc = 0;
  int sgn_leader, sgn_follower;
  for(i = 0; i < length - 1; i++){
    sgn_leader = getsgn(input[i]);
    sgn_follower = getsgn(input[i+1]);
    if(sgn_leader != sgn_follower){
      zc = zc + 1;
    }
  }
  return zc;
}
