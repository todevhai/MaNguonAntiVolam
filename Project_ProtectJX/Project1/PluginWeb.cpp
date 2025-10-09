#include "stdafx.h"
//#include "Main.h"
#include "PluginWeb.h"
#include "MD5.h"
//#include "KItem.h"
//#include "KPlayerNew.h"
//#include "System/Encrypt_Decrypt.h"
#include <sstream>

using namespace std;

PluginWeb gPluginWeb;

PluginWeb::PluginWeb()
{
    //m_AuthenURL = "https://auth.volam1pk.net/";

    time_t now = time(NULL);
    int random = 30 + (rand() % 60);
    size_t Minutes = random;
    m_TimeReCheck = now + (60 * Minutes);
    //char Md5Check[32];

    gAuthenticate.m_AuthReceived = false;
    memset(&gAuthenticate.AllowClientSerial, 0, sizeof(gAuthenticate.AllowClientSerial));
    memset(gUtil.CheckCode, 0, sizeof(gUtil.CheckCode));
    memset(gUtil.IpAddress, 0, sizeof(gUtil.IpAddress));
    gUtil.PACKETFILTER_ACTIVE = 0;
    gUtil.THANPHAP_ACTIVE = 0;
    gUtil.WORLDRANK_ACTIVE = 0;
    gUtil.nBuySellID = 0;
    gUtil.nPort = 0;
    gUtil.DISABLE_EXP_SKILL_ACTIVE = 0;

    //char cHWID[100];
    //std::string product_uuid = gUtil.exec("cat /sys/class/dmi/id/product_uuid");
    //std::string machineid = gUtil.exec("cat /etc/machine-id");
    //gUtil.GetMACInfo();
    //sprintf(cHWID, "%s|%s|%s|%s", product_uuid.erase(product_uuid.size() - 1).c_str(), machineid.erase(machineid.size() - 1).c_str(), gUtil.MacInterface, gUtil.IpInterface);
    //std::string Md5HWID = md5(cHWID);

    memset(m_LicenseCode, 0, sizeof(m_LicenseCode));

    //strcpy(m_LicenseCode, Md5HWID.c_str());
    //memcpy(m_LicenseCode, Md5HWID.c_str(), sizeof(m_LicenseCode));
    //m_LicenseCode[32] = 0;

    //time_t t = time(NULL);
    //struct tm* tm = localtime(&t);
    //std::string MCheck = md5(asctime(tm));
    //printf("MCheck: %s\n", MCheck.c_str());
    //strcpy(Md5Check, MCheck.c_str());
    //memcpy(m_TimeMd5Check, Md5Check, sizeof(m_TimeMd5Check));
    //m_TimeMd5Check[32] = '\0';
    //printf("m_TimeMd5Check: %s\n", m_TimeMd5Check);
    /*struct tm* tm = localtime(&now);
    std::string MCheck = md5(asctime(tm));
    memset(m_TimeMd5Check, 0, sizeof(m_TimeMd5Check));
    strcpy(m_TimeMd5Check, MCheck.c_str());
    m_TimeMd5Check[32] = 0;*/
}

PluginWeb::~PluginWeb()
{
    memset(m_LicenseCode, 0, sizeof(m_LicenseCode));
    memset(m_TimeMd5Check, 0, sizeof(m_TimeMd5Check));
}

string PluginWeb::UrlEncodeUTF8(const wstring& value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (wstring::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        wstring::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << uppercase;
        escaped << '%' << setw(2) << int((unsigned char)c);
        escaped << nouppercase;
    }

    return escaped.str();
}

string PluginWeb::url_encode(const string& value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }

        // Any other characters are percent-encoded
        escaped << uppercase;
        escaped << '%' << setw(2) << int((unsigned char)c);
        escaped << nouppercase;
    }

    return escaped.str();
}

