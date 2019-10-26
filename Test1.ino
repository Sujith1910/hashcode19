/*
 * EduBioskills Arduino Biobox
 * File Name: Plant Analysis
 * Author List: Arnav Agarwal, Arsh Goyal, Sujith K 
 * Functions: readCO2PWM, setup, loop
 * Global Variables: startTime, eeAddress, ffAddress, flag, customVar
 * Hardware: AM2302 Temperature and Humidity sensor, Citicel 40XV oxygen sensor, Gold plated MHZ19-B CO2 sensor, 16x2 I2C LCD, Arduino Mega 2560
 */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
/* Preprocessor Directives and setup commands */ 

#include "cactus_io_AM2302.h"         // Header file containing predefined methods and objects for AM2302 Humidity and Temperature Sensor
#include <LiquidCrystal_I2C.h>       // Header file containing predefined methods and objects for I2C communication with LCD display
#include <EEPROM.h>                  // To access and manipulate the EEPROM memory ( a tiny hard drive of 1024 bytes ) on the Arduino Board
LiquidCrystal_I2C lcd(0x27,16,2);     // Initialises the LiquidCrystal_I2C library; 0x27: Starting address in hex, 16 rows, 2 columns
#define AM2302_PIN 2                  // Assigning AM2302 as an alias name for digital pin 2 ( Since digital pin 2 is connected to AM2302 sensor )
#define oxy A3                         // Assigning oxy as an alias name for digital pin 3 ( Since digital pin 3 is connected to the oxygen sensor )
#define CO2_PIN 9                     // Assigning CO2_PIN as an alias name for digital pin 9 ( Since digital pin 9 is connected to the CO2 sensor )
AM2302 dht(AM2302_PIN);               // Initialises the AM2302 sensor.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Declaring and Initialising Global Variables */
unsigned long startTime = millis();   // Declaring variable startTime that stores the starting time in milliseconds
double th,tl,tpwm,p1,p2,ppm_pwm;      // Declaring variables required to measure the CO2 ppm
int eeAddress = 0;                    // Declaring variable eeAddress to iterate through EEPROM memory ( stores address location in EEPROM memory )
int ffAddress = 0;                    // Declaring variable ffAddress to iterate through EEPROM memory ( stores address location in EEPROM memory )
char flag;                            // Used as a check variable 
struct MyObject                       // Defining object MyObject that will store the various sensor readings and the time
{
  float hum;
  float temp;
  float oxygen;
  float carbon;
  int timee;
};
MyObject customVar =                  // Declaring customVar of type Myobject and initialising all variables to zero 
  {
    0.00f,
    0.00f,
    0.00f,
    0.00f,
    0 };

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Function Name: readCO2PWM
 * Input: None
 * Output: CO2 in ppm 
 * Logic: Reads the incoming PWM signal from CO2 sensor and converts it to corresponding PPM of CO2 using basic knowledge of PPM signals and ratio proportions.
 */

