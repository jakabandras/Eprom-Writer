#include <Arduino.h>
#include <vector>
#include <string>

int receiveFile(String fname);
void readUdp();
std::vector<String> splitString(const String &input, char delimiter);
void writeEprom(String fname);
void sendByte(byte b);
void readEprom(String fname);
void write2Oled(String text);
void write2Oled(String text, int x, int y);
void write2Oled(String text, int x, int y, int size);
void write2Oled(String text, int x, int y, int size, int color);
void write2Oled(String text, int x, int y, int size, int color, int bg);
void write2Oled(String text, int x, int y, int size, int color, int bg, int wrap);
void write2Oled(String text, int x, int y, int size, int color, int bg, int wrap, int font);
void write2Oled(String text, int x, int y, int size, int color, int bg, int wrap, int font, int textsize);