size_t WriteCallback(char* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int PluginWeb::RequestURL(const char* textURL, const char* textNoiDung, void* pReturnData)
{
    int CurrenId = 1;
    char urldata[128];
    memset(urldata, 0, sizeof(urldata));
    char m_textURL[128];
    CheckAagin:
    std::string readBuffer;
    readBuffer.clear();
    /*CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl)
    {
        int res = 0;
        snprintf(curl_url, sizeof(curl_url), "https://cloud.updategame.xyz/fwcld/plugin/notice.php", results);
        snprintf(curl_fields, sizeof(curl_fields), "\"name\":\"%s\", \"key\":\"%s\"", name, data);


        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charset: utf-8");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, curl_url);
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, curl_fields);

        res = curl_easy_perform(curl);
        if (res)
        {
            -- - error-- -
        }
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }*/
    int result = 0;
    CURL* curl;
    CURLcode res;
    struct curl_slist* headers = NULL; // init to NULL is important

    //headers = curl_slist_append(headers, "Accept: application/json");
    //headers = curl_slist_append(headers, "Content-Type: application/json");
    //headers = curl_slist_append(headers, "charsets: utf-8");

    //stringstream ss1;
        //ss1 << textNoiDung;
    //string stringdata = ss1.str();
    //printf("Data: %s\n", this->url_encode(stringdata));
    //sprintf(urldata, "%slicense-%s/", textURL, this->url_encode(stringdata).c_str());
    sprintf(urldata, "%slicense-%s.nnb", textURL, textNoiDung);
    printf("Data: %s\n", urldata);
    //printf("Data: %s\n", this->url_encode(stringdata).c_str());
    curl = curl_easy_init();
    if (curl) {

        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, urldata);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
        //curl_easy_setopt(curl, CURLOPT_KEEP_SENDING_ON_ERROR, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10000L);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.2; WOW64; rv:17.0) Gecko/20100101 Firefox/17.0");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        //curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        res = curl_easy_perform(curl);
        //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        if (CURLE_OK == res) {
            //char* ct;
            //printf("Data: %s\n", readBuffer.c_str());
            //memcpy((void*)(pReturnData), readBuffer.c_str(), readBuffer.size());
            strcpy((char*)pReturnData, readBuffer.c_str());
            result = strlen(readBuffer.c_str());
            /*res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
            if ((CURLE_OK == res) && ct)
                printf("Header: %s\n", ct);*/
        }
        else
        {
            strcpy((char*)pReturnData, "ERROR");
            result = 0;
            //CurrenId++;
            //sprintf(m_textURL, "https://auth%d.jxguard365.xyz/", CurrenId);
            //textURL = m_textURL;
            //goto CheckAagin;
            /*result = readBuffer.size();
            printf("Buffer size: %s\n", result);
            char* ct;
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
            printf("Header: %s\n", ct);*/
            //if ((CURLE_OK == res) && ct)
                //printf("Header: %s\n", ct);
            //memcpy((void*)(pReturnData), "error", 5);
        }
    }
    curl_easy_cleanup(curl);
    return result;
    //curl_global_cleanup();
}

