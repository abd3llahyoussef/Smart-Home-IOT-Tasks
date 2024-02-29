#include <Keypad.h>
#include <LiquidCrystal.h>

//UltraSonic Configrattions
int triger = 10;
int echo = 2;
float duration,distance,filteredArray[20];
double finalDistance,sum =0;

String pass;
String inputPass;
String realPass="2";

//LCD Configrations
LiquidCrystal lcd(13, 12, 11, 6, 5, 4, 3);  /* For 4-bit mode */
////Keypad configrations
const int Rows_Number = 4;
const int Cols_Number =4;

char keys [Rows_Number][Cols_Number]{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
  };

byte Rows_Pins[Rows_Number]={A0,9,8,7};
byte Cols_Pins[Cols_Number] ={A1,A2,A3,A4};

Keypad keypad = Keypad(makeKeymap(keys),Rows_Pins,Cols_Pins,Rows_Number,Cols_Number);

void setup() {
    // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(triger,OUTPUT);
  pinMode(echo,INPUT);
  
  lcd.begin(16,2);          /* Initialize 16x2 LCD */
  lcd.clear();            /* Clear the LCD */;

  lcd.setCursor(0,0);
  lcd.print("Enter A Password:");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Store data in array
  for(int i=0;i<20;i++){
    filteredArray[i]=ultraSonicMeasure();
    delay(500);
    }
//Arrange them in ASC Order
    for(int x=0;x<19;x++){
      for(int y=x+1;y<20;y++){
        filteredArray[x]>filteredArray[y];
        float swap = filteredArray[x];
        filteredArray[x]=filteredArray[y];
        filteredArray[y]= swap;
        }
      }
//Calculate the Average
for(int sample=5;sample<15;sample++){
  sum = 0;
  sum += filteredArray[sample];
  Serial.println(sum);
  }

  finalDistance = sum /10;
  Serial.print("The Real Distance is:");
  Serial.println(finalDistance);
  if(finalDistance == 0.01){
  char key = keypad.getKey();

  if(key){
    Serial.println(key);

    if(key=='#'){
      Serial.print(inputPass);
    if(inputPass == realPass){
      lcd.clear();
      lcd.setCursor(0,0);
      Serial.print("Correct Password");
      lcd.print("Correct Password");
      }
    }
     if(key=='#' && inputPass != realPass){
       lcd.clear();
      lcd.setCursor(0,0);
      Serial.print("Access Denied");
      lcd.print("Access Denied");
      }
  if(key !='#'){
    pass += '*';
    inputPass += key;
    Serial.println(pass);
    lcd.setCursor(0,1);
    lcd.print(pass);
      }

    }if(key=='C'){
      lcd.clear();
      lcd.setCursor(0,0);
  lcd.print("Enter A Pssword:");
  inputPass="";
  pass="";
      }
}
}
float ultraSonicMeasure(){
  digitalWrite(triger,HIGH);
  delayMicroseconds(10);
  digitalWrite(triger,LOW);

  duration = pulseIn(echo,HIGH);
  Serial.print (duration);

  distance = 0.017*duration;

  Serial.print("The distance is:");
  Serial.println(distance);
  return distance;
  }
