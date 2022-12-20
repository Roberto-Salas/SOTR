/* 
Alumno: Roberto Miguel Salas Ayala
Grupo: 3MM6
Lo que el codigo permite es realizar la conexión wifi de nuestra tarjeta de desarrollo y 
también la conexión con un bot de la aplicación de telegram, el cual se puede configurar por
medio del token y ID chat. Una vez la conexión se realizo, se controlo un led para su encencido
y apagado e igulmente se podia ver el estado de los comandos. Se puede verificar la conexion
con nuestro bot y con el led encendido.
*/
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
const char* ssid = "SALAS AYALA's Galaxy A21s";
const char* password = "dxye6524";
#define BOTtoken "5375472026:AAEE3hRvH3qIWb2-K6NDzdJPRT0DykPUmz4"
#define CHAT ID "1587969172"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int ledPin = 4;
bool ledState = LOW;

void handleNewMessages(int numNewMessages){
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  
for (int i=0; i<numNewMessages; i++){
  String chat_id = String(bot.messages[i].chat_id);
  if (chat_id != chat_id){
    bot.sendMessage(chat_id, "Unauthorized user", "");
    continue;
  }

  
   String text = bot.messages[i].text;
   Serial.println(text);

String from_name = bot.messages[i].from_name;

   if (text == "/start"){
    String Welcome = "Welcome, " + from_name + ".\n";
    Welcome += "Use the following commands to control your outputs. \n\n";
    Welcome += "/led_on to turn GPIO ON \n";
    Welcome += "/led_off to turn GPIO OFF \n";
    Welcome += "/state to request current GPIO state \n";
    bot.sendMessage(chat_id, Welcome, "");
   }
   
  if (text == "/led_on"){
    bot.sendMessage(chat_id, "LED state set to ON", "");
    ledState = LOW;
    digitalWrite(ledPin, ledState);
  }

  if (text == "/led_off"){
    bot.sendMessage(chat_id, "LED state set to OFF", "");
    ledState = HIGH;
    digitalWrite(ledPin, ledState);
  }
  
  if (text == "/state"){
    if (digitalRead(ledPin)){
      bot.sendMessage(chat_id, "LED is ON", "");
    }
    else{
      bot.sendMessage(chat_id, "LED is OFF", "");
    }
  }
}
}
  
void setup() {
  Serial.begin(115200);

/*  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");
    client.setTrustAnchors(&cert);
  #endif
*/
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  //connet to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  #endif
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay){
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    
    while(numNewMessages){
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}