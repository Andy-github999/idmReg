#pragma once
void rc2_decrypt(unsigned short xkey[64], unsigned char* plain, unsigned char* cipher);

void rc2_encrypt(unsigned short xkey[64], unsigned char* plain, unsigned char* cipher);

void rc2_keygen(unsigned short xkey[64], const unsigned char* key, unsigned len, unsigned bits);