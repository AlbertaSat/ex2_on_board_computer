/*
Luca Fossati (Luca.Fossati@esa.int), European Space Agency

Software distributed under the "European Space Agency Public License  v2.0".

All Distribution of the Software and/or Modifications, as Source Code or Object Code,
must be, as a whole, under the terms of the European Space Agency Public License  v2.0.
If You Distribute the Software and/or Modifications as Object Code, You must:
(a) provide in addition a copy of the Source Code of the Software and/or
Modifications to each recipient; or
(b) make the Source Code of the Software and/or Modifications freely accessible by reasonable
means for anyone who possesses the Object Code or received the Software and/or Modifications
from You, and inform recipients how to obtain a copy of the Source Code.

The Software is provided to You on an as is basis and without warranties of any
kind, including without limitation merchantability, fitness for a particular purpose,
absence of defects or errors, accuracy or non-infringement of intellectual property
rights.
Except as expressly set forth in the "European Space Agency Public License  v2.0",
neither Licensor nor any Contributor shall be liable, including, without limitation, for direct, indirect,
incidental, or consequential damages (including without limitation loss of profit),
however caused and on any theory of liability, arising in any way out of the use or
Distribution of the Software or the exercise of any rights under this License, even
if You have been advised of the possibility of such damages.

*****************************************************************************************

XXXXXXXXXXXXXXXXXXXXXXX
*/

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>

#include "unpredict.h"
#include "utils.h"
#include "predictor.h"

/// Given the mapped residual and the prediction it extracts the original sample
unsigned short int get_sample(unsigned short int residual, int scaled_predicted, unsigned int s_min, unsigned int s_max){
    int predicted_sample = scaled_predicted/2;
    unsigned int omega = predicted_sample - s_min;
    unsigned int selected_omega = 0;
    unsigned short int sample = 0;
    int delta = 0;

    if(omega > s_max - predicted_sample){
        omega = s_max - predicted_sample;
        selected_omega = 1;
    }

    if(residual > 2*omega){
        if(selected_omega == 0)
            delta = residual - omega;
        else
            delta = omega - residual;
    }else{
        if((residual & 0x1) == 0){
            int sign_scaled = (scaled_predicted & 0x1) != 0 ? -1 : 1;
            delta = sign_scaled*(residual/2);
        }else{
            int sign_scaled = (scaled_predicted & 0x1) != 0 ? 1 : -1;
            delta = sign_scaled*((residual + 1)/2);
        }
    }

    sample = delta + predicted_sample;

    return sample;
}

/// Given the mapped residuals saved in BSQ format it iterates over them, computing
/// the prediction and, then extracting the original sample.
int unpredict(input_feature_t input_params, predictor_config_t predictor_params, unsigned short int * residuals, char outputFile[128]){
    unsigned short int *samples = NULL;
    unsigned int s_min = 0;
    unsigned int s_max = (0x1 << input_params.dyn_range) - 1;
    unsigned int s_mid = 0x1 << (input_params.dyn_range - 1);
    unsigned int x = 0, y = 0, z = 0;
    int * weights = NULL;
    int weights_len = predictor_params.pred_bands + (predictor_params.full != 0 ? 3 : 0);

    samples = (unsigned short int *)malloc(sizeof(unsigned short int)*input_params.x_size*input_params.y_size*input_params.z_size);
    if(samples == NULL){
        fprintf(stderr, "Error in allocating %lf kBytes for the output image buffer\n\n", ((double)sizeof(unsigned short int)*input_params.x_size*input_params.y_size*input_params.z_size)/1024.0);
        return -1;
    }
    weights = (int *)malloc(sizeof(int)*weights_len);
    if(weights == NULL){
        fprintf(stderr, "Error in allocating the weights vector\n\n");
        return -1;
    }
    // Now actually it goes over the various samples and it computes the prediction
    // residual for each of them; with that and the residual the original sample
    // can be reconstructed.
    for(z = 0; z < input_params.z_size; z++){
        for(y = 0; y < input_params.y_size; y++){
            for(x = 0; x < input_params.x_size; x++){
                int error = 0;
                int predicted_sample = 0;
                unsigned short int cur_sample = 0;
                predicted_sample = compute_predicted_sample(input_params, predictor_params,
                    x, y, z, s_min, s_mid, s_max, samples, weights);
                cur_sample = get_sample(MATRIX_BSQ_INDEX(residuals, input_params, x, y, z), predicted_sample, s_min, s_max);
                // printf("%c\n", cur_sample);
                MATRIX_BSQ_INDEX(samples, input_params, x, y, z) = cur_sample;
                if(x == 0 && y == 0){
                    //  weights initialization
                    init_weights(weights, predictor_params, z);
                }else{
                    // finally I can update the weights, preparing for the prediction of the next sample
                    error = 2*cur_sample - predicted_sample;
                    update_weights(weights, input_params, predictor_params, x, y, z, error, samples);
                }
            }
        }
    }

    // Now I simply have to save the samples to the output file and in the correct format (BSQ or BI)
    // remember that in the samples array they are saved in BSQ format
    if(write_samples(input_params, outputFile, samples, s_mid) != 0){
        fprintf(stderr, "Error in writing the uncompressed samples to the output file\n");
        return -1;
    }

    // Freeing allocated memory
    if(samples != NULL){
        free(samples);
    }
    if(weights != NULL){
        free(weights);
    }

    return 0;
}
