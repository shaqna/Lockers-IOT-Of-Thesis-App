#include "FirebaseService.h"
#include "MyKeypad.h"
#include <LiquidCrystal_I2C.h>

FirebaseService *service;
MyKeypad *myKeypad;

LiquidCrystal_I2C lcd(0x27, 16, 2);

String lockerKey = "";
String myKey = "";

unsigned long prevTime = 0;
unsigned long currentTime = 0;
int timeTarget = 5000;

const int RELAY_PIN = 15;
int statusCode;

void setup(){
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  relayState(LOW);

  setupFirebaseService();
  setupKeypad();

  lcd.begin();
  lcd.backlight();
  
  
}
  
void loop(){
  
  setLcd(0,0,"Masukkan Key:", "");

  do {
    statusCode = service->updateDocument();
    delay(1000);
  } while(statusCode != 200);

  Serial.println(statusCode);
  
  if(statusCode == 200) {
     getKeyDoc();  
     delay(1000);
  }
  Serial.print("Locker Key: :");
  Serial.println(lockerKey);
  delay(300);


  if(lockerKey != "") {
        checkKey();
        delay(100);  
  }
  

  Serial.print("Locker Key: :");
  Serial.println(lockerKey);
  delay(300);
  Serial.print("My Key: ");
  Serial.println(myKey);
  delay(300);

  resetValue();
  relayState(LOW);
}

void setLcd(int x, int y, String top, String bottom) {
  lcd.clear();
  lcd.setCursor(x,y);
  lcd.print(top);
  lcd.setCursor(x, y+1);
  lcd.print(bottom);
}

void setupFirebaseService() {
  service = new FirebaseService();
  service->initWiFi();
}

void setupKeypad() {
  myKeypad = new MyKeypad();
  myKeypad->initKeypad();
}

void resetValue() {
  lockerKey = "";
  myKey = "";
  statusCode = 0;
}

void relayState(uint8_t state) {
  digitalWrite(RELAY_PIN, state);
}

void getKeyDoc() {
  do {
    service->getDocument();
    delay(1000);
    lockerKey = service->getKeyLocker();
    delay(50);
  }while(lockerKey == "");

}

void checkKey() {
  do {
    myKey = myKeypad->onPressed();
    
    if(myKey == lockerKey) {
      setLcd(0,0,"Mengecek key: ", myKey);
      delay(3000);
      Serial.println("Sama");
      setLcd(0,0,"Benar", "Loker Terbuka");
      delay(100);
      relayState(HIGH);
      delay(5000);
    } else if(myKey == ""){
      Serial.println("Ulangi");
      setLcd(0,0,"Masukkan Key:", "");
      delay(300);
    } else {
      Serial.println("Tidak Sama");
      setLcd(0,0,"Masukkan Key:", " (Kunci Salah)");
      delay(300);
    }
    
  }while (myKey != lockerKey);
  delay(10);
}
