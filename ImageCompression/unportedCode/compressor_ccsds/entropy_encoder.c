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
Entropy Encoder as described in Section 5 of the CCSDS 123.0-R-1,
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
#include "entropy_encoder.h"
#include "utils.h"
#include "predictor.h"

/******************************************************
* Routines for the Sample Adaptive Encoder
*******************************************************/

/// Given a single residual and the statistics accumulated so far, it computes the code
/// for the residual and it updates the statistics.
int encode_pixel(unsigned int x, unsigned int y, unsigned int z, unsigned int * counter, unsigned int * accumulator,
    unsigned int *written_bytes, unsigned int * written_bits, unsigned char * compressed_stream, unsigned short int * residuals,
    input_feature_t input_params, encoder_config_t encoder_params){
    unsigned int curIndex = x + y*input_params.x_size + z*input_params.x_size*input_params.y_size;
    
    if((y == 0 && x == 0)){
        // I simply save on the output stream the unmodified
        // residual (which should actually be the unmodified pixel)
        bitStream_store(compressed_stream, written_bytes, written_bits, input_params.dyn_range, residuals[curIndex]);
    }
    else{
        int temp_k = 0;
        unsigned int divisor = 0;
        unsigned int reminder = 0;
        // Now, general case, I have to actually perform the compression ...
        temp_k = (int)log2(((49*counter[z])/0x080 + accumulator[z])/((double)counter[z]));
        if(temp_k < 0)
            temp_k = 0;
        if(temp_k > (input_params.dyn_range - 2))
            temp_k = input_params.dyn_range - 2;
        divisor = residuals[curIndex]/(0x1 << temp_k);
        reminder = residuals[curIndex] & (((unsigned short)0xFFFF) >> (16 - temp_k));

        // ... save the computation on the output stream ...
        if(divisor < encoder_params.u_max){
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, divisor, 0);
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
            bitStream_store(compressed_stream, written_bytes, written_bits, temp_k, reminder);
        }
        else{
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, encoder_params.u_max, 0);
            bitStream_store(compressed_stream, written_bytes, written_bits, input_params.dyn_range, residuals[curIndex]);
        }
        
        // ... and finally update the statistics
        if(counter[z] < ((((unsigned int)0x1) << encoder_params.y_star) -1)){
            accumulator[z] += residuals[curIndex];
            counter[z]++;
        }
        else{
            accumulator[z] = (accumulator[z] + residuals[curIndex] + 1)/2;
            counter[z] = (counter[z] + 1)/2;
        }
    }
    
#ifndef NDEBUG
    if(*written_bytes > (((input_params.dyn_range + 7)/8)*input_params.x_size*input_params.y_size*input_params.z_size)){
        fprintf(stderr, "Error in encode_pixel, writing outside the compressed_stream boundaries: it means that the compressed image is greater than the original\n");
        return -1;
    }
#endif
    return 0;
}

