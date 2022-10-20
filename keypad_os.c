#include <PJONSoftwareBitBang.h>
#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>

#define SWBB_MODE 1 // 1.95kB/s - 15625Bd
#define PJON_MAX_PACKETS 1
#define PJON_PACKET_MAX_LENGTH 24
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define PJONPIN A0
PJONSoftwareBitBang bus;

....

void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  display.clearDisplay();
  display.display();
  display.setCursor(5,28);
  display.print("No");
  display.setCursor(5,48);
  display.println("link!");
  display.fillRect(110,52,118,63, BLACK);
  display.display();
  delay(1500);
  resetIKey();
 };

#define MAXKEYSIZE 10
char currentKeys[MAXKEYSIZE];
int currentKeyIndex = 0;

void sendKey(){
  if (currentKeyIndex <= 0) return;
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(5,48);
  display.print("Sending");
  display.display();
  bus.send(45, currentKeys, currentKeyIndex);
  bus.update();
  delay(2000);
  resetIKey();
}

....

void setup(){
  bus.strategy.set_pin(PJONPIN);
  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.begin();

  display.clearDisplay();
  display.setTextSize(2);            
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(" KeyPad OS");
  
....

void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
    switch(key) {
      case '#': sendKey();
                break;
      case '*': resetIKey();
                break;
 ....