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
#ifndef WIN32
#include <getopt.h>
#else
#include "win32_getopt.h"
#endif

#include "decoder.h"
#include "utils.h"
#include "unpredict.h"


#define MAXW 64  // maximum number of lines to read
#define MAXC 32  // longest word in abridged Dict. is 28 char
size_t getwords (const char *words[MAXW], FILE *fp);




//String specifying the program command line
#define USAGE_STRING "Usage: %s --input compressedImage --output uncompressedImage --out_format [BSQ|bi] --out_depth num \
--out_byte_ordering [LITTLE|big] --dump_residuals\n"

//Lets declare the available program options: lets start
//with their long description...
struct option options[] = {
    {"input", 1, NULL, 1},
    {"output", 1, NULL, 2},
    {"out_format", 1, NULL, 3},
    {"out_depth", 1, NULL, 4},
    {"dump_residuals", 0, NULL, 5},
    {"out_byte_ordering", 1, NULL, 6},
    {NULL, 0, NULL, 0}
};

//Lets finally start with the main program; actually it does nothing but
//parsing the options and calling the appropriate functions which do the
//actual job
int main(int argc, char *argv[]){
    //Configuration and command line variables
    char in_file[128];
    char out_file[128];
    int foundOpt = 0;
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
    input_params.byte_ordering = LITTLE;
    char* filename = "decompress_test.hdr";
    const char *words[MAXW];    /* array to hold words  */
    size_t nwords = 0;                  /* number of words read */
    size_t i;
    
    // File I/O Parsing
    FILE *fp = fopen (filename, "r");

    // validate file open 
    if (!fp) {  
        fprintf (stderr, "error: file open failed '%s'.\n", filename);
        return 1;
    }
    nwords = getwords (words, fp);

    if (fp != stdin) fclose (fp);   /* close file */

    printf ("\n %ld words read from '%s':\n\n", nwords, filename);

    for (i = 0; i < nwords; i++)
        printf ("  words[%2zu] : %s\n", i, words[i]);
 
    // Lets do some simple command line option parsing
    printf("argc: %d\n", argc);
    for(int i = 0; i < argc; i++){
        printf("argv is:%s\n", argv[i]);
    }
    do{
        foundOpt = getopt_long(argc, argv, "", options, NULL);
        
        switch(foundOpt){
            case 1:
                strcpy(in_file, optarg);
            break;
            case 2:
                strcpy(out_file, optarg);
            break;
            case 3:
                if(strcmp(optarg, "BI") == 0 || strcmp(optarg, "bi") == 0){
                    input_params.in_interleaving = BI;
                }
                else if(strcmp(optarg, "BSQ") == 0 || strcmp(optarg, "bsq") == 0){
                    input_params.in_interleaving = BSQ;
                }
                else{
                    fprintf(stderr, "\nError, %s unknown image format\n\n", optarg);
                    fprintf(stderr, USAGE_STRING, argv[0]);
                    return -1;
                }
            break;
            case 4:
                input_params.in_interleaving_depth = (unsigned int)atoi(optarg);
            break;
            case 5:
                dump_residuals = 1;
            break;
            case 6:
                if(strcmp(optarg, "little") == 0 || strcmp(optarg, "LITTLE") == 0){
                    input_params.byte_ordering = LITTLE;
                }
                else if(strcmp(optarg, "big") == 0 || strcmp(optarg, "BIG") == 0){
                    input_params.byte_ordering = BIG;
                }
                else{
                    // fprintf(stderr, "\nError, %s unknown input byte ordering\n\n", optarg);
                    // fprintf(stderr, USAGE_STRING, argv[0]);
                    return -1;
                }
            break;
            case -1:
                //Do nothing, we have finished parsing the options
            break;
            case '?':
            default:
                // fprintf(stderr, "\nError in the program command line!!\n\n");
                // fprintf(stderr, USAGE_STRING, argv[0]);
                return -1;
            break;
        }
    }while(foundOpt >= 0);

    ///Now we need to perform a few checks that the necessary options have been provided
    //and we can call the function to perform the encoding.
    if(in_file[0] == '\x0'){
        // fprintf(stderr, "\nError, please indicate the file containing the input compressed file\n\n");
        // fprintf(stderr, USAGE_STRING, argv[0]);
        return -1;
    }
    if(out_file[0] == '\x0'){
        // fprintf(stderr, "\nError, please indicate the file where the decompressed image will be saved\n\n");
        // fprintf(stderr, USAGE_STRING, argv[0]);
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

/* function to read words, 1 - per line, from 'fp' */
size_t getwords (const char *words[MAXW], FILE *fp)
{
    size_t idx = 0;                     /* index of words read */

    /* read each line in file into words array
       note: includes trailing newline character */
    while (idx < MAXW && fgets (words[idx], MAXC, fp)) {
        idx++;
        /* note you should check if chars remain in line */
    }

    if (idx == MAXW)   /* check word count against MAXW */
        fprintf (stderr, "warning: MAXW words read.\n");

    return idx;
}