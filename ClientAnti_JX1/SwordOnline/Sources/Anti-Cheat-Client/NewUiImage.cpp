#include "KWin32.h"
#include "KIniFile.h"
#include "NewUiImage.h"
#include <fstream>
#include "iRepresentShell.h"

using namespace std;

//KWndImage* gWndImage;
//KWndImage* gWndImage = (KWndImage*)0x63CF4C;
//KWndImage* gWndImage = (KWndImage*)CreateWndImage((unsigned int*)(KWndImage*)gWndImage);
extern iRepresentShell* g_pRepresentShell;
//extern iRepresentShell* g_pRepresentUiShell;
//KWndImage gWndImage;
class KIniFile;

KWndImage::KWndImage()
{

	//CreateWndImage((DWORD*)this);
	//*this = *static_cast<KWndImage*>(*((KWndImage**)0x659D40));
	*(void**)this = (static_cast<void*>((void**)0x63CF4C));

	//void* vptr_WndImage = *((void**)0x659040);
	//vptr_WndImage = ((void**)this);

	//*this = *dynamic_cast<KWndImage*>((KWndImage*)0x659040);
	//*((KWndImage*)this) = *dynamic_cast<KWndImage*>((KWndImage*)0x659040);
	//void* image_base = (void*)0x65906C;
	//void* image_ptr = new KWndImage;
	//void* button_ptr = new KWndButton;
	//void* image_ptr = new KWndImage;
	//void* button_ptr = new KWndButton;

	//void* window_ptr = (void*)0x63DB6C;
	//void** image_ptr = (void**)0x67C024;
	
	//void* button_ptr = (void*)0x67D0C4;
	//window_ptr = this;
	//CreateWndImage(*(DWORD**)this);

	//static_cast<KWndWindow*>((KWndWindow*)0x659D40);

	//* (void**)this = +((char**)((void**)0x63CF4C));
	//*((void**)this) = ((char**)((void**)0x63CF4C));

	//*this = *static_cast<KWndImage*>((KWndImage*)CreateWndImage((DWORD*)this));
	//CreateWndImage((unsigned int*)image_ptr);
	//image_ptr = ((void*)this);
	//void* window_ptr = new KWndWindow;
	//window_ptr = reinterpret_cast<KWndWindow*>(this);
	//button_ptr = dynamic_cast<KWndWindow*>(this);
	//window_ptr = this;
	// reference to base class => 0x659040 | 0x659D40 
	//void* vTablePtr = ++*((char**)((void*)0x63CF4C));
	//vTablePtr = this;
	//gWndImage = (KWndImage*)0x63CF4C;
	//*(void**)gWndImage = +((char**)((void**)0x63CF4C)); // refen array 0x65906C, refen base class 0x659040, vtable 0x659D54, object 0x659D40
	//*(void**)this = +((char**)((void**)0x63CF4C)); // refen array 0x65906C, refen base class 0x659040, vtable 0x659D54, object 0x659D40
	//void* button_ptr = (unsigned int*)0x63CF4C;
	//image_ptr = this;
	
	//*(this) = *static_cast<KWndImage*>((KWndImage*)((void*)0x63CF4C));
	//window_ptr = dynamic_cast<KWndWindow*>(this);

	//*(void**)this = static_cast<void**>(+((void**)((void***)0x63CF4C)));
	//image_ptr = dynamic_cast<void*>(this);
	//image_ptr = &image_base;
	//*(this) = *static_cast<KWndImage*>((void*)0x659040);
	//*(this) = *static_cast<KWndImage*>((KWndImage*)((void*)0x65906C));
	//*(this) = *static_cast<KWndImage*>((KWndImage*)((void*)0x659D54));
	//*(this) = *static_cast<KWndImage*>((KWndImage*)((void*)0x659D40));

	//RefCountBaseClass(this, 480, 1, (void*)RefWndImage, (void*)RefWndBase);

	//CreateWndWindow((BYTE*)window_ptr);

	//window_ptr = this;

	//KWndImage& image_vtable = *(KWndImage*)0x659D40;
	//KWndImage* base_vtable = (KWndImage*)0x659040;
	//KWndImage* array_vtable = (KWndImage*)0x65906C;
	//KWndImage* vptr_vtable = (KWndImage*)0x63CF4C;
	/*image_vtable = this;
	*this = static_cast<KWndImage&>(*(KWndImage*)(image_vtable));*/

	//CreateWndImage((DWORD*)gWndImage);
	//CreateWndButton((void*)this);
	

	//void* vTablePtr = (void*)0x63CF4C;
	//KWndWindow& vTablePtr = *(KWndWindow*)0x67C03C;

	//vTablePtr = new KWndImage;
	
	//gWndWindow = *dynamic_cast<KWndImage*>((KWndImage*)vTablePtr);

	//KWndImage* image_ptr = (KWndImage*)0x63CF4C;

	//base_vtable = this;

	//KWndWindow& vTablePtr = *this;
	
	//KWndWindow& vTablePtr = *(KWndWindow*)0x67C03C;
	//KWndWindow* vTablePtr = *(KWndWindow**)(this);

	//vTablePtr = new KWndImage;

	//gWndWindow = *dynamic_cast<KWndImage*>((KWndImage*)vTablePtr);

	//KWndImage* image_ptr = (KWndImage*)0x63CF4C;

	//vTablePtr = this;
	//CreateWndImage((DWORD*)this);
	//CreateWndImage((DWORD*)&vTablePtr);

	//(vTablePtr) = static_cast<KWndWindow&>(*image_vtable);

	//*this = *static_cast<KWndImage*>(array_vtable);

	//gWndImage = reinterpret_cast<KWndImage*>(image_ptr);

	//*(this) = *dynamic_cast<KWndImage*>((KWndImage*)(DWORD*)0x63CF4C);

	//*(this) = *dynamic_cast<const KWndImage*>((const KWndImage*)0x659D40);
	//*(const KWndWindow**)vTablePtr = +*(static_cast<const KWndImage**>((const KWndImage**)0x659D40));

	//*(this) = *static_cast<KWndImage*>((KWndImage*)image_ptr);

	//void* vptr_WndWindow = (void*)0x63DB6C;

	//vptr_WndWindow = this;

	//*((KWndWindow*)this) = *dynamic_cast<KWndWindow*>((KWndWindow*)vptr_WndWindow);

	//*(this) = *static_cast<KWndWindow*>((KWndWindow*)vptr_WndWindow);

	//m_Image.nType = 0;
	//m_Image.uImage = 0;
	//m_Image.uImage = 0;
	//m_Image.nNumFrames = 0;
	
	//*((DWORD*)&m_Image.nNumFrames) = 0;
	//*(DWORD*)&m_Image.nNumFrames = 0;
	//m_Image.nNumFrames = 0;
	//*((void***)(&m_Image.nNumFrames)) = 0;
	//m_Image.nInterval = 0;
	//m_Image.nFlipTime = 0;
	//* ((DWORD*)this + 106) = 0;
	//*(&m_Image.nNumFrames) = 0
	//m_Image.nNumFrames = 0;
	//IR_InitUiImageRef(m_Image);
	//*(&m_Image.nNumFrames) = 0;
	//IR_InitUiImageRef(m_Image);
	//m_Image.Color.Color_dw = 0;

	//*((DWORD*)&m_Image.nNumFrames) = 0;
	//void** vTablePtr = *(void***)0x63CF4C;

	//DWORD oldProtection;

	//VirtualProtect(vTablePtr, 4, PAGE_EXECUTE_READWRITE, &oldProtection); //Removes page protection at the start of the VMT so we can overwrite its first pointer.

	//gWndImage = reinterpret_cast<KWndImage*>(*vTablePtr); //Stores the pointer to VirtualFn1 from the VMT in a global variable so that it can be accessed again later after its entry in the VMT has been 
																  //overwritten with our hook function.

	//*vTablePtr = &gWndImage; //Overwrite the pointer to VirtualFn1 within the virtual table to a pointer to our hook function (hkVirtualFn1).

	//VirtualProtect(vTablePtr, 4, oldProtection, 0);

	//this->ptr_img = reinterpret_cast<KWndImage*>(0x63CF4C);
	//void* vptr = (void*)this;
	//vptr = (void*)(KWndImage*)0x63CF4C;
	//void* vptr = (void*)this;
	
	//CreateWndImage((unsigned int*)this);
}

