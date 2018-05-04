// gamma correction table from https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
const uint8_t PROGMEM gamma8[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};


uint8_t white(uint32_t c) {
  return (c >> 24);
}
uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}

uint32_t color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) |
         ((uint32_t)g << 8) |
         b;
}
uint32_t color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  return ((uint32_t)w << 24) |
         ((uint32_t)r << 16) |
         ((uint32_t)g << 8) |
         b;
}

uint32_t dim(uint32_t color, uint8_t brightness) {
  uint8_t r = red(color);
  uint8_t g = green(color);
  uint8_t b = blue(color);
  uint8_t w = white(color);

  r = (r * brightness) >> 8;
  g = (g * brightness) >> 8;
  b = (b * brightness) >> 8;
  w = (w * brightness) >> 8;

  return ((uint32_t)w << 24) |
         ((uint32_t)r << 16) |
         ((uint32_t)g << 8) |
         b;
}

uint32_t gammaDim(uint32_t color, uint8_t brightness) {
  uint8_t r = red(color);
  uint8_t g = green(color);
  uint8_t b = blue(color);
  uint8_t w = white(color);

  r = (r * brightness) >> 8;
  g = (g * brightness) >> 8;
  b = (b * brightness) >> 8;
  w = (w * brightness) >> 8;

  return gammaColor(r, g, b, w);
}

uint32_t gammaDim(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
  r = (r * brightness) >> 8;
  g = (g * brightness) >> 8;
  b = (b * brightness) >> 8;

  return gammaColor(r, g, b);
}

uint32_t gammaDim(uint8_t r, uint8_t g, uint8_t b, uint8_t w, uint8_t brightness) {
  r = (r * brightness) >> 8;
  g = (g * brightness) >> 8;
  b = (b * brightness) >> 8;
  w = (w * brightness) >> 8;

  return gammaColor(r, g, b, w);
}

uint32_t gammaColor(uint32_t color) {
  uint8_t r = red(color);
  uint8_t g = green(color);
  uint8_t b = blue(color);
  uint8_t w = white(color);

  return gammaColor(r, g, b, w);
}

uint32_t gammaColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  return ((uint32_t)pgm_read_byte(&gamma8[w]) << 24) |
         ((uint32_t)pgm_read_byte(&gamma8[r]) << 16) |
         ((uint32_t)pgm_read_byte(&gamma8[g]) << 8) |
         pgm_read_byte(&gamma8[b]);
}

uint32_t gammaColor(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)pgm_read_byte(&gamma8[r]) << 16) |
         ((uint32_t)pgm_read_byte(&gamma8[g]) << 8) |
         pgm_read_byte(&gamma8[b]);
}

