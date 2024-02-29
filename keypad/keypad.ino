#include <Keypad.h>

const int Rows_Number=4;
const int Columns_Number=4;

 char keys[Rows_Number][Columns_Number] {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
    };

byte pin_Rows[Rows_Number]={2,3,4,5};
byte pin_Columns[Columns_Number]={6,7,8,9};

Keypad keypad = Keypad(makeKeymap(keys),pin_Rows,pin_Columns,Rows_Number,Columns_Number);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();
  if(key){
    Serial.print(key);
    }

}
