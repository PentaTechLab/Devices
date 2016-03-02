#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>

/*******************************************************

This program controlls heating system with the LCD panel and the buttons, reley module and 18B20 seonsors.
Taras Petriv, May 2015

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define ONE_WIRE_BUS 33

// Read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 if (adc_key_in > 1000) return btnNONE; 
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}

float heat[8] = {21,22,23,24,25,26,27,28};                                       // Sectors temperature
float maxt[8] = {21.00f,22.00f,23.00,24,25,26,27,28};                                       // Maximal temp values
int  relay[8] = {53,51,49,47,45,43,41,39};                                       // Relay pins
int i = 0;                                                                       // Supplementary counters
int n = 0;                                
int s = 0;
int adress = 0;
int fault;                                                                       // Faulty sector indicator
int selected;                                                                    // Indication of selected sector 
bool smode = false;                                                              // Selection Mode

// OneWire definition
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire); 
DeviceAddress Sensor[8] = { { 0x28, 0xFF, 0x05, 0x35, 0x4C, 0x04, 0x00, 0xE2 },    // sensors ROM numbers
                            { 0x28, 0xFF, 0x77, 0x38, 0x4C, 0x04, 0x00, 0x91 }, 
                            { 0x28, 0xFF, 0xA5, 0x2C, 0x4E, 0x04, 0x00, 0x35 },
                            { 0x28, 0xFF, 0x2D, 0x20, 0x4E, 0x04, 0x00, 0xD2 },
                            { 0x28, 0xFF, 0xA8, 0x35, 0x4C, 0x04, 0x00, 0x3D },
                            { 0x28, 0xFF, 0xB9, 0x2F, 0x4E, 0x04, 0x00, 0xE0 },
                            { 0x28, 0xFF, 0x3F, 0x36, 0x4C, 0x04, 0x00, 0x53 },
                            { 0x28, 0xFF, 0x91, 0x13, 0x4D, 0x04, 0x00, 0x98 }};  
void setup()

{
 
 // LCD
 lcd.print(" WE GROW STRONG ");         // HELLO sign
 delay(2777);
 lcd.clear();
 lcd.begin(16, 2);             
 lcd.setCursor(0,0);
 lcd.print("Num Temp  MAX");                 
 lcd.setCursor(0,1);            
 lcd.print(i+1);
 lcd.setCursor(4,1);            
 lcd.print(heat[0]);
 lcd.setCursor(10,1);            
 lcd.print(maxt[0]);  

// Sensor
sensors.begin();
  sensors.setResolution(Sensor[0], 10);
  sensors.setResolution(Sensor[1], 10);
  sensors.setResolution(Sensor[2], 10);
  sensors.setResolution(Sensor[3], 10);
  sensors.setResolution(Sensor[4], 10);
  sensors.setResolution(Sensor[5], 10);
  sensors.setResolution(Sensor[6], 10);
  sensors.setResolution(Sensor[7], 10);
  
// Relay 
   pinMode(53, OUTPUT);
   pinMode(51, OUTPUT);
   pinMode(49, OUTPUT);
   pinMode(47, OUTPUT);
   pinMode(45, OUTPUT);
   pinMode(43, OUTPUT);
   pinMode(41, OUTPUT);
   pinMode(39, OUTPUT);
   
   // Maxt restoring from EEPROM
   for (s=0; s<8; s++)
   { EEPROM.get(adress, maxt[s]); 
     adress = adress + 4; 
   }
 }
 
void loop()
{
  lcd_key = read_LCD_buttons();  // Read the buttons

 switch (lcd_key)                // Buttons actions
 {
   case btnRIGHT:                // Next sector
     {delay(100);
     ++i; 
     if (i > 7) i=0; 
     lcd.setCursor(0,1);            
     lcd.print(i+1);
     lcd.setCursor(4,1); 
     lcd.print(heat[i]);
     selected = i;
     lcd.setCursor(10,1); 
     lcd.print(maxt[i]);
     break;}
   case btnLEFT:                // Previous sector
     {delay(100);
     --i; 
     if (i < 0) i=7; 
     lcd.setCursor(0,1);            
     lcd.print(i+1);
     lcd.setCursor(4,1); 
     lcd.print(heat[i]);
     selected = i;
     lcd.setCursor(10,1); 
     lcd.print(maxt[i]);
     break;}
   case btnUP:                  // increase temp
     {delay(50);
     maxt[i]= maxt[i]+0.25f;            
     lcd.setCursor(10,1); 
     lcd.print(maxt[i]);
     adress = i * 4;
     EEPROM.put(adress, maxt[i]);
     break;}
   case btnDOWN:                // decrease temp
     {delay(50);
     maxt[i]= maxt[i]-0.25f;            
     lcd.setCursor(10,1); 
     lcd.print(maxt[i]);
     adress = i * 4;
     EEPROM.put(adress, maxt[i]);
     break;}
   case btnSELECT:
     {break;}
   case btnNONE:
     {break;}
 }
 

     lcd.setCursor(9,1); 
     lcd.print(" ");

// Sensors reading
  delay (100);
  sensors.requestTemperatures();
  for (s=0; s<8; s++){
   float temp = sensors.getTempC(Sensor[s]);
   if (heat [s] != temp) heat[s] = temp;
   if (selected == s)
   {
     heat[s] = temp;
     lcd.setCursor(4,1); 
     lcd.print(heat[s]);
   }
  }
    
// Relay heat control
  for (n=0; n<8; n++) 
  {
  if (heat[n] <= (maxt[n] - 0.5)) digitalWrite(relay[n], LOW);   // Relay ON
  else 
  if (heat[n] >= (maxt[n] + 0.5)) digitalWrite(relay[n], HIGH);  // Relay OFF
  }
  
}
