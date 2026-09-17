//---------------------------------------------------------------------------
// Sword3 Core (c) 2002 by Kingsoft
//
// File:	KBasPropTbl.h
// Date:	2002.08.14
// Code:	DongBo
// Desc:    header file. 本文件定义的类用于从tab file中读出道具的初始属性,
//			并生成对应的属性表
//---------------------------------------------------------------------------

#ifndef	KBasPropTblH
#define	KBasPropTblH

#include <map>
#include <vector>

#define		SZBUFLEN_0	80		// 典型的字符串缓冲区长度
#define		SZBUFLEN_1	128		// 典型的字符串缓冲区长度

#define		MAX_MAGIC_PREFIX	20
#define		MAX_MAGIC_SUFFIX	20
// 以下结构用于描述矿石的基本属性. 相关属性由配置文件(tab file)提供
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	int			m_nItemGenre;				// 道具种类
	int			m_nDetailType;				// 具体类别
	int			m_nParticularType;			// 详细类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	int			m_nObjIdx;					// 对应物件索引
	int			m_nWidth;					// 道具栏中所占宽度
	int			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	int			m_nSeries;					// 五行属性
	int			m_nPrice;					// 价格
	int			m_nLevel;					// 等级
	BOOL		m_bStack;					// 是否可叠放	
	char		m_szScript[SZBUFLEN_1];		// 执行脚本
	int			m_Magic[5];
	int			m_nDelet;
} KBASICPROP_MINE;

// 以下结构用于描述药品属性的特性：数值与时间
typedef struct
{
	int			nAttrib;
	int			nValue;
	int			nTime;
} KMEDATTRIB;

// 以下结构用于描述药品的基本属性. 相关属性由配置文件(tab file)提供
// 适用于以下药品: 生命补充类,内力补充类,体力补充类,毒药类,解毒类,
//					解燃烧类,解冰冻类
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	int			m_nItemGenre;				// 道具种类
	int			m_nDetailType;				// 具体类别
	int			m_nParticularType;			// 详细类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	int			m_nObjIdx;					// 对应物件索引
	int			m_nWidth;					// 道具栏中所占宽度
	int			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	int			m_nSeries;					// 五行属性
	int			m_nPrice;					// 价格
	int			m_nLevel;					// 等级
	BOOL		m_bStack;					// 是否可叠放
	KMEDATTRIB	m_aryAttrib[2];				// 药品的属性
} KBASICPROP_MEDICINE;

// 以下结构用于描述药材的基本属性. 相关属性由配置文件(tab file)提供
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	int			m_nItemGenre;				// 道具种类
	int			m_nDetailType;				// 具体类别
	int			m_nParticularType;			// 详细类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	int			m_nObjIdx;					// 对应物件索引
	int			m_nWidth;					// 道具栏中所占宽度
	int			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	int			m_nSeries;					// 五行属性
	int			m_nPrice;					// 价格
	int			m_nLevel;					// 等级
	BOOL		m_bStack;					// 是否可叠放
	int			m_nAttrib1_Type;			// 属性1类型
	int			m_nAttrib1_Para;			// 属性1参数
	int			m_nAttrib2_Type;			// 属性2类型
	int			m_nAttrib2_Para;			// 属性2参数
	int			m_nAttrib3_Type;			// 属性2类型
	int			m_nAttrib3_Para;			// 属性2参数
} KBASICPROP_MEDMATERIAL;

// 以下结构用于描述一对最大,最小值
typedef struct
{
	int			nMin;
	int			nMax;
} KMINMAXPAIR;

// 以下结构用于给出装备的核心参数: 基础属性
typedef struct
{
	int			nType;						// 属性类型
	KMINMAXPAIR	sRange;						// 取值范围
} KEQCP_BASIC;	// Equipment_CorePara_Basic

// 以下结构用于给出装备的核心参数: 需求属性
typedef struct
{
	int			nType;						// 属性类型
	int			nPara;						// 数值
} KEQCP_REQ;	// Equipment_CorePara_Requirment

