#include <Arduino.h>
#include <vector>
#include <string>

int receiveFile(String fname);
void readUdp();
std::vector<String> splitString(const String &input, char delimiter);
void writeEprom(String fname);
void sendByte(byte b);