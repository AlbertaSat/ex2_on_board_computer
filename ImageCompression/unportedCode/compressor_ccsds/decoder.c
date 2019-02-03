
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
#include <string.h>
#include <math.h>

#include "utils.h"
#include "decoder.h"

/******************************************************
* Routines for the Sample Adaptive Encoder
*******************************************************/
/// Reads a compressed sample when compressed using the sample adaptive encoding method.
int read_element_sample(FILE * compressedStream, encoder_config_t encoder_params, input_feature_t input_params,
                        unsigned int temp_k, unsigned char *buffer, unsigned int *buffer_len){
    int sample = 0;
    unsigned int divisor = read_fs(compressedStream, encoder_params.u_max, buffer, buffer_len);
#ifndef NDEBUG
    if(divisor == (unsigned int)-1){
        fprintf(stderr, "Error in reading the FS sample\n");
        return -1;
    }
#endif
    if(divisor > encoder_params.u_max){
        // the element is saved uncompressed
        sample = read_bits(compressedStream, input_params.dyn_range, buffer, buffer_len);
#ifndef NDEBUG
        if(sample == -1){
            fprintf(stderr, "Error in reading uncompressed sample, asked %d bits\n", input_params.dyn_range);
            return -1;
        }
#endif
    }else{
        // standard compression method
        unsigned int temp_bits = read_bits(compressedStream, temp_k, buffer, buffer_len);
#ifndef NDEBUG
        if(temp_bits == (unsigned int)-1){
            fprintf(stderr, "Error in reading sample\n");
            return -1;
        }
#endif
        sample = (divisor << temp_k) | temp_bits;
    }
    return sample;
}

/// Main routine for decoding the input stream compressed according to the sample adaptive
/// method: it iterates over the various compressed samples, calling read_element_sample to extract
/// each of them from the compressed stream
int decode_sample_adaptive(FILE * compressedStream, input_feature_t input_params, encoder_config_t encoder_params,
                           unsigned short int * residuals){
    unsigned int read_elems = 0;
    unsigned char buffer = 0;
    unsigned int buffer_len = 0;
    unsigned int * counter = NULL;
    unsigned int * accumulator = NULL;
    const unsigned int samplesNum = input_params.x_size*input_params.y_size*input_params.z_size;
    const unsigned int band_size = input_params.x_size*input_params.y_size;
    unsigned int i = 0;

    counter = (unsigned int *)malloc(sizeof(unsigned int)*input_params.z_size);
    if(counter == NULL){
        fprintf(stderr, "Error in the allocation of the counter statistic\n\n");
        return -1;
    }
    accumulator = (unsigned int *)malloc(sizeof(unsigned int)*input_params.z_size);
    if(accumulator == NULL){
        fprintf(stderr, "Error in the allocation of the accumulator statistic\n\n");
        return -1;
    }
    for(i = 0; i < input_params.z_size; i++){
        counter[i] = 0x1 << encoder_params.y_0;
        accumulator[i] = (counter[i]*(3*(0x1 << (encoder_params.k_init[i] + 6))-49))/0x080;
    }

    // Let's read until the end of the file
    while((read_elems < samplesNum) && feof(compressedStream) == 0){
        unsigned int temp_sample = 0;
        unsigned int BSQidx = indexToBSQ(encoder_params.out_interleaving, encoder_params.out_interleaving_depth,
                    input_params.x_size, input_params.y_size, input_params.z_size, read_elems);
        if((BSQidx % (band_size)) == 0){
            // uncompressed element
            temp_sample = read_bits(compressedStream, input_params.dyn_range, &buffer, &buffer_len);
        }else{
            unsigned int z = BSQidx/band_size;
            int temp_k = 0;
            // normal element
            temp_k = (int)log2(((49*counter[z])/0x080 + accumulator[z])/((double)counter[z]));
            if(temp_k < 0)
                temp_k = 0;
            if(temp_k > (input_params.dyn_range - 2))
                temp_k = input_params.dyn_range - 2;

            temp_sample = read_element_sample(compressedStream, encoder_params, input_params, temp_k, &buffer, &buffer_len);

            // ... and finally update the statistics and prepare for the next sample
            if(counter[z] < ((((unsigned int)0x1) << encoder_params.y_star) -1)){
                accumulator[z] += temp_sample;
                counter[z]++;
            }else{
                accumulator[z] = (accumulator[z] + temp_sample + 1)/2;
                counter[z] = (counter[z] + 1)/2;
            }
        }
#ifndef NDEBUG
        if(temp_sample == (unsigned int)-1){
            fprintf(stderr, "Error in reading sample with BSQidx = %d, element %d\n", BSQidx, read_elems);
            return -1;
        }
#endif
        residuals[BSQidx] = (unsigned short int)temp_sample;

        read_elems++;
    }
    
#ifndef NDEBUG
        if(read_elems < samplesNum){
            fprintf(stderr, "Error read only %d samples out of %d\n", read_elems, samplesNum);
            return -1;
        }
#endif
    
    return 0;
}

