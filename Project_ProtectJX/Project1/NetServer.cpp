#include "stdafx.h"
#include "Util.h"
#include "NetServer.h"
#include <sys/mman.h>
#include <stdio.h>
#include <iostream>
#include <cstdarg>
#include <thread>
#include <cassert>
#include <cstring>
#include <assert.h>
#include <fstream>
#include <unistd.h>

NetServer::NetServer() {
	//Hook();
}
// -----------------------------------------------------------------------------

NetServer::~NetServer() {
}

void NetServer::HookFixAll()
{
	//size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x8122BE8; // set rank
	//uintptr_t pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}
	//gUtil.SetCompleteHook(0xE8, start, &FixGetIP); // loi nBuyIdx
}