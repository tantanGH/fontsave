#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iocslib.h>

#define VERSION "0.1.0 (2023/02/06)"

static uint8_t* get_font_addr(uint16_t code) {

  struct REGS in_regs = { 0 };
  struct REGS out_regs = { 0 };

  in_regs.d0 = 0x16;      // IOCS _FNTADR
  in_regs.d1 = code;
  in_regs.d2 = 8;

  TRAP15(&in_regs, &out_regs);

  return (uint8_t*)out_regs.d0;
}

static void show_help() {
  printf("usage: fontsave <mode> <out-file>\n");
  printf(" mode: 0:8x16 * 128chars (2,048bytes)\n");
  printf("       1:8x16 * 256chars (4,096bytes)\n");
  printf("       2:8x16 * 256chars + 16x16 * non-kanji (49,216bytes)\n");
  printf("       3:8x16 * 256chars + 16x16 * non-kanji+1st level kanji (145,472bytes)\n");
  printf("       4:8x16 * 256chars + 16x16 * non-kanji+1st/2nd level kanji (286,848bytes)\n");
}

int32_t main(int32_t argc, uint8_t* argv[]) {

  int32_t rc = 1;

  printf("FONTSAVE.X - Active Font Data Saver " VERSION " by tantan\n");

  if (argc < 3) {
    show_help();
    goto exit;
  }

  int16_t mode = atoi(argv[1]);
  if (mode < 0 || mode > 4) {
    show_help();
    goto exit;
  }

  uint8_t* out_file = argv[2];
  FILE* fp = fopen(out_file,"wb");
  if (fp == NULL) {
    printf("error: file open error.\n");
    goto exit;

  }

  B_SUPER(0);

  for (int16_t i = 0; i < 128; i++) {
    uint8_t* font_addr = get_font_addr(i);
    fwrite(font_addr, 1, 16, fp);
  }

  if (mode >= 1) {
    for (int16_t i = 0; i < 128; i++) {
      uint8_t* font_addr = get_font_addr(i + 128);
      fwrite(font_addr, 1, 16, fp);
    }
  }

  if (mode >= 2) {
    for (int16_t i = 0; i < 15; i++) {
      for (int16_t j = 0; j < 94; j++) {
        uint8_t* font_addr = get_font_addr(JISSFT(0x2121 + i * 0x100 + j));   // in some font drivers, only SJIS is hooked
        fwrite(font_addr, 1, 32, fp);
      }
    }
  }

  if (mode >= 3) {
    for (int16_t i = 0; i < 32; i++) {
      for (int16_t j = 0; j < 94; j++) {
        uint8_t* font_addr = get_font_addr(JISSFT(0x2121 + (i + 15) * 0x100 + j));
        fwrite(font_addr, 1, 32, fp);
      }
    }
  }

  if (mode >= 4) {
    for (int16_t i = 0; i < 47; i++) {
      for (int16_t j = 0; j < 94; j++) {
        uint8_t* font_addr = get_font_addr(JISSFT(0x2121 + (i + 15 + 32) * 0x100 + j));
        fwrite(font_addr, 1, 32, fp);
      }
    }
  }

  size_t out_size = ftell(fp);

  fclose(fp);

  printf("Saved font data to %s (%d bytes) successfully.\n", out_file, out_size);

  rc = 0;

exit:
  return rc;
}