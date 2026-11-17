// C++ code
//

int pot = A0;
void setup()
{
  pinMode(pot, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int state = analogRead(pot);
  Serial.println(state);

}
