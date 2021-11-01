#include "idm.h"
int main(int argc, char** argv)
{
	unsigned long VolumeSerialNumber = getVolumeSerialNumber();
	string Serial= GenerateSerial();
	string SerialCipher1 = GenerateSerialCipher1(Serial);
	string SerialCipher2 = GenerateSerialCipher2(Serial);

	setRegistry(Serial, VolumeSerialNumber, SerialCipher1, SerialCipher2);

	system("pause");
	return 0;
}