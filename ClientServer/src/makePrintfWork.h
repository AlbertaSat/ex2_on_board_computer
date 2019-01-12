/*
 * makePrintfWork.h
 *
 *  Created on: May 10, 2018
 *      Author: keith
 *
 *      Credit to https://www.silabs.com/community/software/simplicity-studio/forum.topic.html/how_to_enable_printf-mfn8#U133981).
 *      Mainly the post by "usbeeman" on 12/04/15
 *
 *		This is me putting the code into a c/h file pair for your usage.
 *		I did not develop any of this, but am sparing you a google search.
 *
 *      Include this header file in your code to enable printf, puts, etc.
 *      Call the function SWO_SetupForPrint(void) before your first printf and things should be fine.
 *      Good for debugging, "... from a certain point of view" - Obi Wan Kenobi
 */

#ifndef MAKEPRINTFWORK_H
#define MAKEPRINTFWORK_H

#include <stdio.h>
#include "bsp_trace.h"

int _write(int file, const char *ptr, int len);

// CALL THIS FUNCTION IN MAIN OR WHENEVER JUST BEFORE YOUR FIRST PRINTF(...)
void SWO_SetupForPrint(void);


#endif /* MAKEPRINTFWORK_H */
