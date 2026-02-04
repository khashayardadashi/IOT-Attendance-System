#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 5
#define RST_PIN 4

const char* ssid = "Khashayar";
const char* password = "31653165";

MFRC522 rfid(SS_PIN, RST_PIN); 

void setup() { 
  Serial.begin(115200);
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}
 
void loop() {

  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  if ( ! rfid.PICC_ReadCardSerial())
    return;

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  Serial.println(uid);
  sendUID(uid);


  rfid.PICC_HaltA();
  delay(1500);
}
void sendUID(String uid) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }

  HTTPClient httpAdd;
  String urlAdd = "http://192.168.1.8:5278/CardId/" + uid;
  Serial.println("Sending to: " + urlAdd);

  httpAdd.begin(urlAdd);
  int httpResponseCodeAdd = httpAdd.GET();
  
  Serial.print("HTTPAdd Response code: ");
  Serial.println(httpResponseCodeAdd);

  if (httpResponseCodeAdd > 0) {
    String responseAdd = httpAdd.getString();
    Serial.println(responseAdd);
    
    if (responseAdd == "Success") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Card Scan");
      httpAdd.end();
      return;
    } 
    else 
    {
      httpAdd.end(); 
      
      HTTPClient httpEdit;
      String urlEdit = "http://192.168.1.8:5278/Edit/" + uid;
      httpEdit.begin(urlEdit);
      Serial.println("Sending to: " + urlEdit);
      int httpResponseCodeEdit = httpEdit.GET();
      Serial.print("HTTPEdit Response code: ");
      Serial.println(httpResponseCodeEdit);
      
      if (httpResponseCodeEdit > 0) {
        String responseEdit = httpEdit.getString();
        Serial.println(responseEdit);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(responseEdit);
      }
      
      httpEdit.end();
    }
  } 
  else
  {
    httpAdd.end();
    Serial.println("Add request failed");
  }
}


