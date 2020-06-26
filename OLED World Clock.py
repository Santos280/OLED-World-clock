from machine import Pin,I2C
from time import sleep
import ssd1306
import network,ntptime,utime,gfx,math


i2c=I2C(-1,scl=Pin(5),sda=Pin(4))
oled=ssd1306.SSD1306_I2C(128,64,i2c)
graphics = gfx.GFX(128,64,oled.pixel)

next=Pin(14,Pin.IN,Pin.PULL_UP)   # enabling pull-up resistor
back=Pin(12,Pin.IN,Pin.PULL_UP)

week=["Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"]

ssid= "Enter SSID"
password="Enter Password"

oled.text("OLED",0,0)
oled.text("WORLD",0,10)
oled.text("CLOCK",0,20)
oled.show()

sleep(1)
oled.fill(0)
wifi=network.WLAN(network.STA_IF)
wifi.active(True)
wifi.connect(ssid,password)
while(wifi.isconnected()==False):
  oled.text("CONNECTING",15,0,2)
  oled.text("To",60,10)
  oled.text("Most wanted Open",0,20)
  oled.show()
oled.text("Connected",0,30)
oled.show()

xcenter=95     # X and Y center coordinates of clock circle
ycenter=32
radius=30     # radius of cloock circle
count=0      # to move next/previous 

ntptime.settime()

#UTC offset time of state/country in hrs, mins stored in dictionary
time_zones={       
0:["JUNEAU",-8,0],
1:["LOS ANGELES",-7,0],
2:["MEXICO CITY",-5,0],
3:["MIAMI",-4,0],
4:["SANTIAGO",-4,0],
5:["NEW YORK",-4,0],
6:["RIO",-3,0],
7:["NUUK",-2,0],
8:["ACCRA",0,0],
9:["LISBON",1,0],
10:["LONDON",1,0],
11:["PARIS",2,0],
12:["MOSCOW",3,0],
13:["DUBAI",4,0],
14:["KARACHI",5,0],
15:["NEW DELHI",5,30],
16:["COLOMBO",5,30],
17:["DHAKA",5,45],
18:["BANGKOK",7,0],
19:["BEIJING",8,0],
20:["SINGAPORE",8,0],
21:["HONGKONG",8,0],
22:["TOKYO",9,0],
23:["SYDNEY",10,0],
24:["AUCKLAND",12,0],
}


def display_clock(hrs,mins):
  
  oled.text('12',90,4)     # Clock hour positions
  oled.text('3',117,30)
  oled.text('6',92,52)
  oled.text('9',67,30)
  graphics.circle(xcenter,ycenter,radius,1)
  
  Sangle=curr_time[5]*6       #Second angle
  Mangle=mins*6                #Minute angle
  if hrs>=0 and hrs<=12:
    Hangle=30*hrs          #Hour angle
  elif hrs>12:             # Hour format is 24
    Hangle=(hrs-12)*30
  # Obtaining the (x,y) coordinates of the Second hand
  shift_sec_x=0.8*radius*math.sin(math.radians(Sangle)) # 0.8 is the ratio of the length
  shift_sec_y=0.8*radius*math.cos(math.radians(Sangle))
  #plotting the second hand
  graphics.line(xcenter,ycenter,round(xcenter+shift_sec_x),round(ycenter-shift_sec_y),1)
  # Obtaining the (x,y) coordinates of the Minute hand
  shift_min_x=radius*math.sin(math.radians(Mangle))
  shift_min_y=radius*math.cos(math.radians(Mangle))
  #plotting the Minute hand
  graphics.line(xcenter,ycenter,round(xcenter+shift_min_x),round(ycenter-shift_min_y),1)
  #Obtaining the (x,y) coordinated of hour hand
  shift_hour_x=0.6*radius*math.sin(math.radians(Hangle))
  shift_hour_y=0.6*radius*math.cos(math.radians(Hangle))
  #plotting the hour hand
  graphics.line(xcenter,ycenter,round(xcenter+shift_hour_x),round(ycenter-shift_hour_y),1)
  
 

while True:
  oled.fill(0)        # Clear screen
  if next.value()==0:   # move to next city/state
   count+=1
  elif back.value()==0:  #move to previous city/state
   count-=1
  if (count<=0):     # circular rotation
   count=25+count
  count=count%25 
  
  offset_sec=time_zones[count][1]*60*60+ time_zones[count][2]*60  # convert UTC offset to seconds
  curr_time=utime.localtime(offset_sec+utime.time())     # provides day,date and time from epoch time
  oled.text(time_zones[count][0],0,0)         # display the city/state
  (Hr,Min)=(curr_time[3],curr_time[4])      # obtain current hour,min of the city/state
  time_str=str(Hr)+":"+str(Min)+":"+str(curr_time[5])   
  oled.text(time_str,0,20)   # display time of city/state
  display_clock(Hr,Min)      # display analog clock
  oled.text(week[curr_time[6]],0,35)       # display day of the week
  curr_date=str(curr_time[2])+":"+str(curr_time[1])+":"+str(curr_time[0])
  oled.text(curr_date,0,50)        # display current date of city/state
  oled.show()
  sleep(0.9)
 
 
  
  
 


