///Given the characteristics of the input stream, the parameters describing the desired behavior
///of the encoder and the list of residuals to be encoded (note that each residual is treated as
///an integer) it returs the size in bytes of the stream containing the compressed residuals (saved into compressed_stream)
///After usage, the caller has to deallocate the memory area pointed by compressed_stream and allocated by this function.
///@param input_params describe the image whose residuals are contained in the input file
///@param encoder_params set of options determining the behavior of the encoder
///@param residuals array containing the information to be compressed
///@param compressed_stream pointer to the array which, at the end of this function, will contain the compressed information
///the array is allocated inside this function
///@return a negative number if an error occurred
int encode_sampleadaptive(input_feature_t input_params, encoder_config_t encoder_params, unsigned short int * residuals,
    unsigned char * compressed_stream, unsigned int * written_bytes, unsigned int * written_bits){
    //First of all we proceed with the compression of the residuals according to the
    //sample adaptive encodying method, as specified in the header of this file.
    //For simplicity I proceed with encoding in the order into which the encoded samples
    //have to be saved into the output stream. Statistics (counter and accumulator) are anyway maintained per band,
    //so, even if samples from different bands are interleaved on the output stream,
    //it is as if each band were encoded separately.
    unsigned int * counter = NULL;
    unsigned int * accumulator = NULL;

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

    // Let's remember that the elements are saved in residuals so that
    // element(x, y, z) = residuals[x + y*x_size + z*x_size*y_size], i.e.
    // they are saved in BSQ order
    if(encoder_params.out_interleaving == BSQ){
        unsigned int x = 0, y = 0, z = 0;
        for(z = 0; z < input_params.z_size; z++){
            counter[z] = 0x1 << encoder_params.y_0;
            accumulator[z] = (counter[z]*(3*(0x1 << (encoder_params.k_init[z] + 6))-49))/0x080;
            for(y = 0; y < input_params.y_size; y++){
                for(x = 0; x < input_params.x_size; x++){
                    if(encode_pixel(x, y, z, counter, accumulator, written_bytes, written_bits, compressed_stream, residuals, input_params, encoder_params) != 0)
                        return -1;
                }
            }
        }
    }
    else{
        unsigned int x = 0, y = 0, z = 0, i = 0;
        unsigned int interleaving_counter = input_params.z_size/encoder_params.out_interleaving_depth;
        if((input_params.z_size % encoder_params.out_interleaving_depth) != 0){
            interleaving_counter++;
        }
        for(z = 0; z < input_params.z_size; z++){
            counter[z] = 0x1 << encoder_params.y_0;
            accumulator[z] = (counter[z]*(3*(0x1 << (encoder_params.k_init[z] + 6))-49))/0x080;
        }
        for(y = 0; y < input_params.y_size; y++){
            for(i = 0; i < interleaving_counter; i++){
                for(x = 0; x < input_params.x_size; x++){
                    for(z = i*encoder_params.out_interleaving_depth; z < MIN((i+1)*encoder_params.out_interleaving_depth, input_params.z_size); z++){
                        if(encode_pixel(x, y, z, counter, accumulator, written_bytes, written_bits, compressed_stream, residuals, input_params, encoder_params) != 0)
                            return -1;
                    }
                }
            }
        }
    }

    free(counter);
    free(accumulator);

    return 0;
}

/******************************************************
* END Sample Adaptive Routines
*******************************************************/

/******************************************************
* Routines for the Block Adaptive Encoder
*******************************************************/

/// This procedure simply computes the code corresponding to the num_zero_blocks number
/// of sequential blocks whose samples are all 0.
/// Such code is saved in the output bitstream.
void zero_block_code(input_feature_t input_params, encoder_config_t encoder_params,
    int num_zero_blocks, unsigned char * compressed_stream, unsigned int * written_bytes, unsigned int * written_bits, int end_of_segment){
    //First of all I have to save the ID of the zero block code option
    if(input_params.dyn_range <= 4 && encoder_params.restricted != 0){
        if(input_params.dyn_range < 3)
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 2, 0);
        else
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 3, 0);
    }else{
        if(input_params.dyn_range <= 8){
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 4, 0);
        }else if(input_params.dyn_range <= 16){
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 5, 0);
        }else{
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 6, 0);
        }
    }
    //Now I can compute and save the code indicating the number of zero blocks.
    if(num_zero_blocks > 1){
        if(num_zero_blocks < 5){
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, num_zero_blocks - 1, 0);
        }else{
            if(end_of_segment != 0)
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 4, 0);
            else
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, num_zero_blocks, 0);
        }
    }
    bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
}

/// Computes the values for the second extension compression option and the length
/// of the compression considering such option
unsigned int compute_second_extension(encoder_config_t encoder_params, unsigned short int * block_samples, unsigned int second_extension_values[32]){
    unsigned int code_len = 0;
    int i = 0;
    for(i = 0; i < encoder_params.block_size; i+=2){
        second_extension_values[i/2] = (((unsigned int)block_samples[i] + block_samples[i + 1])*((unsigned int)block_samples[i] + block_samples[i + 1] + 1))/2 + block_samples[i + 1];
        code_len += second_extension_values[i/2] + 1;
    }
    return code_len;
}

