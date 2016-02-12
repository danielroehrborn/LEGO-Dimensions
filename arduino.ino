/**************************************************************************/
/*!
    @file     readntag203.pde
    @author   KTOWN (Adafruit Industries)
    @license  BSD (see license.txt)

    This example will wait for any NTAG203 or NTAG213 card or tag,
    and will attempt to read from it.

    This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
    This library works with the Adafruit NFC breakout
      ----> https://www.adafruit.com/products/364

    Check out the links above for our tutorials and wiring diagrams
    These chips use SPI or I2C to communicate.

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
#include "characters.h"
#include "vehicles_and_gadgets.h"

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
    while (1000); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A Card ...");
}

void loop(void) {
int c;
String charArray;




 int i = 0;
    
    
        delay(1);
        charArray= Serial.readStringUntil('#');     //Read command in array until #
        if (charArray[0]=='r') { //if r# -- read tag
    readtag();
          
        }
        
  if (charArray[0]=='w') { //if w# write to tag
     WriteTag(15, 1);		//where 15 char id from 1 to 45 and 1000 to 1999 for vehicle
  }
  


}
void readtag() {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint16_t tag = 1;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");


    if (uidLength == 7)
    {
      uint8_t  data[4];
      uint8_t datachar[8];
      uint8_t data0[4] = {0x00, 0x00, 0x00, 0x00};
      uint8_t data38[4] = {0x00, 0x01, 0x00, 0x00};
      uint8_t data43[4] = {0x00, 0x00, 0x00, 0x00};
      // We probably have an NTAG2xx card (though it could be Ultralight as well)
      Serial.println("Seems to be an NTAG2xx tag (7 byte UID)");
      
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
    }
  }
}

uint8_t WriteTag(uint16_t charid, boolean backup) {
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint16_t tag = 1;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");


    if (uidLength == 7)
    {
      uint8_t  data[4];
      uint8_t datachar[8];
      uint8_t data0[4] = {0x00, 0x00, 0x00, 0x00};
      uint8_t data38[4] = {0x00, 0x01, 0x00, 0x00};
      uint8_t data43[4] = {0x00, 0x00, 0x00, 0x00};
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
 
      if (backup) {
        for (uint8_t i = 0; i < 45; i++)
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
      }
      for (uint8_t i = 4; i < 39; i++) //Erase tag
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
      pwdgen(uid, data43);		//Gen PWD
      
      if (charid < 0xFF) {
        Serial.println(legoCharacterStr[charid]);
        encryptID(uid, charid, datachar);	//Encrypt Character
        //nfc.PrintHexChar(datachar, 8);
        success = nfc.ntag2xx_WritePage(36, &datachar[0]);
        if (success)
        {
          Serial.print("Page36_:");
          nfc.PrintHexChar(&datachar[0], 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }
        success = nfc.ntag2xx_WritePage(37, &datachar[4]);
        if (success)
        {
          Serial.print("Page37_:");
          nfc.PrintHexChar(&datachar[4], 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }
      }



      if (charid >= 1000) {
        Serial.println(legoVehicleStr[charid - 1000]);
        data0[0] = charid & 0xff;
        data0[1] = (charid >> 8) & 0xff;


        success = nfc.ntag2xx_WritePage(36, data0);
        if (success)
        {
          Serial.print("Page36_Vehicle_:");
          nfc.PrintHexChar(data0, 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }
/*		//test powerup's
        data0[0] = 0x76;
        data0[1] = 0xD4;
        data0[2] = 0x30;
        data0[3] = 0x18;
        success = nfc.ntag2xx_WritePage(35, data0);
        if (success)
        {
          Serial.print("Page35_Vehicle_:");
          nfc.PrintHexChar(data0, 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }
        data0[0] = 0xFF;
        data0[1] = 0x0;
        data0[2] = 0x0;
        data0[3] = 0x0;
        success = nfc.ntag2xx_WritePage(37, data0);
        if (success)
        {
          Serial.print("Page37_Vehicle_:");
          nfc.PrintHexChar(data0, 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }
*/


        success = nfc.ntag2xx_WritePage(38, data38); //00 01 
        if (success)
        {
          Serial.print("Page38_Vehicle_:");
          nfc.PrintHexChar(data38, 4);
        }
        else
        {
          Serial.println("Unable to read the requested page!");
        }
      }

      success = nfc.ntag2xx_WritePage(43, data43);  //write PWD
      if (success)
      {
        Serial.print("Page43_PWD:");
        nfc.PrintHexChar(data43, 4);
      }
      else
      {
        Serial.println("Unable to read the requested page!");
      }
    }

  }

}
