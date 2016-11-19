#ifndef CAN_H
#define CAN_H
uint8_t caninitok;
uint16_t  canxto_id11 =    0; // 11 bit to id
uint16_t  canxfrom_id11 =  0;// 11 bit from id
uint8_t  canxcmd_id7 = 0;  // 7bit command

unsigned char sending_canid = 0;
uint32_t rx_can1id = 0;   //stores the received can1 id
uint32_t rx_can2id = 0;   // stores the received can2 id
int requesting_canid = 0;

//int can_MsgRx.data[8];
// unsigned char canMsg[8] = {sending_canid, canrpmlowbyte, canrpmhighbyte, 0x00, 0x00, 0x00, 0x00, 0x00};

//unsigned char canMsg[8] = {canbyte[0], canbyte[1], canbyte[2], canbyte[3], canbyte[4], canbyte[5], canbyte[6], canbyte[7]};
uint8_t std_ext = 0;
uint8_t canmsglen = 0;
uint8_t rxcan1Msg[8]; //can1 received message buffer
uint8_t rxcan2Msg[8]; // can2 received message buffer


uint32_t bigbufaddress[500];
uint8_t bigbufdata[4000];

#define CANSPEED_125      125000        // CAN speed at 125 kbps
#define CANSPEED_250      250000        // CAN speed at 250 kbps
#define CANSPEED_500      500000        // CAN speed at 500 kbps

/* CANBUS messages to operate outputs */
#define Indicator_ON    0x01

void initialiseCAN();
void Send_CAN_message(uint8_t cannum, uint8_t canbyte1, uint8_t canbyte2, uint8_t canbyte3, uint8_t canbyte4, uint8_t canbyte5, uint8_t canbyte6, uint8_t canbyte7, uint8_t canbyte8);
void receive_CAN_message();
void broadcast_Astream_11bit();
void broadcast_Astream_29bit();
void toggle(int pinNum);
unsigned long build_29bits(unsigned int toid11, unsigned int fromid11, byte cmd7);
void unbuild_29bits(unsigned long id29);
#endif
