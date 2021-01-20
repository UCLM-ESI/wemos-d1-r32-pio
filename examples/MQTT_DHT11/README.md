# MQTT DHT11

Ejemplo donde el ESP32 publica la temperatura y humedad de una estancia mediante el uso del sensor DHT11 y el protocolo MQTT


## Material
* 1 Sensor DHT11

## Output
None


## Input
* D4 >> DHT11


## Run

Deps
----
mosquitto and mosquitto-clients


Run Broker
----
mosquitto -c broker.conf


Run Subscriber
----
mosquitto_sub -h 192.168.1.103 -p 2883 -t "esi/room1/temp" -v
Also with Node-RED (deploy flow stored in flows.json)

