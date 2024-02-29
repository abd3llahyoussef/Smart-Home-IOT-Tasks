const int EN = 6;
const int IN1= 5;
const int IN2=3;

void setup() {
  // put your setup code here, to run once:
  pinMode(EN,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(IN1,HIGH);
  analogWrite(IN2,LOW);

  for(int i=0;i<=255;i++){
    analogWrite(EN,i);
    delay(500);
    }

    delay(5000);

  analogWrite(IN1,LOW);
  analogWrite(IN2,HIGH);

  for(int i=255;i>=0;i--){
    analogWrite(EN,i);
        delay(500);
    }

    delay(5000);
}
