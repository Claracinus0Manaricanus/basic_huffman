#include <CM/basic_huffman.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
  HuffmanNode *data;
  int count;
  struct ListNode *next;
} ListNode;

/* inserts into priority list and returns new root */
ListNode *priorityInsert(ListNode *root, ListNode *toInsert) {
  if (root == NULL)
    return toInsert;

  if (toInsert->count < root->count) {
    toInsert->next = root;
    return toInsert;
  }

  ListNode *lnode = root;
  while (lnode->next != NULL) {
    if (toInsert->count < lnode->next->count) {
      toInsert->next = lnode->next;
      lnode->next = toInsert;
      break;
    }
    lnode = lnode->next;
  }

  if (toInsert->next == NULL) {
    lnode->next = toInsert;
  }

  return root;
}

/* do not call Bit functions without initializing these values properly */
int current_byte_bit_len = 0; // should be 0 init
int current_size = 0;         // in bytes
int compressed_data_len = 0; // should be what compressed_data's size it at init
uint8_t *compressed_data = NULL; // should be allocated
char current_byte = 0;           // should be 0 init

void writeBit(int bit) {
  current_byte = (current_byte << 1) | bit;
  current_byte_bit_len++;

  if (current_byte_bit_len == 8) {
    compressed_data[current_size] = current_byte;
    current_byte = 0;
    current_size++;
    current_byte_bit_len = 0;

    if (current_size == compressed_data_len) {
      compressed_data_len = compressed_data_len + (current_size / 6);
      compressed_data =
          (uint8_t *)realloc(compressed_data, compressed_data_len);
    }
  }
}

void finalizeBitWrite() {
  if (current_byte_bit_len <= 5) {
    compressed_data[current_size] =
        (current_byte << (8 - current_byte_bit_len)) |
        (5 - current_byte_bit_len);
    current_size++;
    compressed_data = (uint8_t *)realloc(compressed_data, current_size);
  } else {
    compressed_data = (uint8_t *)realloc(compressed_data, current_size + 2);
    compressed_data[current_size] =
        (current_byte << (8 - current_byte_bit_len));
    current_size++;
    compressed_data[current_size] = (13 - current_byte_bit_len);
    current_size++;
  }
}

void writeHuffmanTree(HuffmanNode *node, int depth) {
  if (node->right != NULL && node->left != NULL) {
    writeHuffmanTree(node->left, depth + 1);
    writeHuffmanTree(node->right, 0);
  } else {
    for (int i = 0; i < depth; i++) {
      writeBit(0);
    }
    writeBit(1);

    for (int i = 7; i >= 0; i--) {
      writeBit((node->data >> i) & 1);
    }
  }
}

uint8_t *compressWithHuffman(uint8_t *data, uint32_t data_size,
                             uint32_t *out_size, HuffmanNode **getTree) {
  int counts[256] = {0};
  ListNode *root = NULL;

  /* counting byte types */
  for (int i = 0; i < data_size; i++)
    counts[data[i]]++;

  /* priority queue creation */
  for (int i = 0; i < 256; i++) {
    if (counts[i] == 0)
      continue;

    /* node creation */
    ListNode *tmpNode = malloc(sizeof(ListNode));
    tmpNode->data = malloc(sizeof(HuffmanNode));
    tmpNode->data->data = i;
    tmpNode->data->left = NULL;
    tmpNode->data->right = NULL;
    tmpNode->count = counts[i];
    tmpNode->next = NULL;

    /* insertion */
    root = priorityInsert(root, tmpNode);
  }

  /* huffman tree from priority queue */
  while (root->next != NULL) {
    HuffmanNode *hnode = malloc(sizeof(HuffmanNode));
    hnode->data = 0;
    hnode->left = root->data;
    hnode->right = root->next->data;

    /* updating frequency */
    root->count += root->next->count;

    ListNode *tmpNode = root->next;

    root->data = hnode;
    root->next = tmpNode->next;

    free(tmpNode);

    if (root->next == NULL)
      break;

    tmpNode = root;
    root = root->next;

    /* insertion */
    root = priorityInsert(root, tmpNode);
  }

  /* compressing data according to tree */
  HuffmanCode *codes = getCodes(root->data);
  compressed_data_len = (data_size / 10) * 6;
  compressed_data = malloc(compressed_data_len);

  /* adding huffman tree to the beginning */
  writeHuffmanTree(root->data, 0);

  /* encoding and writing the data */
  for (int i = 0; i < data_size; i++) {
    HuffmanCode code = codes[data[i]];
    for (int j = 0; j < code.code_len; j++) {
      writeBit((code.code[j / 32] >> (j % 32)) & 1);
    }
  }

  finalizeBitWrite();

  /* cleaning */
  if (getTree != NULL) {
    *getTree = root->data;
  } else {
    freeTree(root->data);
    free(root);
  }
  freeCodes(codes);

  (*out_size) = current_size;
  return compressed_data;
}

uint8_t *uncompressWithHuffman(uint8_t *data, uint32_t data_size,
                               uint32_t *out_size) {
}

void getCodesHelper(HuffmanNode *node, HuffmanCode *codeArr,
                    uint32_t currentCode[4], uint32_t currentCodeLen) {
  if (node->left != NULL && node->right != NULL) {
    getCodesHelper(node->left, codeArr, currentCode, currentCodeLen + 1);

    uint32_t currentCodeRight[4] = {0};
    for (int i = 0; i < 4; i++)
      currentCodeRight[i] = currentCode[i];
    currentCodeRight[currentCodeLen / 32] =
        currentCodeRight[currentCodeLen / 32] | (1 << (currentCodeLen % 32));

    getCodesHelper(node->right, codeArr, currentCodeRight, currentCodeLen + 1);
  } else {
    for (int i = 0; i < 4; i++)
      codeArr[node->data].code[i] = currentCode[i];
    codeArr[node->data].code_len = currentCodeLen;
  }
}

HuffmanCode *getCodes(HuffmanNode *root) {
  HuffmanCode *arr = calloc(256, sizeof(HuffmanCode));

  uint32_t currentCode[4] = {0};
  uint32_t currentCodeLen = 0;

  getCodesHelper(root, arr, currentCode, currentCodeLen);

  return arr;
}

void freeCodes(HuffmanCode *array) {
  free(array);
}

void freeTree(HuffmanNode *root) {
  if (root == NULL)
    return;

  freeTree(root->left);
  freeTree(root->right);

  free(root);
}
