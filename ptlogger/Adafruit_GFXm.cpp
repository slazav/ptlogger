#ifdef __AVR__
 #include <avr/pgmspace.h>
#elif defined(ESP8266)
 #include <pgmspace.h>
#endif
#include "Adafruit_GFXm.h"
#include "glcdfont.c"

// Many (but maybe not all) non-AVR board installs define macros
// for compatibility with existing PROGMEM-reading AVR code.
// Do our own checks and defines here for good measure...

#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

// Pointers are a peculiar case...typically 16-bit on AVR boards,
// 32 bits elsewhere.  Try to accommodate both...

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

Adafruit_GFXm::Adafruit_GFXm(int16_t w, int16_t h):  WIDTH(w), HEIGHT(h) {
  _width    = WIDTH;
  _height   = HEIGHT;
  cursor_y  = cursor_x = 0;
}

size_t Adafruit_GFXm::write(uint8_t c) {
  if(c == '\n') {
    cursor_y += 8;
    cursor_x  = 0;
  } else if(c == '\r') {
    // skip em
  } else {
    if((cursor_x + 6) >= _width) { // Heading off edge?
      cursor_x  = 0;            // Reset x to zero
      cursor_y += 8;            // Advance y one line
    }
    drawChar(cursor_x, cursor_y, c);
    cursor_x += 6;
  }
  return 1;
}

// Draw a character
void Adafruit_GFXm::drawChar(int16_t x, int16_t y, unsigned char c) {

  if((x >= _width)     || // Clip right
     (y >= _height)    || // Clip bottom
     ((x + 6 - 1) < 0) || // Clip left
     ((y + 8 - 1) < 0))   // Clip top
    return;

  if (c >= 176) c++; // Handle 'classic' charset behavior

  for(int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if(i < 5) line = pgm_read_byte(font+(c*5)+i);
    else      line = 0x0;
    for(int8_t j=0; j<8; j++, line >>= 1) {
      if(line & 0x1) drawPixel(x+i, y+j, 1);
      else  drawPixel(x+i, y+j, 0);
    }
  }
}

void Adafruit_GFXm::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

int16_t Adafruit_GFXm::getCursorX(void) const {
  return cursor_x;
}

int16_t Adafruit_GFXm::getCursorY(void) const {
  return cursor_y;
}

// Return the size of the display (per current rotation)
int16_t Adafruit_GFXm::width(void) const {
  return _width;
}

int16_t Adafruit_GFXm::height(void) const {
  return _height;
}

