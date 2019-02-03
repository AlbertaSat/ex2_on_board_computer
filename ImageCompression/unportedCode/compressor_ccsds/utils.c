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
Miscellaneous routines to be used by the entrpy encoder and the
predictor
*/

#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "utils.h"

/// Returns 0 if the host machine byte ordering is big endian, a value different from 0 if it is
/// little endian
static long _TestEndian = 0x1;
int is_little_endian(){
    return *(char*)&_TestEndian;
}

///Given the name of the file containing the Accumulator Initiation Value table
///it parses the file and reads the content into the table array which must
///have been pre-allocated.
///The file format simply consists of a text file with a number on each line
///@return 0 if the operation succesfully completes, a negative value otherwise
int parse_acc_table(char fileName[128], unsigned int * table, unsigned int max_value, unsigned int num_toRead){
    FILE * acc_tableFile = NULL;
    unsigned int i = 0;
    acc_tableFile = fopen(fileName, "r+t");
    if(acc_tableFile == NULL){
        fprintf(stderr, "Error in opening the accumulator table file %s\n\n", fileName);
        return -1;
    }
    for(i = 0; i < num_toRead; i++){
        if(fscanf(acc_tableFile, "%d\n", &(table[i])) == EOF){
            fprintf(stderr, "Error in reading the accumulator table: EOF while read only %d values\n\n", i);
            fclose(acc_tableFile);
            return -1;
        }
        if((table[i]) > max_value){
            fprintf(stderr, "Error in reading the accumulator table: value %d greater than %d\n\n", i, max_value);
            fclose(acc_tableFile);
            return -1;
        }
    }
    return 0;
}

///Given the name of the file containing the weights initialization table, it
///parses the file and reads the content into the table matrix which
///must have been preallocated
///The file format simply consists of a text file with a number on each line,
///an empty line every Cz values (i.e. every time we move to the weights for the
///successive band).
int parse_weights_table(char fileName[128], int ** table, int max_value, int min_value, unsigned int num_toRead, unsigned int num_perBand){
    FILE * constantFile = NULL;
    unsigned int i = 0;
    int readValue = 0;
    constantFile = fopen(fileName, "r+t");
    if(constantFile == NULL){
        fprintf(stderr, "Error in opening the weights file %s\n\n", fileName);
        return -1;
    }
    for(i = 0; i < num_toRead; i++){
        if(i % num_perBand == 0 && i > 0){
            if(fscanf(constantFile, "\n") == EOF){
                fprintf(stderr, "Error in reading the weights file: EOF while read only %d values - blank line read\n\n", i);
                fclose(constantFile);
                return -1;
            }
        }
        if(fscanf(constantFile, "%d\n", &readValue) == EOF){
            fprintf(stderr, "Error in reading the weights file: EOF while read only %d values\n\n", i);
            fclose(constantFile);
            return -1;
        }
        if(readValue > max_value){
            fprintf(stderr, "Error in reading the weights file: value %d (%d) greater than %d\n\n", i, readValue, max_value);
            fclose(constantFile);
            return -1;
        }
        if(readValue < min_value){
            fprintf(stderr, "Error in reading the weights file: value %d (%d) smaller than %d\n\n", i, readValue, min_value);
            fclose(constantFile);
            return -1;
        }
        table[i/num_perBand][i % num_perBand] = readValue;
    }
    return 0;
}

/// Computes the mod*R of a number according to the definition of the
/// blue book
long long mod_star(long long arg, long long op, int debug){
    long long power2 = ((long long)0x1) << (op - 1);
    // I have to use the trick of shifting not of the op quantity altogether as
    // when op == 64 no shift is actually performed by the system
    return ((arg + power2) - (((((arg + power2) >> (op - 1) >> 1)) << (op - 1)) << 1)) - power2;
}

///Writes the numBitsToWrite bits from bitToWrite into compressedStream, starting at byte
///writtenBytes and in that byte at bit writtenBits. It also updates writtenBytes and
///writtenBits according to the number of bits written
///@param compressed_stream pointer to the array holding the stream cotnaining the compressed data
///@param written_bytes number of bytes so far written to the stream
///@param written_bits number of bits so far written to the stream
///@param num_bits_to_write number of least significant bits of the bits_to_write word which we have to
///write into the stream
///@param bits_to_write word containing the bits to be written to the stream
void bitStream_store(unsigned char * compressed_stream, unsigned int * written_bytes,
                    unsigned int * written_bits, unsigned int num_bits_to_write, unsigned int bits_to_write){
    int i = 0;
    //fprintf(stderr, "bits_to_write %#x, num_bits_to_write %d\n", bits_to_write, num_bits_to_write);
    for(i = num_bits_to_write - 1; i >= 0; i--){
        compressed_stream[*written_bytes] |= ((bits_to_write >> i) & 0x1) << (7 - *written_bits);
        (*written_bits)++;
        if(*written_bits >= 8){
            *written_bits = 0;
            (*written_bytes)++;
        }
    }
}

