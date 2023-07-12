//Líbrerías utilizadas
#include "CTBot.h" //Bot  de Telegram
#include "token.h" //Archivo con credenciales WiFi, servidor MQTT y bot de Telegram
#include <Adafruit_NeoPixel.h> //Neopixeles
#include <ESP32Servo.h> //Servomotor
#include "DHT.h" //DHT integrado
#include <WiFi.h> //Conexión WiFi para conectar al servidor
#include <PubSubClient.h> //MQTT 

#ifdef __AVR__
#include <avr/power.h>
#include <avr/dtostrf.h>
#endif

#define DHTPIN 26 //Pin donde se encuentra conectado el DHT11
#define DHTTYPE DHT11 //Tipo de sensor DHT
#define PIN 14 //PIN en el cual esta conectado el Neopixeles
#define NUMPIXELS 5 // Numero de Neopixeles conectados
const int PIR_SENSOR_OUTPUT_PIN = 27; //Pin donde se encuentra el sensor PIR

int modo = 0; //Variable para intercambiar entre modo automático y manual
int servoPin = 13; //Pin donde se encuentra el servomotor
unsigned long prevMillis = 0; //Variables para manejo de tiempos
unsigned long currentMillis = 0;
const long interval = 120000;
unsigned long prevMillis1 = 0;
float h; //Variable para la humedad
float t;//Variable para la temperatura


CTBot miBot;
Servo myservo;
DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  //Conexión WiFi con el servidor
  setup_wifi();
  client.setCallback(callback);
  client.setServer(mqtt_server, 1883);
  //Conexión con el Bot de Telegram
  Serial.println("Iniciando Bot de Telegram");
  miBot.wifiConnect(ssid, password);
  miBot.setTelegramToken(token);

  if (miBot.testConnection()) {
    Serial.println("\n Conectado");
    pixels.setPixelColor(1, pixels.Color(0,189,255));   //Color LED color Celeste
      pixels.show(); //Refrescamos los pixeles
  }
  else {
    Serial.println("\n Problemas Auxilio");
    pixels.setPixelColor(1, pixels.Color(255,0,0));   //Color LED color Rojo
      pixels.show(); //Refrescamos los pixeles
      delay(250);
  }

  // Configuraciones para el servomotor
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 500, 2400); // attaches the servo on pin 18 to the servo object
 //Inicio del sensor DHT11
  dht.begin();
}

