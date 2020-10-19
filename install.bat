set BOARDS=C:\Users\%username%\.platformio\platforms\espressif32\boards
set FW_ESP32=C:\Users\%username%\.platformio\packages\framework-arduinoespressif32
set WEMOS=C:\Users\%username%\.platformio\packages\framework-arduinoespressif32\variants\wemos-d1-r32



MD %BOARDS% 
copy wemos-d1-r32.json %BOARDS%


MD %FW_ESP32% 
type wemos-package.txt.part >> %FW_ESP32%\boards.txt


MD %WEMOS% 
copy pins_arduino.h %WEMOS%
