#define INHA PA8
#define INLA PA9

#define INHB PA10
#define INLB PB6

#define INHC PB7
#define INLC PB8
#define LED PC13

#define EN_GATE PB12

void setup() {
  pinMode(INHA, OUTPUT);
  pinMode(INLA, OUTPUT);
  pinMode(INHB, OUTPUT);
  pinMode(INLB, OUTPUT);
  pinMode(INHC, OUTPUT);
  pinMode(INLC, OUTPUT);
  pinMode(EN_GATE, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(EN_GATE, HIGH);

}
void loop() {
  digitalWrite(INHA, HIGH);
  digitalWrite(INHB, HIGH);
  digitalWrite(INHC, HIGH);


  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(INHA, LOW);
  digitalWrite(INHB, LOW);
  digitalWrite(INHC, LOW);
  digitalWrite(LED, LOW);
  delay(1000);
}
