#include <CM/basic_huffman.h>
#include <stdio.h>
#include <stdlib.h>

/* huffman node */
typedef struct HuffmanNode {
  uint8_t data;

  struct HuffmanNode *left;
  struct HuffmanNode *right;
} HuffmanNode;

uint8_t *compressWithHuffman(uint8_t *data, uint32_t data_size,
                             uint32_t *out_size) {
}
