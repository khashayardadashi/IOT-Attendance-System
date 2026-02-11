#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 5
#define RST_PIN 4

const char* ssid = "WifiUsername";
const char* password = "WifiPassword";

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  Serial.println(uid);
  HTTPClient ApiSubmit;
  String UrlApi = "http://45.138.132.164/api/v1/user/submit?key=6e175f42af3eab3510ae072a8496c49423b725c38455b1ffeadd87891a9d3f55&code=" + uid;
  ApiSubmit.begin(UrlApi);
  int httpResponseCode = ApiSubmit.GET();
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {
    String response = ApiSubmit.getString();
    Serial.println(response);
    lcd.clear();
    if(response.length()<=16){
    lcd.setCursor(0, 0);
    lcd.print(response);
    }
    else{
    lcd.setCursor(0, 0);
    lcd.print(response.substring(0,16));
    lcd.setCursor(0, 1);
    lcd.print(response.substring(16,32));
    }
    ApiSubmit.end();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error");
    ApiSubmit.end();
  }

  rfid.PICC_HaltA();
  delay(1500);

}