/******************************************************
* Routines for the Block Adaptive Encoder
*******************************************************/
/// Reads a compressed block when using the block adaptive encoding method.
int read_nocomp_block(input_feature_t input_params, encoder_config_t encoder_params, FILE * compressedStream,
                      unsigned short int * residuals, unsigned int * read_elems, unsigned char *buffer,
                      unsigned int *buffer_len, unsigned int block_size){
    // no compression applied
    unsigned int i = 0;
    for(i = 0; i < block_size; i++){
        residuals[indexToBSQ(encoder_params.out_interleaving, encoder_params.out_interleaving_depth,
                        input_params.x_size, input_params.y_size, input_params.z_size, *read_elems + i)] =
                        read_bits(compressedStream, input_params.dyn_range, buffer, buffer_len);
    }
    *read_elems += block_size;
    return 0;
}

void decorrelate(unsigned int value, unsigned int *a, unsigned int *b){
    *a = 0;
    *b = 0;
    while(value > *a + *b){
        *b = *b + 1;
        *a += *b;
    }
}

int read_second_block(input_feature_t input_params, encoder_config_t encoder_params, FILE * compressedStream,
                      unsigned short int * residuals, unsigned int * read_elems, unsigned char *buffer,
                      unsigned int *buffer_len, unsigned int block_size){
    unsigned int second_extension_values[32];
    unsigned int i = 0;
    // First of all I read the values from file
    for(i = 0; i < encoder_params.block_size/2; i++){
        second_extension_values[i] = read_fs(compressedStream, -1, buffer, buffer_len);
    }
    // now I convert them back
    for(i = 0; i < block_size; i+=2){
        unsigned int a = 0, b = 0;
        unsigned int cur_value = second_extension_values[i/2];
        decorrelate(cur_value, &a, &b);
        residuals[indexToBSQ(encoder_params.out_interleaving, encoder_params.out_interleaving_depth,
                        input_params.x_size, input_params.y_size, input_params.z_size, *read_elems + i)] = b + a - cur_value;
        residuals[indexToBSQ(encoder_params.out_interleaving, encoder_params.out_interleaving_depth,
                        input_params.x_size, input_params.y_size, input_params.z_size, *read_elems + i + 1)] = cur_value - a;
    }
    *read_elems += block_size;

    return 0;
}

int read_ksplit_block(input_feature_t input_params, encoder_config_t encoder_params, FILE * compressedStream,
                      unsigned int k, unsigned short int * residuals, unsigned int * read_elems,
                      unsigned char *buffer, unsigned int *buffer_len, unsigned int block_size){
    // The various elements are simply saved with the FS code of the
    // result of the division and, then, the k-bits of the reminder
    // All results are first, then all reminders
    unsigned int division_result[64];
    unsigned int reminders[64];
    unsigned int i = 0;
    for(i = 0; i < encoder_params.block_size; i++){
        division_result[i] = read_fs(compressedStream, -1, buffer, buffer_len);
    }
    for(i = 0; i < encoder_params.block_size; i++){
        reminders[i] = read_bits(compressedStream, k, buffer, buffer_len);
    }
    for(i = 0; i < block_size; i++){
        residuals[indexToBSQ(encoder_params.out_interleaving, encoder_params.out_interleaving_depth,
                        input_params.x_size, input_params.y_size, input_params.z_size, *read_elems + i)] =
                        (division_result[i] << k) | reminders[i];
    }
    *read_elems += block_size;
    return 0;
}
int read_zero_block(input_feature_t input_params, encoder_config_t encoder_params, FILE * compressedStream,
                    unsigned short int * residuals, unsigned int * read_elems, unsigned char *buffer, unsigned int *buffer_len){
    // While for the other options I always decode one block at a time, here
    // I might need to decode more than one block
//     unsigned int i = 0;
    unsigned int num_blocks = read_fs(compressedStream, -1, buffer, buffer_len);
    if(num_blocks < 4){
        num_blocks++;
    }else if(num_blocks == 4){
        unsigned int num_blocks_reference = encoder_params.ref_interval - ((*read_elems/encoder_params.block_size) % encoder_params.ref_interval);
        // ROS, all zero blocks until the end of the segment or until the
        // end of the reference samples, if we are in the last segment
        num_blocks = SEGMENT_SIZE - ((*read_elems/encoder_params.block_size) % SEGMENT_SIZE);
        if(num_blocks_reference < num_blocks){
            num_blocks = num_blocks_reference;
        }
    }
    // NOTE: as the whole residuals array has been initialized to 0, there is no need to write
    // zero blocks.
//     for(i = 0; i < encoder_params.block_size*num_blocks; i++){
//         residuals[indexToBSQ(encoder_params.out_interleaving, encoder_params.out_interleaving_depth,
//                         input_params.x_size, input_params.y_size, input_params.z_size, *read_elems + i)] = 0;
//     }
    *read_elems += encoder_params.block_size*num_blocks;
    return 0;
}

