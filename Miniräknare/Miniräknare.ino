// C++ code
//
#include <Adafruit_LiquidCrystal.h>
#include <Keypad.h>

Adafruit_LiquidCrystal lcd_1(0);

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //three columns


char mathCalc[16] = "";
char CharacterType;

float characters [3];
float answerNumber;

int columme = 0;
int row = 0;
int seconds = 0;
int index = 0;

bool answer = false;
bool oneTime01 = true;



char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','-'},
  {'4','5','6','+'},
  {'7','8','9','*'},
  {'*','0','=','/'}
};

byte pin_rows[ROW_NUM] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {6, 5, 4, 3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );


void setup()
{
  Serial.begin(9600);
  lcd_1.begin(16, 2);
}

void loop()
{
  lcd_1.setBacklight(1);
  char key = keypad.getKey();

  //true every time keypad button gets pressed
  if (key){
    columme++;
    lcd_1.print(key);
    mathCalc[index] = key; //adding the character to char
    index++;
    
    //Pushes the number up on the calculator
    if(answer == true)
    {
      lcd_1.clear();
      lcd_1.setCursor(intLen(answerNumber), 0);
      lcd_1.print(key);
      lcd_1.setCursor(0, 0);
      lcd_1.print(answerNumber);
      lcd_1.setCursor(intLen(answerNumber)+1, 0);
      answer = false;
    }
    
    //gets true every time you use a math character
    if(key == '*' || key == '-' || key == '/' || key == '+')
    {
      characters[0] = answerNumber;
      CharacterType = key;

      //plays one time
      if(oneTime01==true)
      {
        characters[0] = atoi(mathCalc);
        oneTime01=false;
      }
      
      //clears the lower row
      for(int i = 0; i < 15; i++)
      {
      	mathCalc[i] = 0;
      }
      index=0;
    }
    
    //playes when every time you use the equal character
    if(key == '=')
    {
      characters[2] = atoi(mathCalc);
      characters[2] = atoi(mathCalc);
      for(int i = 0; i < 15; i++)
      {
      	mathCalc[i] = 0;
      }
      Serial.print(characters[0]);
      Serial.print(CharacterType);
      Serial.print(characters[2]);
      calc();
    }
  }
}

//Calculates the numbers
void calc()
{
  float firstNumb = characters[0];
  float secondNumb = characters[2];

  answer = true;
  row = 1;
  columme = 0;
  lcd_1.setCursor(columme, row);
  
  //calculate
  if(CharacterType == '*')
  {
    answerNumber=firstNumb * secondNumb;
  }
  else if(CharacterType == '-')
  {
    answerNumber=firstNumb - secondNumb;
  }
  else if(CharacterType == '+')
  {
    answerNumber=firstNumb + secondNumb;
  }
  else if(CharacterType == '/')
  {
    answerNumber=firstNumb / secondNumb;
  }
  
  //checks if the answer is a float or a int
  if (answerNumber == (int)answerNumber) {
    lcd_1.print((int)answerNumber);//int
  }
  else
  {
    lcd_1.print(answerNumber);//float
  }
  
  CharacterType = 0;
  for(int i = 0; i < 15; i++)
  {
    mathCalc[i] = 0;
  }
}



//returns length of a number
int intLen(float value)
{
  int valLen = 0;
  
  if(value > 9999)
     valLen = 5;
  else if(value > 999)
     valLen = 4;
  else if(value > 99)
     valLen = 3;
  else if(value > 9)
     valLen = 2;
  else
     valLen = 1;
  
  return(valLen+3);//+3 because the desimals
}