// 以下结构用于给出魔法的核心参数
typedef struct
{
	int			nPropKind;					// 修改的属性类型（对同一个数值加百分比和加点数被认为是两个属性）
	KMINMAXPAIR	aryRange[3];				// 修改属性所需的几个参数
} KMACP;	// MagicAttrib_CorePara

// 以下结构用于描述配置文件中给出的魔法属性. 相关属性由配置文件(tab file)提供
// Add by Freeway Chen in 2003.5.30
#define			MATF_CBDR		    11      // 物品类型 type(现在的值为 equip_detailnum)
#define         MATF_PREFIXPOSFIX   2       // 前缀后缀
#define         MATF_SERIES         5       // 五行
#define         MATF_LEVEL          10      // 最多有10个级别

typedef struct
{
	int			m_nPos;						// 前缀还是后缀
	char		m_szName[SZBUFLEN_0];		// 名称
	int			m_nClass;					// 五行要求
	int			m_nLevel;					// 等级要求
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	KMACP		m_MagicAttrib;				// 核心参数
	int			m_DropRate[MATF_CBDR];		// 出现概率
    //add by Freeway Chen in 2003.5.30
    int         m_nUseFlag;                 // 该魔法是否被使用过
} KMAGICATTRIB_TABFILE;

// 以下结构用于描述魔法属性. 相关属性由配置文件(tab file)提供
/*
typedef struct
{
	int			m_nPos;						// 前缀还是后缀
	char		m_szName[SZBUFLEN_0];		// 名称
	int			m_nClass;					// 五行要求
	int			m_nLevel;					// 等级要求
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	KMACP		m_MagicAttrib;				// 核心参数
	int			m_DropRate;					// 出现概率
} KMAGICATTRIB;
*/
// 以下结构用于描述装备的初始属性. 相关数据由配置文件(tab file)提供
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	int			m_nItemGenre;				// 道具种类 (武器? 药品? 矿石?)
	int			m_nDetailType;				// 具体类别
	int			m_nParticularType;			// 详细类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	int			m_nObjIdx;					// 对应物件索引
	int			m_nWidth;					// 道具栏中所占宽度
	int			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	int			m_nSeries;					// 五行属性
	int			m_nPrice;					// 价格
	int			m_nLevel;					// 等级
	BOOL		m_bStack;					// 是否可叠放
	KEQCP_BASIC	m_aryPropBasic[7];			// 基础属性
	KEQCP_REQ	m_aryPropReq[6];			// 需求属性
} KBASICPROP_EQUIPMENT;

// 以下结构用于描述唯一装备的初始属性. 相关数据由配置文件(tab file)提供
typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	int			m_nItemGenre;				// 道具种类 (武器? 药品? 矿石?)
	int			m_nDetailType;				// 具体类别
	int			m_nParticularType;			// 详细类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	int			m_nObjIdx;					// 对应物件索引
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	int			m_nSeries;					// 五行属性
	int			m_nPrice;					// 价格
	int			m_nLevel;					// 等级
	int			m_nRarity;					// 稀有程度
	KEQCP_REQ	m_aryPropReq[6];			// 需求属性
	KMACP		m_aryMagicAttribs[6];		// 魔法属性
} KBASICPROP_EQUIPMENT_UNIQUE;

// 以下结构用于描述黄金装备的初始属性. 相关数据由配置文件(tab file)提供
// flying 根据策划需求修改自KBASICPROP_EQUIPMENT_UNIQUE类型
// Hoang Kim row of settings/item/004/goldequip.txt, read by column position (KItemGoldEquip.cpp).
typedef struct
{
	char		m_szName[SZBUFLEN_0];
	int			m_nItemGenre;
	int			m_nDetailType;
	int			m_nParticularType;
	char		m_szImageName[SZBUFLEN_0];	// column 5
	int			m_nObjIdx;
	int			m_nWidth;
	int			m_nHeight;
	char		m_szIntro[SZBUFLEN_1];		// column 9
	int			m_nSeries;
	int			m_nPrice;
	int			m_nLevel;
	KEQCP_BASIC	m_aryPropBasic[7];
	KEQCP_REQ	m_aryPropReq[6];
	int			m_aryMagicIdx[6];		// columns 47..52: 1-based rows of magicattrib_ge.txt
	int			m_nSuit;				// column 53
	int			m_nExtSuit;				// column 54
	int			m_nExtSuitNo;			// column 55
	int			m_aryExtSuitHidden[2];	// columns 56/57
} KBASICPROP_EQUIPMENT_GOLD;

