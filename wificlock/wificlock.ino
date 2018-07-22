/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>
#include <time.h>

const char* ssid = "";
const char* password = "";

char heure[9]="";
char prev_hour[9]="";

int timezone = 2*3600;
int dst = 1;

#include "TM1637.h"
#define ON 1
#define OFF 0

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};
unsigned char ClockPoint = 1;
#define CLK 14//pins definitions for TM1637 and can be changed to other ports
#define DIO 12
TM1637 tm1637(CLK,DIO);

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  getTime(heure);

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

  tm1637.set(0);
  tm1637.init();

  heure[8]='\0';
  prev_hour[8]='\0';
}

void loop() {

  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  sprintf(heure,"%02d:%02d:%02d",p_tm->tm_hour,p_tm->tm_min,p_tm->tm_sec);

/*  Serial.print(heure);
  Serial.print(" / ");
  Serial.print(prev_hour);
  Serial.print(" -> ");
  Serial.println(strcmp(heure, prev_hour));
*/
  if (strcmp(heure, prev_hour)==0)
    return;
  strcpy(prev_hour, heure);

  TimeUpdate(p_tm);
  tm1637.display(TimeDisp);
  //TimingISR();
  ClockPoint = (~ClockPoint) & 0x01;

}

void getTime(char *h){
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  while(!time(nullptr)){
    Serial.print("*");
    delay(1000);
  }
}

void TimeUpdate(tm* p_tm)
{
  if(ClockPoint)tm1637.point(POINT_ON);
  else tm1637.point(POINT_OFF);
  TimeDisp[0] = p_tm->tm_hour / 10;
  if (TimeDisp[0]==0)
    TimeDisp[0] = 16;
  TimeDisp[1] = p_tm->tm_hour % 10;
  TimeDisp[2] = p_tm->tm_min / 10;
  TimeDisp[3] = p_tm->tm_min % 10;
}

