#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <StringSplitter.h>


//-------------------VARIABLES GLOBALES--------------------------
int contconexion = 0;

const char *ssid = "Bebe2";
const char *password = "Actopan2066";



//-------Función para Enviar Datos a la Base de Datos SQL--------

String enviardatos(String valor) {
  HTTPClient Post;
  Post.begin("http://192.168.1.73/smartHouse/setValores.php");
  Post.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Post.POST("valorT=" + valor);
  String payload = Post.getString();
  Post.end();
  return payload;
}




String leerDatos() {
  char json[1024];
  HTTPClient http;  //Declare an object of class HTTPClient

  http.begin("http://192.168.1.73/smartHouse/getSensors.php");  //Specify request destination
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
    String g = jsonBuffer[x]["id"];
    String h = jsonBuffer[x]["temperatura"];
     String i = jsonBuffer[x]["luz"];
    confi += ("/" + a + "-" + b + "-" + c + "-" + d + "-" + e + "-" + f + "-" + g+ "-" + h+ "-" + i);



  }

  return confi;

}

//-------------------------------------------------------------------------
String leerDatosConfig() {
  char json[400];
  HTTPClient http;  //Declare an object of class HTTPClient

  http.begin("http://192.168.1.73/smartHouse/getConfig.php");  //Specify request destination
  int httpCode = http.GET();                                                                  //Send the request

  if (httpCode > 0) { //Check the returning code

    String payload = http.getString();   //Get the request response payload

    //Print the response payload

    payload.toCharArray(json, 500);


  }

  http.end();   //Close connection


  StaticJsonDocument<500> jsonBuffer;


  DeserializationError error = deserializeJson(jsonBuffer, json);

  String confi = "";
  for (int x = 0; x < jsonBuffer.size(); x++) {

    String a = jsonBuffer[x]["clave 1"];
    String b = jsonBuffer[x]["clave 2"];
    String c = jsonBuffer[x]["clave 3"];

    String d = jsonBuffer[x]["upLight"];
    String e = jsonBuffer[x]["offLigth"];
    String f = jsonBuffer[x]["hora"];
    //String g = jsonBuffer[x]["id"];
    //String h = jsonBuffer[x]["temperatura"];
    confi += ("/" + a + "-" + b + "-" + c + "-" + d + "-" + e+ "-" + f);



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
  //IPAddress ip(192, 168, 0, 156);
  //IPAddress gateway(192, 168, 0, 1);
  //IPAddress subnet(255, 255, 255, 0);
  //WiFi.config(ip, gateway, subnet);


}

//--------------------------LOOP--------------------------------
void loop() {


  String a = "";
  while (!Serial.available());
  
  String opcion = Serial.readString();
  if (opcion.substring(0, 1) == "S") {
    a = leerDatos();
    Serial.print(a);

  }
  if (opcion.substring(0, 1) == "H") {
    
    a = leerDatosConfig();
    Serial.print(a);
 
  }
  if (opcion.substring(0, 1) == "A") {
 
    opcion[0] = '\0';
    ///////////////////////////////////////////////////////////777
String s= enviardatos(opcion);




 
    ///



    ///////////////////////////////////////////////////////////////77



  }



  //Send a request every 30 seconds

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
