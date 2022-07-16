#define INHA PA8
#define INHB PA10
#define INHC PB7

//#define INHA 3
//#define INHB 5
//#define INHC 6

void setup() {
  pinMode(INHA, OUTPUT);
  pinMode(INHB, OUTPUT);
  pinMode(INHC, OUTPUT);
}

void loop() {
  //  for (int i = 0; i < 127; i++) {
  //    Serial.print(i);
  //    Serial.print(',');
  //    analogWriteanalogWrite.println(255 - i);
  analogWrite(INHC, 123);
  analogWrite(INHB, 126);
  //  analogWrite(INHB, 129); //129
  //    delay(50);
  //  }

  //    analogWrite(INHB, 200);
  //  analogWrite(INHC, 0);
  //  analogWrite(INHA, 250);
  //  digitalWrite(INHA, LOW);
  //  digitalWrite(INHA, LOW);
  //  for (i
}
