#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

#define SDA_PIN 5
#define SCL_PIN 6
#define I2C_ADDR 0x3C
#define USABLE_WIDTH 72
#define USABLE_HEIGHT 40

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL_PIN, SDA_PIN, U8X8_PIN_NONE);

const uint8_t offsetX = 28;
const uint8_t offsetY = 24;
const uint8_t numLines = 1;
const bool testPixels = false;
const bool invertDisplay = false;
const uint8_t brightness = 255;
const unsigned long tickInterval = 1000;

int counterProgram = 1;
bool toggle = false;
unsigned long counterValue = 0;
unsigned long lastTick = 0;
char displayText[32];

void drawDisplay() {
    u8g2.clearBuffer();
    if (testPixels) {
        u8g2.drawPixel(offsetX, offsetY);
        u8g2.drawPixel(offsetX + USABLE_WIDTH - 1, offsetY);
        u8g2.drawPixel(offsetX, offsetY + USABLE_HEIGHT - 1);
        u8g2.drawPixel(offsetX + USABLE_WIDTH - 1, offsetY + USABLE_HEIGHT - 1);
    }
    if (numLines == 1) {
        if (counterProgram == 0) {
            u8g2.setFont(u8g2_font_logisoso32_tn);
            snprintf(displayText, sizeof(displayText), "%lu", counterValue);
            uint8_t textWidth = u8g2.getStrWidth(displayText);
            uint8_t fontHeight = u8g2.getMaxCharHeight();
            uint8_t x = offsetX + (USABLE_WIDTH - textWidth) / 2;
            uint8_t y = offsetY + fontHeight;
            u8g2.drawStr(x, y, displayText);
        } else {
            toggle = !toggle;
            float temp = toggle ? 18.7f : 18.8f;
            snprintf(displayText, sizeof(displayText), "%.1f", temp);
            u8g2.setFont(u8g2_font_logisoso32_tn);
            uint8_t textWidth = u8g2.getStrWidth(displayText);
            uint8_t fontHeight = u8g2.getMaxCharHeight();
            uint8_t x = offsetX + (USABLE_WIDTH - textWidth) / 2;
            uint8_t y = offsetY + fontHeight;
            u8g2.drawStr(x, y, displayText);
        }
    } else if (numLines == 2) {
        u8g2.setFont(u8g2_font_10x20_tf);
        snprintf(displayText, sizeof(displayText), "%lu", counterValue);
        u8g2.drawStr(offsetX + 2, offsetY + 16, displayText);
        u8g2.drawStr(offsetX + 2, offsetY + 36, "Line 2");
    } else if (numLines == 3) {
        u8g2.setFont(u8g2_font_8x13_tf);
        snprintf(displayText, sizeof(displayText), "%lu", counterValue);
        u8g2.drawStr(offsetX + 2, offsetY + 11, displayText);
        u8g2.drawStr(offsetX + 2, offsetY + 24, "Line 2");
        u8g2.drawStr(offsetX + 2, offsetY + 37, "Line 3");
    }
    u8g2.sendBuffer();
}

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    u8g2.setI2CAddress(I2C_ADDR * 2);
    u8g2.begin();
    u8g2.setBusClock(400000UL);
    u8g2.setContrast(brightness);
    if (invertDisplay) u8g2.setDisplayRotation(U8G2_R2);
    delay(50);
    drawDisplay();
}

void loop() {
    unsigned long now = millis();
    if (now - lastTick >= tickInterval) {
        lastTick = now;
        counterValue++;
        drawDisplay();
    }
}
