#include "Wire.h"
#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8
#define ADDR 0x68
#define REG1 0x11
#define DELAY 3

struct SevenSegmentChar {
  char character;
  byte representation;
};

const SevenSegmentChar sevenSegmentChars[] = {
  { '0', 0xC0 },
  { '1', 0xF9 },
  { '2', 0xA4 },
  { '3', 0xB0 },
  { '4', 0x99 },
  { '5', 0x92 },
  { '6', 0x82 },
  { '7', 0xF8 },
  { '8', 0x80 },
  { '9', 0x90 }
};

const byte SEGMENT[] = { 0xF1, 0xF2, 0xF4, 0xF8 };

void setup() {
  Wire.begin();
  pinMode(LATCH_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);
}

void loop() {
  char hundreds, tens, units;
  byte temp = getTemperature();
  splitDataIntoChars(temp, hundreds, tens, units);
  byte digit1 = findRepresentation('0');
  byte digit2 = findRepresentation(hundreds);
  byte digit3 = findRepresentation(tens);
  byte digit4 = findRepresentation(units);
  writeToDisplay(digit1, digit2, digit3, digit4);
}

byte getTemperature() {
  byte data = 0xFF;
  Wire.beginTransmission(ADDR);
  Wire.write(REG1);
  Wire.endTransmission();
  Wire.requestFrom(ADDR, 1);
  if (Wire.available()) { data = Wire.read(); }
  return data;
}

void splitDataIntoChars(byte value, char &digit1, char &digit2, char &digit3) {
  digit1 = (value / 100) + '0';
  digit2 = ((value / 10) % 10) + '0';
  digit3 = (value % 10) + '0';
}

byte findRepresentation(char c) {
  for (int i = 0; i < sizeof(sevenSegmentChars) / sizeof(sevenSegmentChars[0]); i++) {
    if (sevenSegmentChars[i].character == c) {
      return sevenSegmentChars[i].representation;
    }
  }
  return 0xFF;
}

void writeToDisplay(byte b1, byte b2, byte b3, byte b4) {
  writeToSegment(SEGMENT[0], b1);
  delay(DELAY);
  writeToSegment(SEGMENT[1], b2);
  delay(DELAY);
  writeToSegment(SEGMENT[2], b3);
  delay(DELAY);
  writeToSegment(SEGMENT[3], b4);
  delay(DELAY);
}

void writeToSegment(byte Segment, byte Value) {
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, Value);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, Segment);
  digitalWrite(LATCH_DIO, HIGH);
}