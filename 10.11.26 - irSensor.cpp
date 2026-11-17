int irSensor = 7;

void setup() {
  // put your setup code here, to run once:

  pinMode(irSensor,INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int state = digitalRead(irSensor);
  Serial.println(state);
  delay(1000);

}
