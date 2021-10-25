# Modbus RTU (Master-Multiple Slaves FreeRTOS)

Note: Responsiveness is better than Master-Multiple Slaves

## Master
### Input
* D2 -> ON/OFF Slave1: Switch ON/OFF the D13 LED of the Slave 1
* D3 -> ON/OFF Slave2: Switch ON/OFF the D13 LED of the Slave 2

### Output
* D12 -> LED: It is switched ON/OFF in accordace with the state of the Slave 1 SW1
* D13 -> LED: It is switched ON/OFF in accordace with the state of the Slave 2 SW1



## Slave 1
### Input
* D2 -> ON/OFF Master: Switch ON/OFF the D13 LED of the Master

### Output
* D13 -> LED: It is switched ON/OFF in accordace with the SW1 of the Master



## Slave 2
### Input
* D2 -> ON/OFF Master: Switch ON/OFF the D12 LED of the Master

### Output
* D13 -> LED: It is switched ON/OFF in accordace with the SW2 of the Master




