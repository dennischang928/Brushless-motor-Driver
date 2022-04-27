#define INHA PA8
#define INHB PA10
#define INHC PB7

void setup() {
  pinMode(INHA, OUTPUT);
  pinMode(INHB, OUTPUT);
  pinMode(INHC, OUTPUT);
}

void loop() {
  analogWrite(INHB, 255);
}
