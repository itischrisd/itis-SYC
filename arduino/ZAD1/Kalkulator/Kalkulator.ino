double num1, num2, result;
char oper = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("--- Arduino Calculator ---");
}

void loop() {
  Serial.println("Enter full expression or first number:");
  num1 = fetchNextDouble();


  if (!Serial.available())
    Serial.println("Enter operation symbol (+ - * /):");
  oper = fetchNextChar();

  if (!Serial.available())
    Serial.println("Enter second number:");
  num2 = fetchNextDouble();

  switch (oper) {
    case '+':
      result = num1 + num2;
      break;
    case '-':
      result = num1 - num2;
      break;
    case '*':
      result = num1 * num2;
      break;
    case '/':
      if (num2 == 0) {
        Serial.println("ERROR: Division by zero");
        return;
      } else {
        result = num1 / num2;
      }
      break;
    default:
      Serial.println("ERROR: Unrecognized operand");
      return;
  }

  Serial.print("Your result is: ");
  Serial.println(result, 4);
}

double fetchNextDouble() {
  consumeWhiteChars();
  while (!Serial.available())
    ;
  double num = Serial.parseFloat();
  delay(100);
  consumeWhiteChars();
  return num;
}

char fetchNextChar() {
  consumeWhiteChars();
  while (!Serial.available())
    ;
  char ch = Serial.read();
  delay(100);
  consumeWhiteChars();
  return ch;
}

void consumeWhiteChars() {
  while (Serial.peek() == '\n' || Serial.peek() == '\r' || Serial.peek() == '\t' || Serial.peek() == ' ')
    Serial.read();
}