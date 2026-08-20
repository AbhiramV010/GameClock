#include <WiFi.h>
#include <WiFiUdp.h>

// alarm clock on a pico w, wakes user up with a memory game

const uint8_t LED_PINS[4]    = {22, 21, 20, 19}; // leftmost, left, right, rightmost
const uint8_t BUTTON_PINS[4] = {12, 13, 14, 15}; // leftmost, left, right, rightmost
const uint16_t TONES[4]      = {330, 392, 494, 587}; // one note per pad

const uint8_t BUZZER_PIN = 2;

// j1 display header, probably a tm1637, swap these if the numbers look garbled
const uint8_t TM_CLK = 17;
const uint8_t TM_DIO = 16;

// ---- alarm settings, edit these and reflash ----
const int ALARM_HOUR   = 7;
const int ALARM_MINUTE = 30;
const int ALARM_DISMISS_LEVEL = 5; // rounds in a row to shut it up

// ---- wifi + ntp, so the clock doesn't just drift off ----
const char* WIFI_SSID     = "your-network";
const char* WIFI_PASSWORD = "your-password";
const char* NTP_SERVER    = "pool.ntp.org";
const int NTP_PORT        = 123;
const int TIMEZONE_OFFSET_HOURS = 0; // the server speaks utc, shift to yours
const unsigned long RESYNC_INTERVAL_MS = 6UL * 3600UL * 1000UL; // roughly every 6 hours

const uint16_t SHOW_ON_MS  = 350;
const uint16_t SHOW_GAP_MS = 150;
const uint16_t DEBOUNCE_MS = 20;

uint8_t sequence[ALARM_DISMISS_LEVEL];

// ---- clock, kept honest by the ntp pool ----

unsigned long bootMillis;
int32_t baseSecondsOfDay;
unsigned long lastSyncMillis;
WiFiUDP ntpUdp;

void initClockFromBuildTime() {
  // backup plan if wifi or the time server won't cooperate
  int hh = (__TIME__[0] - '0') * 10 + (__TIME__[1] - '0');
  int mm = (__TIME__[3] - '0') * 10 + (__TIME__[4] - '0');
  int ss = (__TIME__[6] - '0') * 10 + (__TIME__[7] - '0');
  baseSecondsOfDay = hh * 3600L + mm * 60L + ss;
  bootMillis = millis();
}

// asks the pool what time it is and resets the clock
bool syncTimeFromNtp() {
  if (WiFi.status() != WL_CONNECTED) return false;

  uint8_t packet[48] = {0};
  packet[0] = 0b11100011; // plain old ntp client request

  ntpUdp.begin(2390);
  ntpUdp.beginPacket(NTP_SERVER, NTP_PORT);
  ntpUdp.write(packet, 48);
  ntpUdp.endPacket();

  unsigned long waitStart = millis();
  while (ntpUdp.parsePacket() == 0) {
    if (millis() - waitStart > 3000) { // server never answered, give up
      ntpUdp.stop();
      return false;
    }
    delay(10);
  }
  ntpUdp.read(packet, 48);
  ntpUdp.stop();

  // bytes 40-43 are seconds since 1900, straight out of the spec
  uint32_t secsSince1900 = ((uint32_t)packet[40] << 24) | ((uint32_t)packet[41] << 16) |
                            ((uint32_t)packet[42] << 8)  | (uint32_t)packet[43];
  uint32_t secsSince1970 = secsSince1900 - 2208988800UL;
  uint32_t localSecs = secsSince1970 + (uint32_t)TIMEZONE_OFFSET_HOURS * 3600UL;

  baseSecondsOfDay = localSecs % 86400UL;
  bootMillis = millis();
  return true;
}

int32_t secondsOfDay() {
  unsigned long elapsedMs = millis() - bootMillis;
  int32_t s = baseSecondsOfDay + (int32_t)(elapsedMs / 1000);
  return s % 86400L;
}

// ---- tm1637 driver, hand rolled so we skip pulling in a library ----

const uint8_t TM_DIGIT_SEGMENTS[10] = {
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

void tmStart() {
  digitalWrite(TM_DIO, HIGH);
  digitalWrite(TM_CLK, HIGH);
  delayMicroseconds(2);
  digitalWrite(TM_DIO, LOW);
}

void tmStop() {
  digitalWrite(TM_CLK, LOW);
  delayMicroseconds(2);
  digitalWrite(TM_DIO, LOW);
  delayMicroseconds(2);
  digitalWrite(TM_CLK, HIGH);
  delayMicroseconds(2);
  digitalWrite(TM_DIO, HIGH);
}

void tmWriteByte(uint8_t b) {
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(TM_CLK, LOW);
    digitalWrite(TM_DIO, (b & 0x01) ? HIGH : LOW);
    b >>= 1;
    delayMicroseconds(3);
    digitalWrite(TM_CLK, HIGH);
    delayMicroseconds(3);
  }
  // ack bit the display sends back, we just ignore it
  digitalWrite(TM_CLK, LOW);
  pinMode(TM_DIO, INPUT_PULLUP);
  delayMicroseconds(3);
  digitalWrite(TM_CLK, HIGH);
  delayMicroseconds(3);
  digitalWrite(TM_CLK, LOW);
  pinMode(TM_DIO, OUTPUT);
}

