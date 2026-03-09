#ifndef CM_BASIC_HUFFMAN_HEADER
#define CM_BASIC_HUFFMAN_HEADER

#include <stdint.h>

#define _Nullable

/* huffman node, left branch is a 0 right branch is a 1. Huffman tree is a
 * strictly full binary tree. */
typedef struct HuffmanNode {
  uint8_t data;

  struct HuffmanNode *left;
  struct HuffmanNode *right;
} HuffmanNode;

/* huffman code */
typedef struct HuffmanCode {
  /* bit code, array because fuck you I want them to be bits */
  uint32_t code[4];

  /* length in bits */
  uint32_t code_len;
} HuffmanCode;

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
 * @param out_size is the data size in bytes of data returned by this function
 * @param getTree is a pointer to a pointer which will
 * be set to the root HuffmanNode generated bby this function if not NULL
 *
 * @see ArrayData uncompressWithHuffman();
 */
uint8_t *compressWithHuffman(uint8_t *data, uint32_t data_size,
                             uint32_t *out_size,
                             HuffmanNode **_Nullable getTree);

/**
 * @brief uncompresses data compressed by compressWithHuffman
 *
 * @param data to be uncompressed
 * @param data_size is the size of the data to be uncompressed
 *
 * @see ArrayData compressWithHuffman();
 */
uint8_t *uncompressWithHuffman(uint8_t *data, uint32_t data_size,
                               uint32_t *out_size);

/**
 * @brief returns an array that contains encodings of bytes by the given tree,
 * the returned array is exactly 256 elements long
 *
 * @param root root node of the tree to be printed
 * @param mustBeZero is a paramater that must be zero
 */
HuffmanCode *getCodes(HuffmanNode *root);

/**
 * @brief call on HuffmanCode arrays returned from getCodes
 *
 * @param array is the array to be freed
 */
void freeCodes(HuffmanCode *array);

/**
 * @brief frees the tree given in the parameters
 *
 * @param root root node for the tree to be freed
 */
void freeTree(HuffmanNode *root);

#endif
