// C++ code
//

int pot = A0;
int LED = 6;
int buzzer = 5;
void setup()
{
  pinMode(pot, INPUT);
  pinMode(LED,OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int state = analogRead(pot);
  Serial.println(state);
  analogWrite(LED, state/4);
  tone(buzzer,state);
  delay(500);

}
