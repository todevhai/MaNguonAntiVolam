#include "stdafx.h"
#include "KFirewall.h"


KFirewall g_Firewall;

KFirewall::KFirewall()
{
}

KFirewall::~KFirewall()
{
}

bool KFirewall::CheckIpAddress(char* IpAddress, bool pBlock)
{
	std::map<std::string, IPAddress>::iterator it = m_ListIPAddress.find(std::string(IpAddress));

	if (it == m_ListIPAddress.end())
	{
		return false;
	}
	else
	{
		it->second.m_IsBlocked = pBlock;
		it->second.m_TotalLeftTime = gUtil.GetTimeNow();
		return true;
	}
}

void KFirewall::InsertIpAddress(char* IpAddress, bool pBlock, bool pCheck) // OK
{
	if (pCheck)
	{
		std::map<std::string, IPAddress>::iterator it = m_ListIPAddress.find(std::string(IpAddress));
		if (it == m_ListIPAddress.end())
		{
			IPAddress info;
			strcpy(info.m_IpAddress, IpAddress);
			info.m_IsBlocked = pBlock;
			info.m_TotalLeftTime = gUtil.GetTimeNow();
			m_ListIPAddress.insert(std::pair<std::string, IPAddress>(std::string(IpAddress), info));
		}
		else
		{
			it->second.m_IsBlocked = pBlock;
			it->second.m_TotalLeftTime = gUtil.GetTimeNow();
		}
	}
	else
	{
		IPAddress info;
		strcpy(info.m_IpAddress, IpAddress);
		info.m_IsBlocked = pBlock;
		info.m_TotalLeftTime = gUtil.GetTimeNow();
		m_ListIPAddress.insert(std::pair<std::string, IPAddress>(std::string(IpAddress), info));
	}
}

void KFirewall::RemoveIpAddress(char* IpAddress) // OK
{
	std::map<std::string, IPAddress>::iterator it = m_ListIPAddress.find(std::string(IpAddress));

	if (it != m_ListIPAddress.end())
	{
		m_ListIPAddress.erase(it);
	}
}

void KFirewall::ClearIpAddress() // OK
{
	if (m_ListIPAddress.size() > 0)
	{
		for (std::map<std::string, IPAddress>::iterator it = m_ListIPAddress.begin(); it != m_ListIPAddress.end(); it++)
		{
			if ((gUtil.GetTimeNow() - it->second.m_TotalLeftTime) >= 60)
			{
				m_ListIPAddress.erase(it);
			}
		}
	}
}