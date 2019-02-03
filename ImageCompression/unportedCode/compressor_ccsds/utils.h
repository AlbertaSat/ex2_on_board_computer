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
Miscellaneous routines to be used by the entrpy encoder and the
predictor
*/

#ifndef UTILS_H
#define UTILS_H

#define MIN(x, y) ((x) < (y) ? x : y)

#ifdef WIN32
#define log2(x) (log((double)(x))/log((double)2))
#endif

#define SEGMENT_SIZE 64

//Macro used to move from a matrix notation to a linear array, when the matrix
//is ordered according to the BSQ order
//#define MATRIX_BSQ_INDEX(matrix, input_params, x, y, z) matrix[(z)*input_params.x_size*input_params.y_size + (y)*input_params.x_size + (x)]

#define MATRIX_BSQ_INDEX(matrix, input_params, x, y, z) matrix[input_params.x_size*((z)*input_params.y_size + (y)) + (x)]

typedef enum{BSQ, BI} interleaving_t;
typedef enum{LITTLE, BIG} endianness_t;

///Type representing the characteristics of the input data fed to
///the algorithm
typedef struct input_feature{
    char signed_samples;
    unsigned char dyn_range;
    unsigned int x_size;
    unsigned int y_size;
    unsigned int z_size;
    interleaving_t in_interleaving;
    unsigned int in_interleaving_depth;
    endianness_t byte_ordering;
    unsigned char regular_input;
} input_feature_t;

///Given the name of the file containing the Accumulator Initiation Value table
///it parses the file and reads the content into the table array which must
///have been pre-allocated.
///The file format simply consists of a text file with a number on each line
///@return 0 if the operation succesfully completes, a negative value otherwise
int parse_acc_table(char fileName[128], unsigned int * table, unsigned int max_value, unsigned int num_toRead);

///Given the name of the file containing the weights initialization table, it
///parses the file and reads the content into the table matrix which
///must have been preallocated
///The file format simply consists of a text file with a number on each line,
///an empty line every Cz values (i.e. every time we move to the weights for the
///successive band).
int parse_weights_table(char fileName[128], int ** table, int max_value, int min_value,
                                            unsigned int num_toRead, unsigned int num_perBand);

/// Computes the mod*R of a number according to the definition of the
/// blue book
long long mod_star(long long arg, long long op, int debug);

/// Returns 0 if the host machine byte ordering is big endian, a value different from 0 if it is
/// little endian
int is_little_endian();

///Given the file samples to be written to files (stored in memory in BSQ order) they
///are saved to file.
///While the samples are provided as unsigned integers, if needed they are converted
///to signed integers. Note also that, disrespective of the actual width of the samples,
///they are always saved on 16 bits (in case they are negative and they use less than 16 bits
///the most significant bits will be stored as 0s, i.e. no sign extension is done)
int write_samples(input_feature_t input_params, char fileName[128], unsigned short int * samples, unsigned int s_mid);

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
int read_samples(input_feature_t input_params, char fileName[128], unsigned short int * samples);

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
                    unsigned int * written_bits, unsigned int num_bits_to_write, unsigned int bits_to_write);

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
                    unsigned int * written_bits, unsigned int num_bits_to_write, unsigned char bit_to_repeat);

///Reads from file the specified ammount of bits and returns the read value into
///as unsigned integer.
unsigned int read_bits(FILE * compressedStream, unsigned int num_bits, unsigned char *buffer, unsigned int *buffer_len);

///Reads from file an unsigned int value coded according to the FS scheme (n-zeros followed by a 1);
///if more than max_bits zeros are encountered, 0xffffffff is returned
unsigned int read_fs(FILE * compressedStream, unsigned int max_bits, unsigned char *buffer, unsigned int *buffer_len);

///Sign extends the unsigned value, to make it signed
int sign_extend(unsigned int orig_value, unsigned int bit_width);

///Given the index of an element in an array where pixels are stored according to
///the specified ordering, it returns the index of the same image element
///in an array specified using BSQ ordering.
unsigned int indexToBSQ(interleaving_t interleaving, unsigned int interleaving_depth,
                        unsigned int x_size, unsigned int y_size, unsigned int z_size, unsigned int index);

///Given the index of an element in an array where pixels are stored according
///to the BSQ ordering, it returns the index of the same image element
///in an array ordered using the specified ordering
unsigned int BSQToIndex(interleaving_t interleaving, unsigned int interleaving_depth,
                        unsigned int x_size, unsigned int y_size, unsigned int z_size, unsigned int index);


#endif
