# MQTT Subscriber

Deps
----
mosquitto and mosquitto-clients


Run Broker
----
mosquitto -c broker.conf


Run Publisher
----
mosquitto_sub -h 192.168.1.103 -p 2883 -t "esi/room1/notices" -m "Hello MQTT from my PC"

