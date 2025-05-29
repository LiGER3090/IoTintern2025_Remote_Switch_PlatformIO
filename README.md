# Remote Switch

This is the Final Project work in Turin Polytechnic University in Tashkent by Abdulloh Toliboev.
This project an IoT-based system that allows you to remotely shut down your laptop using an ESP32 microcontroller, a Telegram bot, and an MQTT broker hosted on a Google Cloud VM. It offers a hardware-level control solution for situations where traditional remote access tools aren't available or practical.
## 🚀 How It Works

1. The user sends a command (e.g., `/shutdown`) to a Telegram bot.
2. ESP32 receives the command and publishes it to a specific MQTT topic.
3. The laptop is subscribed to this topic and executes the corresponding system command.

## 🧠 System Architecture
- **ESP32**: Publishes commands over MQTT
- **Telegram Bot**: Interface for user input
- **MQTT Broker**: Mosquitto running on Google Cloud VM
- **Laptop**: Listens to MQTT topic and executes system actions

---

## 🔧 Hardware & Software Used

| Component           | Purpose                                           |
|---------------------|--------------------------------------------------|
| ESP32 Dev Board     | Publishes control commands to MQTT               |
| Laptop              | Executes shutdown/reboot via MQTT instructions  |
| Telegram Bot        | Sends commands to ESP32                          |
| MQTT Broker         | Mosquitto running on Google Cloud VM             |
| Python + paho-mqtt  | Laptop MQTT client for command handling          |

---

## 🖥️ Laptop Client Script (Python Example)

```python
import paho.mqtt.client as mqtt
import os

def on_message(client, userdata, msg):
    command = msg.payload.decode()
    if command == "shutdown":
        os.system("shutdown /s /t 0")  # Windows
    elif command == "reboot":
        os.system("shutdown /r /t 0")

client = mqtt.Client()
client.connect("YOUR_VM_IP", 1883, 60)
client.subscribe("remote/laptop")
client.on_message = on_message
client.loop_forever()
```

## 🔐 Security Considerations ##
Use MQTT authentication with mosquitto_passwd and acl_file

Configure firewall rules for the Google Cloud VM

Validate Telegram users by ID before accepting commands
