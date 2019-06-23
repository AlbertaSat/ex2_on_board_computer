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
Command line interface for the lossless compressor for hyperspectral and multispectral images 
according to the Draft Recommended Standard CCSDS 123.0-R-1 as of 09/11/2011.
*/

/*
USAGE
compressor --input original_samples --output compressedImage --rows num_rows --columns num_col --bands num_bands
--in_format [BSQ|BI] --in_depth num --in_byte_ordering [LITTLE|big] --dyn_range num --word_len num --out_format [BSQ|bi]
--out_depth num --signed_sample --pred_bands num --full --neighbour_sum --reg_size num --w_resolution num --w_interval num
--w_initial num --w_final num --w_init_resolution num --weight_init_file file_path --sample_adaptive --u_max num
--y_star num --y_0 num --k num --k_init_file file_path --block_size num -- restricted_enc --ref_interval num --reg_input

*/

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#ifndef WIN32
#include <getopt.h>
#else
#include "win32_getopt.h"
#endif

#include "entropy_encoder.h"
#include "utils.h"
#include "predictor.h"

//String specifying the program command line
#define USAGE_STRING "Usage: %s --input original_samples --output compressedImage --rows num_rows --columns num_col --bands num_bands \
--in_format [BSQ|BI] --in_depth num --in_byte_ordering [LITTLE|big] --dyn_range num --word_len num --out_format [BSQ|bi] \
--out_depth num --signed_sample --pred_bands num --full --neighbour_sum --reg_size num --w_resolution num --w_interval num \
--w_initial num --w_final num --w_init_resolution num --weight_init_file file_path --sample_adaptive --u_max num \
--y_star num --y_0 num --k num --k_init_file file_path --block_size num -- restricted_enc --ref_interval num --reg_input\n"

//Lets declare the available program options: lets start
//with their long description...
struct option options[] = {
    {"input", 1, NULL, 1},
    {"output", 1, NULL, 2},
    {"rows", 1, NULL, 3},
    {"columns", 1, NULL, 4},
    {"bands", 1, NULL, 5},
    {"in_format", 1, NULL, 6},
    {"in_depth", 1, NULL, 7},
    {"in_byte_ordering", 1, NULL, 8},
    {"out_format", 1, NULL, 9},
    {"out_depth", 1, NULL, 10},
    {"u_max", 1, NULL, 12},
    {"y_star", 1, NULL, 13},
    {"y_0", 1, NULL, 14},
    {"k", 1, NULL, 15},
    {"k_init_file", 1, NULL, 16},
    {"help", 0, NULL, 17},
    {"dyn_range", 1, NULL, 18},
    {"word_len", 1, NULL, 19},
    {"signed_sample", 0, NULL, 20},
    {"pred_bands", 1, NULL, 21},
    {"full", 0, NULL, 22},
    {"neighbour_sum", 0, NULL, 23},
    {"reg_size", 1, NULL, 24},
    {"w_resolution", 1, NULL, 25},
    {"w_interval", 1, NULL, 26},
    {"w_initial", 1, NULL, 27},
    {"w_final", 1, NULL, 28},
    {"w_init_resolution", 1, NULL, 29},
    {"weight_init_file", 1, NULL, 30},
    {"sample_adaptive", 0, NULL, 31},
    {"block_size", 1, NULL, 32},
    {"restricted_enc", 0, NULL, 33},
    {"ref_interval", 1, NULL, 34},
    {"dump_residuals", 0, NULL, 35},
    {"reg_input", 0, NULL, 36},
    {NULL, 0, NULL, 0}
};

