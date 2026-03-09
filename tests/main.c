#include <CM/basic_huffman.h>
#include <stdio.h>

int main(int argc, char **argv) {
  const uint32_t arr_len = 16;
  uint32_t arr[arr_len];

  for (int i = 0; i < arr_len; i++)
    arr[i] = i + 1;

  uint8_t *data = (uint8_t *)arr;
  uint32_t data_size = arr_len * 4;

  uint32_t returned_size = 0;
  HuffmanNode *hroot;
  uint8_t *compressed_data =
      compressWithHuffman(data, data_size, &returned_size, &hroot);

  HuffmanCode *codes = getCodes(hroot);

  for (int i = 0; i < 256; i++) {
    if (codes[i].code_len == 0)
      continue;

    printf("data:  %x \tcodeLen:  %d\tcode:  ", i, codes[i].code_len);
    for (int j = 0; j < codes[i].code_len; j++) {
      printf("%c ", ((codes[i].code[j / 32] >> (j % 32)) & 1) ? '1' : '0');
    }
    printf("\n");
  }

  printf("compresssed %d bytes to %d bytes\n%.1f%% of original filesize\n",
         data_size, returned_size,
         100.0f * (float)returned_size / (float)data_size);

  freeCodes(codes);
  freeTree(hroot);
  return 0;
}
