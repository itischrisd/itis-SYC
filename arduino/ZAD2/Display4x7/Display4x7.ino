#define LATCH_DIO 4
#define CLK_DIO 7
#define DATA_DIO 8

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
  { '9', 0x90 },
  { 'A', 0x88 },
  { 'b', 0x83 },
  { 'C', 0xC6 },
  { 'd', 0xA1 },
  { 'E', 0x86 },
  { 'F', 0x8E },
  { 'H', 0x89 },
  { 'I', 0xCF },
  { 'J', 0xE1 },
  { 'L', 0xC7 },
  { 'n', 0xAB },
  { 'o', 0xA3 },
  { 'P', 0x8C },
  { 'q', 0x98 },
  { 'r', 0xAF },
  { 't', 0x87 },
  { 'U', 0xC1 },
  { 'y', 0x91 }
};

const byte SEGMENT[] = { 0xF1, 0xF2, 0xF4, 0xF8 };

void setup() {
  pinMode(LATCH_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);
}

void loop() {
  byte H = findRepresentation('H');
  byte E = findRepresentation('E');
  byte L = findRepresentation('L');
  byte P = findRepresentation('P');
  writeToSegment(SEGMENT[0], H);
  writeToSegment(SEGMENT[1], E);
  writeToSegment(SEGMENT[2], L);
  writeToSegment(SEGMENT[3], P);
}

byte findRepresentation(char c) {
  for (unsigned int i = 0; i < sizeof(sevenSegmentChars) / sizeof(sevenSegmentChars[0]); i++) {
    if (sevenSegmentChars[i].character == c) {
      return sevenSegmentChars[i].representation;
    }
  }
  return 0xFF;
}

byte withDot(byte ch) {
  return ch & 0x7F;
}

void writeToSegment(byte Segment, byte Value) {
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, Value);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, Segment);
  digitalWrite(LATCH_DIO, HIGH);
}
