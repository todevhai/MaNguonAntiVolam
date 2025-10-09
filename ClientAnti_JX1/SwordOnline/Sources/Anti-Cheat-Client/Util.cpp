#include "stdafx.h"
#include "Util.h"
#include "Anti-Cheat-Client.h"
#include "HackServerProtocol.h"
#include "Log.h"
#include "SplashScreen.h"
#include "resource.h"
#include "Protect.h"
//Modules
#include "SimpleModules.h"
#include "ThreadCheck.h"
#include <Iphlpapi.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <fstream>
#include <base64.cpp>
#include "Authenticate.h"
#include <TlHelp32.h>
#include <Wininet.h>
#include "SkillState.h"
#pragma comment(lib, "wininet.lib")

using namespace std;
//#include <io.h>
//#include <fcntl.h>
//#include <stdio.h>
//#include <string.h>

#pragma comment(lib,"iphlpapi.lib")

BYTE NewAddressData1[240];
BYTE NewAddressData2[240];
BYTE NewAddressData3[6000];

void handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

CUtil gUtil;

CUtil::CUtil()
{
	IsInGame = false;
	m_PlayerIndex = 0;
	InitThreadState = false;
	IsActivePlayerNpc = false;
	IsActiveMissleSame = false;
	IsActiveVisiblePlayer = false;
}

CUtil::~CUtil()
{
}

void CUtil::SendLoginInfo()
{
	InitUiState = true;
	IsInGame = true;
	gProtect.m_IsInGame = true;
	gUtil.IsActivePlayerNpc = false;
	gUtil.IsActiveMissleSame = false;
	gUtil.IsActiveVisiblePlayer = false;

	char* buffURL = (char*)malloc(256);
	char* RoleName = (char*)malloc(32);

	unsigned int PlayerOffset1;
	unsigned int PlayerOffset2;
	unsigned int PlayerOffset3;
	unsigned int PlayerOffset4;
	unsigned int PlayerOffset5;

	ReadProcessMemory(hProcess, (void*)(unsigned int)(KPlayer_Offset1 + 0x8C44), &PlayerOffset1, sizeof(PlayerOffset1), NULL);

	PlayerOffset2 = (PlayerOffset1 * 0x113B8);

	ReadProcessMemory(hProcess, (void*)(unsigned int)(0x12C5540), &PlayerOffset3, sizeof(PlayerOffset3), NULL);

	PlayerOffset4 = PlayerOffset3 + PlayerOffset2;

	PlayerOffset5 = PlayerOffset4 + 0x130D;
	strcpy(RoleName, (char*)(PlayerOffset5));
	sprintf(buffURL, "http://auth.volam1pk.net/login-%s-%s-%s-%s.nnb", c_AccountName, RoleName, c_ClientSerial, c_HardwareId);
	std::string authurl = buffURL;
	std::string dataread = DownloadString(authurl);
	free(buffURL);
	free(RoleName);
}


char* randomGet(int num)
{
	srand(time(NULL));
	static const char ab[] = "0123456789ABCDEFGHIGKLMNOPQRSTUVWXYZ"; //Alphabet&Digit
	char* targ = new char[num + 1];
	for (int i = 0; i < num; ++i) {
		targ[i] = ab[rand() % 36];
	}
	targ[num] = '\0';
	return targ;
}

void cleanup(cipher_params_t* params, FILE* ifp, FILE* ofp, int rc) {
	free(params);
	fclose(ifp);
	fclose(ofp);
	exit(rc);
}

static std::string* OpenFile() {

	static string myArray[3];

	ifstream myfile("fwcld.temp");
	if (myfile.is_open())
	{
		for (int i = 0; i < 3; ++i)
		{
			// line 1 = text to decrypt
			// line 2 = key
			// line 3 = IV
			//myfile >> myArray[i];
		}
		myfile.close();
	}

	return myArray;
}

unsigned char* DecryptFileProtect(char* FileIn) {

	FILE* f_input, * f_enc, * f_dec;

	int cipher_block_size = EVP_CIPHER_block_size(EVP_aes_256_cbc());

	f_input = fopen(FileIn, "rb");
	if (!f_input) {
		/* Unable to open file for reading */
		fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
		//return errno;
	}

	fseek(f_input, 0, SEEK_END);
	size_t len = ftell(f_input);
	fseek(f_input, 0, SEEK_SET);

	unsigned char* out_buf = (unsigned char*)malloc(len + cipher_block_size);
	unsigned char* in_buf = (unsigned char*)malloc(len);

	int num_bytes_read, out_len;

	cipher_params_t* params = (cipher_params_t*)malloc(sizeof(cipher_params_t));

	if (!params) {
		/* Unable to allocate memory on heap*/
		fprintf(stderr, "ERROR: malloc error: %s\n", strerror(errno));
		//return errno;
	}

	unsigned char key[AES_256_KEY_SIZE];
	unsigned char iv[AES_BLOCK_SIZE];

	if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
		/* OpenSSL reports a failure, act accordingly */
		fprintf(stderr, "ERROR: RAND_bytes error: %s\n", strerror(errno));
		//return errno;
	}

	params->key = (unsigned char*)"44da4@f559cfd628586$894@e86f7e2$eb09";
	params->iv = (unsigned char*)"98@74a$7c7@f9a4$4da9";
	params->encrypt = 0;
	params->cipher_type = EVP_aes_256_cbc();

	EVP_CIPHER_CTX* ctx;

	ctx = EVP_CIPHER_CTX_new();

	if (ctx == NULL)
		handleErrors();

	if (!EVP_CipherInit_ex(ctx, params->cipher_type, NULL, NULL, NULL, params->encrypt))
		handleErrors();

	OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == AES_256_KEY_SIZE);
	OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == AES_BLOCK_SIZE);

	if (!EVP_CipherInit_ex(ctx, NULL, NULL, params->key, params->iv, params->encrypt))
		handleErrors();

	while (1) {

		num_bytes_read = fread(in_buf, sizeof(unsigned char), len, f_input);

		if (!EVP_CipherUpdate(ctx, out_buf, &out_len, in_buf, num_bytes_read))
			handleErrors();
		//fwrite(out_buf, sizeof(unsigned char), out_len, f_dec);

		if (num_bytes_read < len)
			break;
	}

	if (!EVP_CipherFinal_ex(ctx, out_buf + len, &out_len))
		handleErrors();

	//fwrite(out_buf, sizeof(unsigned char), out_len, f_dec);
	fclose(f_input);
	//fclose(f_dec);

	EVP_CIPHER_CTX_cleanup(ctx);

	return out_buf;
}

