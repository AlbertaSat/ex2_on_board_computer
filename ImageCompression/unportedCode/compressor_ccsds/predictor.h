/*
Luca Fossati (Luca.Fossati@esa.int), European Space Agency

Software distributed under the "European Space Agency Public License – v2.0".

All Distribution of the Software and/or Modifications, as Source Code or Object Code,
must be, as a whole, under the terms of the European Space Agency Public License – v2.0.
If You Distribute the Software and/or Modifications as Object Code, You must:
(a)	provide in addition a copy of the Source Code of the Software and/or
Modifications to each recipient; or
(b)	make the Source Code of the Software and/or Modifications freely accessible by reasonable
means for anyone who possesses the Object Code or received the Software and/or Modifications
from You, and inform recipients how to obtain a copy of the Source Code.

The Software is provided to You on an “as is” basis and without warranties of any
kind, including without limitation merchantability, fitness for a particular purpose,
absence of defects or errors, accuracy or non-infringement of intellectual property
rights.
Except as expressly set forth in the "European Space Agency Public License – v2.0",
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

#ifndef PREDICTOR_H
#define PREDICTOR_H

#include "utils.h"

///Type representing the configuration of the predictor
typedef struct predictor_config{
    unsigned char user_input_pred_bands;
    unsigned char pred_bands;
    unsigned char full;
    unsigned char neighbour_sum;
    unsigned char register_size;
    unsigned char weight_resolution;
    int weight_interval;
    char weight_initial;
    char weight_final;
    unsigned char weight_init_resolution;
    int ** weight_init_table;
} predictor_config_t;

/// Computes the local sum for the given sample index
int local_sum(input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, unsigned short int * samples);

#ifdef NO_COMPUTE_LOCAL
/// Computes the local differences (central and directional) for the given sample
/// and puts them in the pre-allocated local_difference array
int get_central_difference(input_feature_t input_params, predictor_config_t predictor_params,
    int * central_difference, unsigned short int * samples, unsigned int x, unsigned int y, unsigned int z);
int get_directional_difference(input_feature_t input_params, predictor_config_t predictor_params,
    int directional_difference[3], unsigned short int * samples, unsigned int x, unsigned int y, unsigned int z);
#endif

#ifndef NO_COMPUTE_LOCAL
/// Computes the local differences for the whole image and saves them in the local_differences
/// array
int compute_local_differences(input_feature_t input_params, predictor_config_t predictor_params,
    int *** local_differences, unsigned short int * samples);
#endif

#ifndef NO_COMPUTE_LOCAL
/// Given the prediction error, it updates the weight matrix (local and
/// prediction weights per band).
void update_weights(int * weights, input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, int error, int ** local_differences, unsigned short * samples);
#else
/// Given the prediction error, it updates the weight matrix (local and
/// prediction weights per band).
void update_weights(int * weights, input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, int error, unsigned short * samples);
#endif

#ifndef NO_COMPUTE_LOCAL
/// given the local differences and the samples, it computes the scaled predicted
/// sample value
int compute_predicted_sample(input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, unsigned int s_min, unsigned int s_mid, unsigned int s_max,
    int ** local_differences, unsigned short int * samples, int * weights);
#else
/// given the local differences and the samples, it computes the scaled predicted
/// sample value
int compute_predicted_sample(input_feature_t input_params, predictor_config_t predictor_params,
    unsigned int x, unsigned int y, unsigned int z, unsigned int s_min, unsigned int s_mid, unsigned int s_max,
    unsigned short int * samples, int * weights);
#endif

void init_weights(int * weights, predictor_config_t predictor_params, unsigned int z);

/// High-level routine which actually performs the prediction, by calling the
/// in the right order the other sub-routines.
/// A value different from 0 is returned in case of error
int predict(input_feature_t input_params, predictor_config_t predictor_params, char inputFile[128], unsigned short int * residuals);


/// NOTE: we execute the computation in BSQ order, for simplicity; this means that conversion
/// from the input format into BSQ might be needed.

#endif
