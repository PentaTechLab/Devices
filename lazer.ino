bool lazer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
   delay(1000); 
lazer = digitalRead (8);
 Serial.print("LAzer is ");
 Serial.println(lazer);
}
