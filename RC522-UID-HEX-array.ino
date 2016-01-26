
#include <SPI.h>
#include <MFRC522.h>
#include <Keyboard.h>

#define RST_PIN		9	
#define SS_PIN		10

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance

char message_buff[100];
//int i = 0;
String msgString = String(message_buff);



void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
//  Serial1.begin(9600); // Initialize serial for bluetooth link only for bluetooth version
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
//	ShowReaderDetails();	// Show details of PCD - MFRC522 Card Reader details for debug only
//	Serial.println(F("Skanuj PICC aby zobaczyć UID..."));   //=== this line for debug only
}

void loop() {


//=== Look for new cards
if ( ! mfrc522.PICC_IsNewCardPresent()) {
return;
}

//=== Select one of the cards
if ( ! mfrc522.PICC_ReadCardSerial())return;
// Serial.print("Card UID:");   //Dump UID , this line for debug only
String rfidUid = "";
for (byte i = 0; i < mfrc522.uid.size; i++) {
rfidUid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
rfidUid += String(mfrc522.uid.uidByte[i], HEX);
}

//=== Dump UID for debug
Serial.println(rfidUid);
// Serial.println("");

//=== Create array for bluetooth link and HID keyboard
rfidUid.toCharArray(message_buff, rfidUid.length()+1);

//=== Send array to bluetooth
// Serial1.println(message_buff);  // This line only for bluetooth version

//===Send UID to keyboard
Keyboard.println(rfidUid);

mfrc522.PICC_HaltA(); // Halt PICC
mfrc522.PCD_StopCrypto1();// Stop encryption on PCD
}

/*
//===================== for debug only
void ShowReaderDetails() {
	// Get the MFRC522 software version
	byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
	Serial.print(F("NFC Software Version: 0x"));
	Serial.print(v, HEX);
	if (v == 0x91)
		Serial.print(F(" = v1.0"));
	else if (v == 0x92)
		Serial.print(F(" = v2.0"));
	else
		Serial.print(F(" (Spartan 1.0)"));
	Serial.println("");
	// When 0x00 or 0xFF is returned, communication probably failed
	if ((v == 0x00) || (v == 0xFF)) {
		Serial.println(F("WARNING: Communication failure, is the module properly connected?"));
	}
}
*/

//======================
