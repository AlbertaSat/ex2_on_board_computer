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

#ifndef ENTROPY_ENCODER_H
#define ENTROPY_ENCODER_H

#include "utils.h"
#include "predictor.h"

typedef enum{SAMPLE, BLOCK} encoder_t;

///Type representing the configuration of the encoder algorithm
typedef struct encoder_config{
    unsigned int u_max;
    unsigned int y_star;
    unsigned int y_0;
    unsigned int k;
    unsigned int * k_init;
    interleaving_t out_interleaving;
    unsigned int out_interleaving_depth;
    unsigned int out_wordsize;
    encoder_t encoding_method;
    unsigned char block_size;
    unsigned char restricted;
    unsigned int ref_interval;
} encoder_config_t;

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
    unsigned short int * residuals, char outputFile[128]);

#endif
