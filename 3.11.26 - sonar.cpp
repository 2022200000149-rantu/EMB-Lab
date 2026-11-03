// C++ code
//
int trigPin = 7;
int echoPin = 8;
void setup()
{
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  
  Serial.begin(9600); //Serial monitor initialization
}

void loop()
{
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  
  float duration = pulseIn(echoPin,HIGH);
  
  float distance = (0.0343*duration)/2;
  Serial.println(distance);
  
  delay(5000);
}