///Writes bitToRepeat a number of times equal to numBitsToWrite into compressedStream, starting at byte
///writtenBytes and in that byte at bit writtenBits. It also updates writtenBytes and
///writtenBits according to the number of bits written
///@param compressed_stream pointer to the array holding the stream cotnaining the compressed data
///@param written_bytes number of bytes so far written to the stream
///@param written_bits number of bits so far written to the stream
///@param num_bits_to_write number of times the bit in the least significant position of bit_to_repeat
///has to be added to the stream
///@param bit_to_repeat byte whose least significant bit is to be added to the stream num_bits_to_write times
void bitStream_store_constant(unsigned char * compressed_stream, unsigned int * written_bytes,
                    unsigned int * written_bits, unsigned int num_bits_to_write, unsigned char bit_to_repeat){
    unsigned int i = 0;
    bit_to_repeat = 0x1 & bit_to_repeat;
    for(i = 0; i < num_bits_to_write; i++){
        compressed_stream[*written_bytes] |= bit_to_repeat << (7 - *written_bits);
        (*written_bits)++;
        if(*written_bits >= 8){
            *written_bits = 0;
            (*written_bytes)++;
        }
    }
}

///Given the index of an element in an array where pixels are stored according to
///the specified ordering, it returns the index of the same image element
///in an array specified using BSQ ordering.
unsigned int indexToBSQ(interleaving_t interleaving, unsigned int interleaving_depth,
                        unsigned int x_size, unsigned int y_size, unsigned int z_size, unsigned int index){
    unsigned int x = 0, y = 0, z = 0, reminder = 0, i = 0;
#ifndef NDEBUG
    // Consistency check: I should never go out of the allocated boundaries
    if(index >= x_size*y_size*z_size){
        fprintf(stderr, "indexToBSQ - Error requested index %d out of boundaries %d\n\n", index, x_size*y_size*z_size);
        exit(-1);
    }
#endif
    //Of course if the ordering is already BSQ there is nothing to do
    if(interleaving == BSQ)
        return index;
    //BI ordering: here there might some calculations to do;
    //the quickest way is to extract the pixel coordinates (x, y, z)
    //and then convert them back to sequential numbering according to
    //BSQ ordering
    y = index/(x_size*z_size);
    reminder = index % (x_size*z_size);

    i = reminder/(x_size*interleaving_depth);
    reminder = reminder % (x_size*interleaving_depth);

    if(i == z_size/interleaving_depth){
        x = reminder/(z_size - i*interleaving_depth);
        reminder = reminder % (z_size - i*interleaving_depth);
    }
    else{
        x = reminder/interleaving_depth;
        reminder = reminder % interleaving_depth;
    }

    z = interleaving_depth*i + reminder;
    if(z >= z_size)
        z = z_size - 1;

    return x + y*x_size + z*x_size*y_size;
}

///Given the index of an element in an array where pixels are stored according
///to the BSQ ordering, it returns the index of the same image element
///in an array ordered using the specified ordering
unsigned int BSQToIndex(interleaving_t interleaving, unsigned int interleaving_depth,
                        unsigned int x_size, unsigned int y_size, unsigned int z_size, unsigned int index){
    unsigned int x = 0, y = 0, z = 0, reminder = 0, i = 0;
#ifndef NDEBUG
    // Consistency check: I should never go out of the allocated boundaries
    if(index >= x_size*y_size*z_size){
        fprintf(stderr, "BSQToIndex - Error requested index %d out of boundaries %d\n\n", index, x_size*y_size*z_size);
        exit(-1);
    }
#endif
    //Of course if the target ordering is BSQ there is nothing to do
    if(interleaving == BSQ)
        return index;

    //As usual, I extract the pixel coordinates and then reconstruct the
    //target numbering
    z = index/(x_size*y_size);
    reminder = index % (x_size*y_size);

    y = reminder/x_size;
    reminder = index % x_size;

    x = reminder;

    i = z/interleaving_depth;

    //I am at the last band: it might be that I cannot have enough bands
    //left to use the whole interleaving depth
    if(z == z_size -1){
        return y*x_size*z_size + i*interleaving_depth*x_size + x*(z_size - i*interleaving_depth) + z % interleaving_depth;
    }
    else{
        return y*x_size*z_size + i*interleaving_depth*x_size + x*interleaving_depth + z % interleaving_depth;
    }
}

