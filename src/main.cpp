#include <defVar.h>

EnergyMonitor emon1;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTTuser,MQTTpasw)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
//  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_energymeter_sensor();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    emon1.calcVI(20, 2000);
//    float power = emon1.apparentPower;
    float volt = emon1.Vrms;    //extract Vrms into Variable
    float current = emon1.Irms; //extract Irms into Variable

    snprintf (msg, 80, "{\"v\":\"%.2f\",\"c\":\"%.2f\"}", volt, current);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(pubTopic, msg);
  }
}

void setup_energymeter_sensor(){
  emon1.voltage(32, 234.26, 1.7);
  emon1.current(34, 1.94);
  }