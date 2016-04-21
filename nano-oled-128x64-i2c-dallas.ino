/*********************************************************************
pro maly oled - adr 0 = 03C
pro nano - A4 = SDA / A5 = SCL
  ------> http://www.adafruit.com/category/63_98

*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define S1 0
#define S2 30
#define S3 60
#define S5 90

#define R2 0
#define R3 10
#define R4 20
#define R5 50

#define timeXmax 33
byte t0g[timeXmax];
byte t1g[timeXmax];

#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
//display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
int tempC,max0,max1,min0,min1,max2,min2,gy;

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;
DeviceAddress insideThermometer1;
DeviceAddress insideThermometer2;


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   { 
  //t0g[3]=160;
  //t0g[15]=220;
  //t0g[16]=230;
  
  max0=0;
  max1=0;
  max2=0;
  min0=900;
  min1=900;
  min2=900;
  

  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(insideThermometer1, 1)) Serial.println("Unable to find address for Device 1"); 
  if (!sensors.getAddress(insideThermometer2, 2)) Serial.println("Unable to find address for Device 2"); 
  
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(insideThermometer1);
  
  Serial.print("Device 2 Address: ");
  printAddress(insideThermometer2);
  
  Serial.println();
  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  Serial.println();
   //--------------------------------------------------  
  

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64) //0x3D
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  //display.display();
  //delay(100);

  // Clear the buffer.
  display.clearDisplay();

  // draw a single pixel
  //display.drawPixel(10, 10, WHITE);

  // draw many lines
  //testdrawline0();
  //display.display();
  //delay(1000);
  //display.clearDisplay();

  // draw the first ~12 characters in the font
  testdrawchar();
  display.display();
  delay(1000);
  display.clearDisplay();

  // text display tests
  //display.setTextSize(1);
  //display.setTextColor(WHITE);
  //display.setCursor(0,0);
  //display.println("Hello, world!");
  //display.setTextColor(BLACK, WHITE); // 'inverted' text
  //display.println(3.141592);
  //display.setTextSize(2);
  //display.setTextColor(WHITE);
  //display.print("0x"); display.println(0xDEADBEEF, HEX);
  //display.display();
  //delay(2000);
  //display.clearDisplay();
  display.setTextSize(1);
  display.println("start");
  display.display();
}

int ss=  0;
int mm = 0;

void loop() {
  gy = num2gr(150); 
  //display.drawPixel(20, gy, WHITE);
  display.drawLine(0, gy, 128, gy, WHITE);
  gy = num2gr(200); 
  display.drawLine(0, gy, 128, gy, WHITE);
  
  
  display.setCursor(S5,R3);
  display.println(mm);
    
  display.setCursor(S5+12,R2);
  display.println(ss);
    
  display.setCursor(S5,R2);
  display.println("::");
  
  display.drawLine(0, R2-5, 128, R2-5, WHITE);
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  //Serial.println("DONE");
  
  // It responds almost immediately. Let's print out the data
  //OzOled.printString("   ", 0, 5);
//---------------------------------  
  getTemperature(insideThermometer); // Use a simple function to print out the data
   //OzOled.printNumber((long)min0, 0, 3);
  display.setCursor(S2,R2);
  display.println((long)tempC);
  t0g[mm]=tempC;
 
  if (tempC<min0){ min0=tempC;}
  display.setCursor(S1,R2);  
  display.println((long)min0);
  
  if (max0<tempC){ max0=tempC;}
  display.setCursor(S3,R2);  
  display.println((long)max0);
  
//---------------------------------- 
  getTemperature(insideThermometer1); // Use a simple function to print out the data
  display.setCursor(S2,R3);
  display.println((long)tempC);
  t1g[mm]=tempC;
  
   if (tempC<min1){ min1=tempC;}
  display.setCursor(S1,R3);  
  display.println((long)min1);
  
  if (max1<tempC){ max1=tempC;}
  display.setCursor(S3,R3);  
  display.println((long)max1);
    
//---------------------------------- 
  getTemperature(insideThermometer2); // Use a simple function to print out the data
  display.setCursor(S2,R4);
  display.println((long)tempC);
  
   if (tempC<min2){ min2=tempC;}
  display.setCursor(S1,R4);  
  display.println((long)min2);
  
  if (max2<tempC){ max2=tempC;}
  display.setCursor(S3,R4);  
  display.println((long)max2);
    
    
    
   ss++;
   for (int gx=1;gx<timeXmax;gx++)
    {
   gy = num2gr(t0g[gx]); 
   display.drawPixel(gx*2, gy, WHITE);
   display.drawPixel(gx*2+1, gy, WHITE);
   gy = num2gr(t1g[gx]); 
   display.drawPixel(gx*2, gy, WHITE);
   display.drawPixel(gx*2+1, gy, WHITE);
   //gy = num2gr(t2g[gx]); 
   //display.drawPixel(gx, gy, WHITE);

    } 
  
  
  if (ss>150){ //10 měř/min > 150 = 15 min.
   mm++;
  //t0g[mm]=t0; 
  //t1g[mm]=t1; 
  
   ss=0;
   if (mm>timeXmax)
      {
        mm=0;
        display.clearDisplay();
    
      }   
   
  }     
    
  display.display();
  
  delay(5900);
  display.clearDisplay();
 
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
}

void testdrawline0() {  
    display.drawLine(0, 0, 128, 0, WHITE);
    display.drawLine(0, 20, 128, 20, WHITE);
    display.display();
  }
  
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}  

// function to print the temperature for a device
void getTemperature(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));
  //Serial.print(" Temp F: ");
  //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

  // method 2 - faster
  tempC = sensors.getTempC(deviceAddress)*10;
  Serial.print(" ");
  Serial.print(tempC);
  
 
  //Serial.print(" Temp F: ");
  //Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
}

int num2gr(int num) {
  int gr = 75-(num/10-10)*3;
  return gr;
}