//KWndImage::~KWndImage()
//{
//	ReleaseWndImage((unsigned int*)this);
//}

//void KWndImage::Clone(KWndImage* pCopy)
//{
//	if (pCopy)
//	{
//		KWndWindow::Clone(pCopy);
//		pCopy->m_Image = m_Image;
//	}
//}

//int KWndImage::Init(KIniFile* pIniFile, const char* pSection)
//{
//	ofstream myfile;
//	myfile.open("E:\\KWndImage_Init.txt");
//	myfile << "This size : " << pIniFile << "'\n";
//	myfile << "pSection : " << pSection << "'\n";
//	myfile.close();
//
//	return KWndImage_Init(this, pIniFile, pSection);
//	//if (KWndWindow::Init(pIniFile, pSection))
//	////if (KWndWindow_Init(this, pIniFile, pSection))
//	//{
//	//	return KWndImage_Init(this, pIniFile, pSection);
//	////	int		nValue = 0;
//	////	int		nTrans = 0;
//	////	int		nNoMsg = 0;
//
//	////	pIniFile->GetInteger(pSection, "Trans", 0, &nValue);
//
//	////	if (nValue)
//	////		nTrans = m_Style | WNDIMG_ES_EXCLUDE_TRANS;
//	////	else
//	////		nTrans = m_Style & (unsigned int)0xFFFFFFFE;
//
//	////	m_Style = nTrans;
//	////	m_Image.nFlipTime = 0;
//	////	m_Image.nInterval = 0;
//	////	m_Image.nNumFrames = 0;
//
//	////	pIniFile->GetInteger(pSection, "NoMsg", 0, &nValue);
//
//	////	if (nValue)
//	////		nNoMsg = m_Style | WNDIMG_ES_EXCLUDE_MSG;
//	////	else
//	////		nNoMsg = m_Style & (unsigned int)0xFFFFFFFD;
//
//	////	m_Style = nNoMsg;
//
//	////	pIniFile->GetInteger(pSection, "ImgType", 0, &nValue);
//	////	if (nValue == 1)
//	////	{
//	////		m_Image.nType = ISI_T_BITMAP16;
//	////		m_Image.bRenderStyle = IMAGE_RENDER_STYLE_OPACITY;
//	////		m_Style &= ~WNDIMG_ES_EXCLUDE_TRANS;
//	////	}
//	////	else
//	////	{
//	////		m_Image.nType = ISI_T_SPR;
//	////		m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
//	////		m_Image.Color.Color_b.a = 255;
//	////	}
//	////	m_Image.uImage = 0;
//	////	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
//	////	pIniFile->GetString(pSection, "Image", "", m_Image.szImage, sizeof(m_Image.szImage));
//	////	if (pIniFile->GetInteger(pSection, "Frame", -1, &nValue) && nValue != -1)
//	////		m_Image.nFrame = nValue;
//
//	////	
//
//	////	//g_pRepresentShell = (iRepresentShell*)0x6DE30C;
//
//	////	//KRUImage	Image;
//	////	//Image.nType = ISI_T_SPR;
//	////	//Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
//	////	//Image.uImage = 0;
//	////	//Image.nISPosition = -1;
//	////	//strcpy(Image.szImage, "\\spr\\Ui\\×´Ì¬Í¼±ê\\ÂÞººÕó.spr");
//	////	////Image.szImage[128] = 0;
//	////	////myfile2 << ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Image << "\n";
//	////	////myfile2 << Image.szImage << "\n";
//	////	//Image.Color.Color_b.a = 255;
//	////	//Image.bRenderFlag = 0;
//	////	//Image.nFrame = 0;
//	////	//Image.oPosition.nX = 39;
//	////	//Image.oPosition.nY = 28;
//	////	//Image.oPosition.nZ = 0;
//	////	//Image.nNew1 = 0;
//
//	////	if ((m_Width == 0 || m_Height == 0) && g_pRepresentShell)
//	////	{
//	////		
//
//	////		KImageParam	Param = { 0 };
//	////		if (g_pRepresentShell->GetImageParam(m_Image.szImage, &Param, m_Image.nType))
//	////		{
//	////			if (m_Width == 0)
//	////				m_Width = Param.nWidth;
//	////			if (m_Height == 0)
//	////				m_Height = Param.nHeight;
//	////		}
//	////	}
//
//	/////*	ofstream myfile;
//	////	myfile.open("E:\\KWndImage_Init.txt");
//	////	myfile << "This size : " << g_pRepresentShell << "'\n";
//	////	myfile << "m_Width : " << m_Width << "'\n";
//	////	myfile << "m_Height : " << m_Height << "'\n";
//	////	myfile << "szImage : " << m_Image.szImage << "'g\n";
//	////	myfile.close();*/
//
//	////	return true;
//	//}
//	//return false;
//}

