

// Learning the basics of adafruits TFT LCD Touch
// adafruit's 2.8" TFT Touch Screen 16bit color 320x240 pixels
// more info at http://www.ladyada.net/products/tfttouchbreakout/#bitmaps
// tested using Arduino IDE 0022 and code base from here https://github.com/adafruit/TFTLCD-Library
// Code by Nathan@sobisource.com 3/5/2012




#include<SPI.h>
#include <Adafruit_TFTLCD.h>
#include <Adafruit_GFX.h>
#include <TouchScreen.h>

int motorI1=52;
int motorI2= 50;
int motorpmw= 12;

int ledSC=48;
int ledA =46;
int ledC =44;
#define LCD_D0 22
#define LCD_D1 23
#define LCD_D2 24
#define LCD_D3 25
#define LCD_D4 26
#define LCD_D5 27
#define LCD_D6 28
#define LCD_D7 29
/* For the 8 data pins:
 Duemilanove/Diecimila/UNO/etc ('168 and '328 chips) microcontoller:
 D0 connects to digital 8
 D1 connects to digital 9
 D2 connects to digital 2
 D3 connects to digital 3
 D4 connects to digital 4
 D5 connects to digital 5
 D6 connects to digital 6
 D7 connects to digital 7
 For Mega's use pins 22 thru 29 (on the double header at the end)
 */

#define YP A3  // must be an analog pin
#define XM A2  // must be an analog pin
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 143
#define TS_MINY 148
#define TS_MAXX 935
#define TS_MAXY 884
//Create the touchscreen object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 500);  //(data,data,data,data,sensitivity);

//Some of the tft pins
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// Optional, used to reset the display
#define LCD_RESET A4

#define REDBAR_MINX 80
#define GREENBAR_MINX 130
#define BLUEBAR_MINX 180
#define BAR_MINY 30
#define BAR_HEIGHT 250
#define BAR_WIDTH 30

//Create the tft object
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// Define some TFT readable colour codes to human readable names
#define BLACK   0x0000
int BLUE = tft.color565(50, 50, 255);
#define DARKBLUE 0x0010
#define VIOLET 0x8888
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GREY   tft.color565(64, 64, 64);
#define GOLD 0xFEA0
#define BROWN 0xA145
#define SILVER 0xC618
#define LIME 0x07E0

void setup() {
  pinMode(ledSC, OUTPUT);
   pinMode(ledA, OUTPUT);
    pinMode(ledC, OUTPUT);
    
  pinMode(motorI1, OUTPUT);
  pinMode(motorI2, OUTPUT);
  pinMode (motorpmw, OUTPUT);
//  pinMode( indicator, OUTPUT);
  
  Serial.begin(9600);  //This turns on serial monitor
  
  tft.reset(); //clears LCD Ram
  digitalWrite(35, HIGH); //on mega 35 I use this for backlight. pin 10 for Shield i think.. or //comment out and run backlight pin to 5v
  Serial.print("reading id...");
  delay(500);
  Serial.println(tft.readID(), HEX);
  tft.fillScreen(BLACK);
  tft.begin(0x9341);  // this returns the above results. below you can see how I replaced all this once i know which one I have... 
  //tft.begin(0x9328); //<---Here I bypassed the above. cause I already checked...
 //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==--=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 
  
  tft.setRotation(3); // 3=landscape mode -w- row pins on right...
  
 homescreen();
}// End of setup

#define MINPRESSURE 10       //check for any touch 0 would mostly be too senitive.
#define MAXPRESSURE 1000

//--Start of loop-----------------------------------
void loop(){

  TSPoint p = ts.getPoint(); //checks touch x/y min-max



  // if you're sharing pins, you'll need to fix the directions of the touchscreen pins!
  //pinMode(XP, OUTPUT);    //normally not needed..when used as defualt setup..
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);   //normally not needed..when used as defualt setup..



     // turn from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);//default is (240, 0) [default puts touch cord. 0=x/y upper right.
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);//default is (320, 0) [I change these cause i like 0=xy bottom left.
   

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {    //checks IF theres any touch action then continues
  
else if (p.y > 50 && p.y < 270 && p.x > 180 && p.x < 220) {  //looks for touch within x/y box area of button 1
     
           drawHome();
      } 
else if (p.y > 2 && p.y < 62 && p.x > 90 && p.x < 130){  //back button
  homescreen();
  }

  else if (p.y > 2 && p.y < 62 && p.x > 180 && p.x < 230){
     digitalWrite(ledSC, LOW);
      digitalWrite(ledC, LOW);
       digitalWrite(ledA, LOW);//STOP
 off();
  Serial.println("off"); }
          
if (p.y > 100 && p.y < 280 && p.x > 135 && p.x < 175){ // for adults
 digitalWrite(ledSC, LOW);
  digitalWrite(ledC, LOW);
      
  digitalWrite(ledA, HIGH);
  Adults();
  Serial.println("adults");
  }
  else if (p.y > 100 && p.y < 280 && p.x > 180 && p.x < 230){   //for child
  digitalWrite(ledSC, LOW);
 digitalWrite(ledA, LOW);
 digitalWrite(ledC, HIGH);
Child();
 Serial.println("child");
  }

    else if (p.y > 100 && p.y < 280 && p.x > 80 && p.x < 120){ 
       
     digitalWrite(ledC, LOW);
     digitalWrite(ledA, LOW);
    digitalWrite(ledSC, HIGH);     
SeniorCitizens();
 Serial.println("senior");
  }
    
  }  
}// end of loop


