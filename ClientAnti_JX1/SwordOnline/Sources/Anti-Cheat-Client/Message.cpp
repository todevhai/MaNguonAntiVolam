// Message.cpp: implementation of the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Message.h"
#include "Authenticate.h"

CMessage gMessage;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessage::CMessage() // OK
{
	this->Init();
}

CMessage::~CMessage() // OK
{

}

void CMessage::Init() // OK
{
	memset(this->m_DefaultMessage,0,sizeof(this->m_DefaultMessage));

	for(int n=0;n < MAX_MESSAGE;n++)
	{
		this->m_EngCustomMessageInfo[n].Index = -1;
		this->m_PorCustomMessageInfo[n].Index = -1;
		this->m_SpnCustomMessageInfo[n].Index = -1;
	}
	MESSAGE_INFO info;
	info.Index = 0;
	strcpy(info.Message, "ANTI JX1 - TEAM - NTV");
	strcpy(info.SystemMessage, "ANTI JX1 - TEAM - NTV");
	SetInfoEng(info);

	//memset(&info, 0, sizeof(info));
	MESSAGE_INFO info1;
	info1.Index = 1;
	strcpy(info1.Message, "§­êng truyÒn kh«ng æn ®Þnh mÊt kÕt nèi");
	strcpy(info1.SystemMessage, "Duong truyen khong on dinh, khong the ket noi toi may chu");
	SetInfoEng(info1);

	//memset(&info, 0, sizeof(info));
	MESSAGE_INFO info2;
	info2.Index = 2;
	strcpy(info2.Message, "Sai phiªn b¶n vui lßng cËp nhËt l¹i game");
	strcpy(info2.SystemMessage, "Sai phien ban vui long cap nhat lai game");
	SetInfoEng(info2);

	//memset(&info, 0, sizeof(info));
	MESSAGE_INFO info3;
	info3.Index = 3;
	strcpy(info3.Message, "TËp tin trß ch¬i ®· bÞ söa ®æi");
	strcpy(info3.SystemMessage, "Tap tin tro choi da bi sua doi");
	SetInfoEng(info3);

	MESSAGE_INFO info4;
	info4.Index = 4;
	strcpy(info4.Message, "ChØ ®­îc phÐp më tèi ®a %d cöa sæ trß ch¬i");
	strcpy(info4.SystemMessage, "Chi duoc phep mo toi da %d cua so tro choi");
	SetInfoEng(info4);

	MESSAGE_INFO info5;
	info5.Index = 5;
	strcpy(info5.Message, "Vui lßng ch¹y game b»ng quyÒn Administrator");
	strcpy(info5.SystemMessage, "Vui long chay game bang quyen Administrator");
	SetInfoEng(info5);

	MESSAGE_INFO info6;
	info6.Index = 6;
	strcpy(info6.Message, "Lçi bé nhí trß ch¬i");
	strcpy(info6.SystemMessage, "Loi bo nho tro choi");
	SetInfoEng(info6);

	MESSAGE_INFO info7;
	info7.Index = 7;
	strcpy(info7.Message, "§ang kÕt nèi tíi m¸y chñ...");
	strcpy(info7.SystemMessage, "§ang kÕt nèi tíi m¸y chñ...");
	SetInfoEng(info7);

	MESSAGE_INFO info8;
	info8.Index = 8;
	strcpy(info8.Message, "Ph¸t hiÖn sö dông dÞch vô VPN...");
	strcpy(info8.SystemMessage, "Ph¸t hiÖn sö dông dÞch vô VPN...");
	SetInfoEng(info8);

}

void CMessage::GetLanguage() // OK
{
	strcpy_s(this->m_Language, "Eng");
	/*HKEY key;

	if(RegOpenKeyEx(HKEY_CURRENT_USER,"SOFTWARE\\Webzen\\Mu\\Config",0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS)
	{
		DWORD type=REG_SZ,size=sizeof(this->m_Language);

		if(RegQueryValueEx(key,"LangSelection",0,&type,(BYTE*)this->m_Language,&size) != ERROR_SUCCESS)
		{
			strcpy_s(this->m_Language,"Eng");
		}

		RegCloseKey(key);
	}*/
}

