# modbusrelay
Modbus communication using MegaR3 as pass through device.
The Hardware Serial2 on the MegaR3 is where the master device is to be connected.
The query received from Hardware Serial2 and writen to storedData and relayed to Serial1 for communication with the sensor via Rs485 Module.
The response data is relayed back to the master device. ToDo: CRC
