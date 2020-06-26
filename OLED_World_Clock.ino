/////////////////////////
// OLED World Clock   //
//by Shubham Santosh //
//////////////////////

#include<ESP8266WiFi.h>
#include<NTPClient.h>
#include<WiFiUdp.h>
#include<SPI.h>
#include<Adafruit_SSD1306.h>
#include<Adafruit_GFX.h>
#include<Wire.h>



Adafruit_SSD1306 display(128,64,&Wire,-1);
WiFiUDP ntp;
NTPClient timeclient(ntp,"pool.ntp.org",0);    

const char *ssid= "Enter SSID";               // enter SSID of your WiFi router
const char *password= "Enter Password";        // enter password of the router
String Week[7] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  pinMode(D5,INPUT_PULLUP);         // To move to next city/state
  pinMode(D6,INPUT_PULLUP);         // To move to previous city/state
  display.begin(SSD1306_SWITCHCAPVCC,0x3C);
  /*if(!(display.begin(SSD1306_SWITCHCAPVCC,0x3C)))
  {
    Serial.println("Connection Error");
    while (true)
    {continue;}
  }
  */
  delay(1000);

  // animation
display.clearDisplay();
display.setTextColor(WHITE);
display.setCursor(0,0);
display.setTextSize(2);
display.println("OLED");
display.println("WORLD");
display.setCursor(0,32);
display.println("CLOCK");
display.display();
display.startscrollright(0x00,0x05);
delay(1500);
display.invertDisplay(1);
display.stopscroll();
delay(1000);

    // displays ssid of the router to be connected
display.invertDisplay(0);
display.clearDisplay();
display.setCursor(0,0);
display.setTextColor(WHITE);
display.setTextSize(2);
display.println("CONNECTING");
display.setCursor(52,16);
display.println("TO");
display.setTextSize(1);
display.println(ssid);
display.display();
while(WiFi.status()!=WL_CONNECTED)
{
  Serial.print(".");
  delay(500);
}
display.setCursor(0,48);
display.setTextSize(2);
display.setTextColor(BLACK,WHITE);
display.print("Connected");
display.display();
delay(500);
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
}


const int xcenter=95;  // x and y center points for clock
const int ycenter=32;   
const int radius=30;    // radius of clock

// UTC offset of city/state
String time_zones[25][3]=
{
"JUNEAU","-8","0",  
"LOS ANGELES","-7","0",
"MEXICO CITY","-5","0",
"MIAMI","-4","0",
"SANTIAGO","-4","0",
"NEW YORK","-4","0",
"RIO","-3","0",
"NUUK","-2","0",
"ACCRA","0","0",
"LISBON","1","0",
"LONDON","1","0",
"PARIS","2","0",
"MOSCOW","3","0",
"DUBAI","4","0",
"KARACHI","5","0",
"NEW DELHI","5","30",
"COLOMBO","5","30",
"DHAKA","5","45",
"BANGKOK","7","0",
"BEIJING","8","0",
"HONGKONG","8","0",
"SINGAPORE","8","0",
"TOKYO","9","0",
"SYDNEY","10","0",
"AUCKLAND","12","0",
};

int count=0;
void loop() {
    timeclient.update();
  display.setTextColor(WHITE);
  display.setTextSize(2);
 
  if(digitalRead(D5)==LOW)
    count++;
  if(digitalRead(D6)==LOW)
    count--;
  if(count<0)
    count=25+count;
  count=count%25;

  int offset_hr = time_zones[count][1].toInt();    
  int offset_min = time_zones[count][2].toInt();
  int offset=offset_hr*60*60+offset_min*60;       // convert UTC offset of city state in seconds
  int epochtime=timeclient.getEpochTime()+offset;    // calculate epoch time of city/state
  
  struct tm *ptm = gmtime ((time_t *)&epochtime);   // get local time of city/state 
  int curr_hour = ptm->tm_hour;
  int curr_min = ptm->tm_min;
  int curr_sec = ptm->tm_sec;
  display.setCursor(0,0);
if(time_zones[count][0].length()>6)             //to fit the text in page 0
   display.setTextSize(1);

display.println(time_zones[count][0]);
// print time in OLED
    display.setTextSize(1);
    display.setCursor(0,25);
    display.print(curr_hour);
    display.print(":");
    display.print(curr_min);
    display.print(":");
    display.println(curr_sec);
    
// print day of the week
    display.setCursor(0,40);
    display.print(Week[ptm->tm_wday]);
    
  // display analog clock
    display_clock(curr_hour,curr_min,curr_sec);

// display date  
  display.setCursor(0,55);
  display.print(ptm->tm_mday);
  display.print(":");
  display.print( ptm->tm_mon+1);
  display.print(":");
  display.println(ptm->tm_year+1900);
  display.display();
  display.clearDisplay();
  delay(1000);
}

void display_clock(int hours,int mins,int secs)
{
  display.drawCircle(xcenter,ycenter,radius,WHITE);
  display.setTextSize(1);
  
  display.setCursor(90,4);   // display clock hands
  display.print("12");
  display.setCursor(117,30);
  display.print('3');
  display.setCursor(92,52);
  display.print('6');
  display.setCursor(68,30);
  display.print('9');
  int Sangle=secs*6;                        // second angle 
  int Mangle=mins*6;                         //minute angle
  int Hangle;                            // hour angle
  if(hours>=0 and hours<=12)
  Hangle=30*hours+map(mins,0,60,0,30);      // map for minor adjustments in hour hand
  else if(hours>12)              // hour format is 24hrs
  Hangle=(hours-12)*30+map(mins,0,60,0,30);  
  
    //for displaying seconds
  int shiftSecX=0.8*radius*sin(Sangle*0.0174533);      //shifted x coordinate of second,0.8 is the 
  int shiftSecY=0.8*radius*cos(Sangle*0.0174533);      //shifted y coordinate of second

 // if(Sangle>=0 and Sangle<=90)
 display.drawLine(xcenter,ycenter,xcenter+shiftSecX,ycenter-shiftSecY,WHITE);    //1st quadrant
/* else if(Sangle>90 and Sangle<=180)
 display.drawLine(xcenter,ycenter,xcenter+shiftSecX,ycenter-shiftSecY,WHITE);    //4th quadrant
 else if(Sangle>180 and Sangle<=270)
 display.drawLine(xcenter,ycenter,xcenter+shiftSecX,ycenter-shiftSecY,WHITE);    // 3rd quadrant
  else if(Sangle>270 and Sangle<=354)
display.drawLine(xcenter,ycenter,xcenter+shiftSecX,ycenter-shiftSecY,WHITE);    // 2nd quadrant
*/
//for displaying minutes
  int shiftMinX=radius*sin(Mangle*0.0174533);      //shifted x coordinate of minutes
  int shiftMinY=radius*cos(Mangle*0.0174533);      //shifted y coordinate of minutes

  display.drawLine(xcenter,ycenter,xcenter+shiftMinX,ycenter-shiftMinY,WHITE);

//for displaying hours
  int shiftHourX=0.6*radius*sin(Hangle*0.0174533);      //shifted x coordinate of hours
  int shiftHourY=0.6*radius*cos(Hangle*0.0174533);      //shifted y coordinate of hours

  display.drawLine(xcenter,ycenter,xcenter+shiftHourX,ycenter-shiftHourY,WHITE);

/*
// for testing purpose
  Serial.print(Sangle);
  Serial.print("\t");
  Serial.print(sin(Sangle*0.0174533));
  Serial.print("\t");
  Serial.print(cos(Sangle*0.0174533));
  Serial.print("\t");
  Serial.println(timeclient.getSeconds());
*/
}
