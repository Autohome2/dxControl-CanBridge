/*
Speeduino Can interface
Copyright (C) Darren Siepka
A full copy of the license may be found in the projects root directory
*/
void initialiseCAN1()
{
  
  #if defined(PROCESSOR_MEGA_ALL)
  uint8_t tryInit = 0;
  START_INIT: 
    if(CAN1.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)//(CAN_OK == CAN1.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
    {
       CAN1.setMode(MCP_NORMAL);
       // Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
      //  Serial.println("CAN BUS Shield init fail");
      //  Serial.println("Init CAN BUS Shield again");
        delay(100);
        tryInit++;
        if (tryInit<50){goto START_INIT;} //retry 50 times before quiting init
    }
  #elif defined(PROCESSOR_TEENSY_3_2)
  //CANbus1.begin();
  uint8_t tryInit = 0;
  START_INIT: 
    if(CAN1.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)//(CAN_OK == CAN1.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
    {
       CAN1.setMode(MCP_NORMAL);
        //Serial.println("CAN BUS Shield init ok!");
        caninitok = 1;
    }
    else
    {
      //  Serial.println("CAN BUS Shield init fail");
      //  Serial.println("Init CAN BUS Shield again");
        delay(100);
        tryInit++;
        if (tryInit<50){goto START_INIT;} //retry 50 times before quiting init
    }
  #endif    
}
//----------------------------------------------------------------------------------------
void initialiseCAN2()
{
  uint8_t tryInit2 = 0;
  START2_INIT:

    if(CAN2.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK)//  if(CAN_OK == CAN2.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
    {
         CAN2.setMode(MCP_NORMAL);
       // Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
      //  Serial.println("CAN BUS Shield init fail");
      //  Serial.println("Init CAN BUS Shield again");
        delay(100);
        tryInit2++;
        if (tryInit2<50){goto START2_INIT;} //retry 50 times before quiting init
    }
}
//----------------------------------------------------------------------------------------
void Send_CAN_message(uint8_t canIDnum, uint16_t canaddress, uint8_t cinc)
{
  uint8_t  canbyte[8];
  uint8_t canMsg[8] = {ECUstreambuffer[(0+cinc)], ECUstreambuffer[(1+cinc)], ECUstreambuffer[(2+cinc)], ECUstreambuffer[(3+cinc)], ECUstreambuffer[(4+cinc)], ECUstreambuffer[(5+cinc)], ECUstreambuffer[(6+cinc)], ECUstreambuffer[(7+cinc)]};
// uint8_t canMsg[8] = {*cbyte, ((*cbyte)+1), ((*cbyte)+2), ((*cbyte)+3), ((*cbyte)+4), ((*cbyte)+5), ((*cbyte)+6),((*cbyte)+7)};

  if (canIDnum == 1)     //use can module id1
      {
        //Create data packet for CAN message
  
      #if defined(PROCESSOR_MEGA_ALL)    
          sending_canid = canaddress; //sending_canid = 11;//(gateway_true_canid + 512);  // the id of the sending can device
          std_ext = 0;  //0 if std(11bit), 1 if extended can(29bit)
          canmsglen = 8;  //length of message to send
          // send data:  id = 0x123, standrad flame, data len = 8, stmp: data buf
          CAN1.sendMsgBuf(sending_canid, std_ext, canmsglen, canMsg);

      #elif defined(PROCESSOR_TEENSY_3_2)
 //         txmsg.id = canaddress;
         // Serial.print(" add ");Serial.print (canaddress);
          //txmsg.id = 0x200;  //set the message id for 11 bits 0x000 to 0x7FF
          //txmsg.id = 0xC029001; //set the message id for 29bits 0x00000000 to 0x1FFFFFFF
//          txmsg.len = 8;    //set message data length as 8 bits
//          txmsg.ext = 0;    //set message id as std
          //txmsg.ext = 1;  //set message id as extended
//          for (int i=0; i< txmsg.len; i++)
//              {
//                txmsg.buf[i]=canbyte[i];
                //Serial.print(" byte ");Serial.print(i);Serial.print(" ");Serial.print(canbyte[i]);
//              } 
        //  CANbus1.write(txmsg);
                  sending_canid = canaddress; //sending_canid = 11;//(gateway_true_canid + 512);  // the id of the sending can device
          std_ext = 0;  //0 if std(11bit), 1 if extended can(29bit)
          canmsglen = 8;  //length of message to send
          // send data:  id = 0x123, standrad flame, data len = 8, stmp: data buf
          CAN1.sendMsgBuf(long(canaddress), std_ext, canmsglen, canMsg);

      #endif    
      }
  else if(canIDnum == 2)     //use can module id2
      {
      #if defined(PROCESSOR_MEGA_ALL)|| defined(PROCESSOR_TEENSY_3_2) //use spi can module
          sending_canid = canaddress;//(gateway_true_canid + 512);  // the id of the sending can device
          std_ext = 0;  //0 if std(11bit), 1 if extended can(29bit)
          canmsglen = 8;  //length of message to send

          // send data:  id = 0x123, standrad flame, data len = 8, stmp: data buf
          CAN2.sendMsgBuf(canaddress, std_ext, canmsglen, canMsg);
      #endif  
      }    

}  
//---------------------------------------------------------------------------------------------
void receive_CAN_message(uint8_t canIDnum)
{
  if (canIDnum == 1)     //use can module id1
      {
      #if defined(PROCESSOR_MEGA_ALL)  
          canmsglen = 8;  //length of message to send
          CAN1.readMsgBuf(&rx_can1id, &canmsglen, rxcan1Msg);       // Read data: len = data length, buf = data byte(s)
          // now do something with the data
          
      #elif defined(PROCESSOR_TEENSY_3_2)
          rxmsg.len = 8;    //set message data length as 8 bits
          if ( CANbus1.read(rxmsg) ) 
             {
              //there is a message so do things
              //String a = "";
              for (int i=0; i<rxmsg.len ; i++)
                 {
                  rxcan1Msg[i] = rxmsg.buf[i];
                  //    a += (char)inmsg.buf[i];
                  //  Serial.println(a);
                 }
                 //now do something with the data
                 //how about save it into a big buffer with 500 slots?
                 bigbufaddress[1] = rxmsg.id;
                 bigbufdata[1] = rxmsg.buf[1];
             }
      #endif
      }
  else if(canIDnum == 2)
       {
       #if defined(PROCESSOR_MEGA_ALL)|| defined(PROCESSOR_TEENSY_3_2) //use spi can module 
         CAN2.readMsgBuf(&rx_can2id, &rxmsg.len, rxcan2Msg);       // Read data: len = data length, buf = data byte(s)
         // now if there is data do something
       #endif 
       }       

}

