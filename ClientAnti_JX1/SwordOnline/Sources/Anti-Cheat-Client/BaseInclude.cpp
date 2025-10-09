#include "iRepresent\BaseInclude.h"

HWND		g_hWnd;

D3DFORMAT g_16BitFormat;

bool		g_bRefRast;						// 是否使用软件3D加速
bool		g_bRunWindowed;					// 是否使用窗口模式

int32	g_nScreenWidth;					// 显示窗口宽度
int32	g_nScreenHeight;				// 显示窗口高度

uint32	g_nAntiAliasFSOverSample;		// 多采样级别
uint32	g_nBackBufferCount;				// BackBuffer个数
bool		g_bForceSwapEffectBlt;			// 是否强制显存交换方式为拷贝
bool		g_bForce32Bit;					// 是否强制以32位方式处理ISI_T_BITMAP16资源,暂时不用，都安32位处理

int		g_nFogEnable;					// 是否打开雾化效果

int		g_nWireframe;					// 是否打开线框模式

RenderModel		g_renderModel;				// 是否打开透视模式

bool		g_bUse4444Texture;				// spr资源是否使用4444格式贴图，如果否，则使用8888格式

bool		g_bNonPow2Conditional;			// 是否允许非二的幂次长宽的贴图

int		g_ntest;