//int KWndImage::PtInWindow(int x, int y)
//{
//	int	bIn = KWndWindow::PtInWindow(x, y);
//
//	if ((bIn &&
//		(m_Style & WND_S_SIZE_WITH_ALL_CHILD) == 0) ||
//		((m_Style & WND_S_VISIBLE) &&
//			bIn == false &&
//			(m_Style & WND_S_SIZE_WITH_ALL_CHILD)))
//	{
//		if ((m_Style & WNDIMG_ES_EXCLUDE_TRANS) && g_pRepresentShell)
//			bIn = g_pRepresentShell->GetImagePixelAlpha(m_Image.szImage, m_Image.nFrame, x - m_nAbsoluteLeft, y - m_nAbsoluteTop, m_Image.nType);
//	}
//	return bIn;
//}
//
//void KWndImage::PaintWindow()
//{
//	KWndWindow::PaintWindow();
//
//	ofstream myfile;
//	myfile.open("E:\\KWndImage_PaintWindow.txt");
//	myfile << "This size : " << g_pRepresentShell << "'\n";
//	myfile.close();
//
//	//if (g_pRepresentShell)
//	//{
//	//	//KRUImage	Image;
//	//	//Image.nType = ISI_T_SPR;
//	//	//Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
//	//	//Image.uImage = 0;
//	//	//Image.nISPosition = -1;
//	//	//strcpy(Image.szImage, "\\spr\\Ui\\×´Ì¬Í¼±ê\\ÂÞººÕó.spr");
//	//	////Image.szImage[128] = 0;
//	//	////myfile2 << ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Image << "\n";
//	//	////myfile2 << Image.szImage << "\n";
//	//	//Image.Color.Color_b.a = 255;
//	//	//Image.bRenderFlag = 0;
//	//	//Image.nFrame = 0;
//	//	//Image.oPosition.nX = 39;
//	//	//Image.oPosition.nY = 28;
//	//	//Image.oPosition.nZ = 0;
//	//	//Image.nNew1 = 0;
//
//	//	m_Image.oPosition.nX = m_nAbsoluteLeft;
//	//	m_Image.oPosition.nY = m_nAbsoluteTop;
//	//	g_pRepresentShell->DrawPrimitives(1, &m_Image, RU_T_IMAGE, true);
//	//}
//
//}
//
//void KWndImage::SetImage(short nType, const char* pszImgName, bool bAdjustWndSize)
//{
//	m_Image.nType = nType;
//	strncpy(m_Image.szImage, pszImgName, sizeof(m_Image.szImage));
//	m_Image.szImage[sizeof(m_Image.szImage) - 1] = 0;
//	m_Image.nNumFrames = 0;
//	m_Image.uImage = 0;
//	m_Image.nFlipTime = IR_GetCurrentTime();
//	if (bAdjustWndSize && g_pRepresentShell)
//	{
//		KImageParam	Param;
//		if (g_pRepresentShell->GetImageParam(m_Image.szImage, &Param, nType))
//			SetSize(Param.nWidth, Param.nHeight);
//	}
//
//}
//void KWndImage::SetFrame(int nFrame)
//{
//	m_Image.nFrame = nFrame;
//}
//
//int KWndImage::NextFrame()
//{
//	return IR_NextFrame(m_Image);
//}
//
//void KWndImage::UpdateTimer()
//{
//	m_Image.nFlipTime = IR_GetCurrentTime();
//}
//
//void KWndImage::SetMaxFrame(int nMaxFrame)
//{
//	m_Image.nNumFrames = nMaxFrame;
//}
