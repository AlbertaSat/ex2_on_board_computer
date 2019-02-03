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

XXXXXXXXXXXXXXXXXXXXXXX
*/

#ifndef UNPREDICT_H
#define UNPREDICT_H

#include "utils.h"
#include "predictor.h"

/// Given the mapped residual and the prediction it extracts the original sample
unsigned short int get_sample(unsigned short int residual, int scaled_predicted, unsigned int s_min, unsigned int s_maxs);

/// Given the mapped residuals saved in BSQ format it iterates over them, computing
/// the prediction and, then extracting the original sample.
int unpredict(input_feature_t input_params, predictor_config_t predictor_params, unsigned short int * residuals, char outputFile[128]);

#endif