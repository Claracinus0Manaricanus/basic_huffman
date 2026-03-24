#include <CM/basic_huffman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("usage: %s <c for compression or d for decompression> "
           "<filename>\nCompression example: %s c file.txt\nThis example "
           "outputs the file: file.txt.cmhuff",
           argv[0], argv[0]);
  }

  FILE *file = fopen(argv[2], "r");
  if (file == NULL) {
    printf("ERROR: couldn't open file %s\n", argv[2]);
    return 1;
  }

  fseek(file, 0, SEEK_END);
  size_t filesize = ftell(file);
  rewind(file);

  uint8_t *data = malloc(filesize);
  fread(data, 1, filesize, file);
  fclose(file);

  uint8_t *processed_data = NULL;
  uint32_t processed_data_len = 0;

  char outFileName[strlen(argv[2]) + 8];

  switch (argv[1][0]) {
  case 'c':
    processed_data =
        compressWithHuffman(data, filesize, &processed_data_len, NULL);
    if (processed_data == NULL) {
      free(data);
      printf("ERROR: failed compression\n");
      return 1;
    }

    printf("compressed file from %ld bytes to %d bytes\n%.1f%% of filesize\n",
           filesize, processed_data_len,
           100.0f * (float)processed_data_len / (float)filesize);

    sprintf(outFileName, "%s.cmhuff", argv[2]);
    break;
  case 'd':
    processed_data = uncompressWithHuffman(data, filesize, &processed_data_len);
    if (processed_data == NULL) {
      free(data);
      printf("ERROR: failed decompression\n");
      return 1;
    }

    printf("decompressed file from %ld bytes to %d bytes\n%.1f%% of filesize\n",
           filesize, processed_data_len,
           100.0f * (float)processed_data_len / (float)filesize);

    int i = 0;
    for (; i < strlen(argv[2]); i++) {
      if (argv[2][i] == '.')
        break;

      outFileName[i] = argv[2][i];
    }

    outFileName[i] = 0;
    break;
  default:
    printf("ERROR: invalid operation, must be c or d\n");
    free(data);
    return 1;
  }

  FILE *ofile = fopen(outFileName, "w");
  if (ofile == NULL) {
    printf("ERROR: failed writing to file %s\n", outFileName);
    free(data);
    free(processed_data);
    return 1;
  }

  fwrite(processed_data, 1, processed_data_len, ofile);
  fclose(ofile);

  free(data);
  free(processed_data);
  return 0;
}
