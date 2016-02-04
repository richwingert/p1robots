// include the library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <Servo.h>

//define the LCD variable
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

Servo LServo, RServo;

// These constants won't change.  
// They're used to give names to the pins used:
const int SFSensor = A0;  // Analog input pin that the potentiometer is attached to
const int SLSensor = A1;  // Analog input pin that the potentiometer is attached to
const int SRSensor = A2;  // Analog input pin that the potentiometer is attached to
const int LFSensor = A3;  // Analog input pin that the potentiometer is attached to

int SFValue = 0;    
int SLValue = 0;    
int SRValue = 0;    
int LFValue = 0;    

//default menu selection
int menuOption=1;

//initialize the robot
void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("U/D Selects,");
  lcd.setCursor(0,1);
  lcd.print("L/R Quits");
  
}

//handles menu usage
void menu(){
  
  uint8_t buttons = lcd.readButtons();
  if(buttons){
    
    
    lcd.setCursor(0,0);

    //series of "if" statements that do one of five things,
    //then display the selected menu option
    //UP      increments the menu option
    //DOWN    decrements the menu option
    //SELECT  selects the current menu option and performs that task
    //LEFT    no current functionality
    //RIGHT   no current functionality
    
    if(buttons & BUTTON_UP){      //increment/ display menu option
      lcd.clear();
      if(menuOption==6)
        menuOption=1;
      else
        menuOption++;
      lcd.print("Part ");
      lcd.print(menuOption);
      lcd.print(": ");
      lcd.setCursor(0,1);
      lcd.print(menuParse(menuOption));
      delay(500);   //required to force each button press to equal one change
    }
    if(buttons & BUTTON_DOWN){    //decrement/ display menu option
      lcd.clear();
      if(menuOption==1)
        menuOption=6;
      else
        menuOption--;
      lcd.print("Part ");
      lcd.print(menuOption);
      lcd.print(": ");
      lcd.setCursor(0,1);
      lcd.print(menuParse(menuOption));
      delay(500);   //required to force each button press to equal one change
    }
    if(buttons & BUTTON_LEFT){}   //do nothing
    if(buttons & BUTTON_RIGHT){}  //do nothing
    if(buttons & BUTTON_SELECT){  //performs current menu option
      menuSelect(menuOption);
    }
  }
}
//this function parses which menu option to display
String menuParse(int x){
  switch(x){
    case 1:
      return "Short Sensor ";
      break;
    case 2:
      return "Long Sensor ";
      break;
    case 3:
      return "Simple Move";
      break;
    case 4:
      return "Square Move";
      break;
    case 5:
      return "Figure 8 Move";
      break;
    case 6:
      return "Callibration";
      break;
    default:
      return "error";
  }

  
  }
//this function performs the menu option selected
void menuSelect(int x){
  switch(x){
    case 1:
      part1();
      break;
    case 2:
      part2();
      break;
    case 3:
      part3();
      break;
    case 4:
      part4();
      break;
    case 5:
      part5();
      break;
    case 6:
      callibrate();
      break;
    
      
  }
}

//helper function to check if L/R was pressed to quit
bool quitCheck(){
  bool quit=false;
  uint8_t buttons = lcd.readButtons();
  if(buttons & (BUTTON_LEFT | BUTTON_RIGHT))
  {
    quit=true;
  }
  return quit;
}
//helper function to calibrate servos
void callibrate(){
  LServo.attach(2);
  RServo.attach(3);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hold L/R to");
  lcd.setCursor(0,1);
  lcd.print("quit calibration");
  bool quit=false;
  while(!quit)
  {
    //move forward
    RServo.write(90);
    LServo.write(90);
    delay(500);
    quit=quitCheck();
  }
  RServo.detach();
  LServo.detach();
  lcd.clear();
}

//the following two functions are used to convert the voltage
//outputs from the sensors to cm. These equations were obtained
//by plotting the chart from each respective sensor's data sheet
//and finding a trend line in excel.
//After getting a trendline in the format y=nx^k, i solved for x
double SRtoCm(double x){
  x=x/100;
  return 12.8528/pow(x,1.1170189739);
}
double LRtoCm(double x){
  x=x/100;
  return 62.1164/pow(x,1.13117084376871);
}


//next several functions handle the tasks 
//required from the project specifications