/// The length of the compression considering the bes k-split option
unsigned int compute_ksplit(input_feature_t input_params, encoder_config_t encoder_params, unsigned short int * block_samples, int * k_split, int dump){
    unsigned int code_len = (unsigned int)-1;
    int i = 0, k = 0;
    int k_limit = 0;
    if(input_params.dyn_range == 16){
        k_limit = 14;
    }else if(input_params.dyn_range == 8){
        k_limit = 6;
    }else if(input_params.dyn_range == 4 && encoder_params.restricted != 0){
        k_limit = 2;
    }else{
        k_limit = input_params.dyn_range;
    }
    for(k = 0; k < k_limit; k++){
        unsigned int code_len_temp = 0;
        for(i = 0; i < encoder_params.block_size; i++){
            code_len_temp += (block_samples[i] >> k) + 1 + k;
        }
        if(code_len_temp < code_len){
            code_len = code_len_temp;
            *k_split = k;
        }
    }
    return code_len;
}

/// This procedure computes the codes for all the different k-split, second-extension and
/// no compression options and encodes the block according to the code yielding
/// the highest compression factor.
void compute_block_code(input_feature_t input_params, encoder_config_t encoder_params, 
    unsigned short int * block_samples, unsigned char * compressed_stream, unsigned int * written_bytes, unsigned int * written_bits){
    // I encode the chosen method as the value of k for k-split;
    // second-extension is -1 and no compression -2
    int chosenMethod = -2;
    unsigned int temp_size = 0;
    unsigned int method_code_size = input_params.dyn_range*encoder_params.block_size;
    unsigned int second_extension_values[32];
    int k_split = 0;
    int i = 0;

    // First of all I compute which method is the one yielding smaller compression; note that
    // the second extension compression method has the block ID 1 bit longer
    temp_size = compute_second_extension(encoder_params, block_samples, second_extension_values) + 1;
    if(temp_size < method_code_size){
        // second extension is best, I go for it
        chosenMethod = -1;
        method_code_size = temp_size;
    }
    // Now we have to analyze the k-split
    if(input_params.dyn_range > 2 || encoder_params.restricted == 0){
        if(compute_ksplit(input_params, encoder_params, block_samples, &k_split, *written_bytes > 0x2000 && *written_bytes < 0x3e10) < method_code_size){
            // second extension is best, I go for it
            chosenMethod = k_split;
        }
    }
    // Done, best method chosen. Let's perform the compression, adding the block codes
    // to the bitstream.
    if(chosenMethod == -2){
        // no compression
        //First of all I have to save the ID of the block code option
        if(input_params.dyn_range <= 4 && encoder_params.restricted != 0){
            if(input_params.dyn_range < 3)
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
            else
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 2, 1);
        }else{
            if(input_params.dyn_range <= 8){
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 3, 1);
            }else if(input_params.dyn_range <= 16){
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 4, 1);
            }else{
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 5, 1);
            }
        }
        // and now the codes for the samples
        for(i = 0; i < encoder_params.block_size; i++){
            bitStream_store(compressed_stream, written_bytes, written_bits, input_params.dyn_range, block_samples[i]);
        }
    }else if(chosenMethod == -1){
        // second extension option
        //First of all I have to save the ID of the block code option
        if(input_params.dyn_range <= 4 && encoder_params.restricted != 0){
            if(input_params.dyn_range < 3)
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
            else
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 2, 0);
        }else{
            if(input_params.dyn_range <= 8){
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 3, 0);
            }else if(input_params.dyn_range <= 16){
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 4, 0);
            }else{
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 5, 0);
            }
        }
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
        // and now the codes for the samples
        for(i = 0; i < encoder_params.block_size/2; i++){
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, second_extension_values[i], 0);
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
        }
    }else{
        // k-split
        //First of all I have to save the ID of the block code option
        if(input_params.dyn_range <= 4 && encoder_params.restricted != 0){
            bitStream_store(compressed_stream, written_bytes, written_bits, 2, chosenMethod + 1);
        }else{
            if(input_params.dyn_range <= 8){
                bitStream_store(compressed_stream, written_bytes, written_bits, 3, chosenMethod + 1);
            }else if(input_params.dyn_range <= 16){
                bitStream_store(compressed_stream, written_bytes, written_bits, 4, chosenMethod + 1);
            }else{
                bitStream_store(compressed_stream, written_bytes, written_bits, 5, chosenMethod + 1);
            }
        }
        // and now the codes for the samples
        for(i = 0; i < encoder_params.block_size; i++){
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, (block_samples[i] >> chosenMethod), 0);
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
        }
        for(i = 0; i < encoder_params.block_size; i++){
            bitStream_store(compressed_stream, written_bytes, written_bits, chosenMethod, block_samples[i]);
        }
    }
}

