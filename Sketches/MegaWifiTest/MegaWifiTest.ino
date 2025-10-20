void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1500);
  pinMode(52, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int input = digitalRead(52);
  digitalWrite(LED_BUILTIN, input);
  digitalWrite(2,true);
  delay(1);
}
