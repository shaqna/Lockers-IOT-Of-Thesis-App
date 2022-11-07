#include "Wire.h"
#include "MyKeypad.h"

const uint8_t KEYPAD_ADDRESS = 0x20;
I2CKeyPad keyPad(KEYPAD_ADDRESS);

static MyKeypad* instance = NULL;

char keymap[19] = "123A456B789C*0#DNF";

MyKeypad::MyKeypad() {
  instance = this;
}

MyKeypad::~MyKeypad() {
  delete instance;
}

void MyKeypad::initKeypad() {
    
    Wire.begin();
    Wire.setClock(400000);
    if (keyPad.begin() == false){
      Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
      while (1);
    }

    keyPad.loadKeyMap(keymap);

}

String MyKeypad::onPressed() {
    char buf[20];
    int result = readKeyPadUntil('#', buf, 20, 10000);
    if (result == 0)
    {
      Serial.print("SUCCESS: ");
      Serial.println(buf);

      return String(buf);
    }
    if (result == -1)
    {
      Serial.print("FAILURE: ");
      Serial.println(buf);

    }
    if (result == -2)
    {
      Serial.print("TIMEOUT: ");
      Serial.println(buf);
  
    }
    if (result == -3)
    {
      Serial.print("OVERFLW: ");
      Serial.println(buf);

    }

    return String("");
}

int MyKeypad::readKeyPadUntil(char until, char * buffer, uint8_t length, uint16_t timeout)
{
  uint8_t bufferIndex = 0;
  uint32_t start = millis();
  char lastChar = '\0';

  // empty the return buffer
  buffer[bufferIndex] = 0;

  while (millis() - start < timeout)
  {
    char ch = keyPad.getChar();
    if (ch == 'N')        lastChar = 'N';
    else if (ch == until) return 0;       // success
    else if (ch == 'F')   return -1;      // keyPad fail
    else
    {
      if (ch != lastChar)
      {
        lastChar = ch;
        if ( bufferIndex == length ) return -3;  // overflow
        // add key to buffer
        buffer[bufferIndex++] = ch;
       
        buffer[bufferIndex]   = 0;
      }
    }
    yield();
  }
  return -2;    //  timeout
}