void loop() {
  currentMillis = millis();
  if(currentMillis - prevMillis >= interval){ //Tiempo de espera de todo el programa para que inicie el sensor DHT11 (2 min)
  TBMessage msg;
  pixels.setPixelColor(3, pixels.Color(255,155,0));   //Color LED color naranja (Transcurrió el tiempo del sensor DHT)
      pixels.show(); 
    t = dht.readTemperature(); //Lectura de Temperatura
    h = dht.readHumidity(); //Lectura de Humedad
  if (CTBotMessageText == miBot.getNewMessage(msg)) {
    if(modo == 0){ //Si el modo está en manual "0"
      if (msg.text.equalsIgnoreCase("auto")) { //Mensaje de Telegram "auto", cambia el modo a automático
        Serial.println("Modo Automático");
        modo = 1;
        miBot.sendMessage(msg.sender.id, "Modo Automático Activado");
      }
      else if (msg.text.equalsIgnoreCase("temp")) { //Mensaje de Telegram "temp", recibe un mensaje en telegram con la temperatura actual
        Serial.println("La temperatura es: ");
        Serial.print(t);
        Serial.println(" °C ");
        miBot.sendMessage(msg.sender.id, "La temperatura es: " + String(t,2) + "°C");
      }else if (msg.text.equalsIgnoreCase("hum")) {//Mensaje de Telegram "hum", recibe un mensaje en telegram con la humedad actual
        Serial.println("La Humedad es del: ");
        Serial.print(h);
        Serial.print(" %");
        miBot.sendMessage(msg.sender.id, "La humedad es del: " + String(h,2) + " %");
      }else if (msg.text.equalsIgnoreCase("alimentar")) {//Mensaje de Telegram "alimentar", activa el servomotor
        Serial.println("Alimentando");
        pixels.setPixelColor(4, pixels.Color(255,255,0));   //Color LED color amarillo
        pixels.show(); //Refrescamos los pixeles
        myservo.write(60);
        delay(700);
        myservo.write(90);
        pixels.setPixelColor(4, pixels.Color(0,0,0));
        pixels.show(); //Refrescamos los pixeles
        delay(250); //Retardo de encendido en cada pixel;
        miBot.sendMessage(msg.sender.id, "Alimentación Exitosa");
      }else { //en caso reciba un mensaje que no tenga asignada una función
       miBot.sendMessage(msg.sender.id, "Bienvenido " + msg.sender.firstName + ",intenta usar: alimentar, temp, hum o auto");
      }
    }else if ( modo == 1){ //Si el modo está en automático "1"
      if (msg.text.equalsIgnoreCase("manual")) {//Mensaje de Telegram "manual", cambia el modo a manual
        Serial.println("Modo Manual");
        modo = 0;
        miBot.sendMessage(msg.sender.id, "Modo Manual Activado");
      }else if (msg.text.equalsIgnoreCase("temp")) {//Mensaje de Telegram "temp", recibe un mensaje en telegram con la temperatura actual
        Serial.println("La temperatura es: ");
        Serial.print(t);
        Serial.println(" °C ");
        miBot.sendMessage(msg.sender.id, "La temperatura es: " + String(t,2) + "°C");
      }else if (msg.text.equalsIgnoreCase("hum")) {//Mensaje de Telegram "hum", recibe un mensaje en telegram con la humedad actual
        Serial.println("La Humedad es del: ");
        Serial.print(h);
        Serial.print(" %");
        miBot.sendMessage(msg.sender.id, "La humedad es del: " + String(h,2) + " %");
      }else {//en caso reciba un mensaje que no tenga asignada una función
         miBot.sendMessage(msg.sender.id, "Bienvenido " + msg.sender.firstName + ",intenta usar: manual, temp o hum");
        }
     }
  }
  if(modo == 1){//Si el modo está en automático "1"
    int sensor_output;
  sensor_output = digitalRead(PIR_SENSOR_OUTPUT_PIN);
  if( sensor_output == HIGH ){
  }
  else
  { //Cuando el sensor PIR detecte a la mascota, se activa el servomotor
    Serial.print("Object detected\n\n");  
    pixels.setPixelColor(4, pixels.Color(255,255,0));   //Color LED color amarillo
    pixels.show(); //Refrescamos los pixeles
    delay(1000); //Retardo de encendido en cada pixel;
    myservo.write(60);
    delay(700);
    myservo.write(90);
    pixels.setPixelColor(4, pixels.Color(0,0,0));
      pixels.show(); //Refrescamos los pixeles
      delay(250); //Retardo de encendido en cada pixel;
  }
  }
  //Envío de dator al servidor
  if(currentMillis - prevMillis1 >= interval){ //Tiempo de espera entre un dato y otro (2min)
    prevMillis1 = currentMillis;
    if (!client.connected()) { //Si no está conectado al servidor MQTT, intenta reconectarse
      reconnect();
    }
    client.loop();
    float h = dht.readHumidity(); //Lectura de humedad
    float t = dht.readTemperature(); //Lectura de temperatura
    
    if (isnan(h) || isnan(t)) { //Detecta si no es posible leer datos del sensor
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
    }//Si lee datos del sensor, publica los datos de humedad y temperatura
    Serial.println(F("Publicación de Datos:"));
    Serial.print(F("Humedad: "));
    Serial.print(h);
    char humedad[6];
    dtostrf(h, 1, 2, humedad);
    client.publish("nodealfmaz/hume", humedad);
    Serial.print(F("% Temperatura: "));
    Serial.print(t);
    char temperatura[6];
    dtostrf(t, 1, 2, temperatura);
    client.publish("nodealfmaz/temp", temperatura);
    Serial.println(F("°C "));
    Serial.println(F("Fin de la Publicación de Datos:"));
    }
    
}else{
  //Tiempo de espera para iniciar DHT11
  pixels.setPixelColor(3, pixels.Color(25,0,0));   //Color LED color rojo, si aun no ha pasado el tiempo de espera
      pixels.show(); //Refrescamos los pixeles
      delay(250);
  }
}


void setup_wifi() {//Conexión WiFi para conexión con el servidor

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    pixels.setPixelColor(0, pixels.Color(255,0,0));   //Color LED color rojo, si no se ha conectado a WiFi
    pixels.show(); //Refrescamos los pixeles
    delay(250); //Retardo de encendido en cada pixel;
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pixels.setPixelColor(0, pixels.Color(0,255,0));   //Color LED color verde, si se conecto satisfactoriamente a WiFi
      pixels.show(); //Refrescamos los pixeles
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}



void reconnect() {
  // Loop si no logra conectarse al servidor MQTT
  while (!client.connected()) {
    
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32proydycdealfmaza",mqtt_server_user, mqtt_server_pass)) {
      Serial.println("connected");
      pixels.setPixelColor(2, pixels.Color(255,0,255));   //Color LED color violeta, si logra conectarse al servidor
      pixels.show(); //Refrescamos los pixeles
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      pixels.setPixelColor(2, pixels.Color(255,0,0));   //Color LED color Rojo
      pixels.show(); //Refrescamos los pixeles
      delay(5000);
    }
  }
}
