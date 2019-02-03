/*
Luca Fossati (Luca.Fossati@esa.int), European Space Agency

Software distributed under the "European Space Agency Public License � v2.0".

All Distribution of the Software and/or Modifications, as Source Code or Object Code,
must be, as a whole, under the terms of the European Space Agency Public License � v2.0.
If You Distribute the Software and/or Modifications as Object Code, You must:
(a)	provide in addition a copy of the Source Code of the Software and/or
Modifications to each recipient; or
(b)	make the Source Code of the Software and/or Modifications freely accessible by reasonable
means for anyone who possesses the Object Code or received the Software and/or Modifications
from You, and inform recipients how to obtain a copy of the Source Code.

The Software is provided to You on an �as is� basis and without warranties of any
kind, including without limitation merchantability, fitness for a particular purpose,
absence of defects or errors, accuracy or non-infringement of intellectual property
rights.
Except as expressly set forth in the "European Space Agency Public License � v2.0",
neither Licensor nor any Contributor shall be liable, including, without limitation, for direct, indirect,
incidental, or consequential damages (including without limitation loss of profit),
however caused and on any theory of liability, arising in any way out of the use or
Distribution of the Software or the exercise of any rights under this License, even
if You have been advised of the possibility of such damages.

*****************************************************************************************
Predictor as described in Section 4 of the CCSDS 123.0-R-1,
White Book Draft Recommendation for Space Data System Standards on
LOSSLESS MULTISPECTRAL & HYPERSPECTRAL IMAGE COMPRESSION
as of 09/11/2011.
*/

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "utils.h"
#include "predictor.h"

/// Computes the local sum for the given sample index
int local_sum(input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, unsigned short int * samples){
    unsigned int sum = 0;

#ifndef NDEBUG
    if(x == 0 && y == 0){
        fprintf(stderr, "Error, called local_sum for band %d with x=0, y=0\n\n", z);
        return 0x80000000;
    }
#endif

    if(predictor_params.neighbour_sum != 0){
        if(y > 0){
            if(x > 0){
                sum += MATRIX_BSQ_INDEX(samples, input_params, x - 1, y, z) +
                    MATRIX_BSQ_INDEX(samples, input_params, x, y - 1, z) + MATRIX_BSQ_INDEX(samples, input_params, x - 1, y - 1, z);
                if(x < (input_params.x_size - 1))
                    sum += MATRIX_BSQ_INDEX(samples, input_params, x + 1, y - 1, z);
                else
                    sum += MATRIX_BSQ_INDEX(samples, input_params, x, y - 1, z);
            }else{
                sum += 2*MATRIX_BSQ_INDEX(samples, input_params, x, y - 1, z) + 2*MATRIX_BSQ_INDEX(samples, input_params, x + 1, y - 1, z);
            }
        }else{
            sum += 4*MATRIX_BSQ_INDEX(samples, input_params, x - 1, y, z);
        }
    }else{
        if(y > 0)
            sum = 4*MATRIX_BSQ_INDEX(samples, input_params, x, y - 1, z);
        else
            sum = 4*MATRIX_BSQ_INDEX(samples, input_params, x - 1, y, z);
    }

    return sum;
}

#ifdef NO_COMPUTE_LOCAL
int get_central_difference(input_feature_t input_params, predictor_config_t predictor_params,
    int * central_difference, unsigned short int * samples, unsigned int x, unsigned int y, unsigned int z){

    // Central difference computed: it is common both to reduced and
    // full prediction mode
    if(y == 0 && x == 0){
        *central_difference = 0;
    }else{
        int local_sum_temp = local_sum(input_params, predictor_params, x, y, z, samples);
#ifndef NDEBUG
        if(local_sum_temp == 0x80000000){
            return -1;
        }
#endif
        *central_difference = 4*MATRIX_BSQ_INDEX(samples, input_params, x, y, z) - local_sum_temp;
    }

    return 0;
}

