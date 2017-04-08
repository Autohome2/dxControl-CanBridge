# Speeduino-can-interface

This Git repo describes the firmware and hardware to create an interface to CANBUS enable a Speeduino ECU.
The Speeduino ECU must be running the CAN enabled firmware variant in order for this device to work.

The module can be constructed using either a arduino MEGA2560 and a mcp2515 CAN io module or a Teensy 3.2 and a CAN transceiver module.The teensy version is compact enough to nabke the use of the recommended speeduino case. various transceiver modules can be used with the Teensy the ones below have been tested by the writers of the FLEXCAN library

    TI SN65HVD230D on 3.3V (1MBPS)
    TI SN65HVD232D / SN65HVD232QDQ1 on 3.3V (1MBPS)
    NXP TJA1050T/VM,118 on the same 5V supply as the Teensy. (1MBPS)
    Microchip MCP2551 on 5V (reported at 500KBPS)
    Linear LT1796 on 5V (not speedtested)

The interface module connects to the Speeduino ECU via its Serial3 port to Serial1 on the interface itself.
The interface is configured via TunerStudio over the onboard USB.

#What it does
The interface requests the current data from the Speeduino at 2hz .Later versions will have selectable broadcasts rate of up to 20hz
This is put into packets of 8 bytes with each bundle of 8 bytes being assigned an 11bit CAN ID address.
So if for example the base address is set as 3100dec(see configuration below on how to do this)then the data will be broadcast as follows.The base broadcast address is selectable via a Tunerstudio connection.The ini file loaded must match the firmware version.
(the information below is corrct at time of writing 19/11/2016)

Can address 3100:

- byte 0 - currentStatus.secl; //secl is simply a counter that increments each second
- byte 1 - currentStatus.squirt; //Squirt Bitfield
- byte 2 - currentStatus.engine; //Engine Status Bitfield
- byte 3 - currentStatus.dwell; //Dwell in ms * 10
- byte 4 - currentStatus.MAP >> 1; //map value is divided by 2
- byte 5 - currentStatus.IAT + CALIBRATION_TEMPERATURE_OFFSET; //mat
- byte 6 - currentStatus.coolant + CALIBRATION_TEMPERATURE_OFFSET; //Coolant ADC
- byte 7 - currentStatus.tpsADC; //TPS (Raw 0-255)

Can address 3101:

- byte 0 - currentStatus.battery10; //battery voltage
- byte 1 - currentStatus.O2; //O2
- byte 2 - currentStatus.egoCorrection; //Exhaust gas correction (%)
- byte 3 - currentStatus.iatCorrection; //Air temperature Correction (%)
- byte 4 - currentStatus.wueCorrection; //Warmup enrichment (%)
- byte 5 - lowByte(currentStatus.RPM); //rpm LB
- byte 6 - highByte(currentStatus.RPM); //rpm HB
- byte 7 - currentStatus.TAEamount; //acceleration enrichment (%)

Can address 3102:

- byte 0 - 0x00; //Barometer correction (%) 
- byte 1 - currentStatus.corrections; //Total GammaE (%)
- byte 2 - currentStatus.VE; //Current VE 1 (%)
- byte 3 - currentStatus.afrTarget;
- byte 4 - currentStatus.PW / 100; //Pulsewidth 1 multiplied by 10 in ms. Have to convert from uS to mS.
- byte 5 - currentStatus.tpsDOT; //TPS DOT
- byte 6 - currentStatus.advance;
- byte 7 - currentStatus.TPS; // TPS (0% to 100%)

Can address 3103:

- byte 0 - lowByte(currentStatus.loopsPerSecond);
- byte 1 - highByte(currentStatus.loopsPerSecond);
- byte 2 - lowByte(currentStatus.freeRAM);
- byte 3 - highByte(currentStatus.freeRAM);
- byte 4 - currentStatus.batCorrection; //Battery voltage correction (%)
- byte 5 - currentStatus.spark; //Spark related bitfield
- byte 6 - currentStatus.O2_2; //Second O2
- byte 7 - unused;

Can address 3104:

- byte 1 - lowByte(currentStatus.rpmDOT);
- byte 2 - highByte(currentStatus.rpmDOT);
- byte 3 - currentStatus.flex; //Flex sensor value (or 0 if not used)

#Configuration

The base CAN address is the beginning address with which the broadcast is made sequentially for each block of 8 bytes.
This is an 11bit address and must not clash with any other device on the network with the same address
