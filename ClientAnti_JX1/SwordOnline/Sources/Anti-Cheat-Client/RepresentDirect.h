#pragma once
#define	KRF_ZERO_END	-1
#define	KRF_FOLLOW		(-2147483647 - 1)

enum CHARACTER_CODE_SET
{
	CHARACTER_CODE_SET_GBK = 1,	//GBK±àÂë
	CHARACTER_CODE_SET_GB2132,		//GB2132±àÂë
};

enum RS2_FONT_ITEM_PARAM {
    RS2_MAX_FONT_ITEM_NUM = 5
};

struct iFont
{
	virtual bool	Init(void* pDrawDevice) = 0;
	virtual bool	Load(const char* pszFontFile) = 0;
	virtual void	Terminate() = 0;
	virtual void	SetBorderColor(unsigned int uColor) = 0;
	virtual void	GetFontSize(int* pWidth, int* pHeight) = 0;
	virtual void	SetOutputSize(int nOutputWith, int nOutputHeight) = 0;
	virtual void	OutputText(const char* pszText, int nCount = KRF_ZERO_END,
		int nX = KRF_FOLLOW, int nY = KRF_FOLLOW,
		unsigned int nColor = 0xff000000, int nLineWidth = 0) = 0;
	virtual iFont* Clone() = 0;
	virtual void Release() = 0;
};

class RepresentDirect
{
public:
	RepresentDirect();
    bool Create(int nWidth, int nHeight, bool bFullScreen);
    bool CreateAFont(const char* pszFontFile, CHARACTER_CODE_SET CharaSet, int nId);
	void ReleaseAFont(int nId);
private:
    class KRSFontItem
    {
    public:
        int nId;
        iFont* pFontObj;
    };
	KRSFontItem m_FontTable[RS2_MAX_FONT_ITEM_NUM];
};

