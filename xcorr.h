

/*--------------------------------------------------
  Assumes vectors are same length
---------------------------------------------------*/
#define BUFFERSIZE 200
double xcorr(double vector1[], double vector2[], double corr_1_2[], int n_max){
	int		delay;
	int		delay_max;
	int		i , j;
	float		dot_1_2;
	float		m1 = 0;
	float		m2 = 0;

	delay_max = n_max;
	/*
	 * Compute dot products over delay values
	 */
	for (delay = -delay_max; delay <delay_max; delay++) {
		dot_1_2 = 0;

		/*
 		 * Set dot products to zero for delay
		 * exceeding vector length
		 */

		for (i = 0; i < n_max; i++) {
			j = i + delay;
			if (j >= n_max || j < 0){
				continue;
			}
				dot_1_2 = dot_1_2 + (vector1[i] - m1) * (vector2[j] - m2);
		}
		corr_1_2[delay + delay_max] = dot_1_2;
	}
  double sum = 0;
  for(i = 0; i < n_max; i++){
    sum = sum + corr_1_2[i];
  }
  return sum;
}
