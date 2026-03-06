#include <CM/basic_huffman.h>

int main(int argc, char **argv) {
  const uint32_t arr_len = 16;
  uint32_t arr[arr_len];

  for (int i = 0; i < arr_len; i++)
    arr[i] = i + 1;

  compressWithHuffman((uint8_t *)arr, arr_len * 4);

  return 0;
}
