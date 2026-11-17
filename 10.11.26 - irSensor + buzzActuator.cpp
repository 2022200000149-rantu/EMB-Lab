int buzz = 7;
int irSensor = 8;

void setup() {
  // put your setup code here, to run once:

 pinMode(buzz, OUTPUT);
 pinMode(irSensor, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  int state = digitalRead(irSensor);
  delay(500);

  if (state == 0) {
 digitalWrite(buzz,LOW);} else {
 digitalWrite(buzz,HIGH);
 }
 

}