int create_block(input_feature_t input_params, encoder_config_t encoder_params, unsigned short int * block_samples, int all_zero,
    int * num_zero_blocks, int * segment_idx, int reference_samples,
    unsigned char * compressed_stream, unsigned int * written_bytes, unsigned int * written_bits){
    // I have finished reading the block: we now need to pass it to the compressor, unless
    // it is an all zero block
    if(all_zero == 0){
        // Before encoding this block I have to check if there are zero-block that
        // have already been counted and that need to be encoded
        if(*num_zero_blocks > 0){
            zero_block_code(input_params, encoder_params, *num_zero_blocks, compressed_stream, written_bytes, written_bits, 0);
            *num_zero_blocks = 0;
        }
        compute_block_code(input_params, encoder_params, block_samples, compressed_stream, written_bytes, written_bits);
    }else{
        *num_zero_blocks = *num_zero_blocks + 1;
    }
    *segment_idx = *segment_idx + 1;
    if(*segment_idx == SEGMENT_SIZE || reference_samples == encoder_params.ref_interval){
        // I arrived at the end of a segment: if there are un-encoded
        // zero_blocks I encode them
        if(*num_zero_blocks > 0){
            zero_block_code(input_params, encoder_params, *num_zero_blocks, compressed_stream, written_bytes, written_bits, 1);
            *num_zero_blocks = 0;
        }
        *segment_idx = 0;
    }
#ifndef NDEBUG
    if(*written_bytes > (((input_params.dyn_range + 7)/8)*input_params.x_size*input_params.y_size*input_params.z_size)){
        fprintf(stderr, "Error in create_block, writing outside the compressed_stream boundaries: it means that the compressed image is greater than the original\n");
        return -1;
    }
#endif
    return 0;
}

