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
  
  migaj(1,500);
  
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
        //Serial.println(x);
        
        char bufor2[12*height];
        myFile.read(bufor2, 12*height);
        
        for(int y=0; y<height; y++){

            int offset = 12*y;
            r = bufor2[offset+0]*100+bufor2[offset+1]*10+bufor2[offset+2] - 111*'0';
            g = bufor2[offset+4]*100+bufor2[offset+5]*10+bufor2[offset+6] - 111*'0';
            b = bufor2[offset+8]*100+bufor2[offset+9]*10+bufor2[offset+10] - 111*'0';
            
            ustaw(y, r, g, b);

        }
        ws2811.sendLedData();
        
        delay(10);
        //Serial.print("\n");
    }
    
    czysc();
    
    myFile.close();

  } else {
    // blad pliku
  }
  

}


void loop()
{

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


void migaj(int ile, int time){
  for(int i=0; i<ile; i++){  
    digitalWrite(led, HIGH);
    delay(time);
    digitalWrite(led, LOW);
    if(i != ile-1) delay(time);
  }
}
