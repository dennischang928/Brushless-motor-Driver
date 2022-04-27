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
  digitalWrite(INHA, LOW);
  digitalWrite(INHB, LOW);
  digitalWrite(INHC, LOW);

}
