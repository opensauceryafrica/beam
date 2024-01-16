#define relay D7
#define led D8

void setup() {
  Serial.begin(115200);
  pinMode(relay, OUTPUT);digitalWrite(relay, LOW);
  pinMode(led, OUTPUT);digitalWrite(led, LOW);

}

void loop() {
  // Normally Open configuration, send LOW signal to let current flow
  // (if you're usong Normally Closed configuration send HIGH signal)
  digitalWrite(relay, LOW);
  digitalWrite(led, LOW);
  Serial.println("Current not Flowing");
  delay(1000); 
  
  // Normally Open configuration, send HIGH signal stop current flow
  // (if you're usong Normally Closed configuration send LOW signal)
  digitalWrite(relay, HIGH);
  digitalWrite(led, HIGH);
  Serial.println("Current Flowing");
  delay(3000);
}