void CMessage::LoadEng(MESSAGE_INFO* info) // OK
{
	for(int n=0;n < MAX_MESSAGE;n++)
	{
		this->SetInfoEng(info[n]);
	}
}

void CMessage::LoadPor(MESSAGE_INFO* info) // OK
{
	for(int n=0;n < MAX_MESSAGE;n++)
	{
		this->SetInfoPor(info[n]);
	}
}

void CMessage::LoadSpn(MESSAGE_INFO* info) // OK
{
	for(int n=0;n < MAX_MESSAGE;n++)
	{
		this->SetInfoSpn(info[n]);
	}
}

void CMessage::SetInfoEng(MESSAGE_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_MESSAGE)
	{
		return;
	}

	this->m_EngCustomMessageInfo[info.Index] = info;
}

void CMessage::SetInfoPor(MESSAGE_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_MESSAGE)
	{
		return;
	}

	this->m_PorCustomMessageInfo[info.Index] = info;
}

void CMessage::SetInfoSpn(MESSAGE_INFO info) // OK
{
	if(info.Index < 0 || info.Index >= MAX_MESSAGE)
	{
		return;
	}

	this->m_SpnCustomMessageInfo[info.Index] = info;
}

MESSAGE_INFO* CMessage::GetInfoEng(int index) // OK
{
	if(index < 0 || index >= MAX_MESSAGE)
	{
		wsprintf(this->m_DefaultMessage,"Message %d out of bound!",index);
		return 0;
	}

	if(this->m_EngCustomMessageInfo[index].Index != index)
	{
		wsprintf(this->m_DefaultMessage,"Could not find message %d!",index);
		return 0;
	}

	return &this->m_EngCustomMessageInfo[index];
}

MESSAGE_INFO* CMessage::GetInfoPor(int index) // OK
{
	if(index < 0 || index >= MAX_MESSAGE)
	{
		wsprintf(this->m_DefaultMessage,"Message %d out of bound!",index);
		return 0;
	}

	if(this->m_PorCustomMessageInfo[index].Index != index)
	{
		wsprintf(this->m_DefaultMessage,"Could not find message %d!",index);
		return 0;
	}

	return &this->m_PorCustomMessageInfo[index];
}

MESSAGE_INFO* CMessage::GetInfoSpn(int index) // OK
{
	if(index < 0 || index >= MAX_MESSAGE)
	{
		wsprintf(this->m_DefaultMessage,"Message %d out of bound!",index);
		return 0;
	}

	if(this->m_SpnCustomMessageInfo[index].Index != index)
	{
		wsprintf(this->m_DefaultMessage,"Could not find message %d!",index);
		return 0;
	}

	return &this->m_SpnCustomMessageInfo[index];
}

char* CMessage::GetMessage(int index) // OK
{
	/*if(strcmp(this->m_Language,"Eng") == 0)
	{
		MESSAGE_INFO* lpInfo = this->GetInfoEng(index);
		if(lpInfo==0){return this->m_DefaultMessage;}
		return lpInfo->Message;
	}

	if(strcmp(this->m_Language,"Por") == 0)
	{
		MESSAGE_INFO* lpInfo = this->GetInfoPor(index);
		if(lpInfo==0){return this->m_DefaultMessage;}
		return lpInfo->Message;
	}

	if(strcmp(this->m_Language,"Spn") == 0)
	{
		MESSAGE_INFO* lpInfo = this->GetInfoSpn(index);
		if(lpInfo==0){return this->m_DefaultMessage;}
		return lpInfo->Message;
	}*/

	MESSAGE_INFO* lpInfo = this->GetInfoEng(index);
	if(lpInfo==0){return this->m_DefaultMessage;}
	if (gAuthenticate.m_SplashScreen == 1)
		return lpInfo->Message;
	else
		return lpInfo->SystemMessage;
}
