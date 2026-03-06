#ifndef CM_BASIC_HUFFMAN_HEADER
#define CM_BASIC_HUFFMAN_HEADER

#include <stdint.h>

/* huffman node */
typedef struct HuffmanNode {
  uint8_t data;

  HuffmanNode *left;
  HuffmanNode *right;
} HuffmanNode;

/* for ease of return and distinction */
typedef struct CompressedData {
  uint8_t *data;
  uint32_t data_size;
} CompressedData;

/* for ease of return and distinction */
typedef struct UncompressedData {
  uint8_t *data;
  uint32_t data_size;
} UncompressedData;

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
 * @see UncompressedData uncompressWithHuffman();
 */
CompressedData compressWithHuffman(uint8_t *data, uint32_t data_size);

/**
 * @brief uncompresses data compressed by compressWithHuffman
 *
 * @param data is data to be uncompressed
 * @param data_size is the size of the data to be uncompressed
 *
 * @see CompressedData compressWithHuffman();
 */
UncompressedData uncompressWithHuffman(uint8_t *data, uint32_t data_size);

#endif
