#include <SPI.h>
#include <MFRC522.h>
#include <Keyboard.h>

#define RST_PIN    9 
#define SS_PIN    10

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

char UID_buff[100];
String UIDstring = String(UID_buff);

  void setup() {
    Serial.begin(9600);   // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
//    Serial1.begin(9600);    // Initialize serial for bluetooth link only for bluetooth version
    SPI.begin();      // Init SPI bus
    mfrc522.PCD_Init();   // Init MFRC522
//    ShowReaderDetails();  // Show details of PCD - MFRC522 Card Reader details for debug only
//    Serial.println(F("Skanuj PICC aby zobaczyć UID..."));   //=== this line for debug only
}

  void loop() {

    if ( ! mfrc522.PICC_IsNewCardPresent()) { //=== Look for new cards
    return;
    }

    if ( ! mfrc522.PICC_ReadCardSerial()) //=== Select one of the cards
    return;
//    Serial.print("Card UID:");   //Dump UID , this line for debug only

  String rfidUid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
  rfidUid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
  rfidUid += String(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.println(rfidUid); //=== Dump UID for debug
//  Serial.println("");

    rfidUid.toCharArray(UID_buff, rfidUid.length()+1); //=== Create array for bluetooth link and HID keyboard
//    Serial1.println(UID_buff);  // Send array to bluetooth, This line only for bluetooth version

    Keyboard.println(rfidUid); //===Send UID to keyboard

  mfrc522.PICC_HaltA(); // Halt PICC
  mfrc522.PCD_StopCrypto1();// Stop encryption on PCD
  }

/* //===================== for debug only
  void ShowReaderDetails() {

  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg); // Get the MFRC522 software version
    Serial.print(F("NFC Software Version: 0x"));
    Serial.print(v, HEX);
      if (v == 0x91)
    Serial.print(F(" = v1.0"));
      else if (v == 0x92)
    Serial.print(F(" = v2.0"));
      else
    Serial.print(F(" (Spartan 1.0)"));
    Serial.println("");

    if ((v == 0x00) || (v == 0xFF)) { // When 0x00 or 0xFF is returned, communication probably failed
    Serial.println(F("WARNING: Communication failure, is the module properly connected?"));
  }
}
*/ //======================
