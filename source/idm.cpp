#include "idm.h"
#include "rc2.h"
#include "base64.h"
#include <cassert>

std::string intToHexStr(UINT64 num)
{
	char str[30] = { 0 };
	snprintf(str, 30, "%x", num);
	return string(str);
}


void CreatAndSetRegistry(HKEY hKey, LPCSTR lpSubKey, LPCSTR lpValueName, DWORD dwType, CONST BYTE* lpData, DWORD cbData)
{
	HKEY  hkResult = NULL;
	PHKEY phkResult = &hkResult;
	if (!lpValueName)
	{
		lpValueName = "";
	}

	cout << "create registry" << endl;
	cout << lpSubKey << lpValueName << endl;
	if (RegCreateKeyA(hKey, lpSubKey, phkResult) != ERROR_SUCCESS) {
		cout << "failed" << endl;
	}
	else {
		cout << "success" << endl;
	}
	cout << "-------------" << endl;

	//上面尝试创建注册表键

	if (phkResult)
	{
		if (RegSetValueExA(*phkResult, lpValueName, 0, dwType, lpData, cbData) != ERROR_SUCCESS) {
			cout << "Unable to set ";
			cout << lpSubKey << lpValueName << endl;
		}
		else {
			cout << "set registry" << endl;
			cout << lpSubKey << lpValueName << endl;
			cout << "success" << endl;
		}
		cout << "-------------" << endl;
		if (NULL != phkResult) {
			RegCloseKey(*phkResult);
			cout << "close registry" << endl;
			cout << lpSubKey << lpValueName << endl;
			cout << "success" << endl;
			cout << "-------------" << endl;
		}
	}
}

string GenerateSerial()
{
	string Serial = "";
	UINT number = 43 * 23 * 17 * 53;
	char register_sn_0_5[6] = { 0 };

	for (int i = 0; i < 4; i++)
	{
		char register_sn_0_5[6] = { 0 };
		SnGenerate(number, register_sn_0_5, 6);//43
		Serial += register_sn_0_5;
		if (i != 3)
		{
			Serial += "-";
		}
	}
	return Serial;
}
const char Codebyte[] = { 0x32, 0x59, 0x4F, 0x50, 0x42, 0x33, 0x41, 0x51, 0x43, 0x56, 0x55, 0x58, 0x4D, 0x4E, 0x52, 0x53,0x39, 0x37, 0x57, 0x45, 0x30, 0x49, 0x5A, 0x44, 0x34, 0x4B, 0x4C, 0x46, 0x47, 0x48, 0x4A, 0x38,0x31, 0x36, 0x35, 0x54 };

int SnGenerate(unsigned int base, char* serial_number, int  serial_size)
{
	if (serial_size <= 5)  return 0;

	for (int i = 4; i >= 0; i--) {
		for (int encode_count = 0; encode_count <= 35; encode_count++) {
			if ((base - encode_count) % 37 == 0) {
				base = (base - encode_count) / 37;
				serial_number[i] = Codebyte[encode_count];
				break;
			}

			if (encode_count == 35) return 0;
		}

	}
	return 1;
}


string GenerateSerialCipher1(string Serial) 
{
	unsigned short xkey[64] = {};
	const  char* key = "58BE20ast4si5ls2D13";
	const  char* plain = Serial.data();
	unsigned char cipher[0xff] = {};
	rc2_keygen(xkey, (unsigned char*)key, strlen(key), 1024);
	char out[0xff] = {};
	string SerialCipher = "";
	for (int i = 0; i < strlen(plain); i += 8)
	{

		rc2_encrypt(xkey, (unsigned char*)(plain + i), cipher);
		SerialCipher += (char*)cipher;
	}
	return SerialCipher;
}

string GenerateSerialCipher2(string Serial)
{
	unsigned short xkey[64] = {};
	const  char* key = "lmW02kjM4002FAnzwwsA1";
	const  char* plain = Serial.data();
	unsigned char cipher[0xff] = {};
	rc2_keygen(xkey, (unsigned char*)key, strlen(key), 1024);
	char out[0xff] = {};
	string SerialCipher = "";
	for (int i = 0; i < strlen(plain); i += 8)
	{
		rc2_encrypt(xkey, (unsigned char*)(plain + i), cipher);
		SerialCipher += (char*)cipher;
	}
	char* data = (char*)SerialCipher.data(); // your initialization here
	int data_length = SerialCipher.length(); // your data length here
	int encoded_data_length = Base64encode_len(data_length);
	string base64_string = string(encoded_data_length,0);

	Base64encode((char*)base64_string.data(), data, data_length);
	// "base64_string" is a null terminated string that is an encoding of the
	// binary data pointed to by "data"
	return base64_string;
}



