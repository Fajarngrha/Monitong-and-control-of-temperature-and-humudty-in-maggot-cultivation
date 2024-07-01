#define SD_CS 5
#define TFT_CS 15
#define LED_PIN 2
#define RELAY_PIN_1 32
#define RELAY_PIN_2 33

/* ----- Parameter Thingsboard ----- */
constexpr char TOKEN[] PROGMEM = "O8K46JZQfE7bZ23OQH4q";
constexpr char THINGSBOARD_SERVER[] PROGMEM = "thingsboard.cloud";
constexpr uint16_t THINGSBOARD_PORT PROGMEM = 1883;

/* ----- Parameter Wi-Fi ----- */
String username = "XMAGGOT";
String password = "12345678";

/* ----- Parameter Waktu Interval Data ----- */
unsigned long previousTimeData = 0;
unsigned long intervalGetData = 1000;
unsigned long previousSendData = 0;
unsigned long intervalSendData = 10000;

byte display = 0;
byte keyboard = 1;

uint16_t x, y;
uint16_t calData[5] = { 383, 3516, 275, 3476, 7 };

bool isLux = false;
bool isUsername = false;
bool isPassword = false;
bool isNetworkAvailable = false;
bool isDeviceConnectedToServer = false;