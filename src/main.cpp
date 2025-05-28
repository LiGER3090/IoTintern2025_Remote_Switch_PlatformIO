#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YOUR SSID";
const char* password = "YOUR PASSWORD";
const char* mqtt_server = "YOUR IP";
const int mqtt_port = 1883;
const char* mqtt_user = "YOUR USER";
const char* mqtt_pass = "YOUR PASSWORD";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) message += (char)payload[i];

  Serial.print("Получено сообщение: ");
  Serial.println(message);

  if (message == "on") {
    Serial.println("Команда 'on' получена — включаю LED");
    digitalWrite(2, HIGH);
  } else if (message == "off") {
    Serial.println("Команда 'off' получена — выключаю LED");
    digitalWrite(2, LOW);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      client.subscribe("ttpu/shutdown"); // Замените your/topic на нужный топик
      Serial.println("Подписка на топик ttpu/shutdown выполнена");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH); // Изначально LED выключен

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Подключение к Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Подключено к Wi-Fi с IP: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
