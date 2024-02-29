#include<ESP8266WiFi.h>
#include<PubSubClient.h>
#include<ArduinoJson.h>
#include "DHT.h"

//TLS certificate libraries
#include<time.h>
#include<TZ.h>
#include<LittleFS.h>
#include<CertStoreBearSSL.h>

//Temp. & Humidity Sensor Config.
byte dhtSensor = 4;
DHT dht(dhtSensor, DHT11);

byte ledPin = 5;

//initial Objects
BearSSL::CertStore certStore;
WiFiClientSecure espClient;
void Callback(char* topic,byte* payload,unsigned int length);
PubSubClient *mqtt_Client;

//WiFi Config.
const char* ssid = "Enter Your Network SSID";
const char* password = "Enter Your Password";

//MQTT Config.
String device_id = "";
const char* Client_Id = "";
const char* Mqtt_Server ="";
const int Mqtt_Port = 8883;
const char* Mqtt_Username ="";
const char* Mqtt_Password ="";
const char* topicPublish ="";
const char* topicSubscribe ="";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(dhtSensor, INPUT);
  pinMode(ledPin, OUTPUT);
  dht.begin();

  //TLS Connection
  LittleFS.begin();
  delay(5000);
  setupWifi();
  setDateTime();

  
  int numCert = certStore.initCertStore(LittleFS,PSTR("/certs.idx"),PSTR("/certs.ar"));
  if(numCert==0){
    Serial.println("no cert found");
    return;
    }

  BearSSL::WiFiClientSecure *bear = new BearSSL::WiFiClientSecure();
  
  bear->setCertStore(&certStore);
  mqtt_Client = new PubSubClient(*bear);
  mqtt_Client-> setServer(Mqtt_Server,Mqtt_Port);
  mqtt_Client->setCallback(Callback);
  mqtt_connect();  
}

void loop() {
  // put your main code here, to run repeatedly:
  float tempData = dht.readTemperature();
  float humidityData = dht.readHumidity();

  Serial.print("The Temperature is:");
  Serial.print(tempData);
  Serial.println("C");
  Serial.print("The Humidity is:");
  Serial.print(humidityData);
  Serial.println("%");
  delay(2000);

  //Send Temp. Data
  String pkt = "{";
  pkt+="\"type\":\"Temp\"";
  pkt+=",\"value\":"+String(tempData)+"";
  pkt+="}";

  mqttPublish((char*)pkt.c_str());


  //send Humidity Data
  String pkt1="{";
  pkt1+="\"type\":\"Humidity\"";
  pkt1+=",\"value\":"+String(humidityData)+"";
  pkt1+="}";

  mqttPublish((char*)pkt1.c_str());

  if(!mqtt_Client->loop())
    mqtt_connect();
   delay(2000);
}

void setupWifi(){
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    }
    Serial.print("Connected to WiFi:");
    Serial.print(ssid);
}

void setDateTime(){
  configTime(TZ_Europe_Berlin,"pool.ntp.org","time.nist.gov");
  Serial.print("Synchronize TLS Timming:");
  time_t now = time(nullptr);
  while(now<8*3600*2){
    Serial.print(".");
    delay(100);
    now = time(nullptr);
    }
    struct tm timeinfo;
    gmtime_r(&now,&timeinfo);
    Serial.printf("%s %s",tzname[0],asctime(&timeinfo));
    
  }

void mqtt_connect(){
  while(!mqtt_Client->connected()){
    Serial.println("Not Connected To the MQTT Broker!!!!");

    if(mqtt_Client->connect(Client_Id,Mqtt_Username,Mqtt_Password)){
      Serial.println("Connected To MQTT Broker");
      mqttSubscribe(topicSubscribe);
      }else{
        Serial.println("Failed To Connect!!!");
        Serial.println("Try Again After 5 Seconds!!!");
        delay(5000);
        }
    }
  }
   void mqttPublish(char *data){
  mqtt_connect();
  if(mqtt_Client->publish(topicPublish,data)){
    Serial.println("Published Successfully!!!");
    }else{
      Serial.println("Published Failed!!!!");
      }
  }
  
void mqttSubscribe(const char *topic){
  if(mqtt_Client->subscribe(topic)){
    Serial.println("Subscribed Successfully To the Topic!!!");
    }else{
      Serial.println("Failed to Subscribe???");
      }
  }
  void Callback(char* topic,byte* payload,unsigned int length){
    
    String command;
    Serial.print(topic);
    for(int i=0;i<length;i++)
      command += (char)payload[i];
    
    if(command.length()>0)
      Serial.println("The Comming data is:"+command);
      

      DynamicJsonDocument doc(1024);
      deserializeJson(doc,command);
      JsonObject obj = doc.as<JsonObject>();

      String value = obj[String("value")];
      if(value=="1"){
          digitalWrite(ledPin,HIGH);
          Serial.println("The Light is ON!!!");
        }else if(value == "0"){
            digitalWrite(ledPin,LOW);
            Serial.println("The Light is OFF!!!");
          }
    }
