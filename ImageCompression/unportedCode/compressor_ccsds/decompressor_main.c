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
decompressor --input compressedImage --output uncompressedImage --out_format [BSQ|bi] --out_depth num --dump_residuals
*/

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "decoder.h"
#include "utils.h"
#include "unpredict.h"


//Lets finally start with the main program; actually it does nothing but
//parsing the options and calling the appropriate functions which do the
//actual job
int main(int argc, char *argv[]){
    //Configuration and command line variables
    char in_file[128];
    char out_file[128];
    int foundOpt = 0;
    const int MAX_KEY_LEN = 32;
    char *filename = "sat2_decompress.hdr";
    unsigned char dump_residuals = 0;
    
    //Parameters
    input_feature_t input_params;
    predictor_config_t predictor_params;

    //Statistics
    double decodingStartTime = 0.0;
    double decodingEndTime = 0.0;
    double unpredictionEndTime = 0.0;
    unsigned short int * residuals = NULL;

    // Some initialization of default values
    in_file[0] = '\x0';
    out_file[0] = '\x0';
    memset(&input_params, 0, sizeof(input_feature_t));
    memset(&predictor_params, 0, sizeof(predictor_config_t));

    // Parsing the header file for input
    FILE * fh = fopen(filename, "r");
    char *key = malloc(MAX_KEY_LEN);
    char *val = malloc(MAX_KEY_LEN);
    int input_match;

    // Reading From File
    while(input_match = fscanf(fh, "%s = %s\n", key, val) != -1) {

        if(strcmp("input", key) == 0){
            strcpy(in_file, val);
        }
        else if(strcmp("output", key) == 0){
            strcpy(out_file, val);
        }
        else if(strcmp("out_format", key) == 0){
            if(strcmp(val, "BI") == 0 || strcmp(val, "bi") == 0){
                input_params.in_interleaving = BI;
            }
            else if(strcmp(val, "BSQ") == 0 || strcmp(val, "bsq") == 0){
                input_params.in_interleaving = BSQ;
            }
            else{
                return -1;
            }
        }
        else if(strcmp("out_depth", key) == 0){
            input_params.in_interleaving_depth = (unsigned int) atoi(val);
        }
    }

    free(key);
    free(val);

    ///Now we need to perform a few checks that the necessary options have been provided
    //and we can call the function to perform the encoding.
    if(in_file[0] == '\x0'){
        fprintf(stderr, "\nError, please indicate the file containing the input compressed file\n\n");
        return -1;
    }
    if(out_file[0] == '\x0'){
        fprintf(stderr, "\nError, please indicate the file where the decompressed image will be saved\n\n");
        return -1;
    }

    // *********************** here is the actual decompression step
    decodingStartTime = ((double)clock())/CLOCKS_PER_SEC;
    if(decode(&input_params, &predictor_params, &residuals, in_file)){
        fprintf(stderr, "Error during the decoding stage\n");
        if(residuals != NULL)
            free(residuals);
        return -1;
    }
    if(dump_residuals != 0){
        // dumps the residuals as unsigned short int (16 bits each) in little endian format in
        // BIP order
        char residuals_name[100];
        FILE * residuals_file = NULL;
        unsigned int x = 0, y = 0, z = 0;
        sprintf(residuals_name, "residuals_%s.bip", out_file);
        if((residuals_file = fopen(residuals_name, "w+b")) == NULL){
            fprintf(stderr, "\nError in creating the file holding the residuals\n\n");
            if(residuals != NULL)
                free(residuals);
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
    decodingEndTime = ((double)clock())/CLOCKS_PER_SEC;
    if(unpredict(input_params, predictor_params, residuals, out_file)){
        fprintf(stderr, "Error during the un-prediction stage\n");
        if(residuals != NULL)
            free(residuals);
        return -1;
    }
    unpredictionEndTime = ((double)clock())/CLOCKS_PER_SEC;
    // *********************** end of the actual compression step

    if(residuals != NULL)
        free(residuals);
    
    //and now we print some stats
    printf("Overall Decompression duration %lf (sec)\n", unpredictionEndTime - decodingStartTime);
    printf("Decoding duration %lf (sec)\n", decodingEndTime - decodingStartTime);
    printf("Unprediction duration %lf (sec)\n", unpredictionEndTime - decodingEndTime);

    return 0;
}