void file_encrypt_decrypt(cipher_params_t* params, FILE* ifp, FILE* ofp, bool write) {
	FILE* file;
	FILE* stream;
	//int BUFSIZE = 4096;
	//const int BUF_SIZE = 4096;
	/* Allow enough space in output buffer for additional block */
	int cipher_block_size = EVP_CIPHER_block_size(params->cipher_type);
	//*(int*)BUFSIZE = (BUFSIZE + cipher_block_size);
	//unsigned char in_buf[BUFSIZE], out_buf[4096 + cipher_block_size];
	unsigned char* out_buf = new unsigned char[BUFSIZE + cipher_block_size];
	unsigned char in_buf[BUFSIZE];
	unsigned char* buff = new unsigned char[BUFSIZE + cipher_block_size];
	DWORD dwBytesRead, dwBytesWritten;
	//HANDLE target;
	DWORD dwBytesToWrite = sizeof(unsigned char);
	//target = CreateFile("fwcld.bak", FILE_WRITE_DATA, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, 0);
	int num_bytes_read, out_len;

	EVP_CIPHER_CTX* ctx;

	ctx = EVP_CIPHER_CTX_new();
	if (ctx == NULL) {
		fprintf(stderr, "ERROR: EVP_CIPHER_CTX_new failed. OpenSSL error: %s\n", ERR_error_string(ERR_get_error(), NULL));
		cleanup(params, ifp, ofp, ERR_EVP_CTX_NEW);
	}

	/* Don't set key or IV right away; we want to check lengths */
	if (!EVP_CipherInit_ex(ctx, params->cipher_type, NULL, NULL, NULL, params->encrypt)) {
		fprintf(stderr, "ERROR: EVP_CipherInit_ex failed. OpenSSL error: %s\n", ERR_error_string(ERR_get_error(), NULL));
		cleanup(params, ifp, ofp, ERR_EVP_CIPHER_INIT);
	}

	OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == AES_256_KEY_SIZE);
	OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == AES_BLOCK_SIZE);

	/* Now we can set key and IV EVP_aes_256_cbc() */
	if (!EVP_CipherInit_ex(ctx, NULL, NULL, params->key, params->iv, params->encrypt)) {
		fprintf(stderr, "ERROR: EVP_CipherInit_ex failed. OpenSSL error: %s\n", ERR_error_string(ERR_get_error(), NULL));
		EVP_CIPHER_CTX_cleanup(ctx);
		cleanup(params, ifp, ofp, ERR_EVP_CIPHER_INIT);
	}

	while (1) {
		// Read in data in blocks until EOF. Update the ciphering with each read.
		num_bytes_read = fread(in_buf, sizeof(unsigned char), BUFSIZE, ifp);
		if (ferror(ifp)) {
			fprintf(stderr, "ERROR: fread error: %s\n", strerror(errno));
			EVP_CIPHER_CTX_cleanup(ctx);
			cleanup(params, ifp, ofp, errno);
		}
		if (!EVP_CipherUpdate(ctx, out_buf, &out_len, in_buf, num_bytes_read)) {
			fprintf(stderr, "ERROR: EVP_CipherUpdate failed. OpenSSL error: %s\n", ERR_error_string(ERR_get_error(), NULL));
			EVP_CIPHER_CTX_cleanup(ctx);
			cleanup(params, ifp, ofp, ERR_EVP_CIPHER_UPDATE);
		}
		//memcpy(&out_buf, &buff, sizeof(out_buf));
		if (write == true)
		{
			//WriteFile(target, out_buf, (DWORD)out_len, &dwBytesToWrite, NULL);
			fwrite(out_buf, sizeof(unsigned char), out_len, ofp);
			/*char datarequest[200];
			sprintf(datarequest, "data 2 %d %d", sizeof(out_buf), out_len);
			MessageBox(0, datarequest, "Info", MB_OK | MB_ICONINFORMATION);*/
			if (ferror(ofp)) {
				fprintf(stderr, "ERROR: fwrite error: %s\n", strerror(errno));
				EVP_CIPHER_CTX_cleanup(ctx);
				cleanup(params, ifp, ofp, errno);
			}
		}
		else
		{
			//memcpy(&gProtect.m_MainInfo, &out_buf, sizeof(MAIN_FILE_INFO));
			//gProtect.m_MainInfo_OutSize = out_len;
			//WriteFile(&gProtect.ofp, out_buf, sizeof(unsigned char), (DWORD*)&out_len, NULL);
			//fwrite(out_buf, sizeof(unsigned char), out_len, ofp);
			if (ferror(ofp)) {
				fprintf(stderr, "ERROR: fwrite error: %s\n", strerror(errno));
				EVP_CIPHER_CTX_cleanup(ctx);
				cleanup(params, ifp, ofp, errno);
			}
		}

		if (num_bytes_read < BUFSIZE) {
			//char datarequest[200];
			//sprintf(datarequest, "data 1 %d %d %d", sizeof(out_buf), sizeof(gProtect.m_file), out_len);
			//MessageBox(0, datarequest, "Info", MB_OK | MB_ICONINFORMATION);
			//gProtect.m_MainInfo_OutSize = 0;
			break;
		}
	}

	/* Now cipher the final block and write it out to ofp */
	if (!EVP_CipherFinal_ex(ctx, out_buf, &out_len)) {
		fprintf(stderr, "ERROR: EVP_CipherFinal_ex failed. OpenSSL error: %s\n", ERR_error_string(ERR_get_error(), NULL));
		EVP_CIPHER_CTX_cleanup(ctx);
		cleanup(params, ifp, ofp, ERR_EVP_CIPHER_FINAL);
	}

	if (write == true)
	{
		//WriteFile(target, out_buf, (DWORD)out_len, &dwBytesToWrite, NULL);
		//gProtect.m_MainInfo_OutSize = dwBytesToWrite;
		fwrite(out_buf, sizeof(unsigned char), out_len, ofp);
		//memcpy(&gProtect.m_MainInfo, out_buf, out_len);
		//char datarequest[200];
		//sprintf(datarequest, "data 1 %d %d", sizeof(out_buf), out_len);
		//MessageBox(0, datarequest, "Info", MB_OK | MB_ICONINFORMATION);
		if (ferror(ofp)) {
			fprintf(stderr, "ERROR: fwrite error: %s\n", strerror(errno));
			EVP_CIPHER_CTX_cleanup(ctx);
			cleanup(params, ifp, ofp, errno);
		}
	}
	else
	{
		//(*&maininfo) = out_buf;
		//(*OutSize) = out_len;
		//memcpy(&gProtect.m_MainInfo, &out_buf, sizeof(MAIN_FILE_INFO));
		//gProtect.m_MainInfo_OutSize = out_len;
		//WriteFile(&gProtect.m_file, out_buf, sizeof(unsigned char), (DWORD*)&out_len, NULL);
		//fwrite(out_buf, sizeof(unsigned char), out_len, ofp);
		/*if (ferror(ofp)) {
			fprintf(stderr, "ERROR: fwrite error: %s\n", strerror(errno));
			EVP_CIPHER_CTX_cleanup(ctx);
			cleanup(params, ifp, ofp, errno);
		}*/
	}

	//THdr gProtect.(data);
	//MAIN_FILE_INFO struct(out_buf);

	EVP_CIPHER_CTX_cleanup(ctx);


	//if (target == INVALID_HANDLE_VALUE) {
	//	MessageBox(0, "INVALID_HANDLE_VALUE", "Info", MB_OK | MB_ICONINFORMATION);
	//	exit(EXIT_FAILURE);
	//}

	/*if (GetFileSize(target, 0) != sizeof(MAIN_FILE_INFO))
	{
		MessageBox(0, "GetFileSize 1", "Info", MB_OK | MB_ICONINFORMATION);
	}*/

	//if (sizeof(gProtect.m_MainInfo) != sizeof(MAIN_FILE_INFO))
	//{
	//	MessageBox(0, "GetFileSize 1", "Info", MB_OK | MB_ICONINFORMATION);
	//}

	//char datarequest[200];
	//sprintf(datarequest, "data 2 %d %d  %d %d %d %d", GetFileSize(target, 0), sizeof(out_buf), out_len, sizeof(gProtect.m_MainInfo), sizeof(MAIN_FILE_INFO), gProtect.m_MainInfo_OutSize);
	//MessageBox(0, datarequest, "Info", MB_OK | MB_ICONINFORMATION);

	/*if (ReadFile(target, &gProtect.m_MainInfo, sizeof(MAIN_FILE_INFO), &gProtect.m_MainInfo_OutSize, 0) == 0)
	{
		MessageBox(0, "ReadFile 1", "Info", MB_OK | MB_ICONINFORMATION);
	}*/

	

	/*for (int n = 0; n < sizeof(MAIN_FILE_INFO); n++)
	{
		((BYTE*)&gProtect.m_MainInfo)[n] += 0x78;
		((BYTE*)&gProtect.m_MainInfo)[n] ^= 0xB3;
	}

	gMessage.GetLanguage();

	gMessage.LoadEng(gProtect.m_MainInfo.EngMessageInfo);

	gMessage.LoadPor(gProtect.m_MainInfo.PorMessageInfo);

	gMessage.LoadSpn(gProtect.m_MainInfo.SpnMessageInfo);

	CloseHandle(target);*/

	//MessageBox(0, "Pass", "Info", MB_OK | MB_ICONINFORMATION);

}


