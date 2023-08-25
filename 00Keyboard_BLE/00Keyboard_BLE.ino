//options:
//#define DEBUG


//libs:


#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

#include "Adafruit_Keypad.h"

const byte ROWS = 5; // rows
const byte COLS = 12; // columns
byte rowPins[ROWS] = {13,14,17,18,21}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {1,2,3,4,5,6,7,8,9,10,11,12}; //connect to the column pinouts of the keypad

//define keymaps:
//IMPORTANT MODIFIER SCAN CODES
//177= KEY_ESC
//178= KEY_BACKSPACE
//179= KEY_TAB
//193= KEY_CAPS_LOCK
//129= KEY_LEFT_SHIFT
//128= KEY_LEFT_CTRL
//132= KEY_RIGHT_CTRL
//130= KEY_LEFT_ALT
//134= KEY_RIGHT_ALT
//131= KEY_LEFT_GUI
//XXX = KEY_RIGHT_GUI
//212 = KEY_DELETE
//176 = KEY_RETURN

//default:
const char keys1[ROWS][COLS] = {
  {KEY_ESC,'1','2','3','4','5','6','7','8','9','0',KEY_BACKSPACE},
  {KEY_TAB,'q','w','e','r','t','z','u','i','o','p','+'},
  {KEY_CAPS_LOCK,'a','s','d','f','g','h','j','k','l','#',KEY_RETURN},
  {KEY_LEFT_SHIFT,'y','x','c','v','b','n','m',',','.','-','<'},
  {KEY_LEFT_CTRL,KEY_LEFT_ALT,KEY_LEFT_GUI,' ',' ',' ',' ',' ',KEY_RIGHT_GUI,KEY_RIGHT_ALT,KEY_RIGHT_CTRL,KEY_DELETE}
  };
//SHIFT: //§ INVALID FOR SOME REASON??!
const char keys2[ROWS][COLS] = {
  {KEY_ESC,'!','"',' ','$','%','&','/','(',')','=','?'},
  {KEY_TAB,'Q','W','E','R','T','Z','U','I','O','P','*'},
  {KEY_CAPS_LOCK,'A','S','D','F','G','H','J','K','L','\'',KEY_RETURN},
  {KEY_LEFT_SHIFT,'Y','X','C','V','B','N','M',';',':','_','>'},
  {KEY_LEFT_CTRL,KEY_LEFT_ALT,KEY_LEFT_GUI,' ',' ',' ',' ',' ',KEY_RIGHT_GUI,KEY_RIGHT_ALT,KEY_RIGHT_CTRL,KEY_DELETE}
  };
  
//ALT:
const char keys3[ROWS][COLS] = {
  {KEY_ESC,'1','2','3','4','5','6','{','[',']','}','\\'},
  {KEY_TAB,'@',KEY_UP_ARROW,'e','r','t','z','u','i','o','p','~'},
  {KEY_CAPS_LOCK,KEY_LEFT_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,'f','g','h','j','k','l','#',KEY_RETURN},
  {KEY_LEFT_SHIFT,'y','x','c','v','b','n','m',',','.','-','|'},
  {KEY_LEFT_CTRL,KEY_LEFT_ALT,KEY_LEFT_GUI,KEY_LEFT_ARROW,KEY_UP_ARROW,KEY_DOWN_ARROW,KEY_RIGHT_ARROW,KEY_RIGHT_GUI,KEY_RIGHT_ALT,KEY_RIGHT_CTRL,KEY_DELETE}
  };

//FN:
const char keys4[ROWS][COLS] = {
  {KEY_ESC,'1','2','3','4','5','6','7','8','9','0',KEY_BACKSPACE},
  {KEY_TAB,'q','w','e','r','t','z','u','i','o','p','+'},
  {KEY_CAPS_LOCK,'a','s','d','f','g','h','j','k','l','#',KEY_RETURN},
  {KEY_LEFT_SHIFT,'y','x','c','v','b','n','m',',','.','-','<'},
  {KEY_LEFT_CTRL,KEY_LEFT_ALT,KEY_LEFT_GUI,' ',' ',' ',' ',' ',KEY_RIGHT_GUI,KEY_RIGHT_ALT,KEY_RIGHT_CTRL,KEY_DELETE}
  };



//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys1), rowPins, colPins, ROWS, COLS);
Adafruit_Keypad customKeypadSHIFT = Adafruit_Keypad( makeKeymap(keys2), rowPins, colPins, ROWS, COLS);
Adafruit_Keypad customKeypadALT = Adafruit_Keypad( makeKeymap(keys3), rowPins, colPins, ROWS, COLS);
Adafruit_Keypad customKeypadFN = Adafruit_Keypad( makeKeymap(keys4), rowPins, colPins, ROWS, COLS);


//leds:
#define led1 39
#define led2 40
#define led3 41
#define led4 42
int layer = 1; // active layer number
void setleds(int l){
 if (l == 1){
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
 }
 if (l == 2){
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
 }
 if (l == 3){
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, LOW);
  digitalWrite(led4, HIGH);
 }
 if (l == 4){
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, LOW);
 }
 
}

String msg; //message to be sent




void setup() {
  Serial.begin(9600);//Serial
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  
 
  bleKeyboard.begin();//BLE
  customKeypad.begin();
  customKeypadSHIFT.begin();
  customKeypadALT.begin();
  customKeypadFN.begin();
  delay(1000);
  digitalWrite(led1, LOW);
  delay(250);
  digitalWrite(led1, HIGH);
  delay(250);
  digitalWrite(led2, LOW);
  delay(250);
  digitalWrite(led2, HIGH);
  delay(250);
  digitalWrite(led3, LOW);
  delay(250);
  digitalWrite(led3, HIGH);
  delay(250);
  digitalWrite(led4, LOW);
  delay(250);
  digitalWrite(led4, HIGH);
  delay(250);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  Serial.println("READY");
  delay(1000);
  setleds(layer); //set deafult active layer leds
}

