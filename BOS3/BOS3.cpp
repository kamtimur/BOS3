#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/md4.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator> 
#include <openssl/evp.h> 
#include <sstream>
using namespace std;

char text[] = "C:\\Users\\t440s\\source\\repos\\BOS3\\10\\13\\13data.txt";
char allfiles[] = "C:\\tmp\\file.txt";
string folder = "C:\\Users\\t440s\\source\\repos\\BOS3\\10\\13\\";
string string_to_hex(const std::string& input);
void hex2bin(const char* src, char* target);
int task13();
int task14();

char buffer[100];
int main() 
{
	task13();
	task14();
	return 0;
}

int task14()
{
	ifstream enc_text_file("C:\\Users\\t440s\\source\\repos\\BOS3\\10\\14\\14data.txt.enc", ios::in | ios::binary);
	ifstream key_file("C:\\Users\\t440s\\source\\repos\\BOS3\\10\\14\\14key.key", ios::in | ios::binary);
	if (!enc_text_file) return 1;
	if (!key_file) return 1;
	string temp_string;
	string enc_text;
	//string dec_text;
	string key_string;
	AES_KEY key;
	unsigned char key_buf[24];
	uint8_t IV[24];
	memset(IV, 0, 24);

	unsigned char dec_text[4000];

	while (getline(enc_text_file, temp_string))
	{
		enc_text = enc_text + temp_string;
	}
	const int UserDataSize = (const int)enc_text.length();   // Get the length pre-padding
	int RequiredPadding = (24 - (enc_text.length() % 24));   // Calculate required padding
	std::vector<unsigned char> PaddedTxt(enc_text.begin(), enc_text.end());   // Easier to Pad as a vector
	for (int i = 0; i < RequiredPadding; i++) {
		PaddedTxt.push_back(0); //  Increase the size of the string by
	}                           //  how much padding is necessary

	unsigned char * UserData = &PaddedTxt[0];// Get the padded text as an unsigned char array
	const int UserDataSizePadded = (const int)PaddedTxt.size();// and the length (OpenSSl is a C-API)


	while (getline(key_file, temp_string))
	{
		key_string = key_string + temp_string;
	}

	key_string = string_to_hex(key_string);
	hex2bin(key_string.c_str(), (char*)key_buf);
	int j = 0;
	unsigned char tmp_key_buf[24];
	for (unsigned char i = 0; i < 0xFF; i++)
	{
		key_buf[23] = i;
		memset(IV, 0, 24);
		memcpy(tmp_key_buf, key_buf, 24);
		cout << i << endl;
		j++;
		cout << j << endl;
		AES_set_decrypt_key(key_buf, 192, &key);
		AES_cbc_encrypt(UserData, dec_text, UserDataSizePadded, &key, IV, AES_DECRYPT);
		cout << dec_text << endl;

	}
}
int task13()
{
	const int len = 100;
	const int strings = 100;
	char ch = '\n';
	char chars = '\r';


	string str[1000];
	string passwords[1000];
	DES_cblock password_des;
	DES_key_schedule keysched;
	string temp_string;
	string enc_text;
	unsigned char dec_text[1000];
	string path;

	DES_cblock iv = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };





	ifstream fs(allfiles, ios::in | ios::binary);
	ifstream tmp;
	int length = 0;
	if (!fs) return 1;
	int i = 0;

	while (getline(fs, str[i]))
	{
		str[i].erase(std::remove(str[i].begin(), str[i].end(), chars), str[i].end());
		i++;
	}
	length = i;
	path = folder + str[0];
	tmp.open(path);
	if (tmp)
	{
		while (getline(tmp, temp_string))
		{
			enc_text = enc_text + temp_string;
		}
	}
	tmp.close();
	cout << enc_text << endl;

	for (int i = 1; i < length; i++)
	{
		path = folder + str[i];
		tmp.open(path);

		//cout << path << endl;
		memset(password_des, 0, 8);
		if (!tmp) return 1;
		while (getline(tmp, temp_string))
		{
			passwords[i] = passwords[i] + temp_string;
		}
		passwords[i] = string_to_hex(passwords[i]);
		cout << passwords[i] << endl;

		hex2bin(passwords[i].c_str(), (char*)password_des);
		DES_set_odd_parity(&password_des);
		DES_set_key_checked((C_Block *)password_des, &keysched);

		DES_cbc_encrypt((unsigned char*)enc_text.c_str(), dec_text, enc_text.length(), &keysched, &iv, DES_DECRYPT);
		cout << i << endl;
		cout << dec_text << endl;
		cout << endl;
		tmp.close();
	}
	fs.close();
}
string string_to_hex(const std::string& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}

int char2int(char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	throw std::invalid_argument("Invalid input string");
}

// This function assumes src to be a zero terminated sanitized string with
// an even number of [0-9a-f] characters, and target to be sufficiently large
void hex2bin(const char* src, char* target)
{
	while (*src && src[1])
	{
		*(target++) = char2int(*src) * 16 + char2int(src[1]);
		src += 2;
	}
}