// Row of magicattrib_ge.txt: the attribute type is fixed, each parameter is rolled in [min,max].
typedef struct
{
	int			m_nType;				// column 5
	KMINMAXPAIR	m_aryRange[3];			// columns 6..11
} KBASICPROP_GOLDMAGIC;

// Hoang Kim suits (KItemGoldSuit.cpp). Index into aryDong: equipment slot 0..14, or for a ring
// 14 + level + 10 * series (so two different rings are two parts), hence 65.
#define		MAX_O_BO_TRANG_BI	65
typedef struct
{
	int					nSoBoPhan;							// distinct indexes that have a row
	std::vector<int>	aryDong[MAX_O_BO_TRANG_BI];			// goldequip/platinaequip row numbers (0-based)
} KGOLD_SUITE;

typedef struct
{
	int		nDong;		// goldequip row (goldId - 1)
	int		nO;			// index as above
	BOOL	bNhan;		// ring: counts once per index
} KMON_TRONG_BO;

typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	int			m_nItemGenre;				// 道具种类
	int			m_nDetailType;				// 具体类别
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	int			m_nObjIdx;					// 对应物件索引
	int			m_nWidth;					// 道具栏中所占宽度
	int			m_nHeight;					// 道具栏中所占高度
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
	char		m_szScript[SZBUFLEN_1];		// 执行脚本
	int			m_nPrice;
	int			m_nPriceXu;	
	int			m_nDelet;
	int			m_nIsSell;
	int			m_nIsTrade;
	int			m_nIsDrop;
} KBASICPROP_QUEST;

// Genre 6 row of magicscript.txt (KItemMagicScript.cpp).
typedef struct
{
	char		m_szName[SZBUFLEN_0];
	int			m_nItemGenre;
	int			m_nDetailType;
	int			m_nParticularType;
	char		m_szImageName[SZBUFLEN_0];
	int			m_nObjIdx;
	int			m_nWidth;
	int			m_nHeight;
	char		m_szIntro[SZBUFLEN_1];
	int			m_nSeries;
	int			m_nPrice;
	int			m_nLevel;
	int			m_nParam[4];			// columns 16..19; [3] = allowed on the shortcut bar
	int			m_nMaxStack;			// column 21; > 0 means stackable
} KBASICPROP_MAGICSCRIPT;

// 以下定义的结构用于辅助从tabfile中读出属性的初始值
typedef struct tagPROPINFO
{
	int		m_nType;		// 属性的类型. 详见 PI_VARTYPE_...系列定义
	union
	{
	char*	m_pszBuf;		// 指向字符串缓冲区的指针
	int*	m_pnData;		// 指向int变量的指针
	}m_pData;
	int		m_nBufSize;		// 缓冲区的长度
} PROPINFO;
#define		PI_VARTYPE_CHAR		0
#define		PI_VARTYPE_INT		1

typedef struct
{
	char		m_szName[SZBUFLEN_0];		// 名称
	int			m_nItemGenre;				// 道具种类
	char		m_szImageName[SZBUFLEN_0];	// 界面中的动画文件名
	int			m_nObjIdx;					// 对应物件索引
	int			m_nWidth;					// 道具栏中所占宽度
	int			m_nHeight;					// 道具栏中所占高度
	int			m_nPrice;					// 价格
	char		m_szIntro[SZBUFLEN_1];		// 说明文字
} KBASICPROP_TOWNPORTAL;
//=============================================================================

