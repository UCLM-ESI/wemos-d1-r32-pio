BOARDS=~/.platformio/platforms/espressif32/boards/
FW_ESP32=~/.platformio/packages/framework-arduinoespressif32/
WEMOS=~/.platformio/packages/framework-arduinoespressif32/variants/wemos-d1-r32

mkdir -p $BOARDS
cp wemos-d1-r32.json $BOARDS

mkdir -p $FW_ESP32
cat wemos-package.txt.part >> $FW_ESP32/boards.txt

mkdir -p $WEMOS
cp pins_arduino.h $WEMOS
