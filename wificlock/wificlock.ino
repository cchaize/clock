//#define DEBUG

#include <ESP8266WiFi.h>
#include <time.h>
#include "TM1637.h"
#define ON 1
#define OFF 0

///////////////////////////// Wifi setup
const char* ssid = "";
const char* password = "";

//////////////////////////// KY-040 Rotary encoder
const int PinCLK=5;                   // Used for generating interrupts using CLK signal
const int PinDT=4;                    // Used for reading DT signal
const int PinSW=0;                    // Used for the push button switch
static long encoderPos = -1;    // Au 1er démarrage, il passera à 0
int PinCLKLast = LOW;
volatile boolean TurnDetected;
volatile boolean up;
static long virtualPosition=0;    // position of rotary encoder

//////////////////////////// init global variables
char hour[9]="";
char prev_hour[9]="";
int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};

int alarm_h=0;
int alarm_m=0;

int timezone = 2*3600;
int dst = 1;

//////////////////////////// Menus
enum menu {TIME, SET_ALARM_H, SET_ALARM_M};
volatile enum menu posMenu;
enum menu oldPosMenu;
volatile unsigned long startMenu;
unsigned long delayMenu = 5000;

/////////////////////////// 4 digit display
unsigned char ClockPoint = 1;
#define CLK 14//pins definitions for TM1637 and can be changed to other ports
#define DIO 12
TM1637 tm1637(CLK,DIO);

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

void isrCLK ()  {                    // Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK
 if (digitalRead(PinCLK))
   up = digitalRead(PinDT);
 else
   up = !digitalRead(PinDT);
 TurnDetected = true;
 startMenu=millis();
}

void isrInter() {
  virtualPosition=0;              // reset counter to 0
  switch (posMenu) {
    case TIME        : posMenu=SET_ALARM_H;
                       break;
    case SET_ALARM_H : posMenu=SET_ALARM_M;
                       break;
    case SET_ALARM_M : posMenu=TIME;
                       break;
  }
 startMenu=millis();
}


void setup() {

  startLog();
  delay(10);

  // KY-040 Rotary encoder
  pinMode (PinCLK,INPUT);
  pinMode (PinDT,INPUT);
  pinMode (PinSW,INPUT);
  attachInterrupt (digitalPinToInterrupt(PinCLK),isrCLK,FALLING);   // interrupt CLK
  attachInterrupt (digitalPinToInterrupt(PinSW),isrInter,RISING);   // interrupt
 
  posMenu = TIME;

  // Connect to WiFi network
  mainLog(formatMsg("Connecting to %s",ssid));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    mainLog_(".");
  }

  mainLog("WiFi connected");

  getTime(hour);

  // Start the server
  server.begin();
  mainLog("Server started");

  // Print the IP address
//  mainLog(formatMsg("IP adress=%s",WiFi.localIP()));
  mainLog(WiFi.localIP().toString());

  tm1637.set(0);
  tm1637.init();

  hour[8]='\0';
  prev_hour[8]='\0';
}

void loop() {
 int inc = 0;
 if (TurnDetected)  {        // do this only if rotation was detected
   if (up) {
     inc = 1;
   
     virtualPosition++;
   }
   else {
     inc = -1;
     virtualPosition--;
   }
   TurnDetected = false;          // do NOT repeat IF loop until new rotation detected
   Serial.print ("Count = ");  
   Serial.println (virtualPosition);
 }

 if (millis()-startMenu>delayMenu) {
    posMenu = TIME;
 }

 if (oldPosMenu!=posMenu) {
  Serial.print("nouveau menu:");
  Serial.println(posMenu);
  oldPosMenu=posMenu;
 }
  
  switch (posMenu){
    case TIME        : oclock();
                       break;
    case SET_ALARM_H : setAlarmH(&alarm_h, inc);
                       break;
    case SET_ALARM_M : setAlarmM(&alarm_m, inc);
                       break;
  }
}


