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
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iterator> 
using namespace std;

char text[] = "C:\\Users\\t440s\\source\\repos\\BOS3\\10\\13\\13data.txt";
string folder = "C:\\Users\\t440s\\source\\repos\\BOS3\\10\\13\\";
string string_to_hex(const std::string& input);
void hex2bin(const char* src, char* target);



char buffer[100];
int main() 
{
	//setlocale(LC_ALL, "en_US.UTF-8");
	const int len = 100;
	const int strings = 100;
	char ch = '\n';
	char chars = '\r';
	char mass[len][strings];


	string str[1000];
	string passwords[1000];
	DES_cblock password_des;
	DES_key_schedule keysched;
	string temp_string;
	string enc_text;
	unsigned char dec_text[1000];
	string path;

	DES_cblock iv = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	//DES_set_odd_parity(&iv);





	ifstream fs("C:\\tmp\\file.txt", ios::in | ios::binary);
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

	for (int i = 58; i < 59; i++) 
	{
		path = folder + str[i];
		tmp.open(path);

		//cout << path << endl;
		memset(password_des,0,8);
		if (!tmp) return 1;
		while (getline(tmp, temp_string))
		{
			passwords[i] = passwords[i] + temp_string;
		}
		passwords[i] = string_to_hex(passwords[i]);
		cout << passwords[i] << endl;
		//DES_string_to_key(passwords[i].c_str(), &password_des);
		hex2bin(passwords[i].c_str(), (char*)password_des);
		//cout << password_des << endl;
		/*DES_random_key(&password_des);*/
		DES_set_odd_parity(&password_des);
		//password_des[7] = 0;
		DES_set_key_checked((C_Block *)password_des, &keysched);

		DES_cbc_encrypt((unsigned char*)enc_text.c_str(), dec_text, enc_text.length(), &keysched, &iv, DES_DECRYPT);
		cout <<i<< endl;
		cout << dec_text << endl;
		cout << endl;
		//DES_cbc_encrypt((unsigned char*)folder.c_str(), dec_text, enc_text.length(), &keysched, &iv, 1);
		//cout << dec_text << endl;
		//DES_cbc_encrypt(dec_text, dec_text, enc_text.length(), &keysched, &iv, 0);
		//cout << dec_text << endl;


		tmp.close();
	}
	fs.close();
	return 0;
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

