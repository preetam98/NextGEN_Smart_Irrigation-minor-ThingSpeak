const int ledPin = 2;
const int ldrPin = A0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin,INPUT);
}
void loop(){
  int ldrStatus = analogRead(ldrPin);
  if (ldrStatus > 650){
    digitalWrite(ledPin, HIGH);
  }
  else{
    digitalWrite(ledPin, LOW);
  }
  Serial.println(ldrStatus);
  delay (1000);
}
