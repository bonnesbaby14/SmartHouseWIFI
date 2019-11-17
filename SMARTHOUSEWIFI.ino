#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>


//-------------------VARIABLES GLOBALES--------------------------
int contconexion = 0;

const char *ssid = "Bebe";
const char *password = "Actopan2066";



//-------Función para Enviar Datos a la Base de Datos SQL--------

String enviardatos() {


  HTTPClient Post;
  Post.begin("http://192.168.0.5/SmartHouse/up.php");
  Post.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Post.POST("temp=23.9");
  String payload = Post.getString();
  Post.end();
  return payload;
}













void leerDatos() {
  char json[200];
  HTTPClient http;  //Declare an object of class HTTPClient

  http.begin("http://192.168.0.5/SmartHouse/down.php");  //Specify request destination
  int httpCode = http.GET();                                                                  //Send the request

  if (httpCode > 0) { //Check the returning code

    String payload = http.getString();   //Get the request response payload

    Serial.println(payload);                     //Print the response payload

    payload.toCharArray(json, 50);


  }

  http.end();   //Close connection

  StaticJsonDocument<200> jsonBuffer;



  DeserializationError error = deserializeJson(jsonBuffer, json);


  String s = jsonBuffer[0]["dato"];
  Serial.println(s);
}

//-------------------------------------------------------------------------







void setup() {

  // Inicia Serial
  Serial.begin(115200);
  Serial.println("");


  // Conexión WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Cuenta hasta 50 si no se puede conectar lo cancela
    
    delay(500);
    Serial.print(".");
  }

  
 
    //para usar con ip fija
    IPAddress ip(192, 168, 0, 156);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gateway, subnet);

    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());

}

//--------------------------LOOP--------------------------------
void loop() {
  String s = enviardatos();
  Serial.println(s);


  delay(30000);    //Send a request every 30 seconds

  /*
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 10000) { //envia la temperatura cada 10 segundos
     previousMillis = currentMillis;
     int analog = analogRead(17);
     float temp = analog*0.322265625;
     Serial.println(temp);
     enviardatos("chipid=" + chipid + "&temperatura=" + String(temp, 2));
    }
  */
}