//function to handle the tasks in part 1
//display distances measured from the 3
//short range sensors attached to 
//front, left, and right of robot
void part1(){
  bool quit=false;
  while(!quit)
  {
    /*
    // read the analog in values:
    SFValue = analogRead(SFSensor);
    SLValue = analogRead(SLSensor);
    SRValue = analogRead(SRSensor);
    LFValue = analogRead(LFSensor); 
    
    Serial.print("sensor Front = ");
    Serial.print(SRtoCm(SFValue));
    Serial.print("\t\t ");
    
    Serial.print("sensor Left = ");
    Serial.print(SRtoCm(SLValue));
    Serial.print("\t\t  ");
    
    Serial.print("sensor Right = ");
    Serial.print(SRtoCm(SRValue));
    Serial.print("\t\t  ");
    
    Serial.print("sensor Long Front = ");
    Serial.print(LRtoCm(LFValue));
    Serial.print("\n  ");
  */
    
    if(!quit)
    {
      lcd.clear();
      SFValue = analogRead(SFSensor);
      lcd.setCursor(0,0);
      lcd.print("Short Front");
      lcd.setCursor(0,1);
      lcd.print(SRtoCm(SFValue));
      delay(1000);
      quit=quitCheck();
    }
    if(!quit)
    {
      lcd.clear();
      SLValue = analogRead(SLSensor);
      lcd.setCursor(0,0);
      lcd.print("Short Left");
      lcd.setCursor(0,1);
      lcd.print(SRtoCm(SLValue));
      delay(1000);
      quit=quitCheck();
    }
    if(!quit)
    {
      lcd.clear();
      SRValue = analogRead(SRSensor);
      lcd.setCursor(0,0);
      lcd.print("Short Right");
      lcd.setCursor(0,1);
      lcd.print(SRtoCm(SRValue));
      delay(1000);
      quit=quitCheck();
    }
  }
}

//function to handle the tasks in part 2
//display distance measured from 
//long range sensor in the front
//range 2-59 inches. must also utilize
//short range sensor if too close for
//long range sensor to be accurate
void part2(){
  bool quit=false;
  while(!quit)
  {
    if(!quit)
    {
      SFValue = analogRead(SFSensor);
      LFValue = analogRead(LFSensor);
      Serial.print("short Front = ");
      Serial.print(SFValue);
      Serial.print("\t\t ");
      Serial.print("long Front = ");
      Serial.print(LFValue);
      Serial.print("\n ");
      if(SFValue<500 & SFValue>60)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Short Front");
        lcd.setCursor(0,1);
        lcd.print(SRtoCm(SFValue));
        delay(1000);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Long Front");
        lcd.setCursor(0,1);
        lcd.print(LRtoCm(LFValue));
        delay(1000);
      }
      quit=quitCheck();
    }
  }
}


//function to handle the tasks in part 3
//robot must move without sensor information
// a) straight forward, 50 in
// b) straight backward, 50 in
// c) clockwise circle, 20 in diameter
// d) counter-clockwise circle, 20 in diameter
void part3(){
  LServo.attach(2);
  RServo.attach(3);
  
  //quit flag is used to exit from operation
  bool quit=false;
  //loops until L/R is held
  while(!quit )
  {
    //move forward
    if(!quit)
    {
      RServo.write(0);
      LServo.write(180);
      delay(9000);
      quit=quitCheck();
    }
    //move backward
    if(!quit)
    {
      RServo.write(180);
      LServo.write(0);
      delay(9000);
      quit=quitCheck();
    }
    
    //clockwise
    if(!quit)
    {
      RServo.write(86);
      LServo.write(97);
      delay(20000);
      quit=quitCheck();
    }
    //counter-clockwise
    if(!quit)
    {
      RServo.write(94);
      LServo.write(83);
      delay(20000);
      quit=quitCheck();
    }
  }
  RServo.detach();
  LServo.detach();
  
}

//function to handle the tasks in part 4
//robot must move without sensor information
// a) clockwise square with 20 inch sides
// b) counter-clockwise square with 20 inch sides
void part4(){
  LServo.attach(2);
  RServo.attach(3);
  
  //quit flag is used to exit from operation
  bool quit=false;
  //loops until L/R is held
  while(!quit )
  {
    //clockwise square
    for(int i=0;i<4;i++)
    {
      if (!quit)
      {
        //move forward
        RServo.write(0);
        LServo.write(180);
        delay(4500);
        //turn
        RServo.write(0);
        LServo.write(0);
        delay(650);
        quit=quitCheck();
      }
    }
    
    //counter-clockwise square
    RServo.write(180);
    LServo.write(180);
    delay(650);
    for(int i=0;i<4;i++)
    {
      if (!quit)
      {
        //move backward
        RServo.write(180);
        LServo.write(0);
        delay(4500);
        //turn
        RServo.write(180);
        LServo.write(180);
        delay(650);
        quit=quitCheck();
      }
    }
  }
  RServo.detach();
  LServo.detach();
}

//function to handle the tasks in part 5
//robot must move without sensor information
// a) figure 8 
//  1) based off of 
//    i) 2 circles, 20 in diameter
//    ii) each circle is separated by 10 in
void part5(){
  LServo.attach(2);
  RServo.attach(3);
  //quit flag is used to exit from operation
  bool quit=false;
  //loops until L/R is held
  while(!quit )
  {
    //clockwise
    RServo.write(86);
    LServo.write(97);
    delay(13000);
    //forward
    RServo.write(0);
    LServo.write(180);
    delay(4000);
    //counter-clockwise
    RServo.write(85);
    LServo.write(93);
    delay(17000);
    //forward
    RServo.write(0);
    LServo.write(180);
    delay(4000);
    quit=quitCheck();
  }
  RServo.detach();
  LServo.detach();
}

//loop to continue watching for menu activity
void loop() {
  menu();
}

