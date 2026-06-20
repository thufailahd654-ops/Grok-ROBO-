#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DFRobotDFPlayerMini.h"
#include "HardwareSerial.h"

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- Sound Sensor ----------
#define SOUND_DO_PIN 4

// ---------- DFPlayer ----------
HardwareSerial mySerial(2);
DFRobotDFPlayerMini myDFPlayer;

// ---------- Eye geometry ----------
int eyeW = 22;
int eyeH = 32;
int eyeRadius = 8;
int eyeGap = 16;
int centerX = SCREEN_WIDTH / 2;
int centerY = SCREEN_HEIGHT / 2;

bool isAwake = false;
unsigned long lastBlinkTime = 0;
const unsigned long BLINK_INTERVAL = 4000; // blink every 4s while awake

// ---------- Draw eyes at given offset + height scale ----------
void drawEyes(int xOffset, float heightScale) {
  display.clearDisplay();

  int h = eyeH * heightScale;
  if (h < 2) h = 2; // never fully disappear, just a line

  int leftEyeX  = centerX - eyeGap/2 - eyeW + xOffset;
  int rightEyeX = centerX + eyeGap/2 + xOffset;

  int eyeY = centerY - h/2;

  display.fillRoundRect(leftEyeX, eyeY, eyeW, h, eyeRadius, SSD1306_WHITE);
  display.fillRoundRect(rightEyeX, eyeY, eyeW, h, eyeRadius, SSD1306_WHITE);

  display.display();
}

// ---------- Wake animation: closed -> open, centered ----------
void wakeUpAnimation() {
  for (float scale = 0.05; scale <= 1.0; scale += 0.15) {
    drawEyes(0, scale);
    delay(25);
  }
  drawEyes(0, 1.0);
}

// ---------- Look sideways then return to center ----------
void lookSideways() {
  int travel = 18;

  for (int x = 0; x >= -travel; x -= 3) { drawEyes(x, 1.0); delay(12); }
  delay(300);
  for (int x = -travel; x <= 0; x += 3) { drawEyes(x, 1.0); delay(12); }
  delay(150);

  for (int x = 0; x <= travel; x += 3) { drawEyes(x, 1.0); delay(12); }
  delay(300);
  for (int x = travel; x >= 0; x -= 3) { drawEyes(x, 1.0); delay(12); }
}

// ---------- Blink: shrink to line, back to full ----------
void blink() {
  for (float scale = 1.0; scale >= 0.05; scale -= 0.2) { drawEyes(0, scale); delay(15); }
  for (float scale = 0.05; scale <= 1.0; scale += 0.2) { drawEyes(0, scale); delay(15); }
}

void setup() {
  Serial.begin(115200);
  pinMode(SOUND_DO_PIN, INPUT);

  // OLED init
  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (true);
  }
  display.clearDisplay();
  display.display(); // start OFF (blank)

  // DFPlayer init
  mySerial.begin(9600, SERIAL_8N1, 16, 17);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer init failed!");
    while (true);
  }
  myDFPlayer.volume(25);

  Serial.println("System ready. Waiting for sound...");
}

void loop() {
  int soundDetected = digitalRead(SOUND_DO_PIN);

  // Trigger on LOW
  if (soundDetected == LOW && !isAwake) {
    Serial.println("Sound detected! Waking up...");

    myDFPlayer.play(1); // play 0001.mp3

    wakeUpAnimation();
    lookSideways();
    blink();

    isAwake = true;
    lastBlinkTime = millis();
  }

  // Once awake, stay awake forever — just blink periodically
  if (isAwake) {
    if (millis() - lastBlinkTime > BLINK_INTERVAL) {
      blink();
      lastBlinkTime = millis();
    }
  }

  delay(50);
}