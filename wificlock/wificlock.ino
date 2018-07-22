/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

//#define DEBUG

#include <ESP8266WiFi.h>
#include <time.h>

const char* ssid = "";
const char* password = "";

char heure[9]="";
char prev_hour[9]="";

int timezone = 2*3600;
int dst = 1;

enum menu {TIME, SET_ALARM_H, SET_ALARM_M};

enum menu posMenu;

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

  startLog();
  delay(10);

  posMenu = TIME;

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // Connect to WiFi network
  mainLog(formatMsg("Connecting to %s",ssid));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    mainLog_(".");
  }

  mainLog("WiFi connected");

  getTime(heure);

  // Start the server
  server.begin();
  mainLog("Server started");

  // Print the IP address
//  mainLog(formatMsg("IP adress=%s",WiFi.localIP()));
  mainLog(WiFi.localIP().toString());

  tm1637.set(0);
  tm1637.init();

  heure[8]='\0';
  prev_hour[8]='\0';
}

void loop() {

  switch (posMenu){
    case TIME : oclock();
                break;
  }
}