int readCO2PWM() 
{
  th = pulseIn(CO2_PIN, HIGH, 3000000) ;                    // Measures the time in milliseconds for which the incoming pulse is high
                                                           // Determines the time for which the pulse is low (total pulse time = 1004 = pulse high + pulse low
  ppm_pwm = 5000 * (th-p1)/(tpwm-p2);                    // Determines the CO2 concentration in ppm
  return ppm_pwm;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Function Name: setup
 * Input: None
 * Output: Sets up the Serial Monitor, Temperature and Humidity Sensor, CO2 sensor and the LCD
 * Logic: The setup() function is called when a sketch starts. It is used to initialize variables, pin modes, start using libraries, etc. 
 * The setup() function will only run once, after each powerup or reset of the Arduino board.
 */

void setup() 
{
  Serial.begin(9600);                                                                                 // Sets the Baud Rate to 9600
  //Serial.println("Time(s)\t\tHumidity \t Temp (C) \t Oxygen (mv) \t Carbon Dioxide (ppm) ");          // Prints on the Serial Monitor
  dht.begin();                                                                                        // Fires up the AM2302 sensor
  lcd.init();                                                                                         // Initialises the LCD
  /////////////////////////   for CO2 sensor    ///////////////////////////////////////////////////          
  pinMode(CO2_PIN, INPUT);                                                                            // Declares CO2_Pin as an input pin
  th = pulseIn(CO2_PIN, HIGH, 3000000);                                                               // Measures the time in milliseconds for which the incoming pulse is high
  tl = pulseIn(CO2_PIN, LOW, 3000000);                                                                // Measures the time in milliseconds for which the incoming pulse is low
  tpwm = th + tl;                                                                                     // Measures the actual pulse width
  p1 = tpwm/502;                                                                                      // Measures the start pulse width
  p2 = tpwm/251;                                                                                      // Measures the start and end pulse width combined
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * Function Name: loop
 * Input: None
 * Output: All functioning and implementations are executed 
 * Logic: The loop() function does precisely what its name suggests, and loops consecutively, allowing the program to change and respond. Used to actively control the Arduino board.
 */

void loop() 
{
  lcd.clear();                                                    //Clears the LCD buffer
  lcd.backlight();                                                // To switch on the LCD backlight
  dht.readHumidity();                                             // AM2302 reads live humidity into its object
  dht.readTemperature();                                          // AM2302 reads live temperature into its object
  int ppm_oxy=analogRead(oxy)*4.8876;                             // Reads input voltage from oxygen sensor and converts it into mV
  int ppm_pwm = readCO2PWM();                                    
/*  if (isnan(dht.humidity) || isnan(dht.temperature_C))            // Check statement to make sure AM2302 is working
  {
      Serial.println("DHT sensor read failure!");
      return;
  }*/
  customVar.timee=(((millis() - startTime) / 60000));             // measures the time form start of the experiment in minutes and stores it in the time variable
  lcd.setCursor(0,0);                                             // sets the cursor at the required position on the LCD
  lcd.print("Hum ");                                              // prints the string within quotes on the LCD screen
  lcd.setCursor(4,0);
  lcd.print(dht.humidity);
  lcd.setCursor(11,0); 
  lcd.print(customVar.timee);
  lcd.setCursor(15,0);
  lcd.print("m");
  lcd.setCursor(0,1); 
  lcd.print("Temp ");
  lcd.setCursor(5,1);
  lcd.print(dht.temperature_C);
  delay(3000);
  //////////////////////////////////////////////////////
  lcd.setCursor(0,0);
  lcd.print("                             ");  
  lcd.setCursor(0,1);
  lcd.print("                           ");
  ////////////////////////////////////////////////////////////
  lcd.setCursor(0,0);
  lcd.print("Oxy(mv) ");                  
  lcd.setCursor(9,0);
  lcd.print(ppm_oxy);
  lcd.setCursor(0,1);
  lcd.print("CO2(ppm) ");
  lcd.setCursor(9,1);
  lcd.print(ppm_pwm);
  
  customVar.hum = dht.humidity;                           // stores the value of humidity in the variable hum which is data member of the structure MyObject
  customVar.temp = dht.temperature_C;                     // stores the value of temperature in the variable temp which is data member of the structure MyObject
  customVar.oxygen = ppm_oxy;                             // stores the value of oxygen in millivolts in the variable oxygen which is data member of the structure MyObject  
  customVar.carbon = ppm_pwm;                             // stores the value of CO2 in ppm in the variable carbon which is data member of the structure MyObject
  EEPROM.put(eeAddress, customVar);                       // stores the current readings taken in the internal memory of the Arduino called EEPROM(Electronically Erasable and Programmable Read Only Memory)
  eeAddress += sizeof(MyObject);                          
  if (eeAddress == EEPROM.length())                       // checks if EEPROM is full
  {
    eeAddress = 0;                                        // resets if full
  }
  //if(Serial.available())
  //{
    //flag=Serial.read();
    //while(flag=='p')                                      // if 'p' is entered in the serial monitor, all the previous readings from the start to the current reading will appear on the serial monitor
    //{                 
      EEPROM.get(ffAddress, customVar);
      ffAddress += sizeof(MyObject);
      //Serial.println("------------------------------------------------------------------------------------------");
     // Serial.print("Time from start: ");
      Serial.println(customVar.timee);
      //Serial.print(" min\t\t");
      //Serial Monitor
      Serial.println(customVar.hum); 
      //Serial.print("\t\t ");
      Serial.println(customVar.temp); 
      //Serial.print("\t\t ");                                   
      Serial.println(customVar.oxygen);
      //Serial.print("\t\t ");
      Serial.println(customVar.carbon);
      //Serial.print("\n ");
      //if(ffAddress==eeAddress)
        //break;
      delay(300);
    //}
 // }
delay(5000);                                          // change this delay according to your requirements to change the frequency of input readings from the sensors.
                                                      // value within the parenthesis is in milliseconds. 1sec = 1000 milliseconds.
}