void setRegistry(string Serial, unsigned long VolumeSerialNumber, string SerialCipher1, string SerialCipher2)
{

	string vsn = intToHexStr(VolumeSerialNumber);
	string sn = Serial.substr(0,5);

	string CLSID1 = getCLSID1(VolumeSerialNumber, vsn, sn);
	string CLSID2 = getCLSID2(VolumeSerialNumber, vsn, sn);
	string CLSID3 = getCLSID3(VolumeSerialNumber, Serial.substr(0, 8));
	string CLSID4 = getCLSID4(VolumeSerialNumber);


	char FName[25] = "1";
	char LName[25] = "1";
	char Email[25] = "1";
	CreatAndSetRegistry(HKEY_CURRENT_USER, R"(SOFTWARE\DownloadManager\)", "FName", REG_SZ, (const BYTE*)FName, sizeof(FName) / sizeof(FName[0]));
	CreatAndSetRegistry(HKEY_CURRENT_USER, R"(SOFTWARE\DownloadManager\)", "LName", REG_SZ, (const BYTE*)LName, sizeof(LName) / sizeof(LName[0]));
	CreatAndSetRegistry(HKEY_CURRENT_USER, R"(SOFTWARE\DownloadManager\)", "Email", REG_SZ, (const BYTE*)Email, sizeof(Email) / sizeof(Email[0]));
	CreatAndSetRegistry(HKEY_CURRENT_USER, R"(SOFTWARE\DownloadManager\)", "Serial", REG_SZ, (const BYTE*)Serial.data(), Serial.length());

	//CreatAndSetRegistry(HKEY_CURRENT_USER, R"(SOFTWARE\Classes\WOW6432Node\CLSID\{07999AC3-058B-40BF-984F-69EB1E554CA7}\)", "MData", REG_BINARY, (unsigned char *)SerialCipher1.data(),SerialCipher1.length());
	CreatAndSetRegistry(HKEY_CURRENT_USER, R"(SOFTWARE\Classes\WOW6432Node\CLSID\{5ED60779-4DE2-4E07-B862-974CA4FF2E9C}\)", "MData", REG_BINARY, (unsigned char *)SerialCipher1.data(),SerialCipher1.length());
	CreatAndSetRegistry(HKEY_CURRENT_USER, R"(SOFTWARE\Classes\WOW6432Node\CLSID\{79873CC5-3951-43ED-BDF9-D8759474B6FD}\)", "MData", REG_BINARY, (unsigned char*)SerialCipher1.data(), SerialCipher1.length());

	string Data_crack = SerialCipher2;
	string registrySubKey = R"(SOFTWARE\Classes\WOW6432Node\CLSID\)";
	string registrySubKey1 = registrySubKey + "{" + CLSID1 + "}";
	CreatAndSetRegistry(HKEY_CURRENT_USER, registrySubKey1.data(), NULL, REG_SZ, (BYTE*)Data_crack.data(), Data_crack.length());

	string registrySubKey2 = registrySubKey + "{" + CLSID2 + "}";
	CreatAndSetRegistry(HKEY_CURRENT_USER, registrySubKey2.data(), NULL, REG_SZ, (BYTE*)Data_crack.data(), Data_crack.length());

	string registrySubKey3 = registrySubKey + "{" + CLSID3 + "}";
	CreatAndSetRegistry(HKEY_CURRENT_USER, registrySubKey3.data(), NULL, REG_SZ, (BYTE*)Data_crack.data(), Data_crack.length());
	string registrySubKey4 = registrySubKey + "{" + CLSID4 + "}";
	CreatAndSetRegistry(HKEY_CURRENT_USER, registrySubKey4.data(), NULL, REG_SZ, (BYTE*)Data_crack.data(), Data_crack.length());
}

