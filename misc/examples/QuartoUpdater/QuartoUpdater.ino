#include "nor_boot.h"

extern unsigned long BinaryData_bin[];
extern unsigned long _binary_load_addr;

void (*toggleFn)(void)  = &toggleLEDGreen;

void setup() {
  if (&_binary_load_addr != BinaryData_bin){
    Serial.printf("Error, Binary data (@0x08x) should have same address as binary load address (@0x08x)\n",&_binary_load_addr,BinaryData_bin);
    toggleFn = &toggleLEDRed;
    return;
  }

	if (BinaryData_bin[4] != APPLICATION_BOOT_HEADER) {
		Serial.printf("Invalid boot header: 0x%08x != 0x%08x at 0x%08x\n",APPLICATION_BOOT_HEADER,BinaryData_bin[4],&BinaryData_bin[4]);
    toggleFn = &toggleLEDRed;
		return;
  }

	uint32_t ResetHandler = BinaryData_bin[1];
	uint32_t stack = BinaryData_bin[0];
  __asm__ volatile ("mov SP, %0" : : "r" (stack) : );
  __asm__ volatile ("mov PC, %0" : : "r" (ResetHandler) : );
}

void loop() {
  static unsigned long lastrunLED = 0;
	
  if (millis() > lastrunLED) {  //Run once every 1000ms
    toggleFn();
    lastrunLED = millis() + 250;
  }
}