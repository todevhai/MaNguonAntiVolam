#pragma once
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include "Util.h"

struct IPAddress
{
	char m_IpAddress[16];
	bool m_IsBlocked;
	DWORD m_TotalLeftTime;
};

struct HardwareIdBlock
{
	char m_HardwareId[32];
	bool m_IsBlocked;
	DWORD m_TotalLeftTime;
};

class KFirewall
{
public:
	KFirewall();
	virtual ~KFirewall();
	bool CheckIpAddress(char* IpAddress, bool pBlock);
	void InsertIpAddress(char* IpAddress, bool pBlock, bool pCheck);
	void RemoveIpAddress(char* IpAddress);
	void ClearIpAddress();
public:
	long long m_TickClearIpAddress;
	std::map<std::string, IPAddress> m_ListIPAddress;
	std::map<std::string, IPAddress> m_ListIPAddressBlock;
	std::map<std::string, HardwareIdBlock> m_ListHardwareIdBlock;
};

extern KFirewall g_Firewall;