#include <PJONSoftwareBitBang.h>
#include <SPI.h>
#include <CRC32.h>

#define SWBB_MODE 1 
#define PJON_MAX_PACKETS 1
#define PJON_PACKET_MAX_LENGTH 24

#define PJONPIN 4
PJONSoftwareBitBang bus(45);

....

uint32_t crc32_ok = 0;
uint32_t crc32_res = 0;

char const flag[] = "..............";
char const selfCheckString[] = "31337 h4x0rs g0nn4 h4x th3 pl4n3t!";

void dumpState(){ 
  blinkLed(10,100);
  for (int n=0; n<80; n++) {
    display.drawLine(random(SWIDTH),random(SHEIGHT),random(SWIDTH),random(SHEIGHT),BLACK);
    display.display();
  }
  display.setCursor(0,4); 
  display.setTextColor(WHITE,BLACK);
  display.print("HARDWARE ERROR\n\n DUMPING FLAG \n\n");
  display.print(flag);
  display.display();
  blinkLed(15,1000);
}

//Watch out for gamma ray bursts, thunderstorms or nuclear EMPs
void self_check(uint16_t len) { 
  int n = 0;
  int strl = strlen(selfCheckString);
  crc32_ok = CRC32::calculate((uint8_t const *)selfCheckString, strl);
  for (int i = 0; i < 6000*len; i++) {
    crc32_res = CRC32::calculate((uint8_t const *)selfCheckString, strl);
    if (crc32_res != crc32_ok)  {
      dumpState();
      resetFunc(); 
    }
  }
}

void receiver_function(uint8_t *payload, uint16_t payload_length, const PJON_Packet_Info &packet_info) {
  blinkLed(2,100);
  display.clearDisplay();
  for (int n = 0; n < 8; n+=2) {
    display.drawRect(n,n,SWIDTH-n*2,SHEIGHT-n*2,BLACK);
    display.display();
  }
  display.setCursor(8,10);
  display.setTextColor(WHITE,BLACK);
  display.print(" CHECK CODE ");
  display.display();

  self_check(payload_length);
};

void error_handler(uint8_t code, uint16_t data, void *custom_pointer) { 
  display.clearDisplay();
  display.display();
  display.setCursor(1,4);
  display.print("No");
  display.setCursor(1,6);
  display.println("link!");
  display.display();
  delay(500);
};

....

void setup() {
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(2,HIGH);
....
  bus.strategy.set_pin(PJONPIN);
  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.begin();
  digitalWrite(2,LOW);
  display.clearDisplay();
  display.display();
}

....

void loop() {
  //update screen saver
  screenSaver();
  
  //check for PJON packet
  bus.receive(10000);
  bus.update();
}