//------------------------------------------------------------------------------------

void broadcast_Astream_11bit()
{
  #if defined(PROCESSOR_MEGA_ALL)  

  #elif defined(PROCESSOR_TEENSY_3_2)
        uint16_t baseId = 0x200;
        txmsg.id = baseId; //0x200;  //set the message id for 11 bits 0x000 to 0x7FF
          //txmsg.id = 0xC029001; //set the message id for 29bits 0x00000000 to 0x1FFFFFFF
//        txmsg.len = 8;    //set message data length as 8 bits
//        txmsg.ext = 0;    //set message id as std

   //Abuffer[] contains the data from speedys stream output
                
      for (int i=0; i< 9; i++)
          {
            switch (i)
              {            
                case 0:
                //dwell , Abuffer[3]
                break;
          
                case 1:
                //map , Abuffer[4]
//                txmsg.id = baseId+i;
//                txmsg.buf[0] = Abuffer[4];

                break;
                
                case 2:
                //iat
                break;

     //     case 3:
          //coolant
     //     break;

     //     case 4:
          //tps
     //     break;

       //   case 5:
          //battery
    //      break;
          
      //    case 6:
          //o2
     //     break;

    //      case 7:
          //rpm
    //      break;

     //     case 8:
          //advance
    //      break;
          
//      CANbus1.write(txmsg);
              }
          }
  #endif
}
//------------------------------------------------------------------------------------

void broadcast_Astream_29bit()
{
    #if defined(PROCESSOR_MEGA_NO61)  

  #elif defined(PROCESSOR_TEENSY_3_2)
       //uint16_t baseId = 0x200;
           uint32_t baseId = 0xC029001;
        txmsg.id = baseId; //0x200;  //set the message id for 11 bits 0x000 to 0x7FF
        //  txmsg.id = 0xC029001; //set the base message id for 29bits 0x00000000 to 0x1FFFFFFF
        txmsg.len = 8;    //set message data length as 8 bits
        txmsg.ext = 1;    //set message id as std

   //Abuffer[] contains the data from speedys stream output
                
      for (int i=0; i< 10; i++)
          {
            switch (i)
              {            
                case 0:
                //dwell , Abuffer[3]
                break;
          
                case 1:
                //map , Abuffer[4]
                txmsg.id = baseId+i;
                txmsg.buf[0] = Abuffer[4];
                break;
                
                case 2:
                //iat
                break;

     //     case 3:
          //coolant
     //     break;

          case 4:
          //tps adc [7]
               txmsg.id = baseId+i;
               txmsg.buf[0] = Abuffer[7];
          break;
          
          case 5:
          //tps % [23]
          break;
          
          case 6:
          //battery
          break;
          
          case 7:
          //o2
          break;

          case 8:
          //rpm
          break;

          case 9:
          //advance
          break;
          
      CANbus1.write(txmsg);
              }
          }

  #endif  
}

//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
unsigned long build_29bits(unsigned int toid11, unsigned int fromid11, byte cmd7)
{
  unsigned long id29 = 0;
  id29 = toid11;
  //id29 = id29 & toid11;
  id29 = id29 << 11;
  id29 = id29 + fromid11;
  id29 = id29 << 7;
  id29 = id29 | cmd7;
  
  return id29;  //return the 29bit id (as long) // the 29bit can id to be used made from the to,from and cmd values
}

void unbuild_29bits(unsigned long id29) //returns the cm7, and two 11bit ids in canxcmd_id7, canxfrom_id11 and canxto_id11 variables
{
unsigned long id29temp1 = id29;
unsigned long id29temp2 = id29;
  canxcmd_id7 = id29temp1 & 0x7F;
  canxfrom_id11 = id29temp2 >> 7;
  canxfrom_id11 = canxfrom_id11 & 0x7FF;
  canxto_id11 = id29 >> 18;
  canxto_id11 = canxto_id11 & 0x7FF;
}