/// Main routine for decoding the input stream compressed according to the block adaptive
/// method: it determines the compression method for the block and the calls the appropriate
/// routine for its decoding.
int decode_block_adaptive(FILE * compressedStream, input_feature_t input_params,
                          encoder_config_t encoder_params, unsigned short int * residuals){
    unsigned int compression_id = 0;
    unsigned int mask = 0;
    unsigned char buffer = 0;
    unsigned int buffer_len = 0;
    unsigned int read_elems = 0;
    const unsigned int samplesNum = input_params.x_size*input_params.y_size*input_params.z_size;

    while((read_elems < samplesNum) && feof(compressedStream) == 0){
        unsigned int cur_block_size = MIN(encoder_params.block_size, samplesNum - read_elems);
        if(input_params.dyn_range <= 4 && encoder_params.restricted != 0){
            if(input_params.dyn_range < 3){
                compression_id = read_bits(compressedStream, 1, &buffer, &buffer_len);
                mask = 0x1;
            }else{
                compression_id = read_bits(compressedStream, 2, &buffer, &buffer_len);
                mask = 0x3;
            }
        }else{
            if(input_params.dyn_range <= 8){
                compression_id = read_bits(compressedStream, 3, &buffer, &buffer_len);
                mask = 0x7;
            }else if(input_params.dyn_range <= 16){
                compression_id = read_bits(compressedStream, 4, &buffer, &buffer_len);
                mask = 0xF;
            }else{
                compression_id = read_bits(compressedStream, 5, &buffer, &buffer_len);
                mask = 0x1F;
            }
        }
        if(compression_id == 0){
            // zero compression or second extension option
            if(read_bits(compressedStream, 1, &buffer, &buffer_len) == 0){
                // zero compression
                if(read_zero_block(input_params, encoder_params, compressedStream, residuals,
                    &read_elems, &buffer, &buffer_len) != 0){
                    fprintf(stderr, "Error in reading the zero block\n");
                    return -1;
                }
            }else{
                //second extension
                if(read_second_block(input_params, encoder_params, compressedStream, residuals, &read_elems,
                    &buffer, &buffer_len, cur_block_size) != 0){
                    fprintf(stderr, "Error in reading the second block\n");
                    return -1;
                }
            }
        }else if(compression_id == 1){
            // FS or no compression (when restricted mode used)
            if(mask == 1){
                //no compression 
                if(read_nocomp_block(input_params, encoder_params, compressedStream, residuals, &read_elems, &buffer,
                    &buffer_len, cur_block_size) != 0){
                    fprintf(stderr, "Error in reading the no compression\n");
                    return -1;
                }
            }else{
                // FS (i.e. k-split with with K=0)
                if(read_ksplit_block(input_params, encoder_params, compressedStream, 0, residuals, &read_elems,
                    &buffer, &buffer_len, cur_block_size) != 0){
                    fprintf(stderr, "Error in reading the ksplit block with k = 0\n");
                    return -1;
                }
            }
        }else if(compression_id == mask){
            // no compression
            if(read_nocomp_block(input_params, encoder_params, compressedStream, residuals, &read_elems,
                &buffer, &buffer_len, cur_block_size) != 0){
                fprintf(stderr, "Error in reading the no compression\n");
                return -1;
            }
        }else{
            // k-split with k = (compression_id - 1)
            if(read_ksplit_block(input_params, encoder_params, compressedStream, compression_id - 1,
                residuals, &read_elems, &buffer, &buffer_len, cur_block_size) != 0){
                fprintf(stderr, "Error in reading the ksplit block with k = %d\n", compression_id - 1);
                return -1;
            }
        }
    }
#ifndef NDEBUG
    if(read_elems < samplesNum){
        fprintf(stderr, "Error read only %d samples out of %d\n", read_elems, samplesNum);
        return -1;
    }
#endif

    return 0;
}