int get_directional_difference(input_feature_t input_params, predictor_config_t predictor_params,
    int directional_difference[3], unsigned short int * samples, unsigned int x, unsigned int y, unsigned int z){
    int local_sum_temp = local_sum(input_params, predictor_params, x, y, z, samples);

#ifndef NDEBUG
    if(local_sum_temp == 0x80000000){
        return -1;
    }
    if(predictor_params.full == 0){
        fprintf(stderr, "Error: directional differences asked, but full prediction mode disabled.\n");
        return -1;
    }
#endif

    //full prediction mode, the differences in the directional_difference vector are
    //in this order: central, north, west, north-west
    if(y > 0){
        directional_difference[0] = 4*MATRIX_BSQ_INDEX(samples, input_params, x, y - 1, z) - local_sum_temp;
        printf("%d\n", MATRIX_BSQ_INDEX(samples, input_params, x, y - 1, z));
        if(x > 0){
            directional_difference[1] = 4*MATRIX_BSQ_INDEX(samples, input_params, x - 1, y, z) - local_sum_temp;
            directional_difference[2] = 4*MATRIX_BSQ_INDEX(samples, input_params, x - 1, y - 1, z) - local_sum_temp;
        }else{
            directional_difference[1] = directional_difference[0];
            directional_difference[2] = directional_difference[0];
        }
    }else{
        directional_difference[0] = 0;
        directional_difference[1] = 0;
        directional_difference[2] = 0;
    }

    return 0;
}

#endif

#ifndef NO_COMPUTE_LOCAL
/// Computes the local differences for the whole image and saves them in the local_differences
/// array; note that the image input to this procedure is assumed to be in BSQ format
int compute_local_differences(input_feature_t input_params, predictor_config_t predictor_params,
    int *** local_differences, unsigned short int * samples){
    unsigned int x = 0, y = 0, z = 0;

    // First I have to allocate the memory to hold the result
    if(predictor_params.full != 0)
        *local_differences = (int **)malloc(sizeof(int)*4);
    else
        *local_differences = (int **)malloc(sizeof(int));
    if(*local_differences == NULL){
        fprintf(stderr, "Error in allocating memory for building the local differences matrices\n");
        return -1;
    }
    if(((*local_differences)[0] = (int *)malloc(sizeof(int)*input_params.x_size*input_params.y_size*input_params.z_size)) == NULL){
        fprintf(stderr, "Error in allocating %d bytes for holding the local differences matrix\n", sizeof(int)*input_params.x_size*input_params.y_size*input_params.z_size);
        return -1;
    }
    if(predictor_params.full != 0){
        int i = 0;
        for(i = 1; i < 4; i++){
            if(((*local_differences)[i] = (int *)malloc(sizeof(int)*input_params.x_size*input_params.y_size*input_params.z_size)) == NULL){
                fprintf(stderr, "Error in allocating %d bytes for holding the local differences matrix %d\n", sizeof(int)*input_params.x_size*input_params.y_size*input_params.z_size, i);
                return -1;
            }
        }
    }

    // Ok, now I can actually start the computation
    // Central difference computed: it is common both to reduced and
    // full prediction mode
    for(z = 0; z < input_params.z_size; z++){
        for(y = 0, x = 1; y < input_params.y_size; y++, x = 0){
            for(; x < input_params.x_size; x++){
                int local_sum_temp = local_sum(input_params, predictor_params, x, y, z, samples);
#ifndef NDEBUG
                if(local_sum_temp == 0x80000000){
                    return -1;
                }
#endif
                MATRIX_BSQ_INDEX((*local_differences)[0], input_params, x, y, z) = 4*MATRIX_BSQ_INDEX(samples, input_params, x, y, z) - local_sum_temp;
            }
        }
    }
    if(predictor_params.full != 0){
        //full prediction mode, the differences in the local_differences vector are
        //in this order: central, north, west, north-west
        for(z = 0; z < input_params.z_size; z++){
            for(y = 0; y < input_params.y_size; y++){
                for(x = 0; x < input_params.x_size; x++){
                    if(y > 0){
                        int local_sum_temp = local_sum(input_params, predictor_params, x, y, z, samples);
#ifndef NDEBUG
                        if(local_sum_temp == 0x80000000){
                            return -1;
                        }
#endif
                        MATRIX_BSQ_INDEX((*local_differences)[1], input_params, x, y, z) = 4*MATRIX_BSQ_INDEX(samples, input_params, x, y - 1, z) - local_sum_temp;
                        if(x > 0){
                            MATRIX_BSQ_INDEX((*local_differences)[2], input_params, x, y, z) = 4*MATRIX_BSQ_INDEX(samples, input_params, x - 1, y, z) - local_sum_temp;
                            MATRIX_BSQ_INDEX((*local_differences)[3], input_params, x, y, z) = 4*MATRIX_BSQ_INDEX(samples, input_params, x - 1, y - 1, z) - local_sum_temp;
                        }else{
                            MATRIX_BSQ_INDEX((*local_differences)[2], input_params, 0, y, z) = MATRIX_BSQ_INDEX((*local_differences)[1], input_params, 0, y, z);
                            MATRIX_BSQ_INDEX((*local_differences)[3], input_params, 0, y, z) = MATRIX_BSQ_INDEX((*local_differences)[1], input_params, 0, y, z);
                        }
                    }else{
                        MATRIX_BSQ_INDEX((*local_differences)[1], input_params, x, 0, z) = 0;
                        MATRIX_BSQ_INDEX((*local_differences)[2], input_params, x, 0, z) = 0;
                        MATRIX_BSQ_INDEX((*local_differences)[3], input_params, x, 0, z) = 0;
                    }
                }
            }
        }
    }
    return 0;
}
#endif

