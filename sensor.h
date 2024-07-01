#include <RTClib.h>
#include <BH1750.h>
#include <Wire.h>
#include <DHT.h>
#include <SD.h>
#include <FS.h>

#define DHT_TYPE DHT22
#define PIN_DHT_1 25
#define PIN_DHT_2 26

RTC_DS3231 rtc;
BH1750 lightMeter;
DHT dht1(PIN_DHT_1, DHT_TYPE);
DHT dht2(PIN_DHT_2, DHT_TYPE);

String hari, tanggal, bulan, tahun, jam, menit, detik;
int luxMeter, setLux;
float suhuAtas, suhuBawah;
byte kelembabanAtas, kelembabanBawah;

void getDataSuhu() {
  suhuAtas = dht1.readTemperature();
  suhuBawah = dht2.readTemperature();
}

void getDataKelembaban() {
  kelembabanAtas = dht1.readHumidity() - 10;
  kelembabanBawah = dht2.readHumidity();
}

void getDataLux() {
  luxMeter = lightMeter.readLightLevel();
  luxMeter = luxMeter / 0.52;
}

void RTCinit() {
  if (!rtc.begin()) {
    Serial.println(F("Modul RTC tidak ditemukan!"));
  }

  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (rtc.lostPower()) {
    Serial.println(F("RTC lost power, let's set the time!"));
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void getDataRTC() {
  DateTime now = rtc.now();

  tanggal = String(now.day(), DEC);
  bulan = String(now.month(), DEC);
  tahun = String(now.year(), DEC);
  jam = String(now.hour(), DEC);
  menit = String(now.minute(), DEC);
  detik = String(now.second(), DEC);

  if (detik.length() < 2) {
    detik = "0" + detik;
  }
  if (menit.length() < 2) {
    menit = "0" + menit;
  }
  if (jam.length() < 2) {
    jam = "0" + jam;
  }
  if (tanggal.length() < 2) {
    tanggal = "0" + tanggal;
  }
  if (bulan.length() < 2) {
    bulan = "0" + bulan;
  }
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to open file for writing"));
    return;
  }
  if (file.print(message)) {
    Serial.println(F("File written"));
  } else {
    Serial.println(F("Write failed"));
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println(F("Failed to open file for appending"));
    return;
  }
  if (file.print(message)) {
    Serial.println(F("Message appended"));
  } else {
    Serial.println(F("Append failed"));
  }
  file.close();
}