bool mahoafile(char* FileIn, char* FileOut, bool write) {
	FILE* f_input, * f_enc, * f_dec;

	cipher_params_t* params = (cipher_params_t*)malloc(sizeof(cipher_params_t));

	if (!params) {
		/* Unable to allocate memory on heap*/
		fprintf(stderr, "ERROR: malloc error: %s\n", strerror(errno));
		return false;
	}

	unsigned char key[AES_256_KEY_SIZE];
	unsigned char iv[AES_BLOCK_SIZE];

	if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
		/* OpenSSL reports a failure, act accordingly */
		fprintf(stderr, "ERROR: RAND_bytes error: %s\n", strerror(errno));
		return false;
	}

	params->key = (unsigned char*)"44da4@f559cfd628586$894@e86f7e2$eb09";
	params->iv = (unsigned char*)"98@74a$7c7@f9a4$4da9";

	params->encrypt = 1;

	params->cipher_type = EVP_aes_256_cbc();

	f_input = fopen(FileIn, "rb");
	if (!f_input) {
		fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
		return false;
	}

	f_enc = fopen(FileOut, "wb");
	if (!f_enc) {
		fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
		return false;
	}

	file_encrypt_decrypt(params, f_input, f_enc, write);

	fclose(f_input);
	fclose(f_enc);

	return true;
}

bool giaimafile(char* FileIn, char* FileOut, bool write) {

	FILE* f_input, * f_enc, * f_dec;

	cipher_params_t* params = (cipher_params_t*)malloc(sizeof(cipher_params_t));

	if (!params) {
		/* Unable to allocate memory on heap*/
		fprintf(stderr, "ERROR: malloc error: %s\n", strerror(errno));
		return false;
	}

	unsigned char key[AES_256_KEY_SIZE];
	unsigned char iv[AES_BLOCK_SIZE];

	if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
		/* OpenSSL reports a failure, act accordingly */
		fprintf(stderr, "ERROR: RAND_bytes error: %s\n", strerror(errno));
		return false;
	}
	params->key = (unsigned char*)"44da4@f559cfd628586$894@e86f7e2$eb09";
	params->iv = (unsigned char*)"98@74a$7c7@f9a4$4da9";
	params->encrypt = 0;
	params->cipher_type = EVP_aes_256_cbc();

	f_input = fopen(FileIn, "rb");
	if (!f_input) {
		/* Unable to open file for reading */
		fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
		return false;
	}

	//ifstream inEncryptedFile;
	//inEncryptedFile.open("jsonOut.crypto");


	//ifstream file("fwcld.sys", ios::in | ios::binary | ios::ate);

	//file.seekg(0, ios::end);
	//int length = file.tellg();//length of the text in the file 
	//file.seekg(0);

	//char* memblock = new char[length];
	//file.read(memblock, length);

	//ifstream inEncryptedFile;
	//inEncryptedFile.open("jsonOut.crypto");

	//std::ifstream file(FileOut);

	//while (std::getline(file, str))
	//{
	//	std::vector<unsigned char> encrypted_str(str.size()); // you may need some more bytes
	//	encrypt(reinterpret_cast<unsigned char*>(str.data()), str.size(), key, iv, &encrypted_str[0]);
	//}

	f_dec = fopen(FileOut, "wb");
	if (!f_dec) {
		/* Unable to open file for writing */
		fprintf(stderr, "ERROR: fopen error: %s\n", strerror(errno));
		return false;
	}

	file_encrypt_decrypt(params, f_input, f_dec, write);

	fclose(f_input);
	fclose(f_dec);

	//free(params);

	return true;
}

