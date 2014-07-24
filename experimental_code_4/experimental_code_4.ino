#include <PS2X_lib.h>

PS2X ps2x; // create PS2 Controller Class
 
//right now, the library does NOT support hot pluggable controllers, meaning
//you must always either restart your Arduino after you conect the controller,
//or call config_gamepad(pins) again after connecting the controller.
int error = 0;
byte type = 0;
byte vibrate = 0;
 
//Arduino PWM Speed Control for Arduino Motor Shield (default pins)
int E1 = 3;
int M1 = 12;
int E2 = 11;
int M2 = 13;
int lmotor = 0;
int rmotor = 0;
 
void setup()
{
  Serial.begin(57600);
 
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);

  error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad
  //(clock, command, attention, data, Pressures?, Rumble?) check for error
 
  if(error == 0)
  {
    Serial.println("Found Controller, configured successful");
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Go to www.billporter.info for updates and to report bugs.");
   }
   else if(error == 1)
   {
     Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   }
   else if(error == 2)
   {
     Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   }
   else if(error == 3)
   {
     Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   }
 
   type = ps2x.readType();
   if (type != 1)
   {
     Serial.println("warning: DualShock Controller Not Found!");
   }
}
 
void loop()
{
 //Serial.println("Start of loop");
 if(error == 1){ //skip loop if no controller found
  Serial.println("Skipped Loop");
  return;
 }
 if (type == 1)
 {
   //Serial.println("Reading Controller");
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
 
   lmotor = 0;
   if (ps2x.Button(PSB_L1)){
     //Serial.println("L1 Pressed");
     lmotor = 255;
   }
   if (ps2x.Button(PSB_L2)){
     //Serial.println("L2 Pressed"); 
     lmotor = -255;
   }
 
   rmotor = 0;
   if (ps2x.Button(PSB_R1)){
     //Serial.println("R1 Pressed");
     rmotor = -255;
   }
  if (ps2x.Button(PSB_R2)){
     //Serial.println("R2 Pressed");
     rmotor = 255;
   }
 
 }
 else
 {
   lmotor = 0;
   rmotor = 0;
 }
 
 // update motors
   if (lmotor < 0)
   {
    //Serial.println("M1OFF");
    digitalWrite(M1, LOW);
    analogWrite(E1, -lmotor);   //PWM Speed Control
   }
   else
   {
    //Serial.println("M1ON");
    digitalWrite(M1, HIGH);
    analogWrite(E1, lmotor);   //PWM Speed Control
   }
 
   if (rmotor < 0)
   {
    //Serial.println("M2Off");
    digitalWrite(M2, LOW);
    analogWrite(E2, -rmotor);   //PWM Speed Control
   }
   else
   {
    //Serial.println("M2ON");
    digitalWrite(M2, HIGH);
    analogWrite(E2, rmotor);   //PWM Speed Control
   }
 
 delay(30);
}
