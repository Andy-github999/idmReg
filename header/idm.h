#pragma once
#include <algorithm>
#include <windows.h>
#include <math.h>
#include <string>
#include <iostream>
using namespace std;
std::string intToHexStr(UINT64 num);



string GenerateSerial();

int SnGenerate(unsigned int base, char* serial_number, int serial_size);

string GenerateSerialCipher1(string Serial);

string GenerateSerialCipher2(string Serial);

void setRegistry(string Serial, unsigned long VolumeSerialNumber, string SerialCipher1, string SerialCipher2);

string generationClisd3and4(unsigned long long sum, double v55, double v56);

string getCLSID1(unsigned long VolumeSerialNumber, string clisd, string sn);

string getCLSID2(unsigned long VolumeSerialNumber, string vsn, string sn);

string getCLSID4(unsigned long VolumeSerialNumber);

string getCLSID3(unsigned long VolumeSerialNumber, string sn);

unsigned long getVolumeSerialNumber();
