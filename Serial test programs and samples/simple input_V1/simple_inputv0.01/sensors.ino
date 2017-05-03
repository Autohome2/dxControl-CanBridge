void getsensor_reading(uint16_t param)
{
  switch (param)
  {
    case 65187: 
        //pgn 65187
        EGT1 = 567; //0237 in hex split as 02 and 55 in dec
        EGT2 = 234;
        EGT3 = 978;
        EGT4 = 1011;
        Gdata[0] = lowByte(EGT1);
        Gdata[1] = highByte(EGT1);
        Gdata[2] = lowByte(EGT2);
        Gdata[3] = highByte(EGT2);
        Gdata[4] = lowByte(EGT3);
        Gdata[5] = highByte(EGT3);
        Gdata[6] = lowByte(EGT4);
        Gdata[7] = highByte(EGT4);
    break;

    case 65263:
        //65263
        Gdata[0] = 127;   //engine coolant temp
        Gdata[1] = 77;  //fuel temp
        Eot = 82;
        Gdata[2] = lowByte(Eot);   //engine oil temp
        Gdata[3] = highByte(Eot);   //""
        Tot = 156;
        Gdata[4] = lowByte(Tot);   //turbo oil temp
        Gdata[5] = highByte(Tot);   //""
        Gdata[6] = 111;   //engine intercooler temp
        Gdata[7] = 203;   //engine intercooler thermostat opening
    break;
  }
}

