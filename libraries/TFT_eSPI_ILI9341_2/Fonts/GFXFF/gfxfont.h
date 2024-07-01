// Adopted by Bodmer to support TFT_eSPI library.

// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

#ifdef LOAD_GFXFF

typedef struct { // Data stored PER GLYPH
	uint32_t bitmapOffset;     // Pointer into GFXfont->bitmap
	uint8_t  width, height;    // Bitmap dimensions in pixels
	uint8_t  xAdvance;         // Distance to advance cursor (x axis)
	int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
	uint8_t  *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	uint16_t  first, last; // ASCII extents
	uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;

  // Original Adafruit_GFX "Free Fonts"
  #include <Fonts/GFXFF/TomThumb.h>  // TT1

  //Inter
  #include <Fonts/GFXFF/Inter_Bold8pt7b.h>
  #include <Fonts/GFXFF/Inter_Bold10pt7b.h>
  #include <Fonts/GFXFF/Inter_Bold12pt7b.h>
  #include <Fonts/GFXFF/Inter_Bold16pt7b.h>

  #include <Fonts/GFXFF/FreeSansBold9pt7b.h>  // FF25 or FSSB9
  #include <Fonts/GFXFF/FreeSansBold12pt7b.h> // FF26 or FSSB12
  #include <Fonts/GFXFF/FreeSansBold18pt7b.h> // FF27 or FSSB18
  #include <Fonts/GFXFF/FreeSansBold24pt7b.h> // FF28 or FSSB24

#endif // LOAD_GFXFF

#endif // _GFXFONT_H_