int EncryptStringProtect(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext)
{
	EVP_CIPHER_CTX* ctx;

	int len;

	int ciphertext_len;

	unsigned char* key = (unsigned char*)"44da4@f559cfd628586$894@e86f7e2$eb09";
	//std::cout << key << "\n";
	unsigned char* iv = (unsigned char*)"98@74a$7c7@f9a4$4da9";

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleErrors();

	/*
	 * Initialise the encryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		handleErrors();

	/*
	 * Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		handleErrors();
	ciphertext_len = len;

	/*
	 * Finalise the encryption. Further ciphertext bytes may be written at
	 * this stage.
	 */
	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
		handleErrors();
	ciphertext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int DecryptStringProtect(unsigned char* ciphertext, int ciphertext_len, unsigned char* plaintext)
{
	EVP_CIPHER_CTX* ctx;

	int len;

	int plaintext_len;

	unsigned char* key = (unsigned char*)"44da4@f559cfd628586$894@e86f7e2$eb09";
	//std::cout << key << "\n";
	unsigned char* iv = (unsigned char*)"98@74a$7c7@f9a4$4da9";

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
		handleErrors();

	/*
	 * Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		handleErrors();

	/*
	 * Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary.
	 */
	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		handleErrors();
	plaintext_len = len;

	/*
	 * Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
		handleErrors();
	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}

void SafeExitProcess()
{
	if(API_SCAN() == 0)
	{
		while(true)
		{
			memset(gThreadCheck.m_CheckThreadID,0,sizeof(gThreadCheck.m_CheckThreadID));
			CRASH_APPLICATION_MACRO
		}
	}
	else
	{
		while(true)
		{
			TerminateProcess(GetCurrentProcess(),0);
			CRASH_APPLICATION_MACRO
		}
	}
}

void SetInt(DWORD offset, int value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset, sizeof(int), PAGE_EXECUTE_READWRITE, &OldProtect);

	*(int*)(offset) = value;

	VirtualProtect((void*)offset, sizeof(int), OldProtect, &OldProtect);
}

void SetByte(DWORD offset,BYTE value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,1,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(BYTE*)(offset) = value;

	VirtualProtect((void*)offset,1,OldProtect,&OldProtect);
}

void SetWord(DWORD offset,WORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,2,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(WORD*)(offset) = value;

	VirtualProtect((void*)offset,2,OldProtect,&OldProtect);
}

void SetDword(DWORD offset,DWORD value) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,4,PAGE_EXECUTE_READWRITE,&OldProtect);

	*(DWORD*)(offset) = value;

	VirtualProtect((void*)offset,4,OldProtect,&OldProtect);
}

void SetCompleteHook(BYTE head,DWORD offset,...) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,5,PAGE_EXECUTE_READWRITE,&OldProtect);

	if(head != 0xFF)
	{
		*(BYTE*)(offset) = head;
	}

	DWORD* function = &offset+1;

	*(DWORD*)(offset+1) = (*function)-(offset+5);

	VirtualProtect((void*)offset,5,OldProtect,&OldProtect);
}

void MemoryCpy(DWORD offset,void* value,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	memcpy((void*)offset,value,size);

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}

void MemorySet(DWORD offset,DWORD value,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	memset((void*)offset,value,size);

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}

void VirtualizeOffset(DWORD offset,DWORD size) // OK
{
	DWORD OldProtect;

	VirtualProtect((void*)offset,size,PAGE_EXECUTE_READWRITE,&OldProtect);

	DWORD HookAddr = (DWORD)malloc(size+5);

	memcpy((void*)HookAddr,(void*)offset,size);

	*(BYTE*)(HookAddr+size) = 0xE9;

	*(DWORD*)(HookAddr+size+1) = (offset+size)-((HookAddr+size)+5);

	*(BYTE*)(offset) = 0xE9;

	*(DWORD*)(offset+1) = HookAddr-(offset+5);

	memset((void*)(offset+5),0x90,(size-5));

	VirtualProtect((void*)offset,size,OldProtect,&OldProtect);
}

void* HookVTableFunction(void* pVTable, void* fnHookFunc, int nOffset) {
	intptr_t ptrVtable = *((intptr_t*)pVTable); // Pointer to our chosen vtable
	intptr_t ptrFunction = ptrVtable + sizeof(intptr_t) * nOffset; // The offset to the function (remember it's a zero indexed array with a size of four bytes)
	intptr_t ptrOriginal = *((intptr_t*)ptrFunction); // Save original address

	// Edit the memory protection so we can modify it
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPCVOID)ptrFunction, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

	// Overwrite the old function with our new one
	*((intptr_t*)ptrFunction) = (intptr_t)fnHookFunc;

	// Restore the protection
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);

	// Return the original function address incase we want to call it
	return (void*)ptrOriginal;
}

void HookFunction(void* ptrFunction, void* fnHookFunc) {

	DWORD OldProtect;

	MEMORY_BASIC_INFORMATION mbi;

	VirtualQuery((LPCVOID)ptrFunction, &mbi, sizeof(mbi));

	VirtualProtect(ptrFunction, 5, PAGE_EXECUTE_READWRITE, &OldProtect);

	// Overwrite the old function with our new one
	*((intptr_t*)ptrFunction) = (intptr_t)fnHookFunc;

	VirtualProtect(ptrFunction, 5, OldProtect, &OldProtect);

}

void ErrorMessageBox(char* message,...) // OK
{
	char buff[256];

	memset(buff,0,sizeof(buff));

	va_list arg;
	va_start(arg,message);
	vsprintf_s(buff,message,arg);
	va_end(arg);

	MessageBox(0,buff,"Error",MB_OK | MB_ICONERROR);

	SafeExitProcess();
}

bool DelayMe(DWORD delay,DWORD value) // OK
{
	DWORD tick = GetTickCount();

	while(true)
	{
		if((GetTickCount()-tick) >= delay)
		{
			break;
		}
		else
		{
			value = value + 10;
			Sleep(value);
		}
	}

	return 0;
}

bool ConvertProcessImageFileNameA(char* path,char* out_path,DWORD out_path_size) // OK
{
	char driver[512];
	char device[MAX_PATH];
	char search[3] = " :";

	if(GetLogicalDriveStrings((sizeof(driver)-1),driver) != 0)
	{
		char* fetch = driver;

		do
		{
			(*search) = (*fetch);

			if(QueryDosDevice(search,device,MAX_PATH) != 0)
			{
				std::string str = path;

				size_t index = str.find(device,0);

				if(index != std::string::npos)
				{
					str.replace(index,(std::string(device).length()+1),fetch);

					strcpy_s(out_path,out_path_size,str.data());

					return 1;
				}
			}

			while((*fetch++) != 0);
		}
		while((*fetch) != 0);
	}

	return 0;
}

bool ConvertProcessImageFileNameW(wchar_t* path,wchar_t* out_path,DWORD out_path_size) // OK
{
	wchar_t driver[512];
	wchar_t device[MAX_PATH];
	wchar_t search[3] = L" :";

	if(GetLogicalDriveStringsW((sizeof(driver)-1),driver) != 0)
	{
		wchar_t* fetch = driver;

		do
		{
			(*search) = (*fetch);

			if(QueryDosDeviceW(search,device,MAX_PATH) != 0)
			{
				std::wstring str = path;

				size_t index = str.find(device,0);

				if(index != std::wstring::npos)
				{
					str.replace(index,(std::wstring(device).length()+1),fetch);

					wcscpy_s(out_path,out_path_size,str.data());

					return 1;
				}
			}

			while((*fetch++) != 0);
		}
		while((*fetch) != 0);
	}

	return 0;
}

char* ConvertModuleFileName(char* name) // OK
{
	static char buff[MAX_PATH] = {0};

	for(int n=strlen(name);n > 0;n--)
	{
		if(name[n] == '\\')
		{
			strcpy_s(buff,sizeof(buff),&name[(n+1)]);
			break;
		}
	}

	return buff;
}

wchar_t* ConvertModuleFileName(wchar_t* name) // OK
{
	static wchar_t buff[MAX_PATH] = {0};

	for(int n=wcslen(name);n > 0;n--)
	{
		if(name[n] == '\\')
		{
			wcscpy_s(buff,sizeof(buff),&name[(n+1)]);
			break;
		}
	}

	return buff;
}

void LoadReferenceAddressTable(HMODULE mod, char* name, DWORD address) // OK
{
	if (IS_INTRESOURCE(name) == 0)
	{
		return;
	}

	HRSRC resource = FindResource(mod, name, "BIN");

	if (resource == 0)
	{
		return;
	}

	if (SizeofResource(mod, resource) < sizeof(REFERENCE_INFO))
	{
		return;
	}

	HGLOBAL data = LoadResource(mod, resource);

	if (data == 0)
	{
		return;
	}

	REFERENCE_INFO* ReferenceInfo = (REFERENCE_INFO*)LockResource(data);

	if (ReferenceInfo == 0)
	{
		FreeResource(data);
		return;
	}

	if (ReferenceInfo->start == ReferenceInfo->end)
	{
		if (SizeofResource(mod, resource) < (sizeof(REFERENCE_INFO) + (ReferenceInfo->count * sizeof(REFERENCE_BASE))))
		{
			return;
		}

		REFERENCE_BASE* ReferenceBase = (REFERENCE_BASE*)((DWORD)ReferenceInfo + sizeof(REFERENCE_INFO));

		for (DWORD n = 0; n < ReferenceInfo->count; n++)
		{
			SetDword(ReferenceBase[n].address, address);
		}
	}
	else
	{
		if (SizeofResource(mod, resource) < (sizeof(REFERENCE_INFO) + (ReferenceInfo->count * sizeof(REFERENCE_DATA))))
		{
			return;
		}

		REFERENCE_DATA* ReferenceData = (REFERENCE_DATA*)((DWORD)ReferenceInfo + sizeof(REFERENCE_INFO));

		for (DWORD n = 0; n < ReferenceInfo->count; n++)
		{
			SetDword(ReferenceData[n].address, (address + ReferenceData[n].value));
		}
	}

	FreeResource(data);
}

int PacketDecryptData(BYTE* lpMsg,int size,BYTE key) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]+key)^0xA0;
	}
	return 0;
}

int PacketEncryptData(BYTE* lpMsg,int size,BYTE key) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]^0xA0)-key;
	}

	return 0;
}

bool FileExists(char* filename)
{
	FILE* FileToFind = NULL;

	if (fopen_s(&FileToFind, filename, "rb") != NULL)
	{
		return false; //Failed to open file for read access.
	}

	fclose(FileToFind);

	return true;

	//try
	//{
	//	

	//	//fopen_s(&FileToFind, filename, "r");

	//	/*if (!FileToFind)
	//	{
	//		return 0;
	//	}
	//	else
	//	{
	//		fclose(FileToFind);
	//		return 1;
	//	}*/
	//}
	//catch (...)
	//{
	//	return 0;
	//}
}

bool FileExists(wchar_t* filename)
{
	try
	{
		FILE* FileToFind;

		_wfopen_s(&FileToFind, filename, L"r");

		if (!FileToFind)
		{
			return 0;
		}
		else
		{
			fclose(FileToFind);
			return 1;
		}
	}
	catch (...)
	{
		return 0;
	}
}

DWORD WINAPI SplashThread(CSplash* lpSplash) // OK
{
	lpSplash->ShowSplash();

	SetEvent(lpSplash->m_SplashEvent);

	MSG msg;

	while(GetMessage(&msg,0,0,0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return 0;
}

void SplashScreen(CSplash* ClassInst, int SplashType, bool TextSwitch, char DisplayText[255], int CloseTime)
{
	
	//gLog.Output(LOG_DEBUG,GetEncryptedString(42),SplashType,TextSwitch,DisplayText);
	//if (gAuthenticate.m_SplashScreen == 1)
	//{
		int ResId;

		COLORREF TextColor;

		SplashInit.CloseSplash();
		SplashAlert.CloseSplash();
		SplashError.CloseSplash();

		switch(SplashType)
		{
			case 0:
				ResId = IDB_BITMAP1;
				TextColor = RGB(50,120,200);
				break;
			case 1:
				ResId = IDB_BITMAP2;
				TextColor = RGB(200,150,20);
				break;
			case 2:
				ResId = IDB_BITMAP2;
				TextColor = RGB(0, 255, 253);
				break;
		}
		// "jxguard.bmp"
		if (FileExists(gProtect.m_MainInfo.VerifyName))
		{
			//SplashInit.SetBitmap("jxguard.bmp");
			//SplashAlert.SetBitmap("jxguard.bmp");
			//SplashError.SetBitmap("jxguard.bmp");
			if (TextSwitch)
				ClassInst->Start(gProtect.m_MainInfo.VerifyName, RGB(128, 128, 128), 1, DisplayText, TextColor, SplashType);
			else
				ClassInst->Start(gProtect.m_MainInfo.VerifyName, RGB(128, 128, 128));
		}
		else
		{
			if (TextSwitch)
				ClassInst->Start(ResId, hins, RGB(128, 128, 128), 1, DisplayText, TextColor, SplashType);
			else
				ClassInst->Start(ResId, hins, RGB(128, 128, 128));
		}

		ClassInst->m_SplashEvent = CreateEvent(0,0,0,0);

		CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SplashThread,ClassInst,0,0);

		WaitForSingleObject(ClassInst->m_SplashEvent,INFINITE);

		CloseHandle(ClassInst->m_SplashEvent);

		if(CloseTime > 0)
		{
			Sleep(CloseTime);

			if(SplashType != 0)
			{
				ClassInst->CloseSplash();
				SafeExitProcess();
			}
		}
	//}
}
//char* getMAC() {
//	PIP_ADAPTER_INFO AdapterInfo;
//	DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
//	char *mac_addr = (char*)malloc(18);
//
//	AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
//	if (AdapterInfo == NULL) {
//		printf("Error allocating memory needed to call GetAdaptersinfo\n");
//		free(mac_addr);
//		return NULL; // it is safe to call free(NULL)
//	}
//
//	// Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
//	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
//		free(AdapterInfo);
//		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
//		if (AdapterInfo == NULL) {
//			printf("Error allocating memory needed to call GetAdaptersinfo\n");
//			free(mac_addr);
//			return NULL;
//		}
//	}
//
//	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
//		// Contains pointer to current adapter info
//		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
//		do {
//			// technically should look at pAdapterInfo->AddressLength
//			//   and not assume it is 6.
//			sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
//				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
//				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
//				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
//			printf("Address: %s, mac: %s\n", pAdapterInfo->IpAddressList.IpAddress.String, mac_addr);
//			// print them all, return the last one.
//			// return mac_addr;
//
//			printf("\n");
//			pAdapterInfo = pAdapterInfo->Next;
//		} while (pAdapterInfo);
//	}
//	free(AdapterInfo);
//	return mac_addr; // caller must free.
//}

//std::wstring getComputerName() {
//	wchar_t buffer[8] = { 0 };
//	DWORD cchBufferSize = sizeof(buffer) / sizeof(buffer[0]);
//	if (!GetComputerNameW(buffer, &cchBufferSize))
//		throw std::runtime_error("GetComputerName() failed.");
//	return std::wstring(&buffer[0]);
//}

char* GetHardwareId() //OK
{
	char PhysicalDriveSerial[256];
	/*unsigned char mac[6] = { '\0' };
	char IpAddress[16];
	char MacAddress[18];

	sprintf(IpAddress, "%s", GetLocalIpAddressNew().c_str());
	get_mac(mac, IpAddress);
	sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);*/

	memset(PhysicalDriveSerial,0,sizeof(PhysicalDriveSerial));

	for(int n=0;n < 5;n++)
	{
		if(GetPhysicalDriveSerialNumber(n,PhysicalDriveSerial,sizeof(PhysicalDriveSerial)) != 0)
		{
			break;
		}
	}

	char buffer[256] = "";
	DWORD size = sizeof(buffer);

	//const std::wstring compName = getComputerName();

	DWORD ComputerHardwareId1 = *(DWORD*)(&PhysicalDriveSerial[0x00]) ^ *(DWORD*)(&PhysicalDriveSerial[0x10]) ^ 0x3AD3B74A;

	DWORD ComputerHardwareId2 = *(DWORD*)(&PhysicalDriveSerial[0x04]) ^ *(DWORD*)(&PhysicalDriveSerial[0x14]) ^ 0x94FDC685;

	DWORD ComputerHardwareId3 = *(DWORD*)(&PhysicalDriveSerial[0x08]) ^ *(DWORD*)(&PhysicalDriveSerial[0x18]) ^ 0xF45BBF4C;

	DWORD ComputerHardwareId4 = *(DWORD*)(&PhysicalDriveSerial[0x0C]) ^ *(DWORD*)(&PhysicalDriveSerial[0x1C]) ^ 0x8941D8E7;

	static char HardwareId[36];

	/*if (GetComputerName(buffer, &size))
	{
		wsprintf(HardwareId, "%08X-%08X-%08X-%08X", buffer, ComputerHardwareId2, ComputerHardwareId1, ComputerHardwareId4);
	}
	else
	{
		wsprintf(HardwareId, "%08X-%08X-%08X-%08X", ComputerHardwareId3, ComputerHardwareId2, ComputerHardwareId1, ComputerHardwareId4);
	}*/

	wsprintf(HardwareId,"%08X-%08X-%08X-%08X", ComputerHardwareId4, ComputerHardwareId1, ComputerHardwareId2, ComputerHardwareId3);

	return HardwareId;

}

char* SmallHardwareId() //OK
{
	char PhysicalDriveSerial[256];
	/*unsigned char mac[6] = { '\0' };
	char IpAddress[16];
	char MacAddress[18];

	sprintf(IpAddress, "%s", GetLocalIpAddressNew().c_str());
	get_mac(mac, IpAddress);
	sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);*/

	memset(PhysicalDriveSerial, 0, sizeof(PhysicalDriveSerial));

	for (int n = 0; n < 5; n++)
	{
		if (GetPhysicalDriveSerialNumber(n, PhysicalDriveSerial, sizeof(PhysicalDriveSerial)) != 0)
		{
			break;
		}
	}

	char buffer[256] = "";
	DWORD size = sizeof(buffer);

	DWORD ComputerHardwareId1 = *(DWORD*)(&PhysicalDriveSerial[0x00]) ^ *(DWORD*)(&PhysicalDriveSerial[0x08]) ^ 0x3AD3B74A;

	//DWORD ComputerHardwareId2 = *(DWORD*)(&PhysicalDriveSerial[0x04]) ^ *(DWORD*)(&PhysicalDriveSerial[0xA]) ^ 0x1992;

	//DWORD ComputerHardwareId3 = *(DWORD*)(&PhysicalDriveSerial[0x08]) ^ *(DWORD*)(&PhysicalDriveSerial[0x10]) ^ 0x1311;

	static char HardwareId[8];

	//wsprintf(HardwareId, "%08X%02X%02X", ComputerHardwareId1, ComputerHardwareId2, ComputerHardwareId3);
	wsprintf(HardwareId, "%08X", ComputerHardwareId1);

	return HardwareId;
}

char* GetHardwareIdNow() //OK
{
	static char HardwareId[32];

	unsigned char mac[6] = { '\0' };
	char cIpAddressID[8];
	char cMacAddressID[8];
	char textmix[100];
	char MixMacAddress[50];

	memcpy(gProtect.m_SmallHardwareId, SmallHardwareId(), sizeof(gProtect.m_SmallHardwareId));

	sprintf(gProtect.m_IpAddress, "%s", GetLocalIpAddressNew().c_str());

	get_mac(mac, gProtect.m_IpAddress);

	sprintf(gProtect.m_MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	DWORD IPId = *(DWORD*)(&gProtect.m_IpAddress[0x00]) ^ *(DWORD*)(&gProtect.m_IpAddress[0x16]) ^ 0x68BBBB79;

	sprintf(MixMacAddress, "%08X%02X%02X%02X%02X%02X%s", IPId, mac[1], mac[2], mac[3], mac[4], mac[5], gProtect.m_SmallHardwareId);

	std::string newhwid = licenseKeyFormatting(MixMacAddress, 5, 4);

	//memcpy(gProtect.m_HardwareId, newhwid.c_str(), sizeof(gProtect.m_HardwareId));

	wsprintf(HardwareId, "%s", newhwid.c_str());

	return HardwareId;

}

char* GetHardwareIdMain() //OK
{
	static char HardwareId[32];

	unsigned char mac[6] = { '\0' };
	char cIpAddressID[8];
	char cMacAddressID[8];
	char textmix[100];
	char MixMacAddress[50];

	memcpy(gProtect.m_SmallHardwareId, SmallHardwareId(), sizeof(gProtect.m_SmallHardwareId));

	sprintf(gProtect.m_IpAddress, "%s", GetLocalIpAddressNew().c_str());

	get_mac(mac, gProtect.m_IpAddress);

	sprintf(gProtect.m_MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	DWORD IPId = *(DWORD*)(&gProtect.m_IpAddress[0x00]) ^ *(DWORD*)(&gProtect.m_IpAddress[0x16]) ^ 0x68BBBB79;

	sprintf(MixMacAddress, "%08X%02X%02X%02X%02X%02X%s", IPId, mac[1], mac[2], mac[3], mac[4], mac[5], gProtect.m_SmallHardwareId);

	std::string newhwid = licenseKeyFormattingMain(MixMacAddress, 5, 4);

	//memcpy(gProtect.m_HardwareId, newhwid.c_str(), sizeof(gProtect.m_HardwareId));

	wsprintf(HardwareId, "%s", newhwid.c_str());

	return HardwareId;

}

bool get_mac(unsigned char* mac, char dest[32])
{
	struct in_addr destip;
	ULONG mac_address[2];
	ULONG mac_address_len = 6;

	destip.s_addr = inet_addr(dest);

	SendARP((IPAddr)destip.S_un.S_addr, 0, mac_address, &mac_address_len);

	if (mac_address_len)
	{
		BYTE* mac_address_buffer = (BYTE*)&mac_address;
		for (int i = 0; i < (int)mac_address_len; i++)
		{
			mac[i] = (char)mac_address_buffer[i];
		}
	}
	else
	{
		return false;
	}

	return true;
}

std::string GetLocalIpAddressNew()
{
	
	try
	{
		PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
		PIP_ADAPTER_INFO pAdapter = NULL;
		ULONG            ulOutBufLen = sizeof(IP_ADAPTER_INFO);
		std::string           localIp;

		if (pAdapterInfo != NULL)
		{
			if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
			{
				free(pAdapterInfo);
				pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
			}
		}

		if (pAdapterInfo != NULL)
		{
			if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR)
			{
				pAdapter = pAdapterInfo;

				while (pAdapter)
				{
					if ((strcmp(pAdapter->IpAddressList.IpAddress.String, "0.0.0.0") != 0) &&
						(strcmp(pAdapter->GatewayList.IpAddress.String, "0.0.0.0") != 0) &&
						(pAdapter->IpAddressList.Next == NULL) &&
						(pAdapter->GatewayList.Next == NULL))
					{
						if (localIp.empty())
						{
							localIp = std::string(pAdapter->IpAddressList.IpAddress.String);
						}
						else
						{
							// if there is more than one valid IP, assume we've failed
							//localIp.clear();
							break;
						}
					}

					pAdapter = pAdapter->Next;
				}
			}
			else
				localIp = std::string("127.0.0.1");

			free(pAdapterInfo);
		}
		else
			localIp = std::string("127.0.0.1");

		return localIp;

	}
	catch (...)
	{
		return std::string("127.0.0.1");
	}
	
}

char* HardIdParse(char* Input, char Character) //OK
{
	for(DWORD i=0; i<=strlen(Input); i++)
	{
		if(Input[i] == Character)
		{
			for(DWORD n=i; n<=strlen(Input); n++)
			{
				Input[n] = Input[n+1];
			}
		}
	}
	return Input;
}

char* GetEncryptedString(int index) // OK
{
	static char buff[256];

	memset(buff,0,sizeof(buff));

	for(int n=0;n < sizeof(gProtectString[index]);n++)
	{
		buff[n] = (gProtectString[index][n]-0x1B)^0x0C;
	}

	return buff;
}

bool SetAdminPrivilege(char* PrivilegeName) // OK
{
	HANDLE TokenHandle;

	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&TokenHandle) == 0)
	{
		return 0;
	}

	LUID luid;

	if(LookupPrivilegeValue(0,PrivilegeName,&luid) == 0)
	{
		return 0;
	}

	TOKEN_PRIVILEGES tp;

	memset(&tp,0,sizeof(tp));

	tp.PrivilegeCount = 1;

	tp.Privileges[0].Luid = luid;

	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(AdjustTokenPrivileges(TokenHandle,0,&tp,sizeof(tp),0,0) == 0)
	{
		return 0;
	}

	return 1;
}

std::string licenseKeyFormatting(std::string S, int K, int B) {
	//const int MAX = S.length() * 2;
	const int MAX = 33;
	char output[MAX];
	int k = MAX - 1;
	int j = 0;
	for (int i = S.length() - 1; i >= 0; --i) {
		if (S[i] == '-') continue;
		output[k--] = toupper(S[i]);
		if (B > 0)
		{
			if (i == B || i == (B*2)) {
				output[k--] = '-';
				j = 0;
			}
			else if (i == (S.length() - B) || i == (S.length() - (B*2)))
			{
				output[k--] = '-';
				j = 0;
			}
			else
			{
				if ((++j) % K == 0) {
					output[k--] = '-';
				}
			}
		}
		else
		{
			if ((++j) % K == 0) {
				output[k--] = '-';
			}
		}
	}
	int len = MAX - k - 1;
	std::string xx(len, ' ');
	for (int i = 0; i < len; ++i) {
		xx[i] = output[k + i + 1];
	}
	return xx[0] == '-' ? xx.substr(1) : xx;
}

std::string licenseKeyFormattingMain(std::string S, int K, int B) {
	//const int MAX = S.length() * 2;
	const int MAX = 33;
	char output[MAX];
	int k = MAX - 1;
	int j = 0;
	for (int i = S.length() - 1; i >= 0; --i) {
		if (S[i] == '+') continue;
		output[k--] = toupper(S[i]);
		if (B > 0)
		{
			if (i == B || i == (B * 2)) {
				output[k--] = '+';
				j = 0;
			}
			else if (i == (S.length() - B) || i == (S.length() - (B * 2)))
			{
				output[k--] = '+';
				j = 0;
			}
			else
			{
				if ((++j) % K == 0) {
					output[k--] = '+';
				}
			}
		}
		else
		{
			if ((++j) % K == 0) {
				output[k--] = '+';
			}
		}
	}
	int len = MAX - k - 1;
	std::string xx(len, ' ');
	for (int i = 0; i < len; ++i) {
		xx[i] = output[k + i + 1];
	}
	return xx[0] == '+' ? xx.substr(1) : xx;
}

HANDLE GetProcessByName(PCSTR name)
{
	DWORD pid = 0;

	// Create toolhelp snapshot.
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 process;
	ZeroMemory(&process, sizeof(process));
	process.dwSize = sizeof(process);

	// Walkthrough all processes.
	if (Process32First(snapshot, &process))
	{
		do
		{
			// Compare process.szExeFile based on format of name, i.e., trim file path
			// trim .exe if necessary, etc.
			if (string(process.szExeFile) == string(name))
			{
				pid = process.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &process));
	}

	CloseHandle(snapshot);

	if (pid != 0)
	{
		return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	}

	// Not found


	return NULL;
}


DWORD WriteMemory(const LPVOID lpAddress, const LPVOID lpBuf, const UINT uSize)
{
	DWORD dwErrorCode = 0;
	DWORD dwOldProtect = 0;
	// ----
	int iRes = VirtualProtect(lpAddress, uSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// ----
	if (iRes == 0)
	{
		dwErrorCode = GetLastError();
		return dwErrorCode;
	}
	// ----
	memcpy(lpAddress, lpBuf, uSize);
	// ----
	DWORD dwBytes = 0;
	// ----
	iRes = VirtualProtect(lpAddress, uSize, dwOldProtect, &dwBytes);
	// ----
	if (iRes == 0)
	{
		dwErrorCode = GetLastError();
		return dwErrorCode;
	}
	// ----
	return 0x00;
}

DWORD ReadMemory(const LPVOID lpAddress, LPVOID lpBuf, const UINT uSize)
{
	DWORD dwErrorCode = 0;
	DWORD dwOldProtect = 0;
	// ----
	int iRes = VirtualProtect(lpAddress, uSize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// ----
	if (iRes == 0)
	{
		dwErrorCode = GetLastError();
		return dwErrorCode;
	}
	// ----
	memcpy(lpBuf, lpAddress, uSize);
	// ----
	DWORD dwBytes = 0;
	// ----
	iRes = VirtualProtect(lpAddress, uSize, dwOldProtect, &dwBytes);
	// ----
	if (iRes == 0)
	{
		dwErrorCode = GetLastError();
		return dwErrorCode;
	}
	// ----
	return 0x00;
}

DWORD GetByte(const LPVOID dwOffset, BYTE& btValue)
{
	return ReadMemory(dwOffset, (LPVOID)btValue, sizeof(BYTE));
}

DWORD GetWord(const LPVOID dwOffset, WORD& wValue)
{
	return ReadMemory(dwOffset, (LPVOID)wValue, sizeof(WORD));
}

DWORD GetDword(const LPVOID dwOffset, DWORD& dwValue)
{
	return ReadMemory(dwOffset, (LPVOID)dwValue, sizeof(DWORD));
}

DWORD GetFloat(const LPVOID dwOffset, float& fValue)
{
	return ReadMemory(dwOffset, &fValue, sizeof(float));
}

bool CheckInGame()
{
	unsigned int AddressOffset1 = NULL;
	unsigned int AddressOffset2 = NULL;
	unsigned int AddressOffset3 = NULL;
	int					m_PlayerIndex = 0;

	ReadProcessMemory(hProcess, (LPVOID)(unsigned int)0x8E48F4, &AddressOffset1, sizeof(AddressOffset1), NULL);

	AddressOffset2 = (AddressOffset1 + 0x8B30);

	AddressOffset3 = (AddressOffset2 + 0x114);

	ReadProcessMemory(hProcess, (LPVOID)(unsigned int)AddressOffset3, &m_PlayerIndex, sizeof(m_PlayerIndex), NULL);

	return m_PlayerIndex;
}

string replaceAll(string subject, const string& search,
	const string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

string DownloadString(string URL) {
	HINTERNET interwebs = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	string rtn;
	if (interwebs) {
		urlFile = InternetOpenUrlA(interwebs, URL.c_str(), NULL, NULL, NULL, NULL);
		if (urlFile) {
			char buffer[2000];
			DWORD bytesRead;
			do {
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				rtn.append(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while (bytesRead);
			InternetCloseHandle(interwebs);
			InternetCloseHandle(urlFile);
			string p = replaceAll(rtn, "|n", "\r\n");
			return p;
		}
	}
	InternetCloseHandle(interwebs);
	string p = replaceAll(rtn, "|n", "\r\n");
	return p;
}