void tmSendSegments(const uint8_t segments[4]) {
  tmStart();
  tmWriteByte(0x40); // tell it we're sending 4 bytes in a row
  tmStop();

  tmStart();
  tmWriteByte(0xC0); // start from the first digit
  for (int i = 0; i < 4; i++) tmWriteByte(segments[i]);
  tmStop();

  tmStart();
  tmWriteByte(0x88 | 7); // screen on, brightness cranked up
  tmStop();
}

void tmShowTime(int hour, int minute) {
  uint8_t segments[4];
  segments[0] = TM_DIGIT_SEGMENTS[hour / 10];
  segments[1] = TM_DIGIT_SEGMENTS[hour % 10] | 0x80; // that bit lights the colon
  segments[2] = TM_DIGIT_SEGMENTS[minute / 10];
  segments[3] = TM_DIGIT_SEGMENTS[minute % 10];
  tmSendSegments(segments);
}

// ---- setup and the main loop ----

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TM_CLK, OUTPUT);
  pinMode(TM_DIO, OUTPUT);

  randomSeed(rp2040.hwrand32());

  // try for real time over wifi first, fall back to build time if that flops
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 15000) {
    delay(250);
  }
  if (!syncTimeFromNtp()) {
    initClockFromBuildTime();
  }
  lastSyncMillis = millis();
}

void loop() {
  // clocks drift, so touch base with the time server now and then
  if (millis() - lastSyncMillis > RESYNC_INTERVAL_MS) {
    syncTimeFromNtp();
    lastSyncMillis = millis();
  }

  int32_t sod = secondsOfDay();
  int hour = sod / 3600;
  int minute = (sod / 60) % 60;

  static bool alarmHandledThisMinute = false;
  if (hour == ALARM_HOUR && minute == ALARM_MINUTE) {
    if (!alarmHandledThisMinute) {
      runAlarmGame();
      alarmHandledThisMinute = true;
    }
  } else {
    alarmHandledThisMinute = false;
  }

  static int lastMinuteShown = -1;
  if (minute != lastMinuteShown) {
    tmShowTime(hour, minute);
    lastMinuteShown = minute;
  }
}

// keeps chirping and dealing new rounds until you actually beat it
void runAlarmGame() {
  int completed = 0;
  while (completed < ALARM_DISMISS_LEVEL) {
    alarmChirp();
    completed = playGame();
  }
  for (int i = 0; i < 4; i++) digitalWrite(LED_PINS[i], LOW);
}

void alarmChirp() {
  for (int r = 0; r < 2; r++) {
    for (int i = 0; i < 4; i++) digitalWrite(LED_PINS[i], HIGH);
    tone(BUZZER_PIN, 1000);
    delay(150);
    noTone(BUZZER_PIN);
    for (int i = 0; i < 4; i++) digitalWrite(LED_PINS[i], LOW);
    delay(100);
  }
  delay(200);
}

// one attempt, sequence grows each round until you slip up or win
int playGame() {
  int level = 0;
  while (level < ALARM_DISMISS_LEVEL) {
    sequence[level] = random(4);
    level++;

    showSequence(level);
    if (!readPlayerSequence(level)) {
      return level - 1;
    }
    delay(500);
  }
  return level;
}

void showSequence(int count) {
  delay(400);
  for (int i = 0; i < count; i++) {
    uint8_t pad = sequence[i];
    digitalWrite(LED_PINS[pad], HIGH);
    tone(BUZZER_PIN, TONES[pad]);
    delay(SHOW_ON_MS);
    noTone(BUZZER_PIN);
    digitalWrite(LED_PINS[pad], LOW);
    delay(SHOW_GAP_MS);
  }
}

// watches for `count` presses, bails the moment one is wrong
bool readPlayerSequence(int count) {
  for (int i = 0; i < count; i++) {
    int pad = waitForButtonPress();
    if (pad != sequence[i]) return false;
  }
  return true;
}

// waits for a press, lights and beeps that pad while it's held
int waitForButtonPress() {
  int pad;
  while ((pad = pollButtons()) < 0) {
    // spin
  }
  digitalWrite(LED_PINS[pad], HIGH);
  tone(BUZZER_PIN, TONES[pad]);
  while (digitalRead(BUTTON_PINS[pad]) == LOW) {
    // wait for release
  }
  noTone(BUZZER_PIN);
  digitalWrite(LED_PINS[pad], LOW);
  return pad;
}

// quick scan for a fresh press, debounced so noise doesn't count
int pollButtons() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(BUTTON_PINS[i]) == LOW) {
      delay(DEBOUNCE_MS);
      if (digitalRead(BUTTON_PINS[i]) == LOW) return i;
    }
  }
  return -1;
}
  