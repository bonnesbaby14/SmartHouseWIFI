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
  Post.begin("http://empresaslaraza.com/smartHouse/getSensors.php");
  Post.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Post.POST("temp=23.9");
  String payload = Post.getString();
  Post.end();
  return payload;
}













String leerDatos() {
  char json[1024];
  HTTPClient http;  //Declare an object of class HTTPClient

  http.begin("http://empresaslaraza.com/smartHouse/getSensors.php");  //Specify request destination
  int httpCode = http.GET();                                                                  //Send the request

  if (httpCode > 0) { //Check the returning code

    String payload = http.getString();   //Get the request response payload

                    //Print the response payload

    payload.toCharArray(json, 1024);


  }

  http.end();   //Close connection


  StaticJsonDocument<1024> jsonBuffer;


  DeserializationError error = deserializeJson(jsonBuffer, json);

  String confi = "";
  for (int x = 0; x < jsonBuffer.size(); x++) {

    String a = jsonBuffer[x]["nombre"];
    String b = jsonBuffer[x]["pinT"];
    String c = jsonBuffer[x]["pinM"];
    
    String d = jsonBuffer[x]["pinV"];
    String e = jsonBuffer[x]["pinP"];
     String f = jsonBuffer[x]["pinL"];
    confi+= ("/"+a+"-"+b+"-"+c+"-"+d+"-"+e+"-"+f);



  }

  return confi;

}

//-------------------------------------------------------------------------







void setup() {

  // Inicia Serial
  Serial.begin(115200);
  Serial.setTimeout(200);



  // Conexión WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Cuenta hasta 50 si no se puede conectar lo cancela

    delay(500);

  }



  //para usar con ip fija
  IPAddress ip(192, 168, 0, 156);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);



}

//--------------------------LOOP--------------------------------
void loop() {
  String a = "";
  while (!Serial.available());
  String opcion = Serial.readString();
  if (opcion.substring(0, 1) == "S") {
    a = leerDatos();

  }
  Serial.print(a);



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
