// GameServer.cpp : Defines the entry point for the console application.
//

/*#ifdef _STANDALONE
#ifndef __linux
//#include <winsock2.h>
#endif
#endif*/

#include "StdAfx.h"
#include "KSOServer.h"

#include "nb30.h"
extern "C"
{
#include "miracl.h"
#include "mirdef.h"
}
#include <iostream.h>

#pragma comment( lib, "ms32.lib" ) 

extern KSwordOnLineSever g_SOServer;
/*#ifdef __linux
#include <unistd.h>
#include <signal.h> 
#include <sys/param.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
void init_daemon(void) { 
	int pid; 
	int i; 
	if(pid=fork()) exit(0);			//是父进程，结束父进程 
	else if(pid< 0) exit(1);		//fork失败，退出 
//是第一子进程，后台继续执行 
	setsid();//第一子进程成为新的会话组长和进程组长 
//并与控制终端分离 
	if(pid = fork()) exit(0);//是第一子进程，结束第一子进程 
	else if(pid< 0) 
	exit(1);//fork失败，退出 
//是第二子进程，继续 
//第二子进程不再是会话组长 

	for(i=0; i< NOFILE; ++i) close(i); 
	umask(0);//重设文件创建掩模 
	return; 
} 

#endif*/

typedef struct _ASTAT_
{
    ADAPTER_STATUS adapt;
    NAME_BUFFER    NameBuff [30];
}ASTAT, * PASTAT;
int g_nPort = 0;
int main(int argc, char* argv[])
{

	NCB ncb;
    UCHAR uRetCode;
	ASTAT Adapter;
    memset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBRESET;
    ncb.ncb_lana_num = 0;   // 指定网卡号
	
	//首先对选定的网卡发送一个NCBRESET命令，以便进行初始化 
	uRetCode = Netbios( &ncb );
	
    memset( &ncb, 0, sizeof(ncb) );
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = 0; 	// 指定网卡号
	
    strcpy( (char *)ncb.ncb_callname,"*" );
	// 指定返回的信息存放的变量 
    ncb.ncb_buffer = (unsigned char *) &Adapter;
	ncb.ncb_length = sizeof(Adapter);
	
	// 发送NCBASTAT命令以获取网卡的信息 
	uRetCode = Netbios( &ncb );
		// 把网卡MAC地址格式化成常用的16进制形式，如0010-A4E4-5802 
	TCHAR strMacAddr[256];
	sprintf(strMacAddr,"%02X%02X%02X%02X%02X%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]);

	miracl *mip=mirsys(100,0);
	mip->IOBASE=16;	//16进制模式
	
	//定义并初始化变量
	big m=mirvar(0);	//m 放明文：注册码SN
	big c=mirvar(0);	//c 放密文：用户名Name
	big n=mirvar(0);	//n 模数
	big e=mirvar(0);	//e 公钥
	TCHAR SN[256]={0};
	TCHAR temp[256]={0};
	/*int i,j;

 	cout<<"序号："<<strMacAddr<<"\n注册码："<<endl;
 	scanf("%s",&SN);

	//检查SN是否为16进制
	for (i=0,j=0;i<256;i++)
	{
		if (SN[i] == '\0')
		{
			if (i == 0)
			{
				j = 1;
			}
			break;
		}
		if(isxdigit(SN[i])==0)
		{
			j=1;
			break;
		}
	}
	
	//如果输入的SN为16进制且长度不为0
	if (j!=1)
	{
		
		cinstr(m,SN);									//初始化明文m
		cinstr(n,"963251DC5A9C90D9F203A03C363BA411");	//初始化模数n	 
		cinstr(e,"10001");								//初始化公钥e
		
		//当m<n时
		if(compare(m,n)==-1)
		{
			powmod(m,e,n,c);//计算c=m^e mod n
			big_to_bytes(256,c,temp,FALSE);//将c转换成数组写入temp
			
			//释放内存
			mirkill(m);
			mirkill(c);
			mirkill(n);
			mirkill(e);
			mirexit();
		}
		else
			j=1;
		
	}
	
	
	//对Name、temp， m、n， SN的长度进行检查
	if(lstrcmp(strMacAddr,temp)!=0||j==1)
	{
		MessageBox(0,"输入注册码不对哦.","错误！",MB_OK);
		ExitProcess(0);
	}*/


	BOOL bRunning = TRUE;
	if (argc == 2)
	{
		g_nPort = atoi(argv[1]);
	}
/*#ifdef __linux
	init_daemon();
#endif*/

	if (!g_SOServer.Init())
		return 0;

	while(bRunning)
	{
		bRunning = g_SOServer.Breathe();
	}

	g_SOServer.Release();
	return 1;
}

#ifdef _STANDALONE
extern "C"
{
void lua_outerrmsg(const char * szerrmsg)
{
	fprintf(stderr, szerrmsg);
}
};
#endif