void drawHome()
{

  
  tft.fillScreen(BLACK);
  tft.drawRoundRect(0, 0, 319, 240, 8,YELLOW);     //Page border

  tft.fillRoundRect(0, 80, 70, 160, 8, WHITE);
  tft.drawRoundRect(0, 80, 70, 160, 8, WHITE);
  
  tft.fillRoundRect(100, 190, 180, 40, 8, RED);
  tft.drawRoundRect(100, 190, 180, 40, 8, WHITE);  //child

  tft.fillRoundRect(100, 135,180, 40, 8, RED);   //adult
  tft.drawRoundRect(100, 135, 180, 40, 8, WHITE);

  tft.fillRoundRect(100, 80, 180, 40, 8, RED);
  tft.drawRoundRect(100, 80, 180, 40, 8, WHITE); //SENIOR CITIZEN

  tft.fillRoundRect(2, 90, 60, 50, 8, VIOLET);
  tft.drawRoundRect(2, 90, 60, 50, 8, BLACK); //BACK

    tft.fillRoundRect(2, 180, 60, 50, 8, VIOLET);
    tft.drawRoundRect(2, 180, 60, 50, 8, BLACK); //STOP
  
  tft.setCursor(40, 10);
  tft.setTextSize(2);
//  tft.setFont();
  tft.setTextColor(WHITE);
  tft.print("Select Type of Patient");
  tft.setCursor(10, 40);
  tft.setTextSize(2);
  tft.setTextColor(LIME);
  tft.print("Providing CPR at 100C/min ");
  tft.setTextSize(2);
  tft.setTextColor(BLACK);
  tft.setCursor(160, 199);
  tft.print("CHILD");
  tft.setTextSize(2);
  tft.setCursor(160, 145);
  tft.print("ADULT");

  tft.setTextSize(2);
  tft.setCursor(110, 95);
  tft.print("SENIOR CITIZEN");

tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(7, 108);
  tft.print("BACK");

  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(7, 198);
  tft.print("STOP");


}
void homescreen()
   {
  tft.fillScreen(WHITE);
   tft.drawRoundRect(0, 0, 320, 240, 8,CYAN);
   
 // tft.drawRect(0,0,320,240,MAGENTA);
  
  tft.setCursor(10,30);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print("CPR Device HMI Interface");
  
  tft.setCursor(100,60);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print("Prepared By");
  
  tft.setCursor(60,90);
  tft.setTextColor(RED);
  tft.setTextSize(2);
 tft.print("Nabeel Ahmad Khan");

   
  tft.setCursor(10,120);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
 tft.print("Supervisor:");

 
  tft.setCursor(60,150);
  tft.setTextColor(RED);
  tft.setTextSize(2);
 tft.print("Dr.Shahzad Anwar");
 
  
  tft.fillRect(50,180, 210, 40, BLUE);
  tft.drawRect(50,180,210,40,RED);
  tft.setCursor(68,193);
  tft.setTextColor(CYAN);
  tft.setTextSize(2);
  tft.print("PROCEDE FOR CPR");
  delay(500);

    
    /*tft.fillScreen(BLACK);  //Set's LCD Back Ground as Black
  
  
  
  //=-=--Button 1=-=-=-=-=--=--==-=--=-=-=-=-=-=--=-=-=-=--=
  tft.fillRect(25, 150, 250, 30, RED);   //our Rectangle box for Button 1 ( x, y, w, h, color)
  tft.drawRect(25, 150, 250, 30, WHITE);
  tft.setTextSize(2); //Sets all text font size till called again  (Fontsize 1-5) defualt is "1" 
  tft.setTextColor(WHITE);  //Sets all text color till called again
  tft.setCursor(100, 155); //sets cursor to start writing text from.. (x, y)
  tft.println("PROCEED FOR CPR");  // Text string to write on lc 
  */
  }


 void Child()
{ //analogWrite( motorpmw, 120);
  //digitalWrite(motorI1, LOW);
  //digitalWrite(motorI2, HIGH);
 //digitalWrite(indicator, LOW);
 //delay(1);
 analogWrite( motorpmw, 100);
   digitalWrite(motorI1, HIGH);
  digitalWrite(motorI2, LOW);
  delay(10000);

  analogWrite(motorpmw, 0);
  delay(4000);

  
  }

  void SeniorCitizens()
{ //analogWrite( motorpmw, 130);
  //digitalWrite(motorI1, LOW);
  //digitalWrite(motorI2, HIGH);
  //delay(1);
 //digitalWrite(indicator, LOW);
 analogWrite( motorpmw, 110);
    digitalWrite(motorI1, HIGH);
  digitalWrite(motorI2, LOW);
  delay(10000);

  analogWrite(motorpmw, 0);
  delay(4000);

  }

  void Adults()
{ //analogWrite( motorpmw, 140);
  //digitalWrite(motorI1, LOW);
  //digitalWrite(motorI2, HIGH);
 //digitalWrite(indicator, LOW);
 //delay(1);
 analogWrite( motorpmw, 130);
    digitalWrite(motorI1, HIGH);
  digitalWrite(motorI2, LOW);
   delay(10000);

  analogWrite(motorpmw, 0);
  delay(4000);

  }
  void off()
  { analogWrite( motorpmw, 0);
    digitalWrite(motorI1, HIGH);
  digitalWrite(motorI2, LOW);
    }