class KBasicPropertyTable			// 缩写: BPT,用于派生类
{
public:
	KBasicPropertyTable();
	~KBasicPropertyTable();

// 以下是核心成员变量
protected:
	void*		m_pBuf;						// 指向属性表缓冲区的指针
											// 属性表是一个结构数组,
											// 其具体类型由派生类决定
	int			m_nNumOfEntries;			// 属性表含有多少项数据

// 以下是辅助性的成员变量
    int         m_nSizeOfEntry;				// 每项数据的大小(即结构的大小)
	char		m_szTabFile[MAX_PATH];
	const char*	m_pszThuMuc;				// table directory; NULL = TABFILE_PATH		// tabfile的文件名

// 以下是对外接口
public:
	virtual BOOL Load();					// 从tabfile中读出初始属性值, 填入属性表
	int NumOfEntries() const { return m_nNumOfEntries; }

// 以下是辅助函数
protected:
	BOOL GetMemory();
	void ReleaseMemory();
	void SetCount(int);
	virtual BOOL LoadRecord(int i, KTabFile* pTF) = 0;
};

class KBPT_MagicScript : public KBasicPropertyTable
{
public:
	KBPT_MagicScript();
	~KBPT_MagicScript();
	const KBASICPROP_MAGICSCRIPT* FindRecord(IN int nDetailType, IN int nParticularType) const;
protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

class KBPT_Mine : public KBasicPropertyTable
{
public:
	KBPT_Mine();
	~KBPT_Mine();

// 以下是对外接口
public:
	const KBASICPROP_MINE* GetRecord(IN int) const;
	const KBASICPROP_MINE* FindRecord(IN int, IN int) const;

// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

// =====>药品<=====
class KBPT_Medicine : public KBasicPropertyTable
{
public:
	KBPT_Medicine();
	~KBPT_Medicine();

// 以下是对外接口
public:
	const KBASICPROP_MEDICINE* GetRecord(IN int) const;
	const KBASICPROP_MEDICINE* FindRecord(IN int, IN int) const;

// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

// =====>任务物品<=====
class KBPT_Quest : public KBasicPropertyTable
{
public:
	KBPT_Quest();
	~KBPT_Quest();

// 以下是对外接口
public:
	const KBASICPROP_QUEST* GetRecord(IN int) const;
	const KBASICPROP_QUEST* FindRecord(IN int) const;

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

class KBPT_TownPortal : public KBasicPropertyTable
{
public:
	KBPT_TownPortal();
	~KBPT_TownPortal();

// 以下是对外接口
public:
	const KBASICPROP_TOWNPORTAL* GetRecord(IN int) const;

protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

// =====>药材<=====
class KBPT_MedMaterial : public KBasicPropertyTable
{
public:
	KBPT_MedMaterial();
	~KBPT_MedMaterial();

// 以下是对外接口
public:
	const KBASICPROP_MEDMATERIAL* GetRecord(IN int) const;

// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

class KBPT_Equipment : public KBasicPropertyTable
{
public:
	KBPT_Equipment();
	~KBPT_Equipment();

// 以下是对外接口
public:
	const KBASICPROP_EQUIPMENT* GetRecord(IN int) const;
	const KBASICPROP_EQUIPMENT* FindRecord(IN int, IN int, IN int) const;
	void Init(IN int);
// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

class KBPT_Equipment_Unique : public KBasicPropertyTable
{
public:
	KBPT_Equipment_Unique();
	~KBPT_Equipment_Unique();

// 以下是对外接口
public:
	const KBASICPROP_EQUIPMENT_UNIQUE* GetRecord(IN int) const;
	const KBASICPROP_EQUIPMENT_UNIQUE* FindRecord(IN int, IN int, IN int) const;

// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

// flying modify this class
// 黄金装备
class KBPT_Equipment_Gold : public KBasicPropertyTable
{
public:
	KBPT_Equipment_Gold();
	virtual ~KBPT_Equipment_Gold();
	const KBASICPROP_EQUIPMENT_GOLD* GetRecord(IN int) const;
	int GetRecordCount() const {return KBasicPropertyTable::NumOfEntries();};
protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

class KBPT_GoldMagic : public KBasicPropertyTable
{
public:
	KBPT_GoldMagic();
	virtual ~KBPT_GoldMagic();
	const KBASICPROP_GOLDMAGIC* GetRecord(IN int) const;
protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
};

class KBPT_MagicAttrib_TF : public KBasicPropertyTable
{
public:
	KBPT_MagicAttrib_TF();
	~KBPT_MagicAttrib_TF();

// 以下是辅助成员变量
protected:
	int m_naryMACount[2][MATF_CBDR];	// 每种装备可适用的魔法数目,分前后缀进行统计
										// 共有MATF_CBDR种装备可以具备魔法
// 以下是对外接口
public:
	void GetMACount(int*) const;
	const KMAGICATTRIB_TABFILE* GetRecord(IN int) const;

// 以下是辅助函数
protected:
	virtual BOOL LoadRecord(int i, KTabFile* pTF);
	void Init();
};
/*
class KBPT_MagicAttrib : public KBasicPropertyTable
{
public:
	KBPT_MagicAttrib();
	~KBPT_MagicAttrib();

// 以下是辅助函数
protected:
};
*/

//============================================================================

// Add by Freeway Chen in 2003.5.30
class KBPT_ClassMAIT    // Magic Item Index Table
{
public:
	KBPT_ClassMAIT();
	~KBPT_ClassMAIT();

// 以下是核心成员变量
protected:
	int*	m_pnTable;				// 缓冲区指针, 所存数据为
									// KBPT_MagicAttrib_TF::m_pBuf数组的下标
	int		m_nSize;				// 缓冲区内含多少项数据(并非字节数)

// 以下是辅助成员变量
	int		m_nNumOfValidData;		// 缓冲区中有效数据的个数
									// 初始化工作完成后m_nNumOfValidData < m_nSize
// 以下是对外接口
public:
    BOOL Clear();
	BOOL Insert(int nItemIndex);
	int  Get(int i) const;
    int  GetCount() const { return m_nNumOfValidData; }
};

//============================================================================

class KBPT_ClassifiedMAT
{
public:
	KBPT_ClassifiedMAT();
	~KBPT_ClassifiedMAT();

// 以下是核心成员变量
protected:
	int*	m_pnTable;				// 缓冲区指针, 所存数据为
									// KBPT_MagicAttrib_TF::m_pBuf数组的下标
	int		m_nSize;				// 缓冲区内含多少项数据(并非字节数)

// 以下是辅助成员变量
	int		m_nNumOfValidData;		// 缓冲区中有效数据的个数
									// 初始化工作完成后m_nNumOfValidData==m_nSize
// 以下是对外接口
public:
	BOOL GetMemory(int);
	BOOL Set(int);
	int Get(int) const;
	BOOL GetAll(int*, int*) const;

// 以下是辅助函数
protected:
	void ReleaseMemory();
};

class KLibOfBPT
{
public:
	KLibOfBPT();
	~KLibOfBPT();

// 以下是核心成员变量
protected:
	KBPT_Medicine			m_BPTMedicine;
	KBPT_TownPortal			m_BPTTownPortal;
	KBPT_Quest				m_BPTQuest;
	KBPT_MagicScript		m_BPTMagicScript;	// genre 6
	KBPT_Mine				m_BPTMine;
	KBPT_Equipment			m_BPTHorse;
	KBPT_Equipment			m_BPTMeleeWeapon;
	KBPT_Equipment			m_BPTRangeWeapon;
	KBPT_Equipment			m_BPTArmor;
	KBPT_Equipment			m_BPTHelm;
	KBPT_Equipment			m_BPTBoot;
	KBPT_Equipment			m_BPTBelt;
	KBPT_Equipment			m_BPTAmulet;
	KBPT_Equipment			m_BPTRing;
	KBPT_Equipment			m_BPTCuff;
	KBPT_Equipment			m_BPTPendant;
	KBPT_Equipment			m_BPTMask;	// mat na
	KBPT_Equipment			m_BPTMantle;	// phi phong
	KBPT_Equipment			m_BPTSignet;	// an
	KBPT_Equipment			m_BPTShipin;	// trang suc
	