//Lets finally start with the main program; actually it does nothing but
//parsing the options and calling the appropriate functions which do the
//actual job
int main(int argc, char *argv[]){
    //Configuration and command line variables
    char samples_file[128];
    char out_file[128];
    char init_table_file[128];
    char init_weight_file[128];
    input_feature_t input_params;
    encoder_config_t encoder_params;
    predictor_config_t predictor_params;
    int foundOpt = 0;
    //Statistics
    double compressionStartTime = 0.0;
    double compressionEndTime = 0.0;
    double predictionEndTime = 0.0;
    unsigned int compressed_bytes = 0;
    unsigned int dump_residuals = 0;

    // Some initialization of default values
    unsigned short int * residuals = NULL;
    samples_file[0] = '\x0';
    out_file[0] = '\x0';
    init_table_file[0] = '\x0';
    init_weight_file[0] = '\x0';
    memset(&input_params, 0, sizeof(input_feature_t));
    memset(&encoder_params, 0, sizeof(encoder_config_t));
    memset(&predictor_params, 0, sizeof(predictor_config_t));
    encoder_params.k = (unsigned int)-1;
    input_params.dyn_range = 16;
    encoder_params.encoding_method = BLOCK;

    //Lets do some simple command line option parsing
    do{
        foundOpt = getopt_long(argc, argv, "", options, NULL);
        switch(foundOpt){
            case 1:
                strcpy(samples_file, optarg);
            break;
            case 2:
                strcpy(out_file, optarg);
            break;
            case 3:
                input_params.y_size = (unsigned int)atoi(optarg);
            break;
            case 4:
                input_params.x_size = (unsigned int)atoi(optarg);
            break;
            case 5:
                input_params.z_size = (unsigned int)atoi(optarg);
            break;
            case 6:
                if(strcmp(optarg, "BI") == 0 || strcmp(optarg, "bi") == 0){
                    input_params.in_interleaving = BI;
                }
                else if(strcmp(optarg, "BSQ") == 0 || strcmp(optarg, "bsq") == 0){
                    input_params.in_interleaving = BSQ;
                }
                else{
                    fprintf(stderr, "\nError, %s unknown input image format\n\n", optarg);
                    fprintf(stderr, USAGE_STRING, argv[0]);
                    return -1;
                }
            break;
            case 7:
                input_params.in_interleaving_depth = (unsigned int)atoi(optarg);
            break;
            case 8:
                if(strcmp(optarg, "little") == 0 || strcmp(optarg, "LITTLE") == 0){
                    input_params.byte_ordering = LITTLE;
                }
                else if(strcmp(optarg, "big") == 0 || strcmp(optarg, "BIG") == 0){
                    input_params.byte_ordering = BIG;
                }
                else{
                    fprintf(stderr, "\nError, %s unknown input byte ordering\n\n", optarg);
                    fprintf(stderr, USAGE_STRING, argv[0]);
                    return -1;
                }
            break;
            case 9:
                if(strcmp(optarg, "BI") == 0 || strcmp(optarg, "bi") == 0){
                    encoder_params.out_interleaving = BI;
                }
                else if(strcmp(optarg, "BSQ") == 0 || strcmp(optarg, "bsq") == 0){
                    encoder_params.out_interleaving = BSQ;
                }
                else{
                    fprintf(stderr, "\nError, %s unknown image format\n\n", optarg);
                    fprintf(stderr, USAGE_STRING, argv[0]);
                    return -1;
                }
            break;
            case 10:
                encoder_params.out_interleaving_depth = (unsigned int)atoi(optarg);
            break;
            case 12:
                encoder_params.u_max = (unsigned int)atoi(optarg);
            break;
            case 13:
                encoder_params.y_star = (unsigned int)atoi(optarg);
            break;
            case 14:
                encoder_params.y_0 = (unsigned int)atoi(optarg);
            break;
            case 15:
                encoder_params.k = (unsigned int)atoi(optarg);
            break;
            case 16:
                strcpy(init_table_file, optarg);
            break;
            case 17:
                fprintf(stderr, USAGE_STRING, argv[0]);
                return 0;
            break;
            case 18:
                input_params.dyn_range = (unsigned int)atoi(optarg);
            break;
            case 19:
                encoder_params.out_wordsize = (unsigned int)atoi(optarg);
            break;
            case 20:
                input_params.signed_samples = 1;
            break;
            case 21:
                predictor_params.pred_bands = (unsigned int)atoi(optarg);
                predictor_params.user_input_pred_bands = predictor_params.pred_bands;
            break;
            case 22:
                predictor_params.full = 1;
            break;
            case 23:
                predictor_params.neighbour_sum = 1;
            break;
            case 24:
                predictor_params.register_size = (unsigned int)atoi(optarg);
            break;
            case 25:
                predictor_params.weight_resolution = (unsigned char)atoi(optarg);
            break;
            case 26:
                predictor_params.weight_interval = (int)atoi(optarg);
            break;
            case 27:
                predictor_params.weight_initial = (char)atoi(optarg);
            break;
            case 28:
                predictor_params.weight_final = (char)atoi(optarg);
            break;
            case 29:
                predictor_params.weight_init_resolution = (unsigned char)atoi(optarg);
            break;
            case 30:
                strcpy(init_weight_file, optarg);
            break;
            case 31:
                encoder_params.encoding_method = SAMPLE;
            break;
            case 32:
                encoder_params.block_size = (unsigned int)atoi(optarg);
            break;
            case 33:
                encoder_params.restricted = 1;
            break;
            case 34:
                encoder_params.ref_interval = (unsigned int)atoi(optarg);
            break;
            case 35:
                dump_residuals = 1;
            break;
            case 36:
                input_params.regular_input = 1;
            break;
            case -1:
                //Do nothing, we have finished parsing the options
            break;
            case '?':
            default:
                fprintf(stderr, "\nError in the program command line!!\n\n");
                fprintf(stderr, USAGE_STRING, argv[0]);
                return -1;
            break;
        }
    }while(foundOpt >= 0);
    // END OF INPUT PARSING
    ///Now we need to perform a few checks that the necessary options have been provided
    //and we can call the function to perform the encoding.
    if(samples_file[0] == '\x0'){
        fprintf(stderr, "\nError, please indicate the file containing the input samples to be compressed\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(out_file[0] == '\x0'){
        fprintf(stderr, "\nError, please indicate the file where the compressed stream will be saved\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(input_params.y_size*input_params.x_size*input_params.z_size == 0){
        fprintf(stderr, "\nError, please specify all the x, y, and z dimensions with a number > 0\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(input_params.in_interleaving == BI && (input_params.in_interleaving_depth < 1 || input_params.in_interleaving_depth > input_params.z_size)){
        fprintf(stderr, "\nError, the input interleaving depth has to be a positive integer not bigger than the number of bands\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.out_interleaving == BI && (encoder_params.out_interleaving_depth < 1 || encoder_params.out_interleaving_depth > input_params.z_size)){
        fprintf(stderr, "\nError, the output interleaving depth has to be a positive integer not bigger than the number of bands\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.encoding_method == SAMPLE && (encoder_params.y_0 > 8 || encoder_params.y_0 < 1)){
        fprintf(stderr, "\nError, specify a value between 1 and 8 for the initial count exponent y_0\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.encoding_method == SAMPLE && (encoder_params.y_star > 9 || encoder_params.y_star < 4 || encoder_params.y_star < (encoder_params.y_0 + 1))){
        fprintf(stderr, "\nError, specify a value between max{4, y_0 + 1} and 9 for the rescaling counter size parameter y_star\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.encoding_method == SAMPLE && (encoder_params.u_max > 32 || encoder_params.u_max < 8)){
        fprintf(stderr, "\nError, specify a value between 8 and 32 for the unary length limit u_max\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.encoding_method == SAMPLE && encoder_params.out_wordsize <= 0){
        fprintf(stderr, "\nError, specify a value for the word length\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.encoding_method == SAMPLE && init_table_file[0] == '\x0' && encoder_params.k == (unsigned int)-1){
        fprintf(stderr, "\nError, please specify one between initialization constant and the initialization table (k and k_init_file)\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    else if(init_table_file[0] != '\x0' && encoder_params.k != (unsigned int)-1){
        fprintf(stderr, "\nError, both the initialization constant and the initialization table (k and k_init_file) are specified: only one is allowed\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(input_params.dyn_range < 2 || input_params.dyn_range > 16){
        fprintf(stderr, "\nError, please specify the bit width of the residuals between 2 and 16 bits\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.k != (unsigned int)-1 && encoder_params.k > input_params.dyn_range - 2){
        fprintf(stderr, "\nError, the initialization constant k cannot be bigger than %d\n\n", input_params.dyn_range - 2);
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(predictor_params.pred_bands > input_params.z_size){
        predictor_params.pred_bands = input_params.z_size -1;
    }
    if(predictor_params.register_size > 64){
        fprintf(stderr, "\nError, the register size cannot be bigger than 64\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(predictor_params.weight_resolution > 19 || predictor_params.weight_resolution < 4){
        fprintf(stderr, "\nError, the weight resolution must be in the range [4, 19]\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(predictor_params.weight_interval > (0x1 << 11) || predictor_params.weight_interval < (0x1 << 4)){
        fprintf(stderr, "\nError, the weight update interval must be in the range [%d, %d]\n\n", (0x1 << 11), (0x1 << 4));
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if((0x1 << (int)log2(predictor_params.weight_interval)) != predictor_params.weight_interval){
        fprintf(stderr, "\nError, the weight update interval must be a power of 2\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(predictor_params.weight_initial > 9 || predictor_params.weight_initial < -6){
        fprintf(stderr, "\nError, the weight initial value must be in the range [-6, 9]\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(predictor_params.weight_final > 9 || predictor_params.weight_final < -6){
        fprintf(stderr, "\nError, the weight final value must be in the range [-6, 9]\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(init_weight_file[0] != '\x0' && (predictor_params.weight_init_resolution > (predictor_params.weight_resolution + 3) || predictor_params.weight_init_resolution < 3)){
        fprintf(stderr, "\nError, the weight initial resolution must be in the range [3, %d]\n\n", predictor_params.weight_resolution + 3);
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(predictor_params.weight_init_resolution != 0 && init_weight_file[0] == '\x0'){
        fprintf(stderr, "\nError, either both the weight initialization table and the weight initial resolution are specified or none of them\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.encoding_method == SAMPLE && (encoder_params.block_size != 0 || encoder_params.ref_interval != 0)){
        fprintf(stderr, "\nError, when the sample adaptive encoder is used, the block size or reference interval need not be specified, as they refer to the adaptive encoder\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.encoding_method == BLOCK && (encoder_params.ref_interval <= 0 || encoder_params.ref_interval > 4096)){
        fprintf(stderr, "\nError, the reference interval must be a positive integer not larger than 4096\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(encoder_params.encoding_method == BLOCK && (log2(encoder_params.block_size) < 3 || log2(encoder_params.block_size) > 6)){
        fprintf(stderr, "\nError, either block size must be equal to 8, 16, 32, or 64\n\n");
        fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }

    // Now, I can allocated the accumulation constant table, either
    // with all constant values or with the specified accumulator table
    if((encoder_params.k_init = (unsigned int *)malloc(input_params.z_size*sizeof(unsigned int))) == NULL){
        fprintf(stderr, "\nError, in allocating the accumulator initialization table\n\n");
        return -1;
    }
    if(init_table_file[0] != '\x0'){
        if(parse_acc_table(init_table_file, encoder_params.k_init, 0x0F, input_params.z_size) != 0){
            fprintf(stderr, "\nError, in parsing the accumulator initialization table\n\n");
            return -1;
        }
    }
    else{
        int i = 0;
        for(i = 0; i < input_params.z_size; i++){
            encoder_params.k_init[i] = encoder_params.k;
        }
    }
    // and now I allocate the weights table, if needed
    if(init_weight_file[0] != '\x0'){
        int i = 0;
        int prediction_len = predictor_params.pred_bands;
        if(predictor_params.full != 0)
            prediction_len += 3;
        if((predictor_params.weight_init_table = (int **)malloc(sizeof(int *)*input_params.z_size)) == NULL){
            fprintf(stderr, "\nError, in allocating the weight initialization table - 1\n\n");
            return -1;
        }
        for(i = 0; i < input_params.z_size; i++){
            if((predictor_params.weight_init_table[i] = (int *)malloc(sizeof(int)*prediction_len)) == NULL){
                fprintf(stderr, "\nError, in allocating the weight initialization table - 2\n\n");
                return -1;
            }
        }
        if(parse_weights_table(init_weight_file, predictor_params.weight_init_table,
            (0x1 << (predictor_params.weight_init_resolution - 1)) -1, -1*(0x1 << (predictor_params.weight_init_resolution - 1)),
            input_params.z_size*prediction_len, prediction_len) != 0){
            fprintf(stderr, "\nError, in parsing the weights initialization table\n\n");
            return -1;
        }
    }
    // *********************** here is the actual compression step
    residuals = (unsigned short int *)malloc(sizeof(unsigned short int)*input_params.x_size*input_params.y_size*input_params.z_size);
    if(residuals == NULL){
        fprintf(stderr, "Error in allocating %lf kBytes for the residuals buffer\n\n", ((double)sizeof(unsigned short int)*input_params.x_size*input_params.y_size*input_params.z_size)/1024.0);
        return -1;
    }

    compressionStartTime = ((double)clock())/CLOCKS_PER_SEC;
    if(predict(input_params, predictor_params, samples_file, residuals) != 0){
        fprintf(stderr, "\nError during the computation of the residuals (i.e. prediction)\n\n");
        return -1;
    }
    if(dump_residuals != 0){
        // dumps the residuals as unsigned short int (16 bits each) in little endian format in
        // BIP order
        char residuals_name[100];
        FILE * residuals_file = NULL;
        int x = 0, y = 0, z = 0;
        sprintf(residuals_name, "residuals_%s.bip", out_file);
        if((residuals_file = fopen(residuals_name, "w+b")) == NULL){
            fprintf(stderr, "\nError in creating the file holding the residuals\n\n");
            return -1;            
        }
        for(y = 0; y < input_params.y_size; y++){
            for(x = 0; x < input_params.x_size; x++){
                for(z = 0; z < input_params.z_size; z++){
                    fwrite(&(MATRIX_BSQ_INDEX(residuals, input_params, x, y, z)), 2, 1, residuals_file);
                }
            }
        }
        fclose(residuals_file);
    }
    predictionEndTime = ((double)clock())/CLOCKS_PER_SEC;
    compressed_bytes = encode(input_params, encoder_params, predictor_params, residuals, out_file);
    compressionEndTime = ((double)clock())/CLOCKS_PER_SEC;

    //Finally I can deallocate everything
    if(encoder_params.k_init != NULL)
        free(encoder_params.k_init);
    if(predictor_params.weight_init_table != NULL){
        int i = 0;
        for(i = 0; i < input_params.z_size; i++){
            if(predictor_params.weight_init_table[i] != NULL)
                free(predictor_params.weight_init_table[i]);
        }
        free(predictor_params.weight_init_table);
    }
    if(residuals != NULL)
        free(residuals);
    // *********************** end of the actual compression step

    //and now we print some stats
    printf("Overall Compression duration %lf (sec)\n", compressionEndTime - compressionStartTime);
    printf("Prediction duration %lf (sec)\n", predictionEndTime - compressionStartTime);
    printf("Encoding duration %lf (sec)\n", compressionEndTime - predictionEndTime);
    printf("%d bytes (%.2lf kb) in the compressed image\n", compressed_bytes, ((double)compressed_bytes)/(1024.0));
    printf("Compressed rate %lf bits/sample\n", ((double)compressed_bytes*8)/(input_params.y_size*input_params.x_size*input_params.z_size));

    return 0;
}