///Given the characteristics of the input stream, the parameters describing the desired behavior
///of the encoder and the list of residuals to be encoded (note that each residual is treated as
///an integer) it returs the size in bytes of the stream containing the compressed residuals (saved into compressed_stream)
///After usage, the caller has to deallocate the memory area pointed by compressed_stream and allocated by this function.
///@param input_params describe the image whose residuals are contained in the input file
///@param encoder_params set of options determining the behavior of the encoder
///@param residuals array containing the information to be compressed
///@param compressed_stream pointer to the array which, at the end of this function, will contain the compressed information
///the array is allocated inside this function
///@return a negative number if an error occurred
int encode_block(input_feature_t input_params, encoder_config_t encoder_params, unsigned short int * residuals,
    unsigned char * compressed_stream, unsigned int * written_bytes, unsigned int * written_bits){
    // First of all I have to pick-up the J elements composing a block and
    // then pass them to the compressor; if a block is composed of
    // all zeros, then I read the remaining blocks in the segment, up
    // to the first block containing a non-zero sample.
    // Let's remember that the elements are saved in residuals so that
    // element(x, y, z) = residuals[x + y*x_size + z*x_size*y_size], i.e.
    // they are saved in BSQ order
    int read_samples = 0;
    int all_zero = 1;
    int segment_idx = 0;
    int num_zero_blocks = 0;
    int reference_samples = 0;
    unsigned short int * block_samples = NULL;
    if((block_samples = (unsigned short int *)malloc(encoder_params.block_size*sizeof(unsigned short int))) == NULL){
        fprintf(stderr, "Error in allocating space to hold the block");
        return -1;
    }

    if(encoder_params.out_interleaving == BSQ){
        unsigned int x = 0, y = 0, z = 0;
        for(z = 0; z < input_params.z_size; z++){
            for(y = 0; y < input_params.y_size; y++){
                for(x = 0; x < input_params.x_size; x++){
                    block_samples[read_samples] = residuals[x + y*input_params.x_size + z*input_params.x_size*input_params.y_size];
                    if(all_zero != 0 && block_samples[read_samples] != 0){
                        all_zero = 0;
                    }
                    read_samples++;
                    if(read_samples == encoder_params.block_size){
                        if(y == (input_params.y_size - 1) && z == (input_params.z_size - 1) && x == (input_params.x_size - 1)){
                            // trick used to signal that we are at the end of the residuals, so if there are any
                            // pending 0 blocks they must be dumped
                            //fprintf(stderr, "end of file");
                            segment_idx = SEGMENT_SIZE - 1;
                        }
                        if(create_block(input_params, encoder_params, block_samples, all_zero, &num_zero_blocks, &segment_idx, reference_samples, compressed_stream, written_bytes, written_bits) != 0)
                            return -1;
                        read_samples = 0;
                        all_zero = 1;
                        reference_samples = (reference_samples + 1) % encoder_params.ref_interval;
                    }
                }
            }
        }
    }
    else{
        unsigned int x = 0, y = 0, z = 0, i = 0;
        unsigned int interleaving_counter = input_params.z_size/encoder_params.out_interleaving_depth;
        if((input_params.z_size % encoder_params.out_interleaving_depth) != 0){
            interleaving_counter++;
        }
        for(y = 0; y < input_params.y_size; y++){
            for(i = 0; i < interleaving_counter; i++){
                for(x = 0; x < input_params.x_size; x++){
                    for(z = i*encoder_params.out_interleaving_depth; z < MIN((i+1)*encoder_params.out_interleaving_depth, input_params.z_size); z++){
                        block_samples[read_samples] = residuals[x + y*input_params.x_size + z*input_params.x_size*input_params.y_size];
                        if(all_zero != 0 && block_samples[read_samples] != 0){
                            all_zero = 0;
                        }
                        read_samples++;
                        if(read_samples == encoder_params.block_size){
                            if(y == (input_params.y_size - 1) && z == (input_params.z_size - 1) && x == (input_params.x_size - 1)){
                                // trick used to signal that we are at the end of the residuals, so if there are any
                                // pending 0 blocks they must be dumped
                                //fprintf(stderr, "end of file");
                                segment_idx = SEGMENT_SIZE - 1;
                            }
                            if(create_block(input_params, encoder_params, block_samples, all_zero, &num_zero_blocks, &segment_idx, reference_samples, compressed_stream, written_bytes, written_bits) != 0)
                                return -1;
                            read_samples = 0;
                            all_zero = 1;
                            reference_samples = (reference_samples + 1) % encoder_params.ref_interval;
                        }
                    }
                }
            }
        }
    }
    // Now we have to check if the number of samples was a multiple of the block size;
    // if not we need to add zeros to the block and perform the compression.
    if(read_samples < encoder_params.block_size && read_samples > 0){
        if(all_zero == 0){
            int i = 0;
            for(i = read_samples; i < encoder_params.block_size; i++){
                block_samples[i] = 0;
            }
        }else{
            num_zero_blocks++;
        }
        if(num_zero_blocks > 0){
            zero_block_code(input_params, encoder_params, num_zero_blocks, compressed_stream, written_bytes, written_bits, 0);
        }
        if(all_zero == 0){
            compute_block_code(input_params, encoder_params, block_samples, compressed_stream, written_bytes, written_bits);
        }
    }
    if(block_samples != NULL){
        free(block_samples);
    }

    return 0;
}

/******************************************************
* END Block Adaptive Routines
*******************************************************/