/// Reads the compressed file header, filling-in the appropriate data structures
int read_header(FILE * compressedStream, input_feature_t *input_params, encoder_config_t *encoder_params,
                predictor_config_t *predictor_params){
    unsigned char buffer = 0;
    unsigned char temp = 0;
    unsigned int temp_len = 0;
    /* IMAGE METADATA */
    // User defined data
    fseek(compressedStream, 1, SEEK_CUR);
    // x, y, z dimensions
    fread(&input_params->x_size, 2, 1, compressedStream);
    fread(&input_params->y_size, 2, 1, compressedStream);
    fread(&input_params->z_size, 2, 1, compressedStream);
    if(is_little_endian() != 0){
        unsigned char endian_temp = (unsigned short int)(input_params->x_size & 0xFF00) >> 8;
        input_params->x_size = ((input_params->x_size & 0xFF) << 8) | endian_temp;
        endian_temp = (input_params->y_size & 0xFF00) >> 8;
        input_params->y_size = ((input_params->y_size & 0xFF) << 8) | endian_temp;
        endian_temp = (input_params->z_size & 0xFF00) >> 8;
        input_params->z_size = ((input_params->z_size & 0xFF) << 8) | endian_temp;
    }
    // Sample type
    fread(&buffer, 1, 1, compressedStream);
    if((buffer & 0x80) != 0)
        input_params->signed_samples = 1;
    else
        input_params->signed_samples = 0;
    // dynamic range
    input_params->dyn_range = (buffer >> 1) & 0xF;
    if(input_params->dyn_range == 0)
        input_params->dyn_range = 0x10;
    // Encoding Sample Order and interleaving
    if((buffer & 0x1) != 0){
        encoder_params->out_interleaving = BSQ;
        fseek(compressedStream, 2, SEEK_CUR);
    }
    else{
        encoder_params->out_interleaving = BI;
        fread(&encoder_params->out_interleaving_depth, 2, 1, compressedStream);
        if(is_little_endian() != 0){
            unsigned char endian_temp = (unsigned short int)(encoder_params->out_interleaving_depth & 0xFF00) >> 8;
            encoder_params->out_interleaving_depth = ((encoder_params->out_interleaving_depth & 0xFF) << 8) | endian_temp;
        }
    }
    fread(&buffer, 1, 1, compressedStream);
    // Out word size
    encoder_params->out_wordsize = (buffer >> 3) & 0x7;
    if(encoder_params->out_wordsize == 0)
        encoder_params->out_wordsize = 8;
    // Encoder type
    if(((buffer >> 2) & 0x1) == 0)
        encoder_params->encoding_method = SAMPLE;
    else
        encoder_params->encoding_method = BLOCK;
    // reserved
    fseek(compressedStream, 1, SEEK_CUR);

    /* PREDICTOR METADATA */
    fread(&buffer, 1, 1, compressedStream);
    // prediction bands
    predictor_params->user_input_pred_bands = (buffer >> 2) & 0xF;
    if(predictor_params->user_input_pred_bands > input_params->z_size)
        predictor_params->pred_bands = input_params->z_size;
    else
        predictor_params->pred_bands = predictor_params->user_input_pred_bands;
    // prediction mode
    if((buffer & 0x2) != 0)
        predictor_params->full = 0;
    else
        predictor_params->full = 1;
    fread(&buffer, 1, 1, compressedStream);
    // local sum
    if((buffer & 0x80) == 0)
        predictor_params->neighbour_sum = 1;
    else
        predictor_params->neighbour_sum = 0;
    // Register size
    predictor_params->register_size = buffer & 0x3F;
    if(predictor_params->register_size == 0)
        predictor_params->register_size = 0x40;
    fread(&buffer, 1, 1, compressedStream);
    // Weight resolution
    predictor_params->weight_resolution = (buffer >> 4) + 4;
    // weight update scaling exponent change interval
    predictor_params->weight_interval = 0x1 << ((buffer & 0xF) + 4);
    fread(&buffer, 1, 1, compressedStream);
    // weight update scaling exponent initial parameter
    predictor_params->weight_initial = (buffer >> 4) - 6;
    // weight update scaling exponent final parameter
    predictor_params->weight_final = ((buffer & 0xF) - 6);
    // weight initialization method and weight initialization table flag
    fread(&buffer, 1, 1, compressedStream);
    if((buffer & 0x40) != 0){
        predictor_params->weight_init_resolution = buffer & 0x1F;
        if((buffer & 0x20) != 0){
            unsigned int z = 0, cz = 0;
            unsigned int i = 0;
            int prediction_len = predictor_params->pred_bands;
            if(predictor_params->full != 0)
                prediction_len += 3;
            if((predictor_params->weight_init_table = (int **)malloc(sizeof(int *)*input_params->z_size)) == NULL){
                fprintf(stderr, "\nError, in allocating the weight initialization table - 1\n\n");
                return -1;
            }
            for(i = 0; i < input_params->z_size; i++){
                if((predictor_params->weight_init_table[i] = (int *)malloc(sizeof(int)*prediction_len)) == NULL){
                    fprintf(stderr, "\nError, in allocating the weight initialization table - 2\n\n");
                    return -1;
                }
            }
            temp_len = 0;
            for(z = 0; z < input_params->z_size; z++){
                if(predictor_params->full != 0){
                    for(cz = 0; cz < MIN(predictor_params->pred_bands + 3, z + 3); cz++){
                        predictor_params->weight_init_table[z][cz] = sign_extend(read_bits(compressedStream,
                                                                              predictor_params->weight_init_resolution, &temp,
                                                                              &temp_len), predictor_params->weight_init_resolution);
                    }
                }else{
                    for(cz = 0; cz < MIN(predictor_params->pred_bands, z); cz++){
                        predictor_params->weight_init_table[z][cz] = sign_extend(read_bits(compressedStream,
                                                                              predictor_params->weight_init_resolution, &temp,
                                                                              &temp_len), predictor_params->weight_init_resolution);
                    }
                }
            }
        }
    }
    

    /* ENTROPY CODER METADATA */
    if(encoder_params->encoding_method == SAMPLE){
        if((encoder_params->k_init = (unsigned int *)malloc(input_params->z_size*sizeof(unsigned int))) == NULL){
            fprintf(stderr, "\nError, in allocating the accumulator initialization table\n\n");
            return -1;
        }
        // Unary length limit
        fread(&buffer, 1, 1, compressedStream);
        encoder_params->u_max = buffer >> 3;
        if(encoder_params->u_max == 0)
            encoder_params->u_max = 0x20;
        // rescaling counter size
        encoder_params->y_star = (buffer & 0x7) + 4;
        // initial count exponent
        fread(&buffer, 1, 1, compressedStream);
        encoder_params->y_0 = (buffer >> 5);
        if(encoder_params->y_0 == 0)
            encoder_params->y_0 = 0x8;
        // Accumulator initialization constant and table
        if((buffer & 0x1E) == 0x1E){
            encoder_params->k = (unsigned int)-1;
            if((buffer & 0x1) != 0){
                unsigned int z = 0;
                for(z = 0; z < input_params->z_size; z+=2){
                    fread(&buffer, 1, 1, compressedStream);
                    encoder_params->k_init[z] = buffer >> 4;
                    if(z + 1 < input_params->z_size)
                        encoder_params->k_init[z + 1] = buffer & 0xF;
                }
            }
        }else{
            unsigned int z = 0;
            encoder_params->k = (buffer >> 1) & 0xF;
            for(z = 0; z < input_params->z_size; z++){
                encoder_params->k_init[z] = encoder_params->k;
            }
        }
    }else{
        // reserved
        fread(&buffer, 1, 1, compressedStream);
        // block size
        switch((buffer >> 5) & 0x3){
        case 0:
            encoder_params->block_size = 8;
            break;
        case 1:
            encoder_params->block_size = 16;
            break;
        case 2:
            encoder_params->block_size = 32;
            break;
        case 3:
            encoder_params->block_size = 64;
            break;
        }
        // Restricted code
        if((buffer & 0x10) != 0)
            encoder_params->restricted = 1;
        else
            encoder_params->restricted = 0;
        // Reference Sample Interval
        encoder_params->ref_interval = (buffer & 0xF) << 8;
        fread(&buffer, 1, 1, compressedStream);
        encoder_params->ref_interval |= (unsigned int)buffer;
        if(encoder_params->ref_interval == 0)
            encoder_params->ref_interval = 4096;
    }
    
    // !!!!!!!!!!!!!! DEBUGGING STATEMENTS !!!!!!!!!!!!!
#ifndef NDEBUG
    printf("Dimensions: x, y, z = %d, %d, %d\n", input_params->x_size, input_params->y_size, input_params->z_size);
    if(input_params->signed_samples == 0)
        printf("Unsigned samples\n");
    else
        printf("Signed samples\n");
    printf("Dynamic Range = %d\n", input_params->dyn_range);
    if(encoder_params->out_interleaving == BI)
        printf("Encoding: BI - interleaving %d\n", encoder_params->out_interleaving_depth);
    else
        printf("Encoding: BSQ\n");
    printf("Word length = %d\n", encoder_params->out_wordsize);

    printf("Prediction bands = %d\n", predictor_params->user_input_pred_bands);
    if(predictor_params->full != 0)
        printf("Full prediction\n");
    else
        printf("Reduced prediction\n");
    if(predictor_params->neighbour_sum != 0)
        printf("Neighbor Oriented sum\n");
    else
        printf("Column Oriented sum\n");
    printf("Regsiter size = %d\n", predictor_params->register_size);
    printf("Weight Resolution = %d\n", predictor_params->weight_resolution);
    printf("Weight Update Scaling Exponent Change = %d\n", predictor_params->weight_interval);
    printf("Weight Update Scaling Exponent Init = %d\n", predictor_params->weight_initial);
    printf("Weight Update Scaling Exponent Final = %d\n", predictor_params->weight_final);
    printf("Weight Init Resolution = %d\n", predictor_params->weight_init_resolution);
    if(predictor_params->weight_init_table){
        printf("Custom weights - table included\n");
    }

    if(encoder_params->encoding_method == BLOCK){
        printf("Block Adaptive Encoder\n");
        printf("Block Size = %d\n", encoder_params->block_size);
        printf("Ref Sample Interval = %d\n", encoder_params->ref_interval);
    }else{
        printf("Sample Adaptive Encoder\n");
        printf("Unary length = %d\n", encoder_params->u_max);
        printf("Rescaling counter = %d\n", encoder_params->y_star);
        printf("Initial Exponent = %d\n", encoder_params->y_0);
        printf("Acc Init Constant = %d\n", encoder_params->k);
    }
    fflush(stdout);
#endif
    
    return 0;
}

