#pragma once
#include "Util.h"
#include "Authenticate.h"
#include <curl/curl.h>
#include <string.h>
#include <iomanip>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp);

class PluginWeb
{
protected:
	time_t m_TimeReCheck;
	char m_LicenseCode[33];
	struct tm* m_Time;
	char m_TimeMd5Check[33];
public:
	PluginWeb();
	~PluginWeb();
	string UrlEncodeUTF8(const wstring& value);
	string url_encode(const string& value);
	bool ReconnectCheck();
	bool CheckCurrenTime();
	void RequestURLOld(const char* data);
	void AuthenticateError(const char* LicenseShow);
	void ReAuthenticateError();
	int RequestURL(const char* textURL, const char* textNoiDung, void* pReturnData);
	void RequestPOST(const char* TokenData, const char* ChatID, const char* textNoiDung);
	int RequestURL_APITelegramBot(const char* tokenAPI, const char* nChatID, const char* textAction, const char* textNoiDung, void* pReturnData);
	int RequestURL_APIBotJXGuard365(const char* textLicense, const char* textAction, const char* textType, const char* textNoiDung, void* pReturnData);
	int RequestURL_SendLog2Auth(const char* tokenAPI, const char* nChatID, const char* textAction, const char* textNoiDung, void* pReturnData);
	//static void* pull_one_url(void* url);
	//pthread_t thread_check;
};

extern PluginWeb gPluginWeb;

