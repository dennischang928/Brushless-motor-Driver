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
void loop() {
  //  digitalWrite(INHA, HIGH);
  //  digitalWrite(INLA, LOW);
  digitalWrite(INLC, LOW);
  analogWrite(INHC, 200);
  digitalWrite(INLB, HIGH);
  delay(1000);
//  digitalWrite(INLC, HIGH);
//
//  digitalWrite(INLB, LOW);
//  analogWrite(INHB, 200); 
//  delay(1000); 
  
  //  for (int i = 0; i <= 200; i++) {
  //    analogWrite(INLB, intensity_calculation(i));
  //    analogWrite(INLA, intensity_calculation(i));
  //    analogWrite(INLC, intensity_calculation(i));
  //    Serial.println(intensity_calculation(i));
  //    delay(5);
  //  }
  //  for (int i = 200; i >= 0; i--) {
  //    analogWrite(INLB, intensity_calculation(i));
  //    analogWrite(INLA, intensity_calculation(i));
  //    analogWrite(INLC, intensity_calculation(i));
  //
  //    Serial.println(intensity_calculation(i));
  //    delay(5);
  //  }
}

int intensity_calculation (int x) {
  float a = -(1 / 200.);
  float b = 2.;
  int output = (pow(x, 2) * a + x * b);
  return output;
}
