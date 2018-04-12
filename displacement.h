/*--------------------------------------------------

Assume initially position is at 0
-----------------------------------------------------*/
#include <liquid/liquid.h>
#include <complex.h>
#define dT 0.05
struct filter_options {
	// options
    	unsigned int order; 					//=   4;       // filter order
    	float        fc;    					//=   0.1f;    // cutoff frequency
    	float        f0;    					//=   0.0f;    // center frequency
    	float        Ap;    					//=   1.0f;    // pass-band ripple
    	float        As;    					//=   40.0f;   // stop-band attenuation
    	liquid_iirdes_filtertype ftype;  		//= LIQUID_IIRDES_ELLIP;
    	liquid_iirdes_bandtype   btype; 		//= LIQUID_IIRDES_LOWPASS;
    	liquid_iirdes_format     format; 		//= LIQUID_IIRDES_SOS;
};
double displacement(double input [], int file_length){
  double displacement;
  int i;
  struct filter_options ellip;
  ellip.order =   4;       // filter order
  ellip.fc    =   0.1f;    // cutoff frequency
  ellip.f0    =   0.0f;    // center frequency
  ellip.Ap    =   3.0f;    // pass-band ripple
  ellip.As    =   60.0f;   // stop-band attenuation
  ellip.ftype  = LIQUID_IIRDES_ELLIP;
  ellip.btype  = LIQUID_IIRDES_LOWPASS;
  ellip.format = LIQUID_IIRDES_SOS;
  iirfilt_crcf q = iirfilt_crcf_create_prototype(ellip.ftype, ellip.btype, ellip.format, ellip.order, ellip.fc, ellip.f0, ellip.Ap, ellip.As);
  float complex complex_input[file_length];
  float complex complex_output[file_length];
  for(i = 0; i < file_length; i++){
    complex_input[i] =  input[i] + 0.0*I;
    // execute filter (repeat as necessary)
    iirfilt_crcf_execute(q,complex_input[i],&complex_output[i]);

  }

  // destroy filter object
  iirfilt_crcf_destroy(q);
  // created files for debugging
  // FILE * outputfile;
  // FILE * firstfile;
  // outputfile = fopen("checksmooth.txt", "w");
  // for(i = 0; i < file_length; i++){
  //   fprintf(outputfile, "%f\n", crealf(complex_output[i]));
  // }
  // firstfile = fopen("rawdata.txt", "w");
  // for(i = 0; i < file_length; i++){
  //   fprintf(firstfile, "%f\n", input[i]);
  // }
  // fclose(firstfile);
  // fclose(outputfile);

  /*----------------------------
  https://www.nxp.com/docs/en/application-note/AN3397.pdf
  see this paper for implementation of formula
  */
  double velocity [file_length];
  double curr_output;
  double prev_output;
  //init at rest
  velocity[0] = 0;
  for(i = 1; i <= file_length; i++){
    curr_output = crealf(complex_output[i]);
    prev_output = crealf(complex_output[i-1]);
    velocity[i] = curr_output + (abs(curr_output - prev_output)/2)*dT;

  }
  double position [file_length];
  //init at rest
  position[0] = 0;
  for(i = 1; i <= file_length; i++){
    curr_output = velocity[i];
    prev_output = velocity[i-1];
    position[i] = curr_output + (abs(curr_output - prev_output)/2)*dT;

  }
  displacement = position[file_length] - position[0];
  // printf("%f\n", displacement);
  return displacement;
}
