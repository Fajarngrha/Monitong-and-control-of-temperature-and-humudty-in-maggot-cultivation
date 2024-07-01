#include <WiFi.h>
#include <Preferences.h>
#include <ThingsBoard.h>

#include "parameter.h"
#include "display.h"
#include "sensor.h"

WiFiClient wifiClient;
ThingsBoard tb(wifiClient);
Preferences preferences;

/* ------ Fungsi Reconnecting ke Thingsboard ------ */
void tbReconnect() {
  Serial.println(F("Connecting to Thingsboard ... "));
  if (tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
    isDeviceConnectedToServer = true;
    Serial.println(F("Thingsboard connected"));
  } else {
    isDeviceConnectedToServer = false;
    Serial.println(F("Thingsboard connection failed"));
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);

  preferences.begin("parameterLux", false);
  setLux = preferences.getInt("setLux", 1000);

  digitalWrite(SD_CS, HIGH);
  digitalWrite(TFT_CS, HIGH);
  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, HIGH);

  Wire.begin();
  lightMeter.begin();
  dht1.begin();
  dht2.begin();
  RTCinit();

  SD.begin(SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println(F("Card Mount Failed"));
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println(F("No SD card attached"));
    return;
  }
  Serial.println(F("Initializing SD card..."));
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;  // init failed
  }

  tft.init();
  tft.setRotation(1);
  tft.setTouch(calData);
  tft.setSwapBytes(true);
  tft.setTextDatum(4);
  txtWaktu.setTextDatum(4);
  txtTanggal.setTextDatum(4);
  txtSuhuAtas.setTextDatum(4);
  txtSuhuBawah.setTextDatum(4);
  txtKelembabanAtas.setTextDatum(4);
  txtKelembabanBawah.setTextDatum(4);
  txtLux.setTextDatum(4);
  txtSetLux.setTextDatum(4);

  openingDisplay();
  delay(2000);
  WiFi.begin(username, password);
  tft.fillScreen(PUTIH);
  for (int i = 0; i < 3; i++) {
    loadingDisplay();
  }
  homeMenu1();
  bottomBar();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    isNetworkAvailable = true;
    digitalWrite(LED_PIN, HIGH);
  }
  if (WiFi.status() != WL_CONNECTED) {
    isNetworkAvailable = false;
    digitalWrite(LED_PIN, LOW);
  }

  if (!tb.connected()) {
    tbReconnect();
  }
  tb.loop();

  /* ----- Logika nyala lampu terhadap nilai lux meter ----- */
  if (luxMeter < setLux && !isLux) {
    digitalWrite(RELAY_PIN_1, LOW);
    isLux = true;
  }

  if (luxMeter > setLux && isLux) {
    digitalWrite(RELAY_PIN_1, HIGH);
    isLux = false;
  }
  /* ------------------------------------------------------- */

  unsigned long start = millis();
  if (start - previousTimeData >= intervalGetData) {
    getDataRTC();
    getDataSuhu();
    getDataKelembaban();
    getDataLux();

    if (display == 0) {
      txtWaktu.createSprite(76, 19);
      txtWaktu.fillSprite(HITAM);
      txtWaktu.setTextColor(HIJAU);
      txtWaktu.setFreeFont(&Inter_Bold8pt7b);
      txtWaktu.drawString(jam + ":" + menit + ":" + detik, 38, 7.5);
      txtWaktu.pushSprite(165, 196);

      txtTanggal.createSprite(101, 19);
      txtTanggal.fillSprite(HITAM);
      txtTanggal.setTextColor(HIJAU);
      txtTanggal.setFreeFont(&Inter_Bold8pt7b);
      txtTanggal.drawString(tanggal + "/" + bulan + "/" + tahun, 50.5, 7.5);
      txtTanggal.pushSprite(153, 215);

      txtSuhuAtas.createSprite(37, 19);
      txtSuhuAtas.fillSprite(MERAH);
      txtSuhuAtas.setTextColor(HITAM);
      txtSuhuAtas.setFreeFont(&Inter_Bold8pt7b);
      txtSuhuAtas.drawFloat(suhuAtas, 1, 18.5, 7.5);
      txtSuhuAtas.pushSprite(27, 150);

      txtSuhuBawah.createSprite(37, 19);
      txtSuhuBawah.fillSprite(MERAH);
      txtSuhuBawah.setTextColor(HITAM);
      txtSuhuBawah.setFreeFont(&Inter_Bold8pt7b);
      txtSuhuBawah.drawFloat(suhuBawah, 1, 18.5, 7.5);
      txtSuhuBawah.pushSprite(185, 150);

      txtKelembabanAtas.createSprite(34, 19);
      txtKelembabanAtas.fillSprite(OREN);
      txtKelembabanAtas.setTextColor(HITAM);
      txtKelembabanAtas.setFreeFont(&Inter_Bold8pt7b);
      txtKelembabanAtas.drawNumber(kelembabanAtas, 17, 7.5);
      txtKelembabanAtas.pushSprite(101, 150);

      txtKelembabanBawah.createSprite(34, 19);
      txtKelembabanBawah.fillSprite(OREN);
      txtKelembabanBawah.setTextColor(HITAM);
      txtKelembabanBawah.setFreeFont(&Inter_Bold8pt7b);
      txtKelembabanBawah.drawNumber(kelembabanBawah, 17, 7.5);
      txtKelembabanBawah.pushSprite(259, 150);

      tft.drawFastHLine(319, 239, 1, HITAM);
    }

    if (display == 1) {
      txtLux.createSprite(111, 39);
      txtLux.fillSprite(BIRU);
      txtLux.setTextColor(PUTIH);
      txtLux.setFreeFont(&Inter_Bold16pt7b);
      txtLux.drawNumber(luxMeter, 55.5, 17.5);
      txtLux.pushSprite(140, 90);

      tft.drawFastHLine(319, 239, 1, HITAM);
    }

    String namaFile, isiFile;
    namaFile = "/" + tanggal + "-" + bulan + "-" + tahun + ".txt";
    isiFile = jam + ":" + menit + ":" + detik + ", ";
    isiFile += "Suhu Atas:" + String(suhuAtas, 1) + ", ";
    isiFile += "Kelembaban Atas:" + String(kelembabanAtas) + ", ";
    isiFile += "Suhu Bawah:" + String(suhuBawah, 1) + ", ";
    isiFile += "Kelembaban Bawah:" + String(kelembabanBawah) + ", ";
    isiFile += "Lux Meter:" + String(luxMeter);

    File file = SD.open(namaFile.c_str());
    if (!file) {
      Serial.println(F("Creating file..."));
      writeFile(SD, namaFile.c_str(), isiFile.c_str());
    } else {
      Serial.println(F("File already exists"));
    }

    Serial.print(F("Save data: "));
    Serial.println(isiFile);
    appendFile(SD, namaFile.c_str(), isiFile.c_str());

    previousTimeData = start;
  }

  if (start - previousSendData >= intervalSendData) {
    if (isNetworkAvailable && isDeviceConnectedToServer) {
      Serial.println(F("Sending data to Thingsboard"));
      tb.sendTelemetryData("Suhu Atas", suhuAtas);
      tb.sendTelemetryData("Suhu Bawah", suhuBawah);
      tb.sendTelemetryData("Kelembaban Atas", kelembabanAtas);
      tb.sendTelemetryData("Kelembaban Bawah", kelembabanBawah);
      tb.sendTelemetryData("Lux Meter", luxMeter);
      tb.sendTelemetryString("Latitude", String(-6.612630063190867).c_str());
      tb.sendTelemetryString("Longitude", String(106.86806361245799).c_str());
    }
    previousSendData = start;
  }

  if (display == 0) {
    if (tft.getTouch(&x, &y)) {
      if (x > 240 && x < 320 && y > 0 && y < 40) {
        homeMenu2();
        display = 1;
        delay(50);
      }
    }
  }

  if (display == 1) {
    if (tft.getTouch(&x, &y)) {
      if (x > 0 && x < 210 && y > 0 && y < 40) {
        homeMenu1();
        display = 0;
        delay(50);
      }
    }
  }

  if (display == 0 || display == 1) {
    if (tft.getTouch(&x, &y)) {
      if (x > 281 && x < 311 && y > 200 && y < 230) {
        setParameterLux();

        txtSetLux.createSprite(56, 19);
        txtSetLux.fillSprite(PUTIH);
        txtSetLux.setTextColor(HITAM);
        txtSetLux.setFreeFont(&Inter_Bold8pt7b);
        txtSetLux.drawNumber(setLux, 28, 7.5);
        txtSetLux.pushSprite(132, 105);

        tft.drawFastHLine(319, 239, 1, HITAM);
        display = 2;
        delay(50);
      }
    }
  }

  if (display == 2) {
    if (tft.getTouch(&x, &y)) {
      if (x > 90 && x < 126 && y > 96 && y < 132) {
        setLux -= 25;
        
        preferences.begin("parameterLux", false);
        preferences.putInt("setLux", setLux);
        preferences.end();

        txtSetLux.createSprite(56, 19);
        txtSetLux.fillSprite(PUTIH);
        txtSetLux.setTextColor(HITAM);
        txtSetLux.setFreeFont(&Inter_Bold8pt7b);
        txtSetLux.drawNumber(setLux, 28, 7.5);
        txtSetLux.pushSprite(132, 105);

        tft.drawFastHLine(319, 239, 1, HITAM);
        delay(75);
      }

      if (x > 232 && x < 268 && y > 65 && y < 101) {
        setLux += 25;
        
        preferences.begin("parameterLux", false);
        preferences.putInt("setLux", setLux);
        preferences.end();

        txtSetLux.createSprite(56, 19);
        txtSetLux.fillSprite(PUTIH);
        txtSetLux.setTextColor(HITAM);
        txtSetLux.setFreeFont(&Inter_Bold8pt7b);
        txtSetLux.drawNumber(setLux, 28, 7.5);
        txtSetLux.pushSprite(132, 105);

        tft.drawFastHLine(319, 239, 1, HITAM);
        delay(75);
      }

      if (x > 281 && x < 311 && y > 200 && y < 230) {
        homeMenu1();
        tft.pushImage(281, 200, 30, 30, settings);

        tft.drawFastHLine(319, 239, 1, HITAM);
        display = 0;
        delay(50);
      }
    }
  }
}