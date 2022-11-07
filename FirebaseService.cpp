#include "FirebaseService.h"
#include <addons/TokenHelper.h>

#define WIFI_SSID "uwu"
#define WIFI_PASSWORD "quwel1712"
#define API_KEY "AIzaSyCUgBbI6MJeBy8lhnJ_GOAnsa1YQdbHFLE"
#define FIREBASE_PROJECT_ID "firestorelearn-1865a"
#define USER_EMAIL "firestore@email.com"
#define USER_PASSWORD "firestore"

static FirebaseService *instance = NULL;

FirebaseService::FirebaseService() {
    dataMillisGetDocument = 0;
    dataMillisUpdateDocument = 0;
    instance = this;
    //randomSeed(analogRead(0));
}

FirebaseService::~FirebaseService() {
  delete instance;
}


void FirebaseService::initWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    instance->firebaseInit();
}

void FirebaseService::firebaseInit(){
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.token_status_callback = tokenStatusCallback;
    
    fbdo.setResponseSize(2048);
    
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void FirebaseService::getDocument() {
  int currentMillis = millis();
  if (Firebase.ready() && (millis() - dataMillisGetDocument >= 10000 || dataMillisGetDocument == 0)) {
      Serial.print("Get millis now = ");
      Serial.println(currentMillis - dataMillisGetDocument);
      dataMillisGetDocument = millis();
  
      FirebaseJsonData result;
      FirebaseJson json;
      String documentPath = "locker_password/Locker";
      String fieldPath = "fields/key/stringValue";
  
      Serial.print("Get a document...   ");
  
      if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str())) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          json.setJsonData(fbdo.payload().c_str());
          json.get(result, fieldPath);
              
          if(result.success) {
              Serial.println(result.type);
              Serial.println(result.intValue);
              setKey(result.to<String>());
          } else {
              Serial.println("Failed get data");
          }
              
      } else {
          Serial.println(fbdo.errorReason());
      }
   }
}

int FirebaseService::updateDocument() {
  int currentMillis = millis();
  if (Firebase.ready() && (millis() - dataMillisUpdateDocument > 20000 || dataMillisUpdateDocument == 0)) {
      Serial.print("Update millis now = ");
      Serial.println(currentMillis - dataMillisUpdateDocument);
      dataMillisUpdateDocument = millis();

      std::vector<struct fb_esp_firestore_document_write_t> writes;
      
      struct fb_esp_firestore_document_write_t update_write;
      update_write.type = fb_esp_firestore_document_write_type_update;

      FirebaseJson content;
      String documentPath = "locker_password/Locker";
      String fieldPath = "fields/key/stringValue";

      content.set(fieldPath, generateRandomValue());

      update_write.update_document_content = content.raw();
      update_write.update_document_path = documentPath.c_str();
      update_write.update_masks = "key";
      update_write.current_document.exists = "true";

      writes.push_back(update_write);
  
      Serial.print("Set  a document...   ");
  
      if (Firebase.Firestore.commitDocument(&fbdo, FIREBASE_PROJECT_ID, "" , writes , "" )) {
          Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
          return 200;
      } else {
          Serial.println(fbdo.errorReason());
          return 400;
      }
   }
}

String FirebaseService::generateRandomValue() {
  int randNum = random(1000,10000);
  //int randNum = 2573;
  return String(randNum);
}

void FirebaseService::setKey(String key) {
  Serial.printf("Key is %s \n", &key);
  this->locker_password = key;
}

String FirebaseService::getKeyLocker() {
  return locker_password;
}