/// Creates the header and adds it to the output stream.
void create_header(unsigned int * written_bytes, unsigned int * written_bits, unsigned char * compressed_stream,
    input_feature_t input_params, predictor_config_t predictor_params, encoder_config_t encoder_params){
    /* IMAGE METADATA */
    // User defined data
    bitStream_store_constant(compressed_stream, written_bytes, written_bits, 8, 0);
    // x, y, z dimensions
    bitStream_store(compressed_stream, written_bytes, written_bits, 16, input_params.x_size);
    bitStream_store(compressed_stream, written_bytes, written_bits, 16, input_params.y_size);
    bitStream_store(compressed_stream, written_bytes, written_bits, 16, input_params.z_size);
    // Sample type
    if(input_params.signed_samples != 0)
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
    else
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
    // reserved
    bitStream_store_constant(compressed_stream, written_bytes, written_bits, 2, 0);
    // dynamic range
    bitStream_store(compressed_stream, written_bytes, written_bits, 4, input_params.dyn_range);
    // Encoding Sample Order and interleaving
    if(encoder_params.out_interleaving == BSQ){
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 16, 0);
    }
    else{
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
        bitStream_store(compressed_stream, written_bytes, written_bits, 16, encoder_params.out_interleaving_depth);
    }
    // reserved
    bitStream_store_constant(compressed_stream, written_bytes, written_bits, 2, 0);
    // Out word size
    bitStream_store(compressed_stream, written_bytes, written_bits, 3, encoder_params.out_wordsize);
    // Encoder type
    if(encoder_params.encoding_method == SAMPLE)
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
    else
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
    // reserved
    bitStream_store_constant(compressed_stream, written_bytes, written_bits, 10, 0);

    /* PREDICTOR METADATA */
    // reserved
    bitStream_store_constant(compressed_stream, written_bytes, written_bits, 2, 0);
    // prediction bands
    bitStream_store(compressed_stream, written_bytes, written_bits, 4, predictor_params.user_input_pred_bands);
    // prediction mode
    if(predictor_params.full != 0)
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
    else
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
    // reserved
    bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
    // local sum
    if(predictor_params.neighbour_sum != 0)
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
    else
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
    // reserved
    bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
    // Register size
    bitStream_store(compressed_stream, written_bytes, written_bits, 6, predictor_params.register_size);
    // Weight resolution
    bitStream_store(compressed_stream, written_bytes, written_bits, 4, predictor_params.weight_resolution - 4);
    // weight update scaling exponent change interval
    bitStream_store(compressed_stream, written_bytes, written_bits, 4, ((unsigned int)log2(predictor_params.weight_interval)) - 4);
    // weight update scaling exponent initial parameter
    bitStream_store(compressed_stream, written_bytes, written_bits, 4, predictor_params.weight_initial + 6);
    // weight update scaling exponent final parameter
    bitStream_store(compressed_stream, written_bytes, written_bits, 4, predictor_params.weight_final + 6);
    // reserved
    bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
    // weight initialization method and weight initialization table flag
    if(predictor_params.weight_init_table != NULL)
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 2, 1);
    else
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 2, 0);
    // weight initialization resolution
    if(predictor_params.weight_init_table != NULL)
        bitStream_store(compressed_stream, written_bytes, written_bits, 5, predictor_params.weight_init_resolution);
    else
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 5, 0);
    // Weight initialization table
    if(predictor_params.weight_init_table != NULL){
        unsigned int z = 0, cz = 0;
        for(z = 0; z < input_params.z_size; z++){
            if(predictor_params.full != 0){
                for(cz = 0; cz < MIN(predictor_params.pred_bands + 3, z + 3); cz++){
                    bitStream_store(compressed_stream, written_bytes, written_bits, predictor_params.weight_init_resolution, predictor_params.weight_init_table[z][cz]);
                }
            }else{
                for(cz = 0; cz < MIN(predictor_params.pred_bands, z); cz++){
                    bitStream_store(compressed_stream, written_bytes, written_bits, predictor_params.weight_init_resolution, predictor_params.weight_init_table[z][cz]);
                }
            }
        }
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 8-(*written_bits), 0);
    }

    /* ENTROPY CODER METADATA */
    if(encoder_params.encoding_method == SAMPLE){
        // Unary length limit
        bitStream_store(compressed_stream, written_bytes, written_bits, 5, encoder_params.u_max);
        // rescaling counter size
        bitStream_store(compressed_stream, written_bytes, written_bits, 3, encoder_params.y_star - 4);
        // initial count exponent
        bitStream_store(compressed_stream, written_bytes, written_bits, 3, encoder_params.y_0);
        // Accumulator initialization constant and table
        if(encoder_params.k == (unsigned int)-1){
            unsigned int z = 0;
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 4, 1);
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
            for(z = 0; z < input_params.z_size; z++){
                bitStream_store(compressed_stream, written_bytes, written_bits, 4, encoder_params.k_init[z]);
            }
            if((input_params.z_size % 2) != 0)
                bitStream_store_constant(compressed_stream, written_bytes, written_bits, 4, 0);
        }else{
            bitStream_store(compressed_stream, written_bytes, written_bits, 4, encoder_params.k);
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
        }
    }else{
        // reserved
        bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
        // block size
        switch(encoder_params.block_size){
        case 8:
            bitStream_store(compressed_stream, written_bytes, written_bits, 2, 0x0);
            break;
        case 16:
            bitStream_store(compressed_stream, written_bytes, written_bits, 2, 0x1);
            break;
        case 32:
            bitStream_store(compressed_stream, written_bytes, written_bits, 2, 0x2);
            break;
        case 64:
            bitStream_store(compressed_stream, written_bytes, written_bits, 2, 0x3);
            break;
        }
        // Restricted code
        if(input_params.dyn_range <= 4 && encoder_params.restricted != 0)
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 1);
        else
            bitStream_store_constant(compressed_stream, written_bytes, written_bits, 1, 0);
        // Reference Sample Interval
        bitStream_store(compressed_stream, written_bytes, written_bits, 12, encoder_params.ref_interval);
    }
}

