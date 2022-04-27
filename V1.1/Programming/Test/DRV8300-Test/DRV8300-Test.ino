#define INHA 3
#define INLA 5
#define INHB 10
#define INLB 11
#define INHC 6
#define INLC 9

void setup() {

  pinMode(INHA, OUTPUT);
  pinMode(INLA, OUTPUT);
  pinMode(INHB, OUTPUT);
  pinMode(INLB, OUTPUT);
  pinMode(INHC, OUTPUT);
  pinMode(INLC, OUTPUT);

  Serial.begin(115200);
}
int delayValue = 10;
int PWMValue = 100;
void loop() {
  analogWrite(INHA, PWMValue);
  digitalWrite(INLA, LOW);

  digitalWrite(INHB, LOW);
  digitalWrite(INLB, HIGH);

  digitalWrite(INHC, LOW);
  digitalWrite(INLC, LOW);

  delay(delayValue);

  analogWrite(INHA, PWMValue);
  digitalWrite(INLA, LOW);

  digitalWrite(INHB, LOW);
  digitalWrite(INLB, LOW);

  digitalWrite(INHC, LOW);
  digitalWrite(INLC, HIGH);

  delay(delayValue);

  digitalWrite(INHA, LOW);
  digitalWrite(INLA, LOW);

  analogWrite(INHB, PWMValue);
  digitalWrite(INLB, LOW);

  digitalWrite(INHC, LOW);
  digitalWrite(INLC, HIGH);

  delay(delayValue);

  digitalWrite(INHA, LOW);
  digitalWrite(INLA, HIGH);

  analogWrite(INHB, PWMValue);
  digitalWrite(INLB, LOW);

  digitalWrite(INHC, LOW);
  digitalWrite(INLC, LOW);

  delay(delayValue);

  digitalWrite(INHA, LOW);
  digitalWrite(INLA, HIGH);

  digitalWrite(INHB, LOW);
  digitalWrite(INLB, LOW);

  analogWrite(INHC, PWMValue);
  digitalWrite(INLC, LOW);

  delay(delayValue);

  digitalWrite(INHA, LOW);
  digitalWrite(INLA, LOW);

  digitalWrite(INHB, LOW);
  digitalWrite(INLB, HIGH);

  analogWrite(INHC, PWMValue);
  digitalWrite(INLC, LOW);

  delay(delayValue);

}
//void loop() {
//  analogWrite(INHB, 10);
//  digitalWrite(INLB, LOW);
//
//  digitalWrite(INHC, LOW);
//  digitalWrite(INLC, HIGH);
//
//  digitalWrite(INHA, LOW);
//  digitalWrite(INLA, LOW);
//}

int intensity_calculation (int x) {
  float a = -(1 / 200.);
  float b = 2.;
  int output = (pow(x, 2) * a + x * b);
  return output;
}