/// Given the prediction error, it updates the weight matrix (local and
/// prediction weights per band).
#ifndef NO_COMPUTE_LOCAL
void update_weights(int *weights, input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, int error, int ** local_differences, unsigned short int * samples){
#else
void update_weights(int *weights, input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, int error, unsigned short int * samples){
#endif
        int i = 0;
    int weight_limit = 0x1 << (predictor_params.weight_resolution + 2);
    int sign_error = error < 0 ? -1 : 1;
    int scaling_exp = predictor_params.weight_initial + ((int)(y*input_params.x_size + x - input_params.x_size))/predictor_params.weight_interval;
    if(scaling_exp < predictor_params.weight_initial)
        scaling_exp = predictor_params.weight_initial;
    if(scaling_exp > predictor_params.weight_final)
        scaling_exp = predictor_params.weight_final;
    scaling_exp += input_params.dyn_range - predictor_params.weight_resolution;

    // Now I can update the weights; they are saved in the weights matrix such as
    // the first index of the matrix is the spectral band and the other elements are
    // the various weights: pred_1, pred_2, pred_p, N, W, NW.
    if(z > 0){
        int cur_pred_bands = z < predictor_params.pred_bands ? z : predictor_params.pred_bands;
        for(i = 0; i < cur_pred_bands; i++){
#ifdef NO_COMPUTE_LOCAL
            int central_difference = 0;
            if(get_central_difference(input_params, predictor_params, &central_difference, samples, x, y, z - i - 1) < 0){
                fprintf(stderr, "Error in getting the central differences for band %d", z -i);
            }
#endif

            if(scaling_exp > 0){
#ifndef NO_COMPUTE_LOCAL
                weights[i] = weights[i] + ((((sign_error*(MATRIX_BSQ_INDEX(local_differences[0], input_params, x, y, z - i - 1))) >> scaling_exp) + 1) >> 1);
#else
                weights[i] = weights[i] + ((((sign_error*central_difference) >> scaling_exp) + 1) >> 1);
#endif
            }else{
#ifndef NO_COMPUTE_LOCAL
                weights[i] = weights[i] + ((((sign_error*(MATRIX_BSQ_INDEX(local_differences[0], input_params, x, y, z - i - 1))) << -1*scaling_exp) + 1) >> 1);
#else
                weights[i] = weights[i] + ((((sign_error*central_difference) << -1*scaling_exp) + 1) >> 1);
#endif
            }
            if(weights[i] < (-1*weight_limit)){
                weights[i] = -1*weight_limit;
            }
            if(weights[i] > (weight_limit - 1)){
                weights[i] = weight_limit - 1;
            }
        }
    }
    if(predictor_params.full != 0){
#ifdef NO_COMPUTE_LOCAL
        int directional_difference[3];
        if(get_directional_difference(input_params, predictor_params, directional_difference, samples, x, y, z) < 0){
            fprintf(stderr, "Error in getting the directional differences");
        }
#endif

        for(i = 0; i < 3; i++){
            if(scaling_exp > 0){
#ifndef NO_COMPUTE_LOCAL
                weights[predictor_params.pred_bands + i] = weights[predictor_params.pred_bands + i] + ((((sign_error*(MATRIX_BSQ_INDEX(local_differences[i + 1], input_params, x, y, z))) >> scaling_exp) + 1) >> 1);
#else
                weights[predictor_params.pred_bands + i] = weights[predictor_params.pred_bands + i] + ((((sign_error*directional_difference[i]) >> scaling_exp) + 1) >> 1);
#endif
            }else{
#ifndef NO_COMPUTE_LOCAL
                weights[predictor_params.pred_bands + i] = weights[predictor_params.pred_bands + i] + ((((sign_error*(MATRIX_BSQ_INDEX(local_differences[i + 1], input_params, x, y, z))) << -1*scaling_exp) + 1) >> 1);
#else
                weights[predictor_params.pred_bands + i] = weights[predictor_params.pred_bands + i] + ((((sign_error*directional_difference[i]) << -1*scaling_exp) + 1) >> 1);
#endif
            }
            if(weights[predictor_params.pred_bands + i] < (-1*weight_limit)){
                weights[predictor_params.pred_bands + i] = -1*weight_limit;
            }
            if(weights[predictor_params.pred_bands + i] > (weight_limit - 1)){
                weights[predictor_params.pred_bands + i] = weight_limit - 1;
            }
        }
    }
}

#ifndef NO_COMPUTE_LOCAL
/// given the local differences and the samples, it computes the scaled predicted
/// sample value
int compute_predicted_sample(input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, unsigned int s_min, unsigned int s_mid, unsigned int s_max,
    int ** local_differences, unsigned short int * samples, int * weights){
#else
/// given the local differences and the samples, it computes the scaled predicted
/// sample value
int compute_predicted_sample(input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, unsigned int s_min, unsigned int s_mid, unsigned int s_max,
    unsigned short int * samples, int * weights){
#endif
    long long scaled_predicted = 0;
    long long diff_predicted = 0;
    int i = 0;

//     fprintf(stderr, "compute_predicted_sample=%d, %d, %d - pred_bands = %d\n", x, y, z, predictor_params.pred_bands);

    if(x > 0 || y > 0){
        long long local_sum_temp = 0;

        // predicted local difference
        if(z > 0){
            int cur_pred_bands = z < predictor_params.pred_bands ? z : predictor_params.pred_bands;
            for(i = 0; i < cur_pred_bands; i++){
#ifdef NO_COMPUTE_LOCAL
                int central_difference = 0;
                if(get_central_difference(input_params, predictor_params, &central_difference, samples, x, y, z - i - 1) < 0){
                    fprintf(stderr, "Error in getting the central differences for band %d", z -i);
                }
//                 fprintf(stderr, "central_difference=%d\n", central_difference);
#endif

#ifndef NO_COMPUTE_LOCAL
                diff_predicted += ((long long)weights[i])*(long long)(MATRIX_BSQ_INDEX(local_differences[0], input_params, x, y, z - i - 1));
#else
                diff_predicted += ((long long)weights[i])*(long long)central_difference;
#endif
            }
        }
        if(predictor_params.full != 0){
#ifdef NO_COMPUTE_LOCAL
            int directional_difference[3];
            if(get_directional_difference(input_params, predictor_params, directional_difference, samples, x, y, z) < 0){
                fprintf(stderr, "Error in getting the directional differences");
            }
//             fprintf(stderr, "directional_difference[0]=%d, directional_difference[1]=%d, directional_difference[2]=%d\n", directional_difference[0], directional_difference[1], directional_difference[2]);
#endif

            for(i = 0; i < 3; i++){
#ifndef NO_COMPUTE_LOCAL
                diff_predicted += ((long long)weights[predictor_params.pred_bands + i])*(long long)(MATRIX_BSQ_INDEX(local_differences[i + 1], input_params, x, y, z));
#else
                diff_predicted += ((long long)weights[predictor_params.pred_bands + i])*(long long)directional_difference[i];
#endif
            }
        }
        // scaled predicted sample
        local_sum_temp = local_sum(input_params, predictor_params, x, y, z, samples);
        scaled_predicted = mod_star(diff_predicted + ((local_sum_temp - 4*s_mid) << predictor_params.weight_resolution), predictor_params.register_size, x == 1 && y == 0 && z == 0);
        scaled_predicted = scaled_predicted >> (predictor_params.weight_resolution + 1);
        scaled_predicted = scaled_predicted + 1 + 2*s_mid;
        if(scaled_predicted < 2*s_min)
            scaled_predicted = 2*s_min;
        if(scaled_predicted > (2*s_max + 1))
            scaled_predicted = (2*s_max + 1);
        // printf("%d\n", scaled_predicted);
    }else{
        if(z == 0 || predictor_params.pred_bands == 0){
//             fprintf(stderr, "smid\n");
            scaled_predicted = 2*s_mid;
        }else{
            // fprintf(stderr, "sample\n");
            scaled_predicted = 2*(MATRIX_BSQ_INDEX(samples, input_params, 0, 0, z - 1));
        }
    }
    // printf("%d\n", (int)scaled_predicted/2);
    return (int)scaled_predicted;
}

/// Given the scaled predicted sample value it maps it to an unsigned value
/// enabling it to be represented with D bits
unsigned short int compute_mapped_residual(input_feature_t input_params,
    unsigned int x, unsigned int y, unsigned int z, unsigned int s_min, unsigned int s_mid, unsigned int s_max,
    unsigned short int * samples, int scaled_predicted){
    unsigned short int mapped = 0;
    int delta = ((int)MATRIX_BSQ_INDEX(samples, input_params, x, y, z)) - scaled_predicted/2;
    unsigned int omega = scaled_predicted/2 - s_min;
    unsigned int abs_delta = delta < 0 ? (-1*delta) : delta;
    int sign_scaled = (scaled_predicted & 0x1) != 0 ? -1 : 1;

    if(omega > s_max - scaled_predicted/2){
        omega = s_max - scaled_predicted/2;
    }

    if(abs_delta > omega){
        mapped = abs_delta + omega;
    }else if((sign_scaled*delta) <= omega && (sign_scaled*delta) >= 0){
        mapped = 2*abs_delta;
    }else{
        mapped = 2*abs_delta - 1;
    }

    return mapped;
}

void init_weights(int * weights, predictor_config_t predictor_params, unsigned int z){
    int i = 0;

    if(predictor_params.weight_init_table == NULL){
        // default weights initialization
        if(predictor_params.pred_bands > 0){
            weights[0] = 7 << (predictor_params.weight_resolution - 3);
            for(i = 1; i < predictor_params.pred_bands; i++){
                weights[i] = weights[i - 1] >> 3;
            }
        }
        if(predictor_params.full != 0){
            for(i = 0; i < 3; i++){
                weights[predictor_params.pred_bands + i] = 0;
            }
        }
    }else{
        // custom weights initialization
        if(predictor_params.full != 0){
            for(i = 0; i < 3; i++){
                weights[predictor_params.pred_bands + i] = predictor_params.weight_init_table[z][i] << (predictor_params.weight_resolution + 3 - predictor_params.weight_init_resolution);
                weights[predictor_params.pred_bands + i] += 0x1 << (predictor_params.weight_resolution + 2 - predictor_params.weight_init_resolution);
                weights[predictor_params.pred_bands + i] -= 1;
            }
            for(i = 3; i < predictor_params.pred_bands + 3; i++){
                weights[i - 3] = predictor_params.weight_init_table[z][i] << (predictor_params.weight_resolution + 3 - predictor_params.weight_init_resolution);
                weights[i - 3] += 0x1 << (predictor_params.weight_resolution + 2 - predictor_params.weight_init_resolution);
                weights[i - 3] -= 1;
            }
        }else{
            for(i = 0; i < predictor_params.pred_bands; i++){
                weights[i] = predictor_params.weight_init_table[z][i] << (predictor_params.weight_resolution + 3 - predictor_params.weight_init_resolution);
                weights[i] += 0x1 << (predictor_params.weight_resolution + 2 - predictor_params.weight_init_resolution);
                weights[i] -= 1;
            }
        }
    }
}

/// High-level routine which actually performs the prediction, by calling the
/// in the right order the other sub-routines.
/// A value different from 0 is returned in case of error
int predict(input_feature_t input_params, predictor_config_t predictor_params, char inputFile[128], unsigned short int * residuals){
    // Calls the various routines to parse the input file and
    // to compute the mapped residuals. The steps are:
    // - parse input file (with signed/unsigned conversion and converting to BSQ)
    // - create local differences matrix
    // - now, for each pixel in the image, I compute_predicted_sample,
    //   update the weights and compute the mapped residual which is added to the residuals matrix
    unsigned int s_min = 0;
    unsigned int s_max = (0x1 << input_params.dyn_range) - 1;
    unsigned int s_mid = 0x1 << (input_params.dyn_range - 1);
    unsigned short int * samples = NULL;
#ifndef NO_COMPUTE_LOCAL
    int ** local_differences = NULL;
#endif
    unsigned int x = 0, y = 0, z = 0;
    int * weights = NULL;
    int weights_len = predictor_params.pred_bands + (predictor_params.full != 0 ? 3 : 0);

    // Parse the input image, loading it into memory and appropriately converting it
    samples = (unsigned short int *)malloc(sizeof(unsigned short int)*input_params.x_size*input_params.y_size*input_params.z_size);
    if(samples == NULL){
        fprintf(stderr, "Error in allocating %lf kBytes for the input image buffer\n\n", ((double)sizeof(unsigned short int)*input_params.x_size*input_params.y_size*input_params.z_size)/1024.0);
        return -1;
    }
    if(read_samples(input_params, inputFile, samples) != 0){
        return -1;
    }

#ifndef NO_COMPUTE_LOCAL
    // Computes the local differences to be used in the prediction process; local_differences is a matrix
    // as if contains the local differences (central, north, west, north-west) for every sample in the image
    // (with samples expressed with a linear array)
    if(compute_local_differences(input_params, predictor_params, &local_differences, samples) < 0){
        return -1;
    }
#endif

    weights = (int *)malloc(sizeof(int)*weights_len);
    if(weights == NULL){
        fprintf(stderr, "Error in allocating the weights vector\n\n");
        return -1;
    }
    // Now actually it goes over the various samples and it computes the prediction
    // residual for each of them
    // Note that, for each band, the element in position (0, 0) is not predicted
    for(z = 0; z < input_params.z_size; z++){
        for(y = 0; y < input_params.y_size; y++){
            for(x = 0; x < input_params.x_size; x++){
                int predicted_sample = 0;
                unsigned short int mapped_residual = 0;
                int error = 0;

                // prediction of the current sample and saving the residual
#ifndef NO_COMPUTE_LOCAL
                predicted_sample = compute_predicted_sample(input_params, predictor_params,
                    x, y, z, s_min, s_mid, s_max, local_differences, samples, weights);
#else
                predicted_sample = compute_predicted_sample(input_params, predictor_params,
                    x, y, z, s_min, s_mid, s_max, samples, weights);
#endif
//                 fprintf(stderr, "(%d, %d, %d) = predicted_sample=%d\n", x, y, z, predicted_sample);
//                 fprintf(stderr, "sample=%d\n", MATRIX_BSQ_INDEX(samples, input_params, x, y, z));
                mapped_residual = compute_mapped_residual(input_params, x, y, z,
                    s_min, s_mid, s_max, samples, predicted_sample);
                MATRIX_BSQ_INDEX(residuals, input_params, x, y, z) = mapped_residual;
                if(x == 0 && y == 0){
                    //  weights initialization
                    init_weights(weights, predictor_params, z);
                }else{
                    // finally I can update the weights, preparing for the prediction of the next sample
                    error = 2*(MATRIX_BSQ_INDEX(samples, input_params, x, y, z)) - predicted_sample;
#ifndef NO_COMPUTE_LOCAL
                    update_weights(weights, input_params, predictor_params, x, y, z, error, local_differences, samples);
#else
                    update_weights(weights, input_params, predictor_params, x, y, z, error, samples);
#endif
                }
            }
        }
    }
    // Freeing allocated memory
    if(samples != NULL){
        free(samples);
    }
    if(weights != NULL){
        free(weights);
    }

#ifndef NO_COMPUTE_LOCAL
    if(local_differences != NULL){
        if(predictor_params.full != 0){
            int i = 0;
            for(i = 0; i < 4; i++){
                if(local_differences[i] != NULL){
                    free(local_differences[i]);
                }
            }
        }else{
            if(local_differences[0] != NULL){
                free(local_differences[0]);
            }
        }
        free(local_differences);
    }
#endif

    return 0;
}
