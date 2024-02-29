
#define Ldr A0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(Ldr,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int light = analogRead(Ldr);
  Serial.println(light);

  if(light>=200){
    tone(6,998,1000);
    delay(500);
    tone(6,1047,1000);
    delay(500);
    }
}
