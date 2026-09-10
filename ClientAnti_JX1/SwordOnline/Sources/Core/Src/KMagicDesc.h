#ifndef KMagicDescH
#define	KMagicDescH

#include "KIniFile.h"

class KMagicDesc
{
private:
	KIniFile	m_IniFile;
	/* 128 byte du cho mo ta tran, KHONG du khi cau mang the mau: moi
	   <color=...> ton 13-15 byte o dang CHU, va chuoi autoreplyskill co
	   bon the + mot ten chieu -> 190 byte, bi cat cut ngay giua the. */
	char		m_szDesc[256];
public:
	KMagicDesc();
	~KMagicDesc();
	BOOL		Init();
	const char*	GetDesc(void* pData);
};

extern KMagicDesc	g_MagicDesc;
#endif
