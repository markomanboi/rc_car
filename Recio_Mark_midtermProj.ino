// Simple Bluetooth Controlled Car using App Inventor
// by Mark Raphael Recio


#include <AFMotor.h>                                      // The Adafruit Motor Shield Library ver. 1
#include <Wire.h>
#include <LiquidCrystal_I2C.h>;
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);   
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
                                                      
//****************************************************************************************************
// Setup Drive Motors using the Adafruit Motor Controller version 1.0 Library

AF_DCMotor motor1(1, MOTOR12_64KHZ);                      // create motor #1, 64KHz pwm
AF_DCMotor motor2(2, MOTOR12_64KHZ);                      // create motor #2, 64KHz pwm
AF_DCMotor motor3(3, MOTOR12_64KHZ);                      // create motor #3, 64KHz pwm
AF_DCMotor motor4(4, MOTOR12_64KHZ);                      // create motor #4, 64KHz pwm

int mtr_Spd = 200;                                        // set the speed of the motors
int ledRed=37;
int ledGreen=39;

//*****************************************************************************************************
// Bluetooth Variables & Setup

String str;                                               // raw string received from android to arduino
int blueToothVal;                                         // stores the last value sent over via bluetooth
String blueToothLCD;
//*****************************************************************************************************

void setup() 
{  
  Serial.begin(9600);                                  // Serial 0 is for communication with the serial monitor
  Serial1.begin(9600);                                   // Serial 1 is for Bluetooth communication - DO NOT MODIFY  
  lcd.begin(16,2);
  dht.begin();
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  digitalWrite(ledGreen,LOW);    
  digitalWrite(ledRed,HIGH);   
}

//*****************************************************************************************************
// Main Loop

void loop()
{
  bluetooth();                                          // Run the Bluetooth procedure to see if there is any data being sent via BT
}




//*****************************************************************************************************
// This procedure reads the serial port - Serial1 - for bluetooth commands being sent from the Android device

void bluetooth()
{
 while (Serial1.available())                             // Read bluetooth commands over Serial1 // Warning: If an error with Serial1 occurs, make sure Arduino Mega 2560 is Selected
 {  
  {  
      str = Serial1.readStringUntil('\n');               // str is the temporary variable for storing the last sring sent over bluetooth from Android device
      //Serial.print(str);             
  } 
    
    blueToothVal = (str.toInt());                        //  convert the string 'str' into an integer and assign it to blueToothVal
    blueToothLCD=str; 
  
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    String combineTempHumid="H:"+String(h)+"% "+"T:"+String(t)+"C"; 
    lcd.setCursor(0,1);
    lcd.print(combineTempHumid);
// Bluetooth Section
// *********************************************************************************************************

 switch (blueToothVal) 
   {
        case 1:               
          digitalWrite(ledGreen,HIGH);    
          digitalWrite(ledRed,LOW);                 
          Forward();
          break;
  
        case 2:
          digitalWrite(ledGreen,HIGH);    
          digitalWrite(ledRed,LOW);                    
          Reverse();
          break;
  
        case 3:         
          digitalWrite(ledGreen,HIGH);    
          digitalWrite(ledRed,LOW);   
          LeftTurn();
          Stop();
          break;
          
        case 4:                     
          digitalWrite(ledGreen,HIGH);    
          digitalWrite(ledRed,LOW);   
          RightTurn();
          Stop();
          break;       
        case 5:                       
          digitalWrite(ledGreen,LOW);    
          digitalWrite(ledRed,HIGH);                        
          Stop();
          break;
        case 6:
          Serial1.print(combineTempHumid);
          break;
        default:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(blueToothLCD);    
          lcd.setCursor(0,1);
           lcd.print(combineTempHumid);
          break;  
   }                                                              // end of switch case 

 }                                                              // end of while loop Serial1 read
 
  
}




// Motor Control Section
// **********************************************************************************************************************************************************************

void Forward()
{
  motor1.setSpeed(mtr_Spd);                                                   
  motor2.setSpeed(mtr_Spd);     
  motor3.setSpeed(mtr_Spd);     
  motor4.setSpeed(mtr_Spd);                           
  
  motor1.run(FORWARD);                                                         
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

// **********************************************************************************************************************************************************************


void Reverse()
{
  motor1.setSpeed(mtr_Spd);                                                   
  motor2.setSpeed(mtr_Spd);     
  motor3.setSpeed(mtr_Spd);     
  motor4.setSpeed(mtr_Spd);    
  
  motor1.run(BACKWARD);                                                        
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

// **********************************************************************************************************************************************************************

void LeftTurn()
{
  motor1.setSpeed(250);                                                   
  motor2.setSpeed(250);     
  motor3.setSpeed(250);     
  motor4.setSpeed(250);    
  
  motor1.run(BACKWARD);                                                        
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
  delay(15);
 
}

// **********************************************************************************************************************************************************************

void RightTurn()
{
  motor1.setSpeed(250);                                                   
  motor2.setSpeed(250);     
  motor3.setSpeed(250);     
  motor4.setSpeed(250);   

  motor1.run(FORWARD);                                              
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);        
  delay(15);                                                   
}

// **********************************************************************************************************************************************************************

void Stop()
{ 
  motor1.run(RELEASE);                                                         
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);   
}
