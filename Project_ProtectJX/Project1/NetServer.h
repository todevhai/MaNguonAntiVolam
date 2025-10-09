#pragma once
class NetServer
{
	NetServer();
	virtual ~NetServer();
	void HookFixAll();
};

extern NetServer gNetServer;