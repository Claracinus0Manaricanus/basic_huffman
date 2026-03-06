#ifndef CM_BASIC_HUFFMAN_HEADER
#define CM_BASIC_HUFFMAN_HEADER

#include <stdint.h>

/**
 * anyhting not relevant to the header is in cpp
 */

/**
 * @brief compresses given data with huffman coding
 *
 * @description compresses data given by parameter data of size data_size using
 * huffman coding, the returned compressed data also has the tree embedded in it
 * and can be directly given to the decompress function
 *
 * @param data the data to be compressed
 * @param data_size size of the data to be compressed
 *
 * @see ArrayData uncompressWithHuffman();
 */
uint8_t *compressWithHuffman(uint8_t *data, uint32_t data_size,
                             uint32_t *out_size);

/**
 * @brief uncompresses data compressed by compressWithHuffman
 *
 * @param data is data to be uncompressed
 * @param data_size is the size of the data to be uncompressed
 *
 * @see ArrayData compressWithHuffman();
 */
uint8_t *uncompressWithHuffman(uint8_t *data, uint32_t data_size,
                               uint32_t *out_size);

#endif
