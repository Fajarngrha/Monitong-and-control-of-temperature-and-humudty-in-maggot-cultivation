#include <SPI.h>
#include <TFT_eSPI_ILI9341_2.h>

#include "logo.h"
#include "icon.h"

#define BIRU 0x0176
#define HIJAU 0x0673
#define MERAH 0xC945
#define OREN 0xE4A3
#define PUTIH 0xFFFF
#define HITAM 0x0000
#define PI 3.14159265358979323846264338327120

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite txtWaktu = TFT_eSprite(&tft);
TFT_eSprite txtTanggal = TFT_eSprite(&tft);
TFT_eSprite txtSuhuAtas = TFT_eSprite(&tft);
TFT_eSprite txtSuhuBawah = TFT_eSprite(&tft);
TFT_eSprite txtKelembabanAtas = TFT_eSprite(&tft);
TFT_eSprite txtKelembabanBawah = TFT_eSprite(&tft);
TFT_eSprite txtLux = TFT_eSprite(&tft);
TFT_eSprite txtSetLux = TFT_eSprite(&tft);

void openingDisplay() {
  tft.fillScreen(PUTIH);
  tft.pushImage(20, 58, 280, 124, X_Maggot_logo_0);
}

void loadingDisplay() {
  uint16_t col[8] = { 0xBE3C, 0xA5BA, 0x8D18, 0x7478, 0x5BD6, 0x4354, 0x2AB4, BIRU };

  for (int i = 8; i > 0; i--) {
    tft.fillCircle(160 + 30 * (cos(-i * PI / 4)), 120 + 30 * (sin(-i * PI / 4)), 8, col[0]);
    delay(15);
    tft.fillCircle(160 + 30 * (cos(-(i + 1) * PI / 4)), 120 + 30 * (sin(-(i + 1) * PI / 4)), 8, col[1]);
    delay(15);
    tft.fillCircle(160 + 30 * (cos(-(i + 2) * PI / 4)), 120 + 30 * (sin(-(i + 2) * PI / 4)), 8, col[2]);
    delay(15);
    tft.fillCircle(160 + 30 * (cos(-(i + 3) * PI / 4)), 120 + 30 * (sin(-(i + 3) * PI / 4)), 8, col[3]);
    delay(15);
    tft.fillCircle(160 + 30 * (cos(-(i + 4) * PI / 4)), 120 + 30 * (sin(-(i + 4) * PI / 4)), 8, col[4]);
    delay(15);
    tft.fillCircle(160 + 30 * (cos(-(i + 5) * PI / 4)), 120 + 30 * (sin(-(i + 5) * PI / 4)), 8, col[5]);
    delay(15);
    tft.fillCircle(160 + 30 * (cos(-(i + 6) * PI / 4)), 120 + 30 * (sin(-(i + 6) * PI / 4)), 8, col[6]);
    delay(15);
    tft.fillCircle(160 + 30 * (cos(-(i + 7) * PI / 4)), 120 + 30 * (sin(-(i + 7) * PI / 4)), 8, col[7]);
    delay(15);
  }
}

void bottomBar() {
  tft.fillRect(0, 190, 125, 50, HIJAU);
  tft.pushImage(8, 195, 40, 40, X_Maggot_logo_1);
  tft.pushImage(50, 208, 67, 15, X_Maggot_logo_2);

  tft.fillRect(125, 190, 195, 50, HITAM);
  tft.pushImage(281, 200, 30, 30, settings);

  tft.drawFastHLine(319, 239, 1, HITAM);
}

void homeMenu1() {
  tft.fillRect(0, 0, 320, 190, BIRU);
  tft.fillRect(0, 0, 210, 40, PUTIH);
  tft.fillTriangle(210, 0, 210, 40, 240, 0, PUTIH);
  tft.setFreeFont(&Inter_Bold8pt7b);
  tft.setTextColor(HITAM);
  tft.drawString("SUHU & KELEMBABAN", 107, 17.5);
  tft.setTextColor(PUTIH);
  tft.drawString("LUX", 283.5, 17.5);

  tft.pushImage(19, 51, 50, 50, up);
  tft.setFreeFont(&Inter_Bold8pt7b);
  tft.setTextColor(PUTIH);
  tft.drawString("Bagian", 117, 64.5);
  tft.drawString("Atas", 117, 85.5);
  tft.drawString("Bagian", 203, 64.5);
  tft.drawString("Bawah", 203, 85.5);
  tft.pushImage(250, 51, 50, 50, down);
  tft.fillSmoothRoundRect(158, 55, 4, 120, 2, PUTIH, BIRU);
  tft.fillRect(13, 110, 65, 65, MERAH);
  tft.fillRect(85, 110, 65, 65, OREN);
  tft.fillRect(171, 110, 65, 65, MERAH);
  tft.fillRect(243, 110, 65, 65, OREN);

  tft.pushImage(18, 117, 30, 30, temperature);
  tft.pushImage(93, 117, 30, 30, humidity);
  tft.pushImage(177, 117, 30, 30, temperature);
  tft.pushImage(251, 117, 30, 30, humidity);

  tft.setFreeFont(&Inter_Bold10pt7b);
  tft.setTextColor(HITAM);
  tft.drawString("C", 60, 135);
  tft.drawString("%", 132, 132);
  tft.drawString("C", 219, 135);
  tft.drawString("%", 290, 132);
  tft.setFreeFont(&Inter_Bold8pt7b);
  tft.drawString("o", 50, 122.5);
  tft.drawString("o", 209, 122.5);

  tft.drawFastHLine(319, 239, 1, HITAM);
}

void homeMenu2() {
  tft.fillRect(0, 0, 320, 190, BIRU);
  tft.fillRect(240, 0, 80, 40, PUTIH);
  tft.fillTriangle(240, 0, 240, 40, 210, 0, PUTIH);
  tft.setFreeFont(&Inter_Bold8pt7b);
  tft.setTextColor(PUTIH);
  tft.drawString("SUHU & KELEMBABAN", 107, 17.5);
  tft.setTextColor(HITAM);
  tft.drawString("LUX", 283.5, 17.5);

  tft.pushImage(21, 65, 100, 100, lux);
  tft.setFreeFont(&Inter_Bold16pt7b);
  tft.setTextColor(PUTIH);
  tft.drawString("lx", 284, 107.5);

  tft.drawFastHLine(319, 239, 1, HITAM);
}

void setParameterLux() {
  tft.fillRect(0, 0, 320, 190, BIRU);
  tft.fillRect(0, 0, 320, 40, PUTIH);
  tft.setFreeFont(&Inter_Bold8pt7b);
  tft.setTextColor(HITAM);
  tft.drawString("Set Parameter Lux", 160, 17.5);

  tft.pushImage(85, 92, 150, 45, set);
  tft.pushImage(282, 200, 30, 30, home);

  tft.drawFastHLine(319, 239, 1, HITAM);
}