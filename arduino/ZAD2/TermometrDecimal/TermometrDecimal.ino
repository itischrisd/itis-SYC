#include "Wire.h"
#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8
#define ADDR 0x68
#define REG1 0x11
#define REG2 0x12
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
  char tens, units, tenths, hundredths;
  byte temp = getTemperature(REG1);
  dataToIntegerChars(temp, tens, units);
  byte digit1 = findRepresentation(tens);
  byte digit2 = withDot(findRepresentation(units));
  temp = getTemperature(REG2);
  dataToDecimalChars(temp, tenths, hundredths);
  byte digit3 = findRepresentation(tenths);
  byte digit4 = findRepresentation(hundredths);
  writeToDisplay(digit1, digit2, digit3, digit4);
}

byte getTemperature(int reg) {
  byte data = 0xFF;
  Wire.beginTransmission(ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(ADDR, 1);
  if (Wire.available()) { data = Wire.read(); }
  return data;
}

void dataToIntegerChars(byte value, char &digit1, char &digit2) {
  digit1 = ((value / 10) % 10) + '0';
  digit2 = (value % 10) + '0';
}

void dataToDecimalChars(byte value, char &char1, char &char2) {
  byte msb = value >> 6;

  switch (msb) {
    case 0:
      char1 = '0';
      char2 = '0';
      break;
    case 1:
      char1 = '2';
      char2 = '5';
      break;
    case 2:
      char1 = '5';
      char2 = '0';
      break;
    case 3:
      char1 = '7';
      char2 = '5';
      break;
  }
}

byte findRepresentation(char c) {
  for (int i = 0; i < sizeof(sevenSegmentChars) / sizeof(sevenSegmentChars[0]); i++) {
    if (sevenSegmentChars[i].character == c) {
      return sevenSegmentChars[i].representation;
    }
  }
  return 0xFF;
}

byte withDot(byte ch) {
  return ch & 0x7F;
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