bool PluginWeb::ReconnectCheck()
{
    try
    {
        char requestdata[256];
        //const char* requestdata;
        char licensedata[256];

        memset(licensedata, 0, 256);
        memset(requestdata, 0, 256);

        unsigned char decryptedtext[1024];

        memset(&decryptedtext, 0, sizeof(decryptedtext));

        int RequestSize = 0;

        char cHWID[100];
        std::string product_uuid = gUtil.exec("cat /sys/class/dmi/id/product_uuid");
        std::string machineid = gUtil.exec("cat /etc/machine-id");
        gUtil.GetMACInfo();
        sprintf(cHWID, "%s|%s|%s|%s", product_uuid.erase(product_uuid.size() - 1).c_str(), machineid.erase(machineid.size() - 1).c_str(), gUtil.MacInterface, gUtil.IpInterface);
        std::string Md5HWID = md5(cHWID);

        memcpy(m_LicenseCode, Md5HWID.c_str(), sizeof(m_LicenseCode));
        m_LicenseCode[32] = 0;

        time_t now = time(NULL);
        struct tm* tm = localtime(&now);
        std::string MCheck = md5(asctime(tm));

        memcpy(m_TimeMd5Check, MCheck.c_str(), sizeof(m_TimeMd5Check));
        m_TimeMd5Check[32] = 0;
        sprintf(licensedata, "%s-%s", Md5HWID.c_str(), MCheck.c_str());

        int decryptedtext_len;
        int AuthCount = 1;
        int MaxAuthCount = 2;
        int nMaxCount = 0;

        while (true)
        {
            RequestSize = RequestURL("https://auth.volam1pk.net/", licensedata, &requestdata);

            decryptedtext_len = DecryptStringProtect((unsigned char*)&requestdata, RequestSize, decryptedtext) + 4;

            //printf("License: %s - %s\n", decryptedtext, requestdata);

            decryptedtext[decryptedtext_len] = '\0';

            if (decryptedtext_len > 32)
                break;

            if (nMaxCount >= 3)
                break;

            nMaxCount++;

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

        if (decryptedtext_len <= 32)
        {
            //printf("License: %s - %s\n", decryptedtext, requestdata);
            AuthenticateError(Md5HWID.c_str());
            return false;
        }

        size_t len;

        unsigned char* uc;

        std::string tp(reinterpret_cast<char const*>(decryptedtext), decryptedtext_len);

        string delimiter = "|";

        std::vector<std::string> results = gUtil.split(tp, delimiter);

        //printf("decryptedtext: %s\n", decryptedtext);
        //printf("License: %s - %d\n", results[1].c_str(), strcmp(Md5HWID.c_str(), results[1].c_str()));
        //printf("Md5Check: %s - %d\n", results[3].c_str(), strcmp(MCheck.c_str(), results[3].c_str()));
        //printf("AllowClientSerial: %s\n", results[5].c_str());
        //printf("Active: %s - %d\n", results[7].c_str());
        //printf("GameType: %s\n", results[9].c_str());
        //printf("WorldRank: %s\n", results[11].c_str());
        //printf("BuySell: %s\n", results[13].c_str());
        //printf("MaxIpConnect: %s\n", results[15].c_str());
        //printf("ExpSkillPlayer: %s\n", results[17].c_str());
        //printf("StateSkill: %s\n", results[19].c_str());
        //printf("Window1024: %s\n", results[21].c_str());
        //printf("decryptedtext_len: %d\n", decryptedtext_len);

        if ((strcmp(Md5HWID.c_str(), results[1].c_str()) != 0) || (strcmp(MCheck.c_str(), results[3].c_str()) != 0) || (atoi(results[7].c_str()) <= 0))
        {
            //printf("sender has told me to end the connection\n");
            AuthenticateError(Md5HWID.c_str());
            return false;
        }

        gAuthenticate.m_AuthReceived = true;

        m_Time = tm;

        memcpy(&gAuthenticate.AllowClientSerial, results[5].c_str(), sizeof(gAuthenticate.AllowClientSerial));

        gUtil.PACKETFILTER_ACTIVE = (atoi(results[7].c_str()) > 0);

        gUtil.THANPHAP_ACTIVE = atoi(results[9].c_str());

        gUtil.WORLDRANK_ACTIVE = atoi(results[11].c_str());

        gUtil.nBuySellID = atoi(results[13].c_str());

        gUtil.nLimitIP = atoi(results[15].c_str());

        gUtil.DISABLE_EXP_SKILL_ACTIVE = atoi(results[17].c_str());

        gUtil.nClientVersion = 0;

        return gAuthenticate.m_AuthReceived;
    }
    catch (std::exception const& e)
    {
        //printf("exception: %s\n", e.what());
        AuthenticateError(m_LicenseCode);
        return false;
    }
}

void PluginWeb::RequestPOST(const char* TokenData, const char* ChatID, const char* textNoiDung)
{
    try
    {
        char urldata[128];
        char postdata[128];
        std::string readBuffer;
        readBuffer.clear();
        char mTokenData[128];
        char mChatID[128];
        char NoiDung[128];
        int SizeNoiDung;
        wchar_t dichNoiDung[128];
        CURL* curl;
        CURLcode res;
        struct curl_slist* headers = NULL; // init to NULL is important

        //headers = curl_slist_append(headers, "Accept: application/json");
        //headers = curl_slist_append(headers, "Content-Type: application/json");
        //headers = curl_slist_append(headers, "charsets: utf-8");

        /*memset(mTokenData, 0, sizeof(mTokenData));
        memset(mChatID, 0, sizeof(mChatID));
        memset(NoiDung, 0, sizeof(NoiDung));
        strcpy(mTokenData, TokenData);
        strcpy(mChatID, ChatID);
        strcpy(NoiDung, textNoiDung);*/
        SizeNoiDung = strlen(textNoiDung);
        //printf("Len: %d\n", SizeNoiDung);
        //printf("NoiDung: %s\n", textNoiDung);
        //sprintf(urldata, "https://cloud.updategame.xyz/fwcld/plugin/notice.php?action=datapost&datapost=%s", this->url_encode(stringdata).c_str());
        /*for (int i = 0; i < SizeNoiDung; i++)
        {
            printf("Post Data: %s\n", (char)textNoiDung[i]);
        }*/
        gUtil.Convert(dichNoiDung, textNoiDung);
        wstring ws(dichNoiDung);
        string stringdata(ws.begin(), ws.end());
        //printf("Convert NoiDung: %s\n", stringdata.c_str());
        //stringstream ss1;
        //ss1 << dichNoiDung;
        //string stringdata = ss1.str();
        //printf("Post Data: %s\n", this->ConvertTCVN32UNICODE(stringdata).c_str());
        //dichNoiDung[128] = 0;
       
        sprintf(urldata, "https://api.telegram.org/bot%s/sendMessage", TokenData);
        urldata[128] = 0;
        sprintf(postdata, "chat_id=%s&text=%s&parse_mode=HTML", ChatID, this->url_encode(stringdata).c_str());
        postdata[128] = 0;
        //printf("Post Data: %s\n", postdata);
        //printf("Post Data: %s\n", urldata);
        curl = curl_easy_init();
        //snprintf(postdata, sizeof(postdata), "\"chat_id\":\"%s\", \"text\":\"%s\", \"parse_mode\":\"HTML\"", "5234130828", textNoiDung);
        //printf("Post Data: %s\n", postdata);
        if (curl) {

            //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            //curl_easy_setopt(curl, CURLOPT_URL, urldata);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
            curl_easy_setopt(curl, CURLOPT_URL, urldata);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
            //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, curl_fields);
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            //curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
            //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            //curl_easy_setopt(curl, CURLOPT_KEEP_SENDING_ON_ERROR, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            //curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
            res = curl_easy_perform(curl);
            //printf("Data: %s\n", res);
            //printf("Data 1: %s\n", res);
            if (CURLE_OK == res) {
                //char* ct;
                //printf("Data: %s\n", readBuffer.c_str());
                //memcpy((void*)(pReturnData), readBuffer.c_str(), readBuffer.size());
                /*res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
                if ((CURLE_OK == res) && ct)
                    printf("Header: %s\n", ct);*/
            }
        }

        curl_easy_cleanup(curl);
        //curl_global_cleanup();
    }
    catch (...)
    {
    }
}

//void PluginWeb::RequestURLOld(const char* data)
//{
//    //boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12);
//    //ctx.set_verify_mode(boost::asio::ssl::verify_none);
//    ////ctx.set_default_verify_paths();
//    ///*time_t now = time(nullptr);
//    //stringstream ss;
//    //ss << now;
//    //string strNow = ss.str();*/
//    //boost::asio::io_service io_service;
//
//    ////client c(io_service, ctx, "cloud.updategame.xyz", "/fwcld/check.php?hwid=TEST");
//    //client c(io_service, ctx, "cloud.updategame.xyz", "/fwcld/check.php?hwid=" + Md5HWID);
//    //io_service.run();
//}

//static void* PluginWeb::pull_one_url(void* url)
//{
//    pthread_exit(NULL);
//}

bool PluginWeb::CheckCurrenTime()
{
    time_t now = time(NULL);
    struct tm* tm = localtime(&now);
    //struct tm* tmCheck = localtime(&m_TimeReCheck);
    if ((tm->tm_year + (tm->tm_mon + 1) + tm->tm_mday) >= (m_Time->tm_year + (m_Time->tm_mon + 1) + m_Time->tm_mday))
        return true;
    else
        return false;
}

void PluginWeb::AuthenticateError(const char* LicenseShow)
{
    //printf("LICENSE DISABLE : %s\n", gUtil.GenLicenseID((char*)LicenseShow).c_str());
    printf("===========================================\n");
    printf("Authorization failed.\n");
    printf("%s\n", ANTI_WEBSITE);
    exit(1);
}

void PluginWeb::ReAuthenticateError()
{
    memset(&gAuthenticate.AllowClientSerial, 0, sizeof(gAuthenticate.AllowClientSerial));

    //memcpy(gUtil.AllowClientSerial, lpMsg->LicenseId, sizeof(gUtil.AllowClientSerial));

    memset(gUtil.CheckCode, 0, sizeof(gUtil.CheckCode));

    memset(gUtil.IpAddress, 0, sizeof(gUtil.IpAddress));

    gUtil.THANPHAP_ACTIVE = 0;

    gUtil.WORLDRANK_ACTIVE = 0;

    gUtil.nClientVersion = 0;

    gUtil.nBuySellID = 0;

    gUtil.nPort = 0;

    gUtil.DISABLE_EXP_SKILL_ACTIVE = 0;

    gUtil.m_RateFakeBattles = 0;

    //printf("LICENSE DISABLE: %s\n", gUtil.GenLicenseID(gAuthenticate.LicenseId).c_str());
    printf("===========================================\n");
    printf("Authorization failed.\n");
    printf("%s\n", ANTI_WEBSITE);
}
