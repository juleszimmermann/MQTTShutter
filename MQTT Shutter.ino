#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

/************ WIFI and MQTT INFORMATION (CHANGE THESE FOR YOUR SETUP) ******************/
#define wifi_ssid "" //enter your WIFI SSID
#define wifi_password "" //enter your WIFI Password

#define mqtt_server "192.168.XXX.XXX" // mqtt server
#define mqtt_user "" //enter your MQTT username if needed
#define mqtt_password "" //enter your password if needed

#define delayActive 500
/*************************************************************************************/

/************************************VARIABLE SETUP***********************************/
WiFiClient espClient;
PubSubClient client(espClient);
Servo servo;

/************************************* VOID SETUP ****************************************/
void setup() {
  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883); //CHANGE PORT HERE IF NEEDED
  client.setCallback(callback);
  servo.attach(13);
  
}
/*************************************************************************************/

/******************************** VOID WIFI SETUP *************************************/
void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
/*************************************************************************************/

/*********************************** IMPORTANT LOOP **********************************/

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
  String mytopic(topic);

  if (message.equals("OPEN")) {
    Serial.println("OPEN");
    servo.write(160);
    delay(delayActive);
    servo.write(90);
  }
  
  else if (message.equalsIgnoreCase("MIDDLE")) {
    Serial.println("MIDDLE");
    servo.write(90);
    delay(delayActive);
  }
  
  else if (message.equalsIgnoreCase("CLOSE")) {
    Serial.println("CLOSE");
    servo.write(20);
    delay(delayActive);
    servo.write(90);
  }

}
/*************************************************************************************/

/******************************** VOID LOOP ******************************************/
void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}

/*************************************************************************************/

/***********************************VOID RECONNECT ***********************************/

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESPSwitchChambre", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("Shutter");

    } else {
      Serial.print("failed, rc=");
      // Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    
  }
}
