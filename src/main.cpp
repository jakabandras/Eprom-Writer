#include <Arduino.h>
#include <vector>
#include <string>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPIFFS.h>
#include <WifiClient.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <ClickEncoder.h>
#include <menu.h>
#include <menuIO/SSD1306AsciiOut.h> // include the lcd lib
#include <ClickEncoder.h>
#include <menuIO/clickEncoderIn.h>
#include "main.h"

using namespace Menu;

String ssid = "";
String password = "";
//WiFI Wifi;
WiFiUDP udp;

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if(!SPIFFS.begin(true)){
    Serial.println("Nem sikerült az SPIFFS inicializálása!");
    return;
  }
  Serial.println("SPIFFS inicializálva!");
  File file = SPIFFS.open("/wifi.txt", "r");
  if(!file){
    Serial.println("Nem sikerült megnyitni a wifi.txt fájlt!");
    Serial.println("Alapértelmezett SSID: ESP32");
    ssid = "ESP32";
    Serial.println("Alapértelmezett jelszó: 12345678");
    password = "12345678";
    Serial.println("Wifi.txt fájl létrehozása...");
    file = SPIFFS.open("/wifi.txt", "w");
    if(!file){
      Serial.println("Nem sikerült létrehozni a wifi.txt fájlt!");
      Serial.println("A program leáll!");
      return;
    }
    Serial.println("Wifi.txt fájl létrehozva!");
    file.println(ssid);
    file.println(password);
    file.close();
  }else{
    Serial.println("Wifi.txt fájl megnyitva!");
    ssid = file.readStringUntil('\n');
    password = file.readStringUntil('\n');
    file.close();
  }
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.println("Wifi csatlakozás...");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi csatlakoztatva!");
  Serial.println("IP cím: " + WiFi.localIP().toString());
  udp.begin(1234);
  Serial.println("UDP port nyitva: 1234");

}

void loop() {
  // put your main code here, to run repeatedly:
  readUdp();

}

// put function definitions here:
int receiveFile(String fname) {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.printf("Fogadva: %d bájt\n", packetSize);
    char packetBuffer[packetSize + 1];
    int len = udp.read(packetBuffer, packetSize);
    if (len > 0) {
      packetBuffer[len] = 0;

    }
  }
  return packetSize;
}

void readUdp() {
  int packetSize = udp.parsePacket();
  int len;
  if (packetSize) {
    char packetBuffer[packetSize + 1];
    len = udp.read(packetBuffer, packetSize);
    if (len >0) {
      packetBuffer[len] = 0;
    }
    String msg = packetBuffer;
    std::vector<String> msgParts = splitString(msg, ':');
    const char *cmd = msgParts[0].c_str();
    if (strcmp(cmd, "file") == 0) {
      receiveFile(msgParts[1]);
    } else if (strcmp(cmd, "dir") == 0) {
      File dir = SPIFFS.open("/");
      File file = dir.openNextFile();
      while (file) {
        udp.beginPacket(udp.remoteIP(), udp.remotePort());
        udp.printf("%s\n", file.name());
        udp.endPacket();
        file = dir.openNextFile();
      }
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.printf("end\n");
      udp.endPacket();
    } else if (strcmp(cmd, "del") == 0) {
      SPIFFS.remove(msgParts[1]);
    } else if (strcmp(cmd, "rename") == 0) {
      SPIFFS.rename(msgParts[1].c_str(), msgParts[2].c_str());
    } else if (strcmp(cmd, "write")) {
      writeEprom(msgParts[1]);
    }
  }
}

std::vector<String> splitString(const String &input, char delimiter) {
    std::vector<String> result;
    String token;
    int start = 0;
    int end = input.indexOf(delimiter);
    
    while (end != -1) {
        token = input.substring(start, end);
        result.push_back(token);
        start = end + 1;
        end = input.indexOf(delimiter, start);
    }

    token = input.substring(start);
    result.push_back(token);
    
    return result;
}

void writeEprom(String fname) {
  File file = SPIFFS.open(fname, "r");
  if (!file) {
    Serial.println("Nem sikerült megnyitni a fájlt!");
    return;
  }
  //Eprom írás
  //Ez a rész még nem készült el
}

void sendByte(byte b) {
  //Byte beírása az epromba
}