    KBPT_MagicAttrib_TF		m_BPTMagicAttrib;
	// Add by flying
	KBPT_Equipment_Gold		m_GoldItem;
	KBPT_GoldMagic			m_GoldMagic;	// magicattrib_ge.txt
	std::map<int, KGOLD_SUITE>	m_GoldSuit;			// suit id -> parts
	std::map<int, int>		m_SuitActivate;			// suite_activate_count.txt
	std::map<int, int>		m_ExtSuitActivate[2];	// ext_suite_activate_count.txt
    // Add by Freeway Chen in 2003.5.30
	// 四维分别为前后缀、物品类型、五行、级别
    KBPT_ClassMAIT          m_CMAIT[MATF_PREFIXPOSFIX][MATF_CBDR][MATF_SERIES][MATF_LEVEL];

	KBPT_ClassifiedMAT		m_CMAT[2][MATF_CBDR];

// 以下是对外接口
public:
	BOOL Init();

	const KMAGICATTRIB_TABFILE* GetMARecord(IN int) const;
	const int					GetMARecordNumber() const;
    
    // Add by Freeway Chen in 2003.5.30
    const KBPT_ClassMAIT*       GetCMIT(IN int nPrefixPostfix, IN int nType, IN int nSeries, int nLevel) const;

	const KBPT_ClassifiedMAT*	GetCMAT(IN int, int) const;
	// Add by flying on 2003.6.2
	const KBASICPROP_EQUIPMENT_GOLD*	GetGoldItemRecord(IN int nIndex) const;
	const int							GetGoldItemNumber() const;
	const KBASICPROP_GOLDMAGIC*		GetGoldMagicRecord(IN int nIndex) const;
	const int							GetGoldMagicNumber() const;
	int								FindGoldSuite(const KMON_TRONG_BO* pMon, int nSoMon) const;
	int								GetExtSuitActivate(int nExtSuit, int nCap) const;
	const KBASICPROP_EQUIPMENT*	GetMeleeWeaponRecord(IN int) const;
	const int					GetMeleeWeaponRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetRangeWeaponRecord(IN int) const;
	const int					GetRangeWeaponRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetArmorRecord(IN int) const;
	const int					GetArmorRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetHelmRecord(IN int) const;
	const int					GetHelmRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetBootRecord(IN int) const;
	const int					GetBootRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetBeltRecord(IN int) const;
	const int					GetBeltRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetAmuletRecord(IN int) const;
	const int					GetAmuletRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetRingRecord(IN int) const;
	const int					GetRingRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetCuffRecord(IN int) const;
	const int					GetCuffRecordNumber() const;
	const KBASICPROP_EQUIPMENT*	GetPendantRecord(IN int) const;
	const int					GetPendantRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetHorseRecord(IN int) const;
	const int					GetHorseRecordNumber() const;
	const KBASICPROP_EQUIPMENT_UNIQUE* FindEquipmentUnique(IN int, IN int, IN int) const;
	const KBASICPROP_MEDMATERIAL* GetMedMaterial(IN int) const;
	const KBASICPROP_MEDICINE*	GetMedicineRecord(IN int) const;
	const int					GetMedicineRecordNumber() const;
	const KBASICPROP_MEDICINE*	FindMedicine(IN int, IN int) const;
	const KBASICPROP_MAGICSCRIPT*	FindMagicScript(IN int nDetailType, IN int nParticularType) const;
	const KBASICPROP_QUEST*		GetQuestRecord(IN int) const;
	const int					GetQuestRecordNumber() const;
	const KBASICPROP_TOWNPORTAL*	GetTownPortalRecord(IN int) const;
	const int					GetTownPortalRecordNumber() const;
	const KBASICPROP_MINE*		GetMine(IN int) const;
	const int					GetMineRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetMaskRecord(IN int) const;	// mat na
	const int					GetMaskRecordNumber() const;	// mat na
	const KBASICPROP_EQUIPMENT* GetMantleRecord(IN int) const;
	const int					GetMantleRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetSignetRecord(IN int) const;
	const int					GetSignetRecordNumber() const;
	const KBASICPROP_EQUIPMENT* GetShipinRecord(IN int) const;
	const int					GetShipinRecordNumber() const;

// 	const KBASICPROP_EQUIPMENT_GOLD*	GetGoldRecord(IN int) const;
// 	const int					GetGoldRecordNumber(IN int) const;
// 以下是辅助函数
protected:
	BOOL InitMALib();
	BOOL InitMagicScript();
	BOOL InitGoldEquip();
	void InitGoldSuite();
    
    // Add by Freeway Chen in 2003.5.30
    BOOL InitMAIT();
};
#endif		// #ifndef KBasPropTblH
