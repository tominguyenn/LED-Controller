#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- LCD ----------
LiquidCrystal_I2C lcd(0x27, 20, 4);

// ---------- Pins ----------
// Index 0 -> Blue on D2, Index 1 -> Red on D3, Index 2 -> Green on D4
const int ledPins[3] = {2, 3, 4};                     // D2, D3, D4
const char* ledColors[3] = {"Blue", "Red", "Green"};  // Labels aligned to pins above
const int buttonPin = 5;                              // D5, button to GND
const int potPin = A0;                                // Potentiometer wiper at A0

// ---------- PWM (LEDC) ----------
const int pwmCh[3]     = {0, 1, 2};  // one channel per LED
const int pwmFreq      = 5000;       // 5 kHz
const int pwmResBits   = 10;         // 10-bit resolution -> 0..1023
const int pwmMax       = (1 << pwmResBits) - 1;

int ledIndex = 0;               // which LED is selected: 0=B, 1=R, 2=G
bool buttonLatched = false;

// clear LCD
void clearLCDLines() 
{
  for (int row = 0; row < 4; row++) {
    lcd.setCursor(0, row);
    lcd.print("                    "); // 20 spaces
  }
}

void showStaticInfo() 
{
  clearLCDLines();

  // Current color
  lcd.setCursor(0, 0);
  lcd.print("Current: ");
  lcd.print(ledColors[ledIndex]);

  // Next color
  int nextIndex = (ledIndex + 1) % 3;
  lcd.setCursor(0, 1);
  lcd.print("Next: ");
  lcd.print(ledColors[nextIndex]);

  // Brightness line placeholder
  lcd.setCursor(0, 2);
  lcd.print("Brightness: ----");
  
  // Instruction
  lcd.setCursor(0, 3);
  lcd.print("Press to change");
}

void updateBrightnessLine(int duty) {
  // Convert duty (0..pwmMax) to percent
  int percent = (duty * 100 + pwmMax / 2) / pwmMax; // rounded
  // Overwrite entire line to avoid leftover chars
  lcd.setCursor(0, 2);
  lcd.print("Brightness: ");
  // Ensure fixed width for stable display
  if (percent < 10)       lcd.print("  ");
  else if (percent < 100) lcd.print(" ");
  lcd.print(percent);
  lcd.print("%");
}

void selectOnlyActiveLED(int activeIdx, int duty) {
  for (int i = 0; i < 3; i++) {
    ledcWrite(pwmCh[i], (i == activeIdx) ? duty : 0);
  }
}

void setup() {
  // I2C for Nano ESP32 (SDA=D8, SCL=D9)
  Wire.begin(8, 9);

  // Button
  pinMode(buttonPin, INPUT_PULLUP);

  // PWM channels for each LED pin
  for (int i = 0; i < 3; i++) {
    ledcSetup(pwmCh[i], pwmFreq, pwmResBits);
    ledcAttachPin(ledPins[i], pwmCh[i]);
    ledcWrite(pwmCh[i], 0); // start off
  }

  // LCD
  lcd.init();
  lcd.backlight();
  showStaticInfo();
}

void loop() 
{
  // ----- Button: cycle active LED -----
  bool pressed = (digitalRead(buttonPin) == LOW);
  if (pressed && !buttonLatched) {
    buttonLatched = true;

    // advance to next LED
    ledIndex = (ledIndex + 1) % 3;

    // Update the static info (Current/Next lines)
    showStaticInfo();
  }
  if (!pressed) {
    buttonLatched = false;
  }

  // ----- Potentiometer: live brightness -----
  int raw = analogRead(potPin);                  // 0..4095 on ESP32
  int duty = map(raw, 0, 4095, 0, pwmMax);       // scale to 0..1023

  // Apply brightness only to currently selected LED
  selectOnlyActiveLED(ledIndex, duty);

  // Update only the brightness line to prevent flicker
  updateBrightnessLine(duty);

  // Small delay to keep LCD readable but still "live"
  delay(10);
}
