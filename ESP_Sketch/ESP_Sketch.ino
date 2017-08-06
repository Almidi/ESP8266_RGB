#include <ESP8266WiFi.h>
#include <MQTT.h>

void myDataCb(String& topic, String& data);
void myPublishedCb();
void myDisconnectedCb();
void myConnectedCb();
void transition();

int RED = 0;
int GREEN = 0;
int BLUE = 0;
int WHITE = 0;

int FinalRED = 0;
int FinalGREEN = 0;
int FinalBLUE = 0;
int FinalWHITE = 1000;
int FADE = 0;

int incR = 1;
int incG = 1;
int incB = 0;
int MAX_BRIGHT = 1000;

int RED_PIN = 5;
int GREEN_PIN = 4;
int BLUE_PIN = 12;
int WHITE_PIN = 14;



#define CLIENT_ID "ID"       //ID of this client
#define TOPIC "MQTT TOPIC"   //MQTT Topic to subscribe
#define SERVERIP "SERVERIP"  //MQTT Server address


// create MQTT
MQTT myMqtt(CLIENT_ID, SERVERIP, 1883);


const char* ssid     = "SSID";
const char* password = "PASSWORD";


//
void setup() {

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(WHITE_PIN, OUTPUT);
  
  delay(1000);
  Serial.begin(115200);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  Serial.println("Connecting to MQTT server");  

  // setup callbacks
  myMqtt.onConnected(myConnectedCb);
  myMqtt.onDisconnected(myDisconnectedCb);
  myMqtt.onPublished(myPublishedCb);
  myMqtt.onData(myDataCb);

  
  Serial.println("connect mqtt...");
  myMqtt.connect();

  Serial.println("subscribe to topic...");
  myMqtt.subscribe(TOPIC);

  delay(10);
}

//
void loop() {
    
    if (RED > FinalRED ){
      RED -= 1 ;
    }
    else if(RED < FinalRED ){
      RED += 1 ;
    }

    if (GREEN > FinalGREEN ){
      GREEN -= 1 ;
    }
    else if(GREEN < FinalGREEN ){
      GREEN += 1 ;
    }

    if (BLUE > FinalBLUE ){
      BLUE -= 1 ;
    }
    else if(BLUE < FinalBLUE){
      BLUE += 1 ;
    }
    
    if (WHITE > FinalWHITE ){
      WHITE -= 1 ;
    }
    else if(WHITE < FinalWHITE){
      WHITE += 1 ;
    }
    
    analogWrite(RED_PIN, RED);
    analogWrite(GREEN_PIN, GREEN);
    analogWrite(BLUE_PIN, BLUE);
    analogWrite(WHITE_PIN, WHITE);
    if (FADE > 0 ){    
      if (RED== 1000 ){
        FinalGREEN = 1000;
        FinalRED = 0;
      }
      if (GREEN== 1000 ){
        FinalBLUE = 1000;
        FinalGREEN = 0;
      }
      if (BLUE== 1000 ){
        FinalRED = 1000;
        FinalBLUE = 0;
      }
      
    }
    
    delay(2+FADE);
    
    
    
    myMqtt.onDisconnected(myDisconnectedCb);
}

void myConnectedCb()
{
  Serial.println("connected to MQTT server");
}

void myDisconnectedCb()
{
  Serial.println("disconnected. try to reconnect...");
  delay(500);
  myMqtt.connect();
}

void myPublishedCb()
{
  Serial.println("published.");
}

void myDataCb(String& topic, String& data)
{
  if(topic == "/openHAB/Alpha-RGB/RED"){
    Serial.print("RED IS :");
    Serial.println(data.toInt()*10);
    FinalRED = data.toInt()*10 ;
    FADE=0;
  }
  else if(topic == "/openHAB/Alpha-RGB/GREEN"){
    Serial.print("GREEN IS :");
    Serial.println(data.toInt()*10);
    FinalGREEN = data.toInt()*10 ;
    FADE=0;
  }
  else if(topic == "/openHAB/Alpha-RGB/BLUE"){
    Serial.print("BLUE IS :");
    Serial.println(data.toInt()*10);
    FinalBLUE = data.toInt()*10 ;
    FADE=0;
  }
  else if(topic == "/openHAB/Alpha-RGB/FADE"){
    Serial.print("FADE IS :");
    Serial.println(data.toInt());
    if(FinalRED + FinalBLUE + FinalGREEN == 0){
      FADE = data.toInt() ;
      FinalRED = 1000 ;
    }
  }
  else if(topic == "/openHAB/Alpha-RGB/White"){
    Serial.print("WHITE IS :");
    Serial.println(data.toInt());
    FinalWHITE = data.toInt()*10 ;
  }
}