///Main function for the entropy encoding of a given input file; while it works for any input file,
///it is though to be used when the input file encodes the residuals of each pixel of an image after
///the lossless compression step
///@param input_params describe the image whose residuals are contained in the input file
///@param encoder_params set of options determining the behavior of the encoder
///@param inputFile file containing the information to be compressed
///@param outputFile file where the compressed information will be stored
///@return the number of bytes which compose the compressed stream, a negative value if an error
///occurred
int encode(input_feature_t input_params, encoder_config_t encoder_params, predictor_config_t predictor_params, 
    unsigned short int * residuals, char outputFile[128]){
    // The function is pretty simple; it mainly simply parses the input files,
    // and calls the encode_core routine. After the encoding has ended it writes the
    // result to the output file.
    // all memory allocation/de-allocation takes place inside this routine
    unsigned char * compressed_stream = NULL;
    int encoding_outcome = 0, write_result = 0;
    unsigned int num_padding_bits = 0;
    unsigned int written_bytes = 0, written_bits = 0;
    FILE * outFile = NULL;

    // Note how the compressed stream shall never be greater than the original size of the
    // residuals
    compressed_stream = (unsigned char *)malloc(((input_params.dyn_range + 7)/8)*input_params.x_size*input_params.y_size*input_params.z_size);
    if(compressed_stream == NULL){
        fprintf(stderr, "Error in the allocation of the compressed stream\n\n");
        return -1;
    }
    memset(compressed_stream, 0, ((input_params.dyn_range + 7)/8)*input_params.x_size*input_params.y_size*input_params.z_size);

    // First of all we need to write the headers to the file
    create_header(&written_bytes, &written_bits, compressed_stream, input_params, predictor_params, encoder_params);

    // Finally I can perform the encoding
    if(encoder_params.encoding_method == SAMPLE){
        encoding_outcome = encode_sampleadaptive(input_params, encoder_params, residuals, compressed_stream, &written_bytes, &written_bits);
    }else{
        encoding_outcome = encode_block(input_params, encoder_params, residuals, compressed_stream, &written_bytes, &written_bits);
    }
    if(encoding_outcome < 0){
        fprintf(stderr, "Error in encodying the residuals\n\n");
        return -1;
    }
    
    // Compression has finished; I fill up the compressed stream bits to pad it to
    // word length and deallocate memory
    num_padding_bits = encoder_params.out_wordsize*8 - ((written_bytes*8 + written_bits) % (encoder_params.out_wordsize*8));
    if(num_padding_bits < encoder_params.out_wordsize*8 && num_padding_bits > 0){
        bitStream_store_constant(compressed_stream, &written_bytes, &written_bits, num_padding_bits, 0);
    }

    // and saving the results on the output file
    if((outFile = fopen(outputFile, "wb")) == NULL){
        fprintf(stderr, "Error in creating file %s for writing the compression result\n\n", outputFile);
        return -1;
    }
    write_result = fwrite(compressed_stream, 1, written_bytes, outFile);
    if(write_result != written_bytes){
        fprintf(stderr, "Error in writing compressed stream to %s: only %d bytes out of %d written\n\n", outputFile, write_result, written_bytes);
        return -1;        
    }
    fclose(outFile);

    if(compressed_stream != NULL)
        free(compressed_stream);
    return written_bytes;
}
