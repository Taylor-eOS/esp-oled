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
unsigned long counterValue = 0;
unsigned long lastTick = 0;
const unsigned long tickInterval = 500;

void drawTestFrame(uint8_t ox, uint8_t oy, unsigned long value) {
    u8g2.clearBuffer();
    u8g2.drawPixel(ox, oy);
    u8g2.drawPixel(ox + USABLE_WIDTH - 1, oy);
    u8g2.drawPixel(ox, oy + USABLE_HEIGHT - 1);
    u8g2.drawPixel(ox + USABLE_WIDTH - 1, oy + USABLE_HEIGHT - 1);
    char buf[32];
    snprintf(buf, sizeof(buf), "offset %u,%u", ox, oy);
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(ox + 2, oy + 15, buf);
    snprintf(buf, sizeof(buf), "%lu", value);
    u8g2.drawStr(ox + 2, oy + 30, buf);
    u8g2.sendBuffer();
}

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    u8g2.setI2CAddress(I2C_ADDR * 2);
    u8g2.begin();
    u8g2.setBusClock(400000UL);
    delay(50);
    drawTestFrame(offsetX, offsetY, counterValue);
}

void loop() {
    unsigned long now = millis();
    if (now - lastTick >= tickInterval) {
        lastTick = now;
        counterValue++;
        drawTestFrame(offsetX, offsetY, counterValue);
    }
}