///Given the file samples to be written to files (stored in memory in BSQ order) they
///are saved to file.
///While the samples are provided as unsigned integers, if needed they are converted
///to signed integers. Note also that, disrespective of the actual width of the samples,
///they are always saved on 16 bits (in case they are negative and they use less than 16 bits
///the most significant bits will be stored as 0s, i.e. no sign extension is done)
int write_samples(input_feature_t input_params, char fileName[128], unsigned short int * samples, unsigned int s_mid){
    FILE * outputFile = NULL;

    outputFile = fopen(fileName, "w+b");
    if(outputFile == NULL){
        fprintf(stderr, "Error in opening output file %s\n\n", fileName);
        return -1;
    }

    if(input_params.in_interleaving == BSQ){
        unsigned int x = 0, y = 0, z = 0;
        for(z = 0; z < input_params.z_size; z++){
            for(y = 0; y < input_params.y_size; y++){
                for(x = 0; x < input_params.x_size; x++){
                    if(input_params.signed_samples != 0){
                        unsigned short int mask = 0xFFFF >> (16 - input_params.dyn_range);
                        short int sample_buffer = ((int)MATRIX_BSQ_INDEX(samples, input_params, x, y, z)) - s_mid;
                        sample_buffer = sample_buffer & mask;
                        if(input_params.dyn_range > 8){
                            if((is_little_endian() != 0 && input_params.byte_ordering == BIG) ||
                                (is_little_endian() == 0 && input_params.byte_ordering == LITTLE)){
                                short int temp_endianess = sample_buffer & 0xFF;
                                sample_buffer = ((unsigned short int)sample_buffer) >> 8;
                                sample_buffer |= temp_endianess << 8;
                            }
                            fwrite(&sample_buffer, 2, 1, outputFile);
                        }else
                            fwrite(&sample_buffer, 1, 1, outputFile);
                    }else{
                        unsigned short int sample_buffer = MATRIX_BSQ_INDEX(samples, input_params, x, y, z);
                        if(input_params.dyn_range > 8){
                            if((is_little_endian() != 0 && input_params.byte_ordering == BIG) ||
                                (is_little_endian() == 0 && input_params.byte_ordering == LITTLE)){
                                unsigned short int temp_endianess = sample_buffer & 0xFF;
                                sample_buffer = sample_buffer >> 8;
                                sample_buffer |= temp_endianess << 8;
                            }
                            fwrite(&sample_buffer, 2, 1, outputFile);
                        }else
                            fwrite(&sample_buffer, 1, 1, outputFile);
                    }
                }
            }
        }
    }else{
        unsigned int x = 0, y = 0, z = 0, i = 0;
        unsigned int interleaving_counter = input_params.z_size/input_params.in_interleaving_depth;
        if((input_params.z_size % input_params.in_interleaving_depth) != 0){
            interleaving_counter++;
        }
        for(y = 0; y < input_params.y_size; y++){
            for(i = 0; i < interleaving_counter; i++){
                for(x = 0; x < input_params.x_size; x++){
                    for(z = i*input_params.in_interleaving_depth; z < MIN((i+1)*input_params.in_interleaving_depth, input_params.z_size); z++){
                        if(input_params.signed_samples != 0){
                            unsigned short int mask = 0xFFFF >> (16 - input_params.dyn_range);
                            short int sample_buffer = ((int)MATRIX_BSQ_INDEX(samples, input_params, x, y, z)) - s_mid;
                            sample_buffer = sample_buffer & mask;
                            if(input_params.dyn_range > 8){
                                if((is_little_endian() != 0 && input_params.byte_ordering == BIG) ||
                                    (is_little_endian() == 0 && input_params.byte_ordering == LITTLE)){
                                    short int temp_endianess = sample_buffer & 0xFF;
                                    sample_buffer = ((unsigned short int)sample_buffer) >> 8;
                                    sample_buffer |= temp_endianess << 8;
                                }
                                fwrite(&sample_buffer, 2, 1, outputFile);
                            }else
                                fwrite(&sample_buffer, 1, 1, outputFile);
                        }else{
                            short unsigned int sample_buffer = MATRIX_BSQ_INDEX(samples, input_params, x, y, z);
                            if(input_params.dyn_range > 8){
                                if((is_little_endian() != 0 && input_params.byte_ordering == BIG) ||
                                    (is_little_endian() == 0 && input_params.byte_ordering == LITTLE)){
                                    unsigned short int temp_endianess = sample_buffer & 0xFF;
                                    sample_buffer = sample_buffer >> 8;
                                    sample_buffer |= temp_endianess << 8;
                                }
                                fwrite(&sample_buffer, 2, 1, outputFile);
                            }else
                                fwrite(&sample_buffer, 1, 1, outputFile);
                        }
                    }
                }
            }
        }
    }

    fclose(outputFile);

    return 0;
}

