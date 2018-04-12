/*
----------------------------------------
Performs the fft on an array
----------------------------------------
*/
#include <liquid/liquid.h>
#include <complex.h>

void get_fft_data(double input[], int file_length, double output[]){
    // options
    int flags=0;        // FFT flags (typically ignored)
    int i;
    // allocated memory arrays
    // float complex complex_input [file_length];
    // float complex complex_output [file_length];
    // // create FFT plan
    // fftplan q = fft_create_plan(file_length, complex_input, complex_output, LIQUID_FFT_FORWARD, flags);
    // for(i = 0; i < file_length; i++){
    //   double num = input[i];
    //   complex_input[i] = num;
    //   fft_execute(q);
    // }
    // // ... initialize input ...
    // for(i = 0; i < file_length; i++){
    //   printf("%i\n", crealf(complex_output[i]) );
    // }
    // // execute FFT (repeat as necessary)
    // fft_execute(q);
    //
    // // for(i = 0; i < file_length; i++){
    // //   printf("%f\n", crealf(output_signal[i]));
    // // }
    // // destroy FFT plan and free memory arrays
    // fft_destroy_plan(q);

}
