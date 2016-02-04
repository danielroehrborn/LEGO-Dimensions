/*
    Adafruit invests time and resources providing this open source code, 
    please support Adafruit and open-source hardware by purchasing 
    products from Adafruit!
*/
/**************************************************************************/
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <stdio.h>
#include <stdlib.h>
extern "C" {
#include "tea.h"
#include "LEGOchar.h"
}
// If using the breakout with SPI, define the pins for SPI communication.
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

// If using the breakout or shield with I2C, define just the pins connected
// to the IRQ and reset lines.  Use the values below (2, 3) for the shield!
#define PN532_IRQ   (PB12)
#define PN532_RESET (PB13)  // Not connected by default on the NFC Shield

// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a software SPI connection (recommended):
//Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
Adafruit_PN532 nfc(PA4);

// Or use this line for a breakout or shield with an I2C connection:
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A Card ...");
}

void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint16_t tag=1;
  
  
 // Wait a bit before trying again

    
  


    
  // Wait for an NTAG203 card.  When one is found 'uid' will be populated with
  // the UID, and uidLength will indicate the size of the UUID (normally 7)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  
  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    uint32_t data32[2];
    encr(uid,16,data32);
    
    if (uidLength == 7)
    {
       
      uint8_t data[32];
      uint8_t * data8;
      data8=(uint8_t*)data32;
      uint8_t data36[4];
      uint8_t data37[4];
      uint8_t data0[4]={0x00,0x00,0x00,0x00}; //empty data block
      uint8_t data38[4]={0x00,0x01,0x00,0x00}; //Vehicle block
      uint8_t data43[4];
      data36[0]=data8[3];
      data36[1]=data8[2];
      data36[2]=data8[1];
      data36[3]=data8[0];
      data37[0]=data8[7];
      data37[1]=data8[6];
      data37[2]=data8[5];
      data37[3]=data8[4];
      nfc.PrintHexChar(data36, 4);
      nfc.PrintHexChar(data37, 4);
      data32[0]=pwdgen(uid);
      data43[0]=data8[3];
      data43[1]=data8[2];
      data43[2]=data8[1];
      data43[3]=data8[0];
      
      nfc.PrintHexChar(data43, 4);
      // We probably have an NTAG2xx card (though it could be Ultralight as well)
      Serial.println("Seems to be an NTAG2xx tag (7 byte UID)");	  
      
      // NTAG2x3 cards have 39*4 bytes of user pages (156 user bytes),
      // starting at page 4 ... larger cards just add pages to the end of
      // this range:
      
      // See: http://www.nxp.com/documents/short_data_sheet/NTAG203_SDS.pdf

      // TAG Type       PAGES   USER START    USER STOP
      // --------       -----   ----------    ---------
      // NTAG 203       42      4             39
      // NTAG 213       45      4             39
      // NTAG 215       135     4             129
      // NTAG 216       231     4             225      
     /*
	 Test Block Tardis
	 data36[0]=0x06;
     data36[1]=0x04;
     data36[2]=0;
     data36[3]=0;
	*/
	
	// READ TAG
         for (uint8_t i = 0; i < 42; i++) 
      {
        success = nfc.ntag2xx_ReadPage(i, data);
        
        // Display the current page number
        Serial.print("PAGE ");
        if (i < 10)
        {
          Serial.print("0");
          Serial.print(i);
        }
        else
        {
          Serial.print(i);
        }
        Serial.print(": ");

        // Display the results, depending on 'success'
        if (success) 
        {
          // Dump the page data
          nfc.PrintHexChar(data, 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }
      }      




	//ERASE TAG
	
      for (uint8_t i = 4; i < 39; i++) 
      {
        memset(data, 0, 4);
        success = nfc.ntag2xx_WritePage(i, data);
        
        // Display the current page number
        Serial.print("Page ");
        if (i < 10)
        {
          Serial.print("0");
          Serial.print(i);
        }
        else
        {
          Serial.print(i);
        }
        Serial.print(": ");

        // Display the results, depending on 'success'
        if (success) 
        {
          Serial.println("Erased");
        }
        else
        {
          Serial.println("Unable to write to the requested page!");
        }
      } 

      
	//Write NEW Char
      success =nfc.ntag2xx_WritePage(36,data36);
           if (success) 
        {
          // Dump the page data
          nfc.PrintHexChar(data36, 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }
     // success =nfc.ntag2xx_WritePage(37,data37);
           if (success) 
        {
          // Dump the page data
          nfc.PrintHexChar(data37, 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }


        success =nfc.ntag2xx_WritePage(38,data38);

                 if (success) 
        {
          // Dump the page data
          nfc.PrintHexChar(data38, 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }

        
      success =nfc.ntag2xx_WritePage(43,data43);
           if (success) 
        {
          // Dump the page data
          nfc.PrintHexChar(data43, 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }
 }}
        
    // Wait a bit before trying again
    Serial.println("\n\nSend a character to scan another tag!");
    Serial.flush();
    while (!Serial.available());
    while (Serial.available()) {
    Serial.read()=='+'
    }
    Serial.flush();    
 
}
