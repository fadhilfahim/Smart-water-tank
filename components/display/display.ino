
// SCL = GPIO 22
// SDA = GPIO 21

#include <Wire.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

int percentage = 75;        // Water level %
int volume_liters = 5;      // Water volume in L
String quality = "Good";    // Can be "Good", "Normal", "Bad"

void setup() {
  u8g2.begin();
}

void loop() {
  u8g2.clearBuffer(); // clear screen

  // === 1. Quality and Volume Text ===
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.setCursor(2, 14);
  u8g2.print("quality: ");
  u8g2.print(quality);

  u8g2.setCursor(2, 30);
  u8g2.print("volume: ");
  u8g2.print(volume_liters);
  u8g2.print("L");

  // === 2. Vertical Progress Bar ===
  int barX = 110;
  int barY = 10;
  int barWidth = 10;
  int barHeight = 50;

  // Outline
  u8g2.drawFrame(barX, barY, barWidth, barHeight);

  // Fill height
  int filledHeight = map(percentage, 0, 100, 0, barHeight);
  u8g2.drawBox(barX, barY + (barHeight - filledHeight), barWidth, filledHeight);

  // Percentage text aligned near the bar
  u8g2.setCursor(barX - 25, barY + (barHeight - filledHeight) + 8);
  u8g2.print(percentage);
  u8g2.print("%");

  u8g2.sendBuffer();

  // Simulate data update
  percentage += 5;
  if (percentage > 100) percentage = 0;
  delay(1000);
}
