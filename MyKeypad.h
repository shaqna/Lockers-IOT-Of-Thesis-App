#pragma once
#ifndef KeyPad_H_
#define KeyPad_H_

#include "I2CKeyPad.h" 

class MyKeypad {
  private:
    int readKeyPadUntil(char, char*, uint8_t, uint16_t);
    
  public:
    char buf[20];
    MyKeypad();
    ~MyKeypad();
    String onPressed();
    void initKeypad();
    
  
};


#endif