///Given the file encoding the input samples, it reads them into the pre-allocated samples
///array. The bit width of the samples in the file to read is encoded with input_params.residual_width
///bits.
///The elements are saved in the samples array so that
///element(x, y, z) = residuals[x + y*x_size + z*x_size*y_size] (i.e. BSQ)
///Note that the input elements might be in a different order (e.g. BI), thus requiring
///modifications to such order
///Also, the input elements could be either signed or unsigned values, I will transform it to unsigned
///by adding the quantity 2^(D-1) so that the rest of the compressor only has to deal with
///unsigned images
int read_samples(input_feature_t input_params, char fileName[128], unsigned short int * samples){
    //I simply have to read chunk of input_params.residual_width at a time,
    //saving them in a short int (even if each sample is smaller).
    unsigned short int buffer = 0;
    unsigned int readElements = 0;
    FILE * inputFile = NULL;
    unsigned int num_inBuffer = 0;
    unsigned short int prevBuffer = 0x0;
    int availableBytes = 0;

    inputFile = fopen(fileName, "r+b");
    if(inputFile == NULL){
        fprintf(stderr, "Error in opening input file %s\n\n", fileName);
        return -1;
    }

    if(input_params.regular_input != 0){
        //We deal with endianness only in the case the input file uses a regular representation,
        //which means 16 bits for every value even if the actual size is smaller.
        //Note that in this case it might be necessary to perform a byte swap if
        //the endianness is different
        while(fread(&buffer, 1, 2, inputFile) == 2 && readElements < input_params.x_size*input_params.y_size*input_params.z_size){
            //I compose convert the endianness of the current element, if necessary,
            //and store it
            if((is_little_endian() != 0 && input_params.byte_ordering == BIG) || (is_little_endian() == 0 && input_params.byte_ordering == LITTLE)){
                buffer = ((buffer >> 8) & 0x00FF) | ((buffer << 8) & 0xFF00);
            }
            //Consistency check: let's check that, indeed, the element does not use more than
            //the specified number of bits
            if((input_params.signed_samples == 0) || ((buffer & 0x8000) == 0)){
                if((((unsigned short int)buffer) >> input_params.dyn_range) != 0){
                    fprintf(stderr, "Error the %dth sample %#x is using more than %d bits\n\n", readElements, buffer, input_params.dyn_range);
                    fclose(inputFile);
                    return -1;
                }
            }
            samples[indexToBSQ(input_params.in_interleaving, input_params.in_interleaving_depth, input_params.x_size, input_params.y_size, input_params.z_size, readElements)] = buffer;
            readElements++;
        }
    }
    else{
        //Now, instead, we are in the situation that only the exact D bits are specified for
        //every sample.
        //I read two bytes (16 bits) at a time, eventually eliminating the most
        //signicant bits, in case the length of the residuals is smaller than 16 bits,
        //keeping the remaining bits for the next residual
        //I repeat until the input file is empty
        availableBytes = fread(&buffer, 1, 2, inputFile);
        while(availableBytes == 2 && readElements < (input_params.x_size*input_params.y_size*input_params.z_size)){
            //I compose the current element
            samples[indexToBSQ(input_params.in_interleaving, input_params.in_interleaving_depth, input_params.x_size, input_params.y_size, input_params.z_size, readElements)] = ((buffer << num_inBuffer) | prevBuffer) & (((unsigned short int)0xFFFF) >> (16 - input_params.dyn_range));
            readElements++;
            prevBuffer = buffer >> (input_params.dyn_range - num_inBuffer);
            num_inBuffer = 16 - (input_params.dyn_range - num_inBuffer);
            //Now it might be the case that num_inBuffer >= input_params.residual_width: in this
            //case it is not necessary anymore to read another element, but I can directly use the buffer
            while(num_inBuffer >= input_params.dyn_range && readElements < input_params.x_size*input_params.y_size*input_params.z_size){
                samples[indexToBSQ(input_params.in_interleaving, input_params.in_interleaving_depth, input_params.x_size, input_params.y_size, input_params.z_size, readElements)] = prevBuffer & (((unsigned short int)0xFFFF) >> (16 - input_params.dyn_range));
                readElements++;
                prevBuffer = prevBuffer >> input_params.dyn_range;
                num_inBuffer -= input_params.dyn_range;
            }
            buffer = 0;
            availableBytes = fread(&buffer, 1, 2, inputFile);
        }
        // I still have a byte to go
        if(availableBytes == 1 && readElements < input_params.x_size*input_params.y_size*input_params.z_size){
            samples[indexToBSQ(input_params.in_interleaving, input_params.in_interleaving_depth, input_params.x_size, input_params.y_size, input_params.z_size, readElements)] = ((buffer << num_inBuffer) | prevBuffer) & (((unsigned short int)0xFFFF) >> (8 - input_params.dyn_range));
            readElements++;
            prevBuffer = buffer >> (input_params.dyn_range - num_inBuffer);
            num_inBuffer = 8 - (input_params.dyn_range - num_inBuffer);
            //Now it might be the case that num_inBuffer >= input_params.residual_width: in this
            //case it is not necessary anymore to read another element, but I can directly use the buffer
            while(num_inBuffer >= input_params.dyn_range && readElements < input_params.x_size*input_params.y_size*input_params.z_size){
                samples[indexToBSQ(input_params.in_interleaving, input_params.in_interleaving_depth, input_params.x_size, input_params.y_size, input_params.z_size, readElements)] = prevBuffer & (((unsigned short int)0xFFFF) >> (8 - input_params.dyn_range));
                readElements++;
                prevBuffer = prevBuffer >> input_params.dyn_range;
                num_inBuffer -= input_params.dyn_range;
            }
        }
    }
    fclose(inputFile);

    if(readElements < input_params.x_size*input_params.y_size*input_params.z_size){
        fprintf(stderr, "Error, not enough elements in the input file\n\n");
        return -1;
    }

    // Now I have to go over the elements that I have read and eventually
    // correct them in case they are signed int: I add the their mid-range
    // to make them unsigned
    if(input_params.signed_samples != 0){
        short int mid_range = 0x1 << (input_params.dyn_range - 1);
        unsigned int i = 0;
        unsigned short sign_extend_mask = 0xFFFF << input_params.dyn_range;
        unsigned short sign_bit_mask = 0x1 << (input_params.dyn_range -1);

        for(i = 0; i < readElements; i++){
            short int buffer = samples[i];
            //Now I make the necessary corrections to sign externd the read value
            if(input_params.signed_samples != 0 && (buffer & sign_bit_mask) != 0){
                buffer |= sign_extend_mask;
            }
            samples[i] = (unsigned short int)(buffer + mid_range);
        }
    }

    return 0;
}

