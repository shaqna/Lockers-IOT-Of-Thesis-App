#pragma once
#ifndef FIREBASE_SERVICE_H_
#define FIREBASE_SERVICE_H_

#include <WiFi.h>
#include <Firebase_ESP_Client.h>


class FirebaseService {
private:
  unsigned long dataMillisGetDocument;
  unsigned long dataMillisUpdateDocument;
  String locker_password;
  FirebaseData fbdo;
  FirebaseAuth auth;
  FirebaseConfig config;

  void firebaseInit();
  String generateRandomValue();
 

public:
  FirebaseService();
  ~FirebaseService();
  void initWiFi();
  void getDocument();
  int updateDocument();
  String getKeyLocker();
  void setKey(String key);
  
};


#endif
