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
string folder = "C:\\Users\\t440s\\source\\repos\\BOS3\\10\\13\\";
string string_to_hex(const std::string& input);
void hex2bin(const char* src, char* target);
int task13();
int AesDecrypt(const std::string &sAesKey, const std::string &ToDecrypt, std::string &Decrypted);

char buffer[100];
int main() 
{
	OpenSSL_add_all_algorithms();
	OpenSSL_add_all_ciphers();
	OpenSSL_add_all_digests();
	//task13();
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
	memset(IV,0, 24);

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
	//cout << enc_text << endl;
	//cout << key_string << endl;
	key_string = string_to_hex(key_string);
	//cout << key_string << endl;
	//DES_string_to_key(passwords[i].c_str(), &password_des);
	hex2bin(key_string.c_str(), (char*)key_buf);
	int j = 0;
	unsigned char tmp_key_buf[24];
	for (unsigned char i = 0; i < 0xFF; i++)
	{
		key_buf[23] = i;
		memset(IV, 0, 24);
		memcpy(tmp_key_buf, key_buf, 24);
		//cout << key_buf << endl;
		//string tmp_key = key_string;
		//tmp_key += i;
		//cout << tmp_key << endl;
		j++;
		cout << j << endl;
		AES_set_decrypt_key(tmp_key_buf, 192, &key);
		AES_cbc_encrypt((unsigned char*)enc_text.c_str(), dec_text, enc_text.length(), &key, IV, AES_DECRYPT);
		//AES_cbc_encrypt((unsigned char*)text, dec_text, 50, &key, IV, AES_ENCRYPT);
		//AES_cbc_encrypt(dec_text, dec_text, enc_text.length(), &key, IV, AES_DECRYPT);
		//AES_cbc_encrypt(UserData, dec_text, UserDataSizePadded, &key, IV, AES_DECRYPT);
		//AES_set_decrypt_key(key_buf, 192, &key);
		//AES_decrypt(UserData, dec_text, &key);
		//AesDecrypt(tmp_key, enc_text, dec_text);
		//cout << endl;
		//cout << i << endl;
		cout << dec_text << endl;
		//cout << endl;
		
	}
	return 0;
}
int task13()
{
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
		//DES_string_to_key(passwords[i].c_str(), &password_des);
		hex2bin(passwords[i].c_str(), (char*)password_des);
		//cout << password_des << endl;
		/*DES_random_key(&password_des);*/
		DES_set_odd_parity(&password_des);
		//password_des[7] = 0;
		DES_set_key_checked((C_Block *)password_des, &keysched);

		DES_cbc_encrypt((unsigned char*)enc_text.c_str(), dec_text, enc_text.length(), &keysched, &iv, DES_DECRYPT);
		cout << i << endl;
		cout << dec_text << endl;
		cout << endl;
		//DES_cbc_encrypt((unsigned char*)folder.c_str(), dec_text, enc_text.length(), &keysched, &iv, 1);
		//cout << dec_text << endl;
		//DES_cbc_encrypt(dec_text, dec_text, enc_text.length(), &keysched, &iv, 0);
		//cout << dec_text << endl;


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

int AesDecrypt(const std::string &sAesKey,const std::string &ToDecrypt,std::string &Decrypted) 
{
	const int AesBlockSize = 16;
	unsigned char cInitVector[AesBlockSize];
	EVP_CIPHER_CTX oEncCtx;
	const EVP_CIPHER *oChiper = 0;
	int retval = 0;
	unsigned char *cOutBuffer = 0;

	// Set aes key: 
	if (sAesKey.length() == 16) {
		oChiper = EVP_aes_128_cbc();
	}
	else if (sAesKey.length() == 32) {
		oChiper = EVP_aes_256_cbc();
	}
	else if (sAesKey.length() == 24) {
		oChiper = EVP_aes_192_cbc();
	}
	else {
		return -1; // Wrong key size. 
	}

	// Get init vector: 
	const char *constInitVectorPtr = ToDecrypt.c_str();
	for (int i = 0; i < AesBlockSize; i++) {
		cInitVector[i] = *constInitVectorPtr;
		constInitVectorPtr++;
	}

	// Create decryption context.	
	EVP_CIPHER_CTX_init(&oEncCtx);
	EVP_DecryptInit_ex(&oEncCtx, oChiper, 0, (unsigned char *)sAesKey.c_str(), cInitVector);

	// Decrypt most of the data: 
	int OutBufferByteLen = ToDecrypt.length() - AesBlockSize; // Subtract the InitVec. 
	cOutBuffer = new unsigned char[OutBufferByteLen];
	retval = EVP_DecryptUpdate(
		&oEncCtx,
		cOutBuffer,
		&OutBufferByteLen,
		((unsigned char *)ToDecrypt.c_str()) + AesBlockSize, // Remove the InitVector. 
		OutBufferByteLen);

	// Doese encryption fail? 
	if (retval < 0) {
		delete[] cOutBuffer;
		return retval; // Encryption error. 
	}

	// Add encrypted data to output: 
	Decrypted.append((char *)cOutBuffer, ((char *)cOutBuffer) + OutBufferByteLen);
	delete[] cOutBuffer;

	//Add last block+padding: 
	OutBufferByteLen = 2 * AesBlockSize;
	cOutBuffer = new unsigned char[OutBufferByteLen]; // To be sure add two blocks. 
	retval = EVP_DecryptFinal_ex(
		&oEncCtx,
		cOutBuffer,
		&OutBufferByteLen);

	// Doese encryption fail? 
	if (retval < 0) {
		delete[] cOutBuffer;
		return retval; // Encryption error. 
	}

	// Add encrypted data to output: 
	Decrypted.append((char *)cOutBuffer, ((char *)cOutBuffer) + OutBufferByteLen);
	delete[] cOutBuffer;
	EVP_CIPHER_CTX_cleanup(&oEncCtx);
	//if(OutBufferByteLen)
	cout << OutBufferByteLen << endl;
	return 0; // Success 
}