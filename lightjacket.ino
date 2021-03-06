#include <Adafruit_NeoPixel.h>

#define PIN_STRIP  6
#define N_LEDS 57


// global state
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN_STRIP, NEO_GRBW + NEO_KHZ800);


// modes
enum modes {
  MODE_PALETTE_FADER,
  // MODE_PALETTE_SNAKE,
  MODE_RAINBOW,

  MODES_COUNT
};
uint8_t currentMode = MODE_PALETTE_FADER;


// colors
enum colorNames {
  COLOR_PTW_BLUE,
  COLOR_PTW_GREEN,
  // COLOR_JARVUS,
  // COLOR_WHITE,
  // COLOR_PINK,
  // COLOR_RED,
  // COLOR_BLUE,
  // COLOR_GREEN,

  // COLOR_BLACKLIGHT,
  // COLOR_HIGHPRESSURESODIUM,
  // COLOR_SKY,
  // COLOR_MAGENTA,
  // COLOR_YELLOW,
  // COLOR_GOLD,
  // COLOR_VIOLET,

  // COLOR_OCEAN,
  // COLOR_SADDLE,

  COLORS_COUNT
};
uint32_t colors[COLORS_COUNT];
uint32_t gammaColors[COLORS_COUNT];


// lifecycle methods
void setup() {
  // initialize colors
  colors[COLOR_PTW_BLUE] = color(20, 178, 204);
  colors[COLOR_PTW_GREEN] = color(127, 193, 66);
  // colors[COLOR_JARVUS] = color(255, 127, 32);
  // colors[COLOR_WHITE] = color(0, 0, 0, 255);
  // colors[COLOR_PINK] = color(255,105,180);
  // colors[COLOR_RED] = color(255, 0, 0);
  // colors[COLOR_BLUE] = color(0, 50, 255);
  // colors[COLOR_GREEN] = color(0, 255, 0);

  // colors[COLOR_BLACKLIGHT] = color(167, 0, 255);
  // colors[COLOR_HIGHPRESSURESODIUM] = color(255, 183, 76);
  // colors[COLOR_SKY] = color(135, 206, 250);
  // colors[COLOR_MAGENTA] = color(255, 0, 255);

  // colors[COLOR_YELLOW] = color(255, 255, 0);
  // colors[COLOR_GOLD] = color(255, 215, 0);
  // colors[COLOR_VIOLET] = color(238, 130, 238);

  // colors[COLOR_OCEAN] = color(0, 119, 190);
  // colors[COLOR_SADDLE] = color(	139, 69, 19);

  // initialize gamma-corrected colors
  for (uint8_t i = 0; i < COLORS_COUNT; i++) {
    gammaColors[i] = gammaColor(colors[i]);
  }

  // select random modes
  randomSeed(analogRead(0));
  currentMode = random(0, MODES_COUNT);

  // initialize serial communications at 9600 bps:
  Serial.begin(9600);

  // reset strip
  strip.begin();
  strip.show();
}

void loop() {

  switch (currentMode) {
    case MODE_PALETTE_FADER:
      paletteFader();
      break;
    // case MODE_PALETTE_SNAKE:
    //   paletteSnake();
    case MODE_RAINBOW:
      rainbow();
      break;
  }


  // render to strip
  strip.show();


  // tap the breaks
  delay(10);
}


// PALETTE FADER
#define PALETTE_FADER_CHUNKLEN 4
uint8_t paletteFader_brightness = 0;
bool paletteFader_incoming = true;
uint16_t paletteFader_sleepCycles = 0;
uint8_t paletteFader_startColorIndex = 0;

void paletteFader() {
  uint16_t colorIndex = paletteFader_startColorIndex, pixelIndex;

  if (paletteFader_sleepCycles > 0) {
    paletteFader_sleepCycles--;
    return;
  }

  for (pixelIndex = 0; pixelIndex < N_LEDS; pixelIndex++) {
      // COLORS_COUNT
      strip.setPixelColor(pixelIndex, gammaDim(colors[colorIndex % COLORS_COUNT], paletteFader_brightness));

      if (pixelIndex % PALETTE_FADER_CHUNKLEN == 0) {
        colorIndex++;
      }
  }

  if (paletteFader_incoming) {
    paletteFader_brightness += 5;

    // handle reaching end of fade-in
    if (paletteFader_brightness >= 255) {
      paletteFader_brightness = 255;
      paletteFader_incoming = false;
      paletteFader_sleepCycles = 100;
    }
  } else {
    paletteFader_brightness -= 5;

    // handle reaching end of fade-out
    if (paletteFader_brightness <= 0) {
      paletteFader_brightness = 0;
      paletteFader_incoming = true;
      paletteFader_sleepCycles = 0;

      paletteFader_startColorIndex++;

      if (paletteFader_startColorIndex == COLORS_COUNT) {
        paletteFader_startColorIndex = 0;
      }
    }
  }
}



// PALETTE SNAKE
// #define PALETTE_SNAKE_CHUNKLEN 4
// #define PALETTE_SNAKE_MOVEDELAY 20
// uint8_t paletteSnake_offset = 0;
// uint16_t paletteSnake_waitCycles = PALETTE_SNAKE_MOVEDELAY;

// void paletteSnake() {
//   uint16_t colorIndex = 0, pixelIndex;

//   for (pixelIndex = 0; pixelIndex < N_LEDS; pixelIndex++) {
//       if ((pixelIndex + paletteSnake_offset) % PALETTE_SNAKE_CHUNKLEN == 0) {
//         colorIndex++;
//       }

//       // COLORS_COUNT
//       strip.setPixelColor(pixelIndex, gammaColors[colorIndex % COLORS_COUNT]);
//   }


//   // move every MOVEDELAY
//   paletteSnake_waitCycles--;
//   if (paletteSnake_waitCycles == 0) {
//     paletteSnake_waitCycles = PALETTE_SNAKE_MOVEDELAY;

//     paletteSnake_offset++;
//     // if (paletteSnake_offset == PALETTE_FADER_CHUNKLEN) {
//     //   paletteSnake_offset = 0;
//     // }
//   }
// }


// RAINBOW
uint16_t rainbow_cycle = 0;

void rainbow() {
  uint16_t pixelIndex;

  for (pixelIndex = 0; pixelIndex < N_LEDS; pixelIndex++) {
    strip.setPixelColor(pixelIndex, Wheel(((pixelIndex * 256 / N_LEDS) + rainbow_cycle) & 255));
  }

  rainbow_cycle++;
}