void loop() {
// which layer are we on?


// get key from matrix:
if (layer == 1){
  customKeypad.tick();
  if(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    if (e.bit.KEY == 129 and e.bit.EVENT != KEY_JUST_RELEASED){
    layer = 2;
    setleds(layer);
   }
   else if ((e.bit.KEY == 130 and e.bit.EVENT != KEY_JUST_RELEASED )or( e.bit.KEY == 134 and e.bit.EVENT != KEY_JUST_RELEASED) ){
    layer = 3;
    setleds(layer);
   }
   else if (e.bit.KEY == 131 and e.bit.EVENT != KEY_JUST_RELEASED){
    layer = 4;
    setleds(layer);
   }
    if ((e.bit.KEY == 129 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 130 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 134 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 131 and e.bit.EVENT == KEY_JUST_RELEASED)){
    layer = 1;
    setleds(layer);
   }
   if(bleKeyboard.isConnected()) {
    if(e.bit.EVENT == KEY_JUST_PRESSED)bleKeyboard.press(e.bit.KEY);
    else if(e.bit.EVENT == KEY_JUST_RELEASED) bleKeyboard.release(e.bit.KEY);
   }
   #ifdef DEBUG
    Serial.print(e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
    else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
   #endif 
}
}
else if (layer == 2){
  customKeypadSHIFT.tick();
  if(customKeypadSHIFT.available()){
    keypadEvent e = customKeypadSHIFT.read();
    if (e.bit.KEY == 129 and e.bit.EVENT != KEY_JUST_RELEASED){
    layer = 2;
    setleds(layer);
   }
   else if ((e.bit.KEY == 130 and e.bit.EVENT != KEY_JUST_RELEASED )or( e.bit.KEY == 134 and e.bit.EVENT != KEY_JUST_RELEASED) ){
    layer = 3;
    setleds(layer);
   }
   else if (e.bit.KEY == 131 and e.bit.EVENT != KEY_JUST_RELEASED){
    layer = 4;
    setleds(layer);
   }
    if ((e.bit.KEY == 129 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 130 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 134 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 131 and e.bit.EVENT == KEY_JUST_RELEASED)){
    layer = 1;
    setleds(layer);
   }
   if(bleKeyboard.isConnected()) {
    if(e.bit.EVENT == KEY_JUST_PRESSED)bleKeyboard.press(e.bit.KEY);
    else if(e.bit.EVENT == KEY_JUST_RELEASED) bleKeyboard.release(e.bit.KEY);
   }
    #ifdef DEBUG
    Serial.print(e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
    else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
   #endif
}
}
else if (layer == 3){
  customKeypadALT.tick();
  if(customKeypadALT.available()){
    keypadEvent e = customKeypadALT.read();
    if (e.bit.KEY == 129 and e.bit.EVENT != KEY_JUST_RELEASED){
    layer = 2;
    setleds(layer);
   }
   else if ((e.bit.KEY == 130 and e.bit.EVENT != KEY_JUST_RELEASED )or( e.bit.KEY == 134 and e.bit.EVENT != KEY_JUST_RELEASED) ){
    layer = 3;
    setleds(layer);
   }
   else if (e.bit.KEY == 131 and e.bit.EVENT != KEY_JUST_RELEASED){
    layer = 4;
    setleds(layer);
   }
    if ((e.bit.KEY == 129 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 130 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 134 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 131 and e.bit.EVENT == KEY_JUST_RELEASED)){
    layer = 1;
    setleds(layer);
   }
   if(bleKeyboard.isConnected()) {
    if(e.bit.EVENT == KEY_JUST_PRESSED)bleKeyboard.press(e.bit.KEY);
    else if(e.bit.EVENT == KEY_JUST_RELEASED) bleKeyboard.release(e.bit.KEY);
   }
    #ifdef DEBUG
    Serial.print(e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
    else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
   #endif
}
}
else  if (layer == 4){
  customKeypadFN.tick();
  if(customKeypadFN.available()){
    keypadEvent e = customKeypadFN.read();
    if (e.bit.KEY == 129 and e.bit.EVENT != KEY_JUST_RELEASED){
    layer = 2;
    setleds(layer);
   }
   else if ((e.bit.KEY == 130 and e.bit.EVENT != KEY_JUST_RELEASED )or( e.bit.KEY == 134 and e.bit.EVENT != KEY_JUST_RELEASED) ){
    layer = 3;
    setleds(layer);
   }
   else if (e.bit.KEY == 131 and e.bit.EVENT != KEY_JUST_RELEASED){
    layer = 4;
    setleds(layer);
   }
    if ((e.bit.KEY == 129 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 130 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 134 and e.bit.EVENT == KEY_JUST_RELEASED)
              or (e.bit.KEY == 131 and e.bit.EVENT == KEY_JUST_RELEASED)){
    layer = 1;
    setleds(layer);
   }
   if(bleKeyboard.isConnected()) {
    if(e.bit.EVENT == KEY_JUST_PRESSED)bleKeyboard.press(e.bit.KEY);
    else if(e.bit.EVENT == KEY_JUST_RELEASED) bleKeyboard.release(e.bit.KEY);
   }
    #ifdef DEBUG
    Serial.print(e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
    else if(e.bit.EVENT == KEY_JUST_RELEASED) Serial.println(" released");
   #endif
}   
}   

  }
  
  
  