/// Main decoder function, from the file containing the compressed stream it produces the
/// file containins the mapped residuals, stored in BSQ format.
int decode(input_feature_t *input_params, predictor_config_t *predictor_params, unsigned short int ** residuals, char inputFile[128]){
    FILE * compressedStream = NULL;
    encoder_config_t encoder_params;
    
    if((compressedStream = fopen(inputFile, "r+b")) == NULL){
        fprintf(stderr, "Error in opening file %s containing the compressed stream\n", inputFile);
        return -1;
    }
    read_header(compressedStream, input_params, &encoder_params, predictor_params);
    
    // Allocation of the array holding the residuals
    *residuals = (unsigned short int *)malloc(sizeof(unsigned short int)*input_params->x_size*input_params->y_size*input_params->z_size);
    if(*residuals == NULL){
        fprintf(stderr, "Error in allocating %lf kBytes for the residuals\n\n", ((double)sizeof(unsigned short int)*input_params->x_size*input_params->y_size*input_params->z_size)/1024.0);
        return -1;
    }
    memset(*residuals, 0, sizeof(unsigned short int)*input_params->x_size*input_params->y_size*input_params->z_size);
    
    // Now it is finally time to decode the stream. accodring to the used encoding method
    if(encoder_params.encoding_method == SAMPLE){
        if(decode_sample_adaptive(compressedStream, *input_params, encoder_params, *residuals) < 0){
            fprintf(stderr, "Error in sample adaptive decoding\n");
            return -1;
        }
    }else{
        if(decode_block_adaptive(compressedStream, *input_params, encoder_params, *residuals) < 0){
            fprintf(stderr, "Error in block adaptive decoding\n");
            return -1;
        }
    }
    
    return 0;
}
