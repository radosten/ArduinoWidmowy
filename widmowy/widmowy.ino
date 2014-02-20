/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file  
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
     
 */
 
#include <SD.h>
#include "Ai_WS2811.h"

#define NUM_PIXELS 60
#define DATA_PIN 8

Ai_WS2811 ws2811;

File myFile;

struct CRGB {
  unsigned char g;
  unsigned char r;
  unsigned char b;
} *leds;

int led = 13;

void setup()
{
  Serial.begin(9600);
  
  pinMode(led, OUTPUT);
  
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);

  
  ws2811.init(DATA_PIN,NUM_PIXELS);
  leds = (struct CRGB*)ws2811.getRGBData();
  
  
  if (!SD.begin(10)) {
    //Serial.println("initialization failed!");
    return;
  }
  myFile = SD.open("test.txt");
  
  
  if (myFile) {
    
    czysc();
    start();


    int width = myFile.parseInt();
    int height = myFile.parseInt();
    myFile.read();
    
    int r, g, b;
    
     
    for(int x=0; x<width; x++){       
        Serial.println(x);
        
        char bufor2[12*height];
        myFile.read(bufor2, 12*height);
        
        for(int y=0; y<height; y++){
            /* *
            // 43 sekundy
            r = myFile.parseInt();
            g = myFile.parseInt();
            b = myFile.parseInt();
            /* */
            
            /* *
            // 15 sekund
            r = (myFile.read()-'0')*100 + (myFile.read()-'0')*10 + (myFile.read()-'0')*1;
            myFile.read();
            g = (myFile.read()-'0')*100 + (myFile.read()-'0')*10 + (myFile.read()-'0')*1;
            myFile.read();
            b = (myFile.read()-'0')*100 + (myFile.read()-'0')*10 + (myFile.read()-'0')*1;
            myFile.read();
            /* */

            
            /* *
            // 17 sekund
            char bufor[12];
            myFile.readBytes(bufor, 12);
            r = bufor[0]*100+bufor[1]*10+bufor[2] - 111*'0';
            g = bufor[4]*100+bufor[5]*10+bufor[6] - 111*'0';
            b = bufor[8]*100+bufor[9]*10+bufor[10] - 111*'0';
            //r = bufor[0]*100+bufor[1]*10+bufor[2];
            //g = bufor[4]*100+bufor[5]*10+bufor[6];
            //b = bufor[8]*100+bufor[9]*10+bufor[10];
            /* */
            
            /* */
            // 16 sekund
            int offset = 12*y;
            r = bufor2[offset+0]*100+bufor2[offset+1]*10+bufor2[offset+2] - 111*'0';
            g = bufor2[offset+4]*100+bufor2[offset+5]*10+bufor2[offset+6] - 111*'0';
            b = bufor2[offset+8]*100+bufor2[offset+9]*10+bufor2[offset+10] - 111*'0';
            /* */           
            
            
            ustaw(y, r, g, b);
            /* *
            Serial.print(r);
            Serial.print(' ');
            Serial.print(g);
            Serial.print(' ');
            Serial.print(b);
            Serial.println();
            /* */
        }
        ws2811.sendLedData();
        
        //delay(1);
        //Serial.print("\n");
    }
    
    czysc();
    
    
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
  
}

void loop()
{
    // nothing happens after setup
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
}

void czysc()
{
  for(int y=0; y<NUM_PIXELS; y++){
    ustaw(y, 0, 0, 0);
  }
  
  ustaw(0, 128, 128, 128);
    
  ustaw(NUM_PIXELS-1, 128, 128, 128);

  ws2811.sendLedData();
}

void ustaw(int y, int r, int g, int b){
  float scale = 0.2;
  
  leds[y].r = (int)(r*scale);
  leds[y].g = (int)(g*scale);
  leds[y].b = (int)(b*scale);
}

void start(){
  for(int y=5; y>=0; y--) leds[y].r=leds[y].g=leds[y].b = 128;
  for(int y=5; y>=0; y--){
    leds[y].r=leds[y].g=leds[y].b = 0;
    ws2811.sendLedData();
    delay(500);  
  } 
}