///Reads from file the specified ammount of bits and returns the read value into
///as unsigned integer.
unsigned int read_bits(FILE * compressedStream, unsigned int num_bits, unsigned char *buffer, unsigned int *buffer_len){
    unsigned int read_value = 0;
    unsigned int to_read = num_bits;
    while(to_read > *buffer_len){
        // not enough bits in the buffer, need to get some from file
        read_value = (read_value << *buffer_len) | ((*buffer) >> (8 - *buffer_len));
        to_read -= *buffer_len;
        if(fread(buffer, 1, 1, compressedStream) < 1){
            fprintf(stderr, "feof=%d, to_read=%d, read_value=%#x\n", feof(compressedStream), to_read, read_value);
            return (unsigned int)-1;
        }
        *buffer_len = 8;
    }
    if(to_read > 0){
        read_value = (read_value << to_read) | ((*buffer) >> (8 - to_read));
        *buffer = (*buffer) << to_read;
        *buffer_len = *buffer_len - to_read;
    }

    return read_value;
}

///Reads from file an unsigned int value coded according to the FS scheme (n-zeros followed by a 1);
///if more than max_bits zeros are encountered, 0xffffffff is returned
unsigned int read_fs(FILE * compressedStream, unsigned int max_bits, unsigned char *buffer, unsigned int *buffer_len){
    unsigned int count = 0;
    while(count < max_bits){
        if(*buffer_len == 0){
            if(fread(buffer, 1, 1, compressedStream) < 1){
                return (unsigned int)-1;
            }
            *buffer_len = 8;
        }
        *buffer_len = *buffer_len - 1;
        if(((*buffer) & 0x80) != 0){
            *buffer = (*buffer) << 1;
            return count;
        }else{
            *buffer = (*buffer) << 1;
            count++;
        }
    }
    return max_bits + 1;
}

///Sign extends the unsigned value, to make it signed
int sign_extend(unsigned int orig_value, unsigned int bit_width){
    int extended = orig_value;
    unsigned int mask = 0x1 << (bit_width - 1);

    if(orig_value & mask){
        extended |= (0xFFFFFFFF << bit_width);
    }

    return extended;
}
