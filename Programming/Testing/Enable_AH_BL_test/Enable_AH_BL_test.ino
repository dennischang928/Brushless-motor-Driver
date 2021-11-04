#define INHA PA8
#define INLA PA9
#define SO1 PB1

#define INHB PA10
#define INLB PB6
#define SO2 PB0

#define INHC PB7
#define INLC PB8
#define SO3 PA3

#define LED PC13

#define EN_GATE PB12

void setup() {
  pinMode(INHA, OUTPUT);
  pinMode(INLA, OUTPUT);
  pinMode(SO1, INPUT);

  pinMode(INHB, OUTPUT);
  pinMode(INLB, OUTPUT);
  pinMode(SO2, INPUT);

  pinMode(INHC, OUTPUT);
  pinMode(INLC, OUTPUT);
  pinMode(SO3, INPUT);  

  pinMode(EN_GATE, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(EN_GATE, HIGH);

}
void loop() {

  digitalWrite(INHA, HIGH);
  digitalWrite(INLB, HIGH);
  digitalWrite(LED, HIGH);
  Serial.print(analogRead(SO1));
  Serial.print(",");
  Serial.print(analogRead(SO2));
  Serial.print(",");
  Serial.println(analogRead(SO3));
  delay(10);
  //  delay(1000);
  //  digitalWrite(INHA, LOW);
  //  digitalWrite(INLB, LOW);
  //  digitalWrite(LED, LOW);
  //  delay(1000);
}