string generationClisd3and4(unsigned long long sum, double v55, double v56)
{
	string clisd = "";
	double tmp = 0;
	while (clisd.length() < 0x20) {
		char hex_v3[20] = { 0 };
		char hex_a[20] = { 0 };
		char hex_b[20] = { 0 };

		if (v55 > double(3.70)) {
			tmp = (double)sum * v55;
			v55 += 2.75;
		}
		else
		{
			tmp = (double)sum * v56;
		}

		float a1 = (float)tmp; //esp+10
		unsigned int v1;//esp+18
		memcpy(&v1, &a1, sizeof(v1));
		unsigned long long  v6 = v1; //edx

		double tmp2 = round(tmp);
		unsigned long  v5 = (unsigned long long)tmp2 & (unsigned)0xFFFFFFFF;//double转long eax
		unsigned int v3 = (unsigned long long) tmp2 / (unsigned long long) 0x100000000;//esp+14
	

		unsigned int v7 = 0; // ecx

		if (v5 || v3 != (unsigned)0x7FFFFFFF)
		{
			double tmp10 = tmp - round(tmp);
			float tmp9 = float(tmp10);
			unsigned int flag = 0;
			memcpy(&flag, &tmp9, sizeof(v1));
			v7 = flag;

			if (v6 < 0)
			{
				v7 ^= (unsigned)0x80000000;
				if ((unsigned)0xFFFFFFFF - v7 < 0x7FFFFFFF)
				{
					v5 += 1;
				}
			}
			else
			{
				if ((unsigned)0xFFFFFFFF - v7 < 0x7FFFFFFF)
				{
					v5 -= 1;
				}
			}
		}

		clisd += intToHexStr(v3);

		clisd += intToHexStr(v5);
		sum = tmp;
	}

	clisd = clisd.substr(0, 32);
	clisd.insert(8, "-");
	clisd.insert(13, "-");
	clisd.insert(18, "-");
	clisd.insert(23, "-");
	return clisd;
}

string getCLSID1(unsigned long VolumeSerialNumber, string clisd, string sn) {
	unsigned long sum = 1;
	for (int i = 0; i <= 4; i++) {
		sum = 7 * sum + 2 * sn[i];
	}
	sum += VolumeSerialNumber;

	double v5 = 1.33;

	transform(clisd.begin(), clisd.end(), clisd.begin(), ::tolower);
	while (clisd.length() < 0x20) {
		UINT32 t = (UINT64)floor(sum * v5);
		v5 += 0.27;
		clisd += intToHexStr(t);
	}
	clisd = clisd.substr(0, 32);
	reverse(clisd.begin(), clisd.end());
	clisd.insert(8, "-");
	clisd.insert(13, "-");
	clisd.insert(18, "-");
	clisd.insert(23, "-");
	return clisd;
}

string getCLSID2(unsigned long VolumeSerialNumber, string vsn, string sn) {
	unsigned long  sum = 1;
	for (int i = 0; i <= 4; i++) {
		sum = 5 * sum + 2 * sn[i];
	}
	sum += VolumeSerialNumber;
	sum /= 3;
	double v5 = 1.55;
	string clisd = "";

	double tmp = 0;
	while (clisd.length() < 0x20) {
		sum = (UINT64)floor(sum * v5);
		v5 += 0.25;
		clisd += intToHexStr(sum);
	}
	clisd = clisd.substr(0, 32);
	clisd.insert(8, "-");
	clisd.insert(13, "-");
	clisd.insert(18, "-");
	clisd.insert(23, "-");
	return clisd;
}

string getCLSID3(unsigned long VolumeSerialNumber, string sn) {
	unsigned long long sum = 1;
	for (int i = 0; i <= 7; ++i) {
		sum = sum * 0xA + sn[i];
	}
	sum += VolumeSerialNumber;
	double v56 = 0.7+2.75;
	string clisd = generationClisd3and4(sum, 0, v56);
	return clisd;
}

string getCLSID4(unsigned long VolumeSerialNumber) {
	int i = 0;
	unsigned long long sum = 1;
	unsigned long long constnum = 43;
	sum = constnum * (unsigned long long)(VolumeSerialNumber >> 1);
	sum = *(unsigned long*)&sum;

	double v55 = 3.77;
	string clisd = generationClisd3and4(sum, v55, 0);
	return clisd;
}

unsigned long getVolumeSerialNumber() {
	DWORD   VolumeSerialNumber;
	char   VolumeName[20];
	GetVolumeInformationA("c:\\ ", VolumeName, 12, &VolumeSerialNumber, NULL, NULL, NULL, 10);
	return  VolumeSerialNumber;
}