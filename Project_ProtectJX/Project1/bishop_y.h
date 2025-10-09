#define __int8 char
#define __int16 short
#define __int32 int
#define __int64 long long

struct _IO_FILE;
struct _IO_marker;
struct KService;
struct IMainUi;
struct ICommand;
struct CGamePlayer;
struct CIntercessor;
struct KGoddess;
struct KPaysys;
struct KMsgQueue::KBUFFER;
struct KSG_LogFile;
struct CGameServer;
struct CAccountLock;
struct trip_c::KTripManager;
struct trip_s::KTripManager;
struct trip_s::KTripSession;
struct KNode;
struct IKMemFileNotify;

/* 1 */
struct __attribute__((aligned(4))) Elf32_Sym
{
  unsigned __int32 st_name;
  unsigned __int32 st_value;
  unsigned __int32 st_size;
  unsigned __int8 st_info;
  unsigned __int8 st_other;
  unsigned __int16 st_shndx;
};

/* 2 */
struct Elf32_Rel
{
  unsigned __int32 r_offset;
  unsigned __int32 r_info;
};

/* 3 */
union Elf32_Dyn::$A263394DDF3EC2D4B1B8448EDD30E249
{
  unsigned __int32 d_val;
  unsigned __int32 d_ptr;
};

/* 4 */
struct Elf32_Dyn
{
  __int32 d_tag;
  union Elf32_Dyn::$A263394DDF3EC2D4B1B8448EDD30E249 d_un;
};

/* 5 */
struct __attribute__((aligned(4))) Elf32_Verneed
{
  unsigned __int16 vn_version;
  unsigned __int16 vn_cnt;
  unsigned __int32 vn_file;
  unsigned __int32 vn_aux;
  unsigned __int32 vn_next;
};

/* 6 */
struct __attribute__((aligned(4))) Elf32_Vernaux
{
  unsigned __int32 vna_hash;
  unsigned __int16 vna_flags;
  unsigned __int16 vna_other;
  unsigned __int32 vna_name;
  unsigned __int32 vna_next;
};

/* 7 */
typedef unsigned int size_t;

/* 8 */
struct $9519CC713B5CFE3FF067CE18EB76294E
{
  int quot;
  int rem;
};

/* 9 */
struct $241628C28F1B7EA5D29A3387C8D9DB15
{
  __int64 quot;
  __int64 rem;
};

/* 10 */
typedef __int64 __quad_t;

/* 11 */
typedef int __off_t;

/* 12 */
typedef __quad_t __off64_t;

/* 13 */
typedef int __clock_t;

/* 14 */
typedef int __time_t;

/* 15 */
typedef __clock_t clock_t;

/* 16 */
typedef __time_t time_t;

/* 17 */
typedef int (*__compar_fn_t)(const void *, const void *);

/* 18 */
typedef char CHAR;

/* 19 */
typedef int BOOL;

/* 20 */
typedef unsigned __int8 BYTE;

/* 21 */
typedef const CHAR *LPCSTR;

/* 22 */
typedef void *HANDLE;

/* 23 */
typedef HANDLE HINSTANCE;

/* 24 */
struct std::nothrow_t
{
  __int8 gap0[1];
};

/* 25 */
enum std::float_denorm_style : __int32
{
  denorm_indeterminate = 0xFFFFFFFF,
  denorm_absent = 0x0,
  denorm_present = 0x1,
};

/* 26 */
typedef int ptrdiff_t;

/* 27 */
typedef int wchar_t;

/* 28 */
typedef _IO_FILE FILE;

/* 31 */
typedef void _IO_lock_t;

/* 29 */
struct _IO_FILE
{
  int _flags;
  char *_IO_read_ptr;
  char *_IO_read_end;
  char *_IO_read_base;
  char *_IO_write_base;
  char *_IO_write_ptr;
  char *_IO_write_end;
  char *_IO_buf_base;
  char *_IO_buf_end;
  char *_IO_save_base;
  char *_IO_backup_base;
  char *_IO_save_end;
  _IO_marker *_markers;
  _IO_FILE *_chain;
  int _fileno;
  int _flags2;
  __off_t _old_offset;
  unsigned __int16 _cur_column;
  char _vtable_offset;
  char _shortbuf[1];
  _IO_lock_t *_lock;
  __off64_t _offset;
  void *__pad1;
  void *__pad2;
  void *__pad3;
  void *__pad4;
  size_t __pad5;
  int _mode;
  char _unused2[40];
};

/* 30 */
struct _IO_marker
{
  _IO_marker *_next;
  _IO_FILE *_sbuf;
  int _pos;
};

/* 32 */
typedef _IO_FILE __FILE;

/* 33 */
typedef unsigned int wint_t;

/* 35 */
union mbstate_t::$582B34FC9269E8CFE0F03E4FFEEB0D92
{
  wint_t __wch;
  char __wchb[4];
};

/* 34 */
struct mbstate_t
{
  int __count;
  mbstate_t::$582B34FC9269E8CFE0F03E4FFEEB0D92 __value;
};

/* 36 */
struct fpos_t
{
  __off_t __pos;
  mbstate_t __state;
};

/* 37 */
typedef char *__gnuc_va_list;

/* 38 */
struct __attribute__((aligned(4))) lconv
{
  char *decimal_point;
  char *thousands_sep;
  char *grouping;
  char *int_curr_symbol;
  char *currency_symbol;
  char *mon_decimal_point;
  char *mon_thousands_sep;
  char *mon_grouping;
  char *positive_sign;
  char *negative_sign;
  char int_frac_digits;
  char frac_digits;
  char p_cs_precedes;
  char p_sep_by_space;
  char n_cs_precedes;
  char n_sep_by_space;
  char p_sign_posn;
  char n_sign_posn;
  char int_p_cs_precedes;
  char int_p_sep_by_space;
  char int_n_cs_precedes;
  char int_n_sep_by_space;
  char int_p_sign_posn;
  char int_n_sign_posn;
};

/* 39 */
struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
  int tm_gmtoff;
  const char *tm_zone;
};

/* 40 */
struct __attribute__((aligned(4))) KPARAM
{
  char szIntranetIp[32];
  char szInternetIp[32];
  char szAccount[32];
  char szPassword[64];
  char szPaysysIp[32];
  int nPaysysPort;
  int nPaysysBufferSize;
  char szGoddessIp[32];
  int nGoddessPort;
  int nGoddessBufferSize;
  int nClientPort;
  int nClientBufferSize;
  int nDenialPort;
  int nGameServerPort;
  int nGameServerBufferSize;
  BYTE szMacAddress[6];
};

/* 41 */
struct KApplication
{
  KService *m_pService;
  IMainUi *m_pUi;
};

/* 491 */
struct IService
{
  int (**_vptr_IService)(...);
};

/* 538 */
struct ICoder
{
  int (**_vptr_ICoder)(...);
};

/* 534 */
struct KDll
{
  void *m_hDll;
};

/* 327 */
struct IServer;

/* 537 */
struct __cppobj KHeavenLib : ICoder
{
  KDll m_cDll;
  BOOL (*m_pfnCreate)(unsigned int, unsigned int, IServer **);
};

/* 540 */
struct IClientManager;

/* 539 */
struct __cppobj KRainbowLib : ICoder
{
  IClientManager *m_pClientManager;
  KDll m_cDll;
};

/* 112 */
typedef unsigned int pthread_t;

/* 306 */
struct KSimpleThread
{
  void (*m_pfnFunction)(void *);
  pthread_t m_hThread;
  void *m_pParam;
};

/* 49 */
typedef unsigned int DWORD;

/* 283 */
struct IKLock
{
  int (**_vptr_IKLock)(...);
};

/* 113 */
struct __pthread_internal_slist
{
  __pthread_internal_slist *__next;
};

/* 114 */
typedef __pthread_internal_slist __pthread_slist_t;

/* 117 */
union $2B1E557A35EB785217A56E83668575CF::__pthread_mutex_s::$0BD8A1A0B142CC6BF06E3D3E9C4F6678
{
  int __spins;
  __pthread_slist_t __list;
};

/* 116 */
struct $2B1E557A35EB785217A56E83668575CF::__pthread_mutex_s
{
  int __lock;
  unsigned int __count;
  int __owner;
  int __kind;
  unsigned int __nusers;
  $2B1E557A35EB785217A56E83668575CF::__pthread_mutex_s::$0BD8A1A0B142CC6BF06E3D3E9C4F6678 _anon_0;
};

/* 115 */
union $2B1E557A35EB785217A56E83668575CF
{
  $2B1E557A35EB785217A56E83668575CF::__pthread_mutex_s __data;
  char __size[24];
  int __align;
};

/* 284 */
struct __cppobj KThreadLock : IKLock
{
  $2B1E557A35EB785217A56E83668575CF m_sSection;
};

/* 42 */
struct __cppobj KService : IService
{
  char *m_pBuffer;
  size_t m_nBufLen;
  IMainUi *m_pUi;
  CIntercessor *m_pIntercessor;
  KHeavenLib m_cServerLib;
  KRainbowLib m_cClientLib;
  KPaysys *m_pPaysys;
  KSimpleThread m_cThread;
  BOOL m_bStop;
  time_t m_tLastLiveTime;
  DWORD m_dwStartTime;
  CAccountLock *m_pAccountLock;
  KThreadLock m_cLock;
};

/* 43 */
struct IMainUi
{
  int (**_vptr_IMainUi)(...);
};

/* 70 */
struct std::binary_function<std::string,std::string,bool>
{
  __int8 gap0[1];
};

/* 71 */
struct __cppobj std::less<std::string > : std::binary_function<std::string,std::string,bool>
{
};

/* 62 */
enum std::_Rb_tree_color : __int32
{
  _S_red = 0x0,
  _S_black = 0x1,
};

/* 63 */
struct std::_Rb_tree_node_base
{
  std::_Rb_tree_color _M_color;
  std::_Rb_tree_node_base *_M_parent;
  std::_Rb_tree_node_base *_M_left;
  std::_Rb_tree_node_base *_M_right;
};

/* 227 */
struct std::_Rb_tree<std::string,std::pair<const std::string,CGamePlayer*>,std::_Select1st<std::pair<const std::string,CGamePlayer*> >,std::less<std::string >,std::allocator<std::pair<const std::string,CGamePlayer*> > >::_Rb_tree_impl<std::less<std::string >,false>
{
  std::less<std::string > _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 226 */
struct std::_Rb_tree<std::string,std::pair<const std::string,CGamePlayer*>,std::_Select1st<std::pair<const std::string,CGamePlayer*> >,std::less<std::string >,std::allocator<std::pair<const std::string,CGamePlayer*> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,CGamePlayer*>,std::_Select1st<std::pair<const std::string,CGamePlayer*> >,std::less<std::string >,std::allocator<std::pair<const std::string,CGamePlayer*> > >::_Rb_tree_impl<std::less<std::string >,false> _M_impl;
};

/* 235 */
struct std::map<std::string,CGamePlayer*>
{
  std::_Rb_tree<std::string,std::pair<const std::string,CGamePlayer*>,std::_Select1st<std::pair<const std::string,CGamePlayer*> >,std::less<std::string >,std::allocator<std::pair<const std::string,CGamePlayer*> > > _M_t;
};

/* 170 */
struct std::binary_function<unsigned int,unsigned int,bool>
{
  __int8 gap0[1];
};

/* 169 */
struct __cppobj std::less<unsigned int> : std::binary_function<unsigned int,unsigned int,bool>
{
};

/* 431 */
struct std::_Rb_tree<unsigned int,std::pair<const unsigned int,std::list<CGameServer*> >,std::_Select1st<std::pair<const unsigned int,std::list<CGameServer*> > >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,std::list<CGameServer*> > > >::_Rb_tree_impl<std::less<unsigned int>,false>
{
  std::less<unsigned int> _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 430 */
struct std::_Rb_tree<unsigned int,std::pair<const unsigned int,std::list<CGameServer*> >,std::_Select1st<std::pair<const unsigned int,std::list<CGameServer*> > >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,std::list<CGameServer*> > > >
{
  std::_Rb_tree<unsigned int,std::pair<const unsigned int,std::list<CGameServer*> >,std::_Select1st<std::pair<const unsigned int,std::list<CGameServer*> > >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,std::list<CGameServer*> > > >::_Rb_tree_impl<std::less<unsigned int>,false> _M_impl;
};

/* 439 */
struct std::map<unsigned int,std::list<CGameServer*>>
{
  std::_Rb_tree<unsigned int,std::pair<const unsigned int,std::list<CGameServer*> >,std::_Select1st<std::pair<const unsigned int,std::list<CGameServer*> > >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,std::list<CGameServer*> > > > _M_t;
};

/* 207 */
enum ServiceType : __int32
{
  SERVICE_NORMAL = 0x0,
  SERVICE_DENIAL = 0x1,
  SERVICE_COUNT = 0x2,
};

/* 441 */
struct CIntercessor::ServiceParam
{
  CIntercessor *pIntercessor;
  ServiceType nServiceType;
};

/* 402 */
struct std::_Vector_base<CGamePlayer*>::_Vector_impl
{
  CGamePlayer **_M_start;
  CGamePlayer **_M_finish;
  CGamePlayer **_M_end_of_storage;
};

/* 401 */
struct std::_Vector_base<CGamePlayer*>
{
  std::_Vector_base<CGamePlayer*>::_Vector_impl _M_impl;
};

/* 403 */
struct __cppobj std::vector<CGamePlayer*> : std::_Vector_base<CGamePlayer*>
{
};

/* 398 */
struct std::_List_node_base
{
  std::_List_node_base *_M_next;
  std::_List_node_base *_M_prev;
};

/* 413 */
struct std::_List_base<std::pair<long unsigned int,CGamePlayer*>>::_List_impl
{
  std::_List_node_base _M_node;
};

/* 412 */
struct std::_List_base<std::pair<long unsigned int,CGamePlayer*>>
{
  std::_List_base<std::pair<long unsigned int,CGamePlayer*>>::_List_impl _M_impl;
};

/* 414 */
struct __cppobj std::list<std::pair<long unsigned int,CGamePlayer*>> : std::_List_base<std::pair<long unsigned int,CGamePlayer*>>
{
};

/* 442 */
struct CIntercessor::ServerItem
{
  std::vector<CGamePlayer*> players;
  std::list<std::pair<long unsigned int,CGamePlayer*>> pActiveList;
  IServer *pServer;
  int nMaxCount;
};

/* 315 */
struct $DBE2C87402ECD8AE6ACE3192AA0CE987
{
  bool (*__pfn)(CGameServer *, const void *, size_t);
  int __delta;
};

/* 443 */
struct CIntercessor::ProtocolItem
{
  size_t size;
  $DBE2C87402ECD8AE6ACE3192AA0CE987 pfnCheck;
};

/* 446 */
struct std::_Rb_tree<unsigned int,std::pair<const unsigned int,CGameServer*>,std::_Select1st<std::pair<const unsigned int,CGameServer*> >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,CGameServer*> > >::_Rb_tree_impl<std::less<unsigned int>,false>
{
  std::less<unsigned int> _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 445 */
struct std::_Rb_tree<unsigned int,std::pair<const unsigned int,CGameServer*>,std::_Select1st<std::pair<const unsigned int,CGameServer*> >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,CGameServer*> > >
{
  std::_Rb_tree<unsigned int,std::pair<const unsigned int,CGameServer*>,std::_Select1st<std::pair<const unsigned int,CGameServer*> >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,CGameServer*> > >::_Rb_tree_impl<std::less<unsigned int>,false> _M_impl;
};

/* 444 */
struct std::map<unsigned int,CGameServer*>
{
  std::_Rb_tree<unsigned int,std::pair<const unsigned int,CGameServer*>,std::_Select1st<std::pair<const unsigned int,CGameServer*> >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,CGameServer*> > > _M_t;
};

/* 208 */
struct CIntercessor
{
  int (**_vptr_CIntercessor)(...);
  std::map<std::string,CGamePlayer*> m_sthePlayerTable;
  std::map<unsigned int,std::list<CGameServer*>> m_theMapIDConvert;
  CIntercessor::ServiceParam m_servParams[2];
  CIntercessor::ServerItem m_itemServers[2];
  IServer *m_pGameSvrServer;
  IServer *m_pPlayerServer;
  IServer *m_pDenialServer;
  KPaysys *m_pPaysys;
  DWORD m_dwLastPingPaysys;
  DWORD m_dwIdentity;
  trip_c::KTripManager *m_pTripClientManager;
  trip_s::KTripManager *m_pTripServerManager;
  KGoddess *m_pGoddess;
  const unsigned int m_lnMaxPlayerCount;
  int m_nMaxServerCount;
  CIntercessor::ProtocolItem m_c2sProtocolItems[254];
  DWORD m_dwLastStatTime;
  DWORD m_dwGodessPingTimer;
  KService *m_pService;
  unsigned __int16 m_nClientPort;
  std::map<unsigned int,CGameServer*> m_mapGameServer;
};

/* 210 */
struct IClient;

/* 308 */
struct KReconnectableClient::KBUFFER
{
  char *pBuffer;
  size_t nBufLen;
  size_t nData;
  size_t nSize;
};

/* 307 */
struct KReconnectableClient
{
  int (**_vptr_KReconnectableClient)(...);
  unsigned int m_nBindIp;
  KSimpleThread m_cThread;
  IClient *m_pClient;
  char m_szIp[16];
  unsigned __int16 m_nPort;
  BOOL m_bStop;
  BOOL m_bReconnect;
  BOOL m_bConnected;
  KThreadLock m_cLock;
  KReconnectableClient::KBUFFER m_sBuffer;
  size_t m_nBufLen;
  BOOL m_bEnableReconnect;
};

/* 213 */
struct __cppobj KPaysys : KReconnectableClient
{
  KPARAM m_sParam;
  KService *m_pService;
  time_t m_tLiveTime;
  KThreadLock m_cLock;
};

/* 178 */
struct std::_Rb_tree<std::string,std::pair<const std::string,long unsigned int>,std::_Select1st<std::pair<const std::string,long unsigned int> >,std::less<std::string >,std::allocator<std::pair<const std::string,long unsigned int> > >::_Rb_tree_impl<std::less<std::string >,false>
{
  std::less<std::string > _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 177 */
struct std::_Rb_tree<std::string,std::pair<const std::string,long unsigned int>,std::_Select1st<std::pair<const std::string,long unsigned int> >,std::less<std::string >,std::allocator<std::pair<const std::string,long unsigned int> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,long unsigned int>,std::_Select1st<std::pair<const std::string,long unsigned int> >,std::less<std::string >,std::allocator<std::pair<const std::string,long unsigned int> > >::_Rb_tree_impl<std::less<std::string >,false> _M_impl;
};

/* 186 */
struct std::map<std::string,long unsigned int>
{
  std::_Rb_tree<std::string,std::pair<const std::string,long unsigned int>,std::_Select1st<std::pair<const std::string,long unsigned int> >,std::less<std::string >,std::allocator<std::pair<const std::string,long unsigned int> > > _M_t;
};

/* 319 */
struct CAccountLock
{
  std::map<std::string,long unsigned int> m_mapLocks;
  DWORD m_dwLastTime;
};

/* 206 */
struct IGoddessMsgHandler
{
  int (**_vptr_IGoddessMsgHandler)(...);
};

/* 47 */
typedef int LONG;

/* 159 */
struct std::_Vector_base<ICommand*>::_Vector_impl
{
  ICommand **_M_start;
  ICommand **_M_finish;
  ICommand **_M_end_of_storage;
};

/* 158 */
struct std::_Vector_base<ICommand*>
{
  std::_Vector_base<ICommand*>::_Vector_impl _M_impl;
};

/* 161 */
struct __cppobj std::vector<ICommand*> : std::_Vector_base<ICommand*>
{
};

/* 52 */
typedef unsigned int UINT;

/* 214 */
struct KPlayerTask
{
  CGamePlayer *m_pReceiver;
  std::vector<ICommand*> m_stdCommand;
  size_t m_indexCmd;
  UINT m_nTaskProgID;
};

/* 59 */
struct std::string::_Alloc_hider
{
  char *_M_p;
};

/* 58 */
struct std::string
{
  std::string::_Alloc_hider _M_dataplus;
};

/* 215 */
typedef std::string string;

/* 216 */
struct KMsgQueue
{
  KMsgQueue::KBUFFER *m_aryBuffer[255];
};

/* 219 */
struct tagExtPointInfo
{
  int nExtPoint[8];
};

/* 220 */
struct KACCOUNT_LIMITTIME_INFO
{
  DWORD dwLimitTimeFlag;
  DWORD dwLimitTimeOnlineSeconds;
  DWORD dwLimitTimeOfflineSeconds;
};

/* 218 */
struct tagAccountUserInfo
{
  tagExtPointInfo ExtPointInfo;
  DWORD nTotalLeftTime;
  DWORD nLeftTimeOfPoint;
  KACCOUNT_LIMITTIME_INFO LimitTimeInfo;
};

/* 205 */
struct __cppobj CGamePlayer : IGoddessMsgHandler
{
  ServiceType m_nServiceType;
  BOOL m_bOffline;
  LONG m_lnIdentityID;
  DWORD m_dwRecvIdentityID;
  DWORD m_dwGoddessIdentity;
  BOOL m_bWaiting;
  CIntercessor *m_pIntercessor;
  KGoddess *m_pGoddess;
  KPaysys *m_pPaysys;
  KService *m_pService;
  KPlayerTask m_theLoginTask;
  KPlayerTask m_theSelAddDelTask;
  KPlayerTask m_theLoginCreateRoleTask;
  KPlayerTask m_theLoginSelectRoleTask;
  KPlayerTask m_theLoginDeleteRoleTask;
  LONG m_nCurrentTaskID;
  string m_sAccountName;
  string m_sPassword;
  string m_sRoleName;
  char m_szRoleNameList[15][32];
  int m_nNumRoleInList;
  int m_nRefuseLoginRoleCount;
  BOOL m_bUseSuperPassword;
  BOOL m_bKickOutOldAcc;
  string m_sSuperPassword;
  string m_sDelRoleName;
  size_t m_nAttachServerID;
  int m_nActiveStatus;
  DWORD m_dwTaskBeginTimer;
  DWORD m_dwTaskTotalTimer;
  bool m_bAutoUnlockAccount;
  KMsgQueue m_theDataQueue[3];
  tagAccountUserInfo m_AccountInfo;
};

/* 194 */
struct __cppobj trip_c::KTripManager::icmp_string : std::binary_function<std::string,std::string,bool>
{
};

/* 193 */
struct std::_Rb_tree<std::string,std::string,std::_Identity<std::string >,trip_c::KTripManager::icmp_string,std::allocator<std::string > >::_Rb_tree_impl<trip_c::KTripManager::icmp_string,false>
{
  trip_c::KTripManager::icmp_string _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 192 */
struct std::_Rb_tree<std::string,std::string,std::_Identity<std::string >,trip_c::KTripManager::icmp_string,std::allocator<std::string > >
{
  std::_Rb_tree<std::string,std::string,std::_Identity<std::string >,trip_c::KTripManager::icmp_string,std::allocator<std::string > >::_Rb_tree_impl<trip_c::KTripManager::icmp_string,false> _M_impl;
};

/* 202 */
struct std::set<std::string,trip_c::KTripManager::icmp_string,std::allocator<std::string > >
{
  std::_Rb_tree<std::string,std::string,std::_Identity<std::string >,trip_c::KTripManager::icmp_string,std::allocator<std::string > > _M_t;
};

/* 325 */
struct trip_c::KTripManager
{
  std::set<std::string,trip_c::KTripManager::icmp_string,std::allocator<std::string > > m_setAccount;
  KSG_LogFile *m_pLog;
};

/* 576 */
struct std::_Rb_tree<std::string,std::pair<const std::string,trip_s::KTripManager::KINFO>,std::_Select1st<std::pair<const std::string,trip_s::KTripManager::KINFO> >,std::less<std::string >,std::allocator<std::pair<const std::string,trip_s::KTripManager::KINFO> > >::_Rb_tree_impl<std::less<std::string >,false>
{
  std::less<std::string > _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 575 */
struct std::_Rb_tree<std::string,std::pair<const std::string,trip_s::KTripManager::KINFO>,std::_Select1st<std::pair<const std::string,trip_s::KTripManager::KINFO> >,std::less<std::string >,std::allocator<std::pair<const std::string,trip_s::KTripManager::KINFO> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,trip_s::KTripManager::KINFO>,std::_Select1st<std::pair<const std::string,trip_s::KTripManager::KINFO> >,std::less<std::string >,std::allocator<std::pair<const std::string,trip_s::KTripManager::KINFO> > >::_Rb_tree_impl<std::less<std::string >,false> _M_impl;
};

/* 574 */
struct std::map<std::string,trip_s::KTripManager::KINFO>
{
  std::_Rb_tree<std::string,std::pair<const std::string,trip_s::KTripManager::KINFO>,std::_Select1st<std::pair<const std::string,trip_s::KTripManager::KINFO> >,std::less<std::string >,std::allocator<std::pair<const std::string,trip_s::KTripManager::KINFO> > > _M_t;
};

/* 127 */
struct std::binary_function<long unsigned int,long unsigned int,bool>
{
  __int8 gap0[1];
};

/* 128 */
struct __cppobj std::less<long unsigned int> : std::binary_function<long unsigned int,long unsigned int,bool>
{
};

/* 581 */
struct std::_Rb_tree<long unsigned int,std::pair<const long unsigned int,trip_s::KTripSession*>,std::_Select1st<std::pair<const long unsigned int,trip_s::KTripSession*> >,std::less<long unsigned int>,std::allocator<std::pair<const long unsigned int,trip_s::KTripSession*> > >::_Rb_tree_impl<std::less<long unsigned int>,false>
{
  std::less<long unsigned int> _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 580 */
struct std::_Rb_tree<long unsigned int,std::pair<const long unsigned int,trip_s::KTripSession*>,std::_Select1st<std::pair<const long unsigned int,trip_s::KTripSession*> >,std::less<long unsigned int>,std::allocator<std::pair<const long unsigned int,trip_s::KTripSession*> > >
{
  std::_Rb_tree<long unsigned int,std::pair<const long unsigned int,trip_s::KTripSession*>,std::_Select1st<std::pair<const long unsigned int,trip_s::KTripSession*> >,std::less<long unsigned int>,std::allocator<std::pair<const long unsigned int,trip_s::KTripSession*> > >::_Rb_tree_impl<std::less<long unsigned int>,false> _M_impl;
};

/* 579 */
struct std::map<long unsigned int,trip_s::KTripSession*>
{
  std::_Rb_tree<long unsigned int,std::pair<const long unsigned int,trip_s::KTripSession*>,std::_Select1st<std::pair<const long unsigned int,trip_s::KTripSession*> >,std::less<long unsigned int>,std::allocator<std::pair<const long unsigned int,trip_s::KTripSession*> > > _M_t;
};

/* 326 */
struct __cppobj trip_s::KTripManager : IGoddessMsgHandler
{
  std::map<std::string,trip_s::KTripManager::KINFO> m_mapSession;
  std::map<long unsigned int,trip_s::KTripSession*> m_mapHandlers;
  CIntercessor *m_pIntercessor;
  KGoddess *m_pGoddess;
  char *m_pBuffer;
  size_t m_nBufLen;
  KSG_LogFile *m_pLog;
};

/* 211 */
struct KSendPack
{
  BYTE m_byProtocol;
  unsigned int m_nUserData;
  char *m_pBuffer;
  size_t m_nBufLen;
  size_t m_nOffset;
  size_t m_nDataLen;
  size_t m_nPackIdx;
  char *m_pPack;
  size_t m_nCurPackLen;
  size_t m_nMaxPackLen;
};

/* 212 */
struct KRecvPack
{
  char *m_pBuffer;
  size_t m_nBufLen;
  size_t m_nDataLen;
  BOOL m_bComplete;
};

/* 130 */
struct std::_Rb_tree<long unsigned int,std::pair<const long unsigned int,IGoddessMsgHandler*>,std::_Select1st<std::pair<const long unsigned int,IGoddessMsgHandler*> >,std::less<long unsigned int>,std::allocator<std::pair<const long unsigned int,IGoddessMsgHandler*> > >::_Rb_tree_impl<std::less<long unsigned int>,false>
{
  std::less<long unsigned int> _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 129 */
struct std::_Rb_tree<long unsigned int,std::pair<const long unsigned int,IGoddessMsgHandler*>,std::_Select1st<std::pair<const long unsigned int,IGoddessMsgHandler*> >,std::less<long unsigned int>,std::allocator<std::pair<const long unsigned int,IGoddessMsgHandler*> > >
{
  std::_Rb_tree<long unsigned int,std::pair<const long unsigned int,IGoddessMsgHandler*>,std::_Select1st<std::pair<const long unsigned int,IGoddessMsgHandler*> >,std::less<long unsigned int>,std::allocator<std::pair<const long unsigned int,IGoddessMsgHandler*> > >::_Rb_tree_impl<std::less<long unsigned int>,false> _M_impl;
};

/* 138 */
struct std::map<long unsigned int,IGoddessMsgHandler*>
{
  std::_Rb_tree<long unsigned int,std::pair<const long unsigned int,IGoddessMsgHandler*>,std::_Select1st<std::pair<const long unsigned int,IGoddessMsgHandler*> >,std::less<long unsigned int>,std::allocator<std::pair<const long unsigned int,IGoddessMsgHandler*> > > _M_t;
};

/* 209 */
struct KGoddess
{
  IClient *m_pClient;
  KService *m_pService;
  CIntercessor *m_pIntercessor;
  KSendPack m_cSendPack;
  KRecvPack m_cRecvPack;
  std::map<long unsigned int,IGoddessMsgHandler*> m_mapHandler;
  DWORD m_dwIdentity;
};

/* 144 */
struct __cppobj string_icmp : std::binary_function<std::string,std::string,bool>
{
};

/* 141 */
struct std::_Rb_tree<std::string,std::pair<const std::string,void*>,std::_Select1st<std::pair<const std::string,void*> >,string_icmp,std::allocator<std::pair<const std::string,void*> > >::_Rb_tree_impl<string_icmp,false>
{
  string_icmp _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 140 */
struct std::_Rb_tree<std::string,std::pair<const std::string,void*>,std::_Select1st<std::pair<const std::string,void*> >,string_icmp,std::allocator<std::pair<const std::string,void*> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,void*>,std::_Select1st<std::pair<const std::string,void*> >,string_icmp,std::allocator<std::pair<const std::string,void*> > >::_Rb_tree_impl<string_icmp,false> _M_impl;
};

/* 139 */
struct std::map<std::string,void*,string_icmp,std::allocator<std::pair<const std::string,void*> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,void*>,std::_Select1st<std::pair<const std::string,void*> >,string_icmp,std::allocator<std::pair<const std::string,void*> > > _M_t;
};

/* 313 */
typedef std::map<std::string,void*,string_icmp,std::allocator<std::pair<const std::string,void*> > > stdAccountAttachIn;

/* 314 */
struct CGameServer
{
  $DBE2C87402ECD8AE6ACE3192AA0CE987 m_theProcessProtocolFun[254];
  BOOL m_bRelayServer;
  LONG m_lnIdentityID;
  CIntercessor *m_pIntercessor;
  KService *m_pService;
  KPaysys *m_pPaysys;
  KSendPack m_cSendPack;
  string m_sServerIPAddr_Intraner;
  string m_sServerIPAddr_Internet;
  DWORD m_dwIntranetIp;
  DWORD m_dwInternetIp;
  unsigned __int16 m_nServerPort;
  DWORD m_dwCapability;
  stdAccountAttachIn m_theAccountInThisServer;
  BOOL m_bReady;
};

/* 160 */
struct ICommand
{
  int (**_vptr_ICommand)(...);
};

/* 217 */
struct KMsgQueue::KBUFFER
{
  size_t nBuffLen;
  size_t nDataLen;
};

/* 109 */
enum KE_LOGLEVEL : __int32
{
  emLOGLEVEL_FATAL = 0x0,
  emLOGLEVEL_ERROR = 0x1,
  emLOGLEVEL_WARN = 0x2,
  emLOGLEVEL_INFO = 0x3,
  emLOGLEVEL_DEBUG = 0x4,
};

/* 309 */
struct KSG_LogFile
{
  int (**_vptr_KSG_LogFile)(...);
  FILE *m_pLog;
  UINT m_uDayOfYear;
  BOOL m_bFileNameAutoChangeWithDate;
  char m_szFileNameKey[260];
  char m_szSuffixName[20];
  KE_LOGLEVEL m_nLevel;
  size_t m_nFileSize;
  char m_szFile[260];
};

/* 53 */
typedef CHAR *LPSTR;

/* 106 */
struct tagKeyNode
{
  DWORD dwID;
  LPSTR pKey;
  LPSTR pValue;
  tagKeyNode *pNextNode;
};

/* 105 */
struct tagSecNode
{
  DWORD dwID;
  LPSTR pSection;
  tagKeyNode pKeyNode;
  tagSecNode *pNextNode;
};

/* 104 */
typedef tagSecNode SECNODE;

/* 50 */
typedef BYTE *PBYTE;

/* 107 */
struct KMemStack
{
  PBYTE m_pStack[10];
  int m_nStackTop;
  int m_nStackEnd;
  int m_nChunkTop;
  int m_nChunkSize;
};

/* 44 */
struct KIniFile
{
  SECNODE m_Header;
  LONG m_Offset;
  KMemStack m_MemStack;
};

/* 45 */
typedef unsigned int __rlim_t;

/* 46 */
typedef char *__caddr_t;

/* 48 */
typedef void *PVOID;

/* 51 */
typedef void *LPVOID;

/* 54 */
struct tagRECT
{
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
};

/* 55 */
typedef tagRECT RECT;

/* 57 */
struct __gnu_cxx::new_allocator<char>
{
  __int8 gap0[1];
};

/* 56 */
struct __cppobj std::allocator<char> : __gnu_cxx::new_allocator<char>
{
};

/* 60 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<const char*,std::string > >;

/* 61 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<char*,std::string > >;

/* 64 */
struct std::pair<const std::string,KE_LOGLEVEL>
{
  const std::string first;
  KE_LOGLEVEL second;
};

/* 66 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,KE_LOGLEVEL> >
{
  __int8 gap0[1];
};

/* 65 */
struct __cppobj std::allocator<std::pair<const std::string,KE_LOGLEVEL> > : __gnu_cxx::new_allocator<std::pair<const std::string,KE_LOGLEVEL> >
{
};

/* 68 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,KE_LOGLEVEL> > >
{
  __int8 gap0[1];
};

/* 67 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,KE_LOGLEVEL> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,KE_LOGLEVEL> > >
{
};

/* 69 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,KE_LOGLEVEL> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,KE_LOGLEVEL> _M_value_field;
};

/* 73 */
struct std::_Rb_tree<std::string,std::pair<const std::string,KE_LOGLEVEL>,std::_Select1st<std::pair<const std::string,KE_LOGLEVEL> >,std::less<std::string >,std::allocator<std::pair<const std::string,KE_LOGLEVEL> > >::_Rb_tree_impl<std::less<std::string >,false>
{
  std::less<std::string > _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 72 */
struct std::_Rb_tree<std::string,std::pair<const std::string,KE_LOGLEVEL>,std::_Select1st<std::pair<const std::string,KE_LOGLEVEL> >,std::less<std::string >,std::allocator<std::pair<const std::string,KE_LOGLEVEL> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,KE_LOGLEVEL>,std::_Select1st<std::pair<const std::string,KE_LOGLEVEL> >,std::less<std::string >,std::allocator<std::pair<const std::string,KE_LOGLEVEL> > >::_Rb_tree_impl<std::less<std::string >,false> _M_impl;
};

/* 74 */
struct std::_Rb_tree_iterator<std::pair<const std::string,KE_LOGLEVEL> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 75 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,KE_LOGLEVEL> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 76 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,KE_LOGLEVEL> > >;

/* 77 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,KE_LOGLEVEL> > >;

/* 78 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,KE_LOGLEVEL> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,KE_LOGLEVEL> > first;
  bool second;
};

/* 79 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,KE_LOGLEVEL> >,std::_Rb_tree_iterator<std::pair<const std::string,KE_LOGLEVEL> > >;

/* 80 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,KE_LOGLEVEL> >,std::_Rb_tree_const_iterator<std::pair<const std::string,KE_LOGLEVEL> > >;

/* 81 */
struct std::map<std::string,KE_LOGLEVEL>
{
  std::_Rb_tree<std::string,std::pair<const std::string,KE_LOGLEVEL>,std::_Select1st<std::pair<const std::string,KE_LOGLEVEL> >,std::less<std::string >,std::allocator<std::pair<const std::string,KE_LOGLEVEL> > > _M_t;
};

/* 82 */
struct __gnu_cxx::__normal_iterator<char*,std::string >
{
  char *_M_current;
};

/* 83 */
struct __gnu_cxx::__normal_iterator<const char*,std::string >
{
  const char *_M_current;
};

/* 84 */
typedef __gnuc_va_list va_list;

/* 85 */
typedef unsigned __int16 uint16_t;

/* 86 */
typedef unsigned int uint32_t;

/* 87 */
typedef int _Atomic_word;

/* 88 */
struct std::string::_Rep_base
{
  size_t _M_length;
  size_t _M_capacity;
  _Atomic_word _M_refcount;
};

/* 89 */
struct __cppobj std::string::_Rep : std::string::_Rep_base
{
};

/* 90 */
typedef unsigned __int16 sa_family_t;

/* 91 */
struct sockaddr
{
  sa_family_t sa_family;
  char sa_data[14];
};

/* 92 */
typedef __rlim_t rlim_t;

/* 93 */
struct rlimit
{
  rlim_t rlim_cur;
  rlim_t rlim_max;
};

/* 94 */
typedef uint16_t in_port_t;

/* 95 */
typedef uint32_t in_addr_t;

/* 96 */
struct in_addr
{
  in_addr_t s_addr;
};

/* 97 */
struct sockaddr_in
{
  sa_family_t sin_family;
  in_port_t sin_port;
  in_addr sin_addr;
  unsigned __int8 sin_zero[8];
};

/* 98 */
struct __attribute__((aligned(4))) ifmap
{
  unsigned int mem_start;
  unsigned int mem_end;
  unsigned __int16 base_addr;
  unsigned __int8 irq;
  unsigned __int8 dma;
  unsigned __int8 port;
};

/* 100 */
union ifreq::$EC3FB77B1F5CBB3C6B6AB4B4CE5B261D
{
  char ifrn_name[16];
};

/* 101 */
union ifreq::$E2AD184CB38B2411A96B0F382F8A9A8D
{
  sockaddr ifru_addr;
  sockaddr ifru_dstaddr;
  sockaddr ifru_broadaddr;
  sockaddr ifru_netmask;
  sockaddr ifru_hwaddr;
  __int16 ifru_flags;
  int ifru_ivalue;
  int ifru_mtu;
  ifmap ifru_map;
  char ifru_slave[16];
  char ifru_newname[16];
  __caddr_t ifru_data;
};

/* 99 */
struct ifreq
{
  ifreq::$EC3FB77B1F5CBB3C6B6AB4B4CE5B261D ifr_ifrn;
  ifreq::$E2AD184CB38B2411A96B0F382F8A9A8D ifr_ifru;
};

/* 103 */
union ifconf::$FD4E2001037F9B8D1B6B7C6700646983
{
  __caddr_t ifcu_buf;
  ifreq *ifcu_req;
};

/* 102 */
struct ifconf
{
  int ifc_len;
  ifconf::$FD4E2001037F9B8D1B6B7C6700646983 ifc_ifcu;
};

/* 108 */
struct KIntranet
{
  __int8 gap0[1];
};

/* 110 */
struct std::map<std::string,KE_LOGLEVEL>::value_compare;

/* 111 */
struct CGlobalValues
{
  int m_nDefReviveId;
  int m_nMinMapId;
  int m_nMaxMapId;
  int m_nStatTime;
  BOOL m_bCanDenyInvalidAccount;
  std::map<std::string,KE_LOGLEVEL> m_mapLogLevel;
};

/* 118 */
typedef unsigned __int16 WORD;

/* 119 */
struct _GUID
{
  DWORD Data1;
  WORD Data2;
  WORD Data3;
  BYTE Data4[8];
};

/* 120 */
typedef _GUID GUID;

/* 121 */
struct std::pair<const long unsigned int,IGoddessMsgHandler*>
{
  const unsigned int first;
  IGoddessMsgHandler *second;
};

/* 123 */
struct __gnu_cxx::new_allocator<std::pair<const long unsigned int,IGoddessMsgHandler*> >
{
  __int8 gap0[1];
};

/* 122 */
struct __cppobj std::allocator<std::pair<const long unsigned int,IGoddessMsgHandler*> > : __gnu_cxx::new_allocator<std::pair<const long unsigned int,IGoddessMsgHandler*> >
{
};

/* 125 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const long unsigned int,IGoddessMsgHandler*> > >
{
  __int8 gap0[1];
};

/* 124 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const long unsigned int,IGoddessMsgHandler*> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const long unsigned int,IGoddessMsgHandler*> > >
{
};

/* 126 */
struct __cppobj std::_Rb_tree_node<std::pair<const long unsigned int,IGoddessMsgHandler*> > : std::_Rb_tree_node_base
{
  std::pair<const long unsigned int,IGoddessMsgHandler*> _M_value_field;
};

/* 131 */
struct std::_Rb_tree_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 132 */
struct std::_Rb_tree_const_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 133 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> > >;

/* 134 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> > >;

/* 135 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> > first;
  bool second;
};

/* 136 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> >,std::_Rb_tree_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> > >
{
  std::_Rb_tree_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> > first;
  std::_Rb_tree_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> > second;
};

/* 137 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> >,std::_Rb_tree_const_iterator<std::pair<const long unsigned int,IGoddessMsgHandler*> > >;

/* 143 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,void*> > >
{
  __int8 gap0[1];
};

/* 142 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,void*> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,void*> > >
{
};

/* 145 */
struct std::pair<const std::string,void*>
{
  const std::string first;
  void *second;
};

/* 147 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,void*> >
{
  __int8 gap0[1];
};

/* 146 */
struct __cppobj std::allocator<std::pair<const std::string,void*> > : __gnu_cxx::new_allocator<std::pair<const std::string,void*> >
{
};

/* 148 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,void*> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,void*> _M_value_field;
};

/* 149 */
struct std::_Rb_tree_iterator<std::pair<const std::string,void*> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 150 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,void*> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 151 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,void*> > >;

/* 152 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,void*> > >;

/* 153 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,void*> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,void*> > first;
  bool second;
};

/* 154 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,void*> >,std::_Rb_tree_iterator<std::pair<const std::string,void*> > >;

/* 155 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,void*> >,std::_Rb_tree_const_iterator<std::pair<const std::string,void*> > >;

/* 157 */
struct __gnu_cxx::new_allocator<ICommand*>
{
  __int8 gap0[1];
};

/* 156 */
struct __cppobj std::allocator<ICommand*> : __gnu_cxx::new_allocator<ICommand*>
{
};

/* 162 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<ICommand* const*,std::vector<ICommand*> > >;

/* 163 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<ICommand**,std::vector<ICommand*> > >;

/* 166 */
struct std::_Rb_tree<unsigned int,std::pair<const unsigned int,CGamePlayer*>,std::_Select1st<std::pair<const unsigned int,CGamePlayer*> >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,CGamePlayer*> > >::_Rb_tree_impl<std::less<unsigned int>,false>
{
  std::less<unsigned int> _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 165 */
struct std::_Rb_tree<unsigned int,std::pair<const unsigned int,CGamePlayer*>,std::_Select1st<std::pair<const unsigned int,CGamePlayer*> >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,CGamePlayer*> > >
{
  std::_Rb_tree<unsigned int,std::pair<const unsigned int,CGamePlayer*>,std::_Select1st<std::pair<const unsigned int,CGamePlayer*> >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,CGamePlayer*> > >::_Rb_tree_impl<std::less<unsigned int>,false> _M_impl;
};

/* 164 */
struct std::map<unsigned int,CGamePlayer*>
{
  std::_Rb_tree<unsigned int,std::pair<const unsigned int,CGamePlayer*>,std::_Select1st<std::pair<const unsigned int,CGamePlayer*> >,std::less<unsigned int>,std::allocator<std::pair<const unsigned int,CGamePlayer*> > > _M_t;
};

/* 168 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const unsigned int,CGamePlayer*> > >
{
  __int8 gap0[1];
};

/* 167 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const unsigned int,CGamePlayer*> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const unsigned int,CGamePlayer*> > >
{
};

/* 171 */
struct std::pair<const std::string,long unsigned int>
{
  const std::string first;
  unsigned int second;
};

/* 173 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,long unsigned int> >
{
  __int8 gap0[1];
};

/* 172 */
struct __cppobj std::allocator<std::pair<const std::string,long unsigned int> > : __gnu_cxx::new_allocator<std::pair<const std::string,long unsigned int> >
{
};

/* 175 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,long unsigned int> > >
{
  __int8 gap0[1];
};

/* 174 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,long unsigned int> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,long unsigned int> > >
{
};

/* 176 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,long unsigned int> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,long unsigned int> _M_value_field;
};

/* 179 */
struct std::_Rb_tree_iterator<std::pair<const std::string,long unsigned int> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 180 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,long unsigned int> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 181 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,long unsigned int> > >;

/* 182 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,long unsigned int> > >;

/* 183 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,long unsigned int> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,long unsigned int> > first;
  bool second;
};

/* 184 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,long unsigned int> >,std::_Rb_tree_iterator<std::pair<const std::string,long unsigned int> > >;

/* 185 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,long unsigned int> >,std::_Rb_tree_const_iterator<std::pair<const std::string,long unsigned int> > >;

/* 188 */
struct __gnu_cxx::new_allocator<std::string >
{
  __int8 gap0[1];
};

/* 187 */
struct __cppobj std::allocator<std::string > : __gnu_cxx::new_allocator<std::string >
{
};

/* 190 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::string > >
{
  __int8 gap0[1];
};

/* 189 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::string > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::string > >
{
};

/* 191 */
struct __cppobj std::_Rb_tree_node<std::string > : std::_Rb_tree_node_base
{
  std::string _M_value_field;
};

/* 195 */
struct std::_Rb_tree_iterator<std::string >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 196 */
struct std::_Rb_tree_const_iterator<std::string >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 197 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::string > >;

/* 198 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::string > >;

/* 199 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::string >,bool>
{
  std::_Rb_tree_iterator<std::string > first;
  bool second;
};

/* 200 */
struct std::pair<std::_Rb_tree_iterator<std::string >,std::_Rb_tree_iterator<std::string > >
{
  std::_Rb_tree_iterator<std::string > first;
  std::_Rb_tree_iterator<std::string > second;
};

/* 201 */
struct std::pair<std::_Rb_tree_const_iterator<std::string >,std::_Rb_tree_const_iterator<std::string > >;

/* 203 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_const_iterator<std::string >,bool>
{
  std::_Rb_tree_const_iterator<std::string > first;
  bool second;
};

/* 204 */
struct std::pair<const std::string,CGamePlayer*>
{
  const std::string first;
  CGamePlayer *second;
};

/* 222 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,CGamePlayer*> >
{
  __int8 gap0[1];
};

/* 221 */
struct __cppobj std::allocator<std::pair<const std::string,CGamePlayer*> > : __gnu_cxx::new_allocator<std::pair<const std::string,CGamePlayer*> >
{
};

/* 224 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,CGamePlayer*> > >
{
  __int8 gap0[1];
};

/* 223 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,CGamePlayer*> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,CGamePlayer*> > >
{
};

/* 225 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,CGamePlayer*> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,CGamePlayer*> _M_value_field;
};

/* 228 */
struct std::_Rb_tree_iterator<std::pair<const std::string,CGamePlayer*> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 229 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,CGamePlayer*> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 230 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,CGamePlayer*> > >;

/* 231 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,CGamePlayer*> > >;

/* 232 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,CGamePlayer*> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,CGamePlayer*> > first;
  bool second;
};

/* 233 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,CGamePlayer*> >,std::_Rb_tree_iterator<std::pair<const std::string,CGamePlayer*> > >;

/* 234 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,CGamePlayer*> >,std::_Rb_tree_const_iterator<std::pair<const std::string,CGamePlayer*> > >;

/* 236 */
struct std::pair<const unsigned int,CGamePlayer*>
{
  const unsigned int first;
  CGamePlayer *second;
};

/* 238 */
struct __gnu_cxx::new_allocator<std::pair<const unsigned int,CGamePlayer*> >
{
  __int8 gap0[1];
};

/* 237 */
struct __cppobj std::allocator<std::pair<const unsigned int,CGamePlayer*> > : __gnu_cxx::new_allocator<std::pair<const unsigned int,CGamePlayer*> >
{
};

/* 239 */
struct __cppobj std::_Rb_tree_node<std::pair<const unsigned int,CGamePlayer*> > : std::_Rb_tree_node_base
{
  std::pair<const unsigned int,CGamePlayer*> _M_value_field;
};

/* 240 */
struct std::_Rb_tree_iterator<std::pair<const unsigned int,CGamePlayer*> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 241 */
struct std::_Rb_tree_const_iterator<std::pair<const unsigned int,CGamePlayer*> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 242 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const unsigned int,CGamePlayer*> > >;

/* 243 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const unsigned int,CGamePlayer*> > >;

/* 244 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const unsigned int,CGamePlayer*> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const unsigned int,CGamePlayer*> > first;
  bool second;
};

/* 245 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const unsigned int,CGamePlayer*> >,std::_Rb_tree_iterator<std::pair<const unsigned int,CGamePlayer*> > >;

/* 246 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const unsigned int,CGamePlayer*> >,std::_Rb_tree_const_iterator<std::pair<const unsigned int,CGamePlayer*> > >;

/* 247 */
struct std::pair<const int,std::map<std::string,CGamePlayer*>*>
{
  const int first;
  std::map<std::string,CGamePlayer*> *second;
};

/* 249 */
struct __gnu_cxx::new_allocator<std::pair<const int,std::map<std::string,CGamePlayer*>*> >
{
  __int8 gap0[1];
};

/* 248 */
struct __cppobj std::allocator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > : __gnu_cxx::new_allocator<std::pair<const int,std::map<std::string,CGamePlayer*>*> >
{
};

/* 251 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >
{
  __int8 gap0[1];
};

/* 250 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const int,std::map<std::string,CGamePlayer*>*> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >
{
};

/* 252 */
struct __cppobj std::_Rb_tree_node<std::pair<const int,std::map<std::string,CGamePlayer*>*> > : std::_Rb_tree_node_base
{
  std::pair<const int,std::map<std::string,CGamePlayer*>*> _M_value_field;
};

/* 253 */
struct std::binary_function<int,int,bool>
{
  __int8 gap0[1];
};

/* 254 */
struct __cppobj std::less<int> : std::binary_function<int,int,bool>
{
};

/* 256 */
struct std::_Rb_tree<int,std::pair<const int,std::map<std::string,CGamePlayer*>*>,std::_Select1st<std::pair<const int,std::map<std::string,CGamePlayer*>*> >,std::less<int>,std::allocator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >::_Rb_tree_impl<std::less<int>,false>
{
  std::less<int> _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 255 */
struct std::_Rb_tree<int,std::pair<const int,std::map<std::string,CGamePlayer*>*>,std::_Select1st<std::pair<const int,std::map<std::string,CGamePlayer*>*> >,std::less<int>,std::allocator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >
{
  std::_Rb_tree<int,std::pair<const int,std::map<std::string,CGamePlayer*>*>,std::_Select1st<std::pair<const int,std::map<std::string,CGamePlayer*>*> >,std::less<int>,std::allocator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >::_Rb_tree_impl<std::less<int>,false> _M_impl;
};

/* 257 */
struct std::_Rb_tree_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 258 */
struct std::_Rb_tree_const_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 259 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >;

/* 260 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >;

/* 261 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > first;
  bool second;
};

/* 262 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> >,std::_Rb_tree_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >;

/* 263 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> >,std::_Rb_tree_const_iterator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >;

/* 264 */
struct std::map<int,std::map<std::string,CGamePlayer*>*,std::less<int>,std::allocator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >
{
  std::_Rb_tree<int,std::pair<const int,std::map<std::string,CGamePlayer*>*>,std::_Select1st<std::pair<const int,std::map<std::string,CGamePlayer*>*> >,std::less<int>,std::allocator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > > _M_t;
};

/* 265 */
struct std::unary_function<std::pair<const unsigned int,CGamePlayer*>,const unsigned int>
{
  __int8 gap0[1];
};

/* 266 */
struct __cppobj std::_Select1st<std::pair<const unsigned int,CGamePlayer*> > : std::unary_function<std::pair<const unsigned int,CGamePlayer*>,const unsigned int>
{
};

/* 267 */
struct std::unary_function<std::pair<const int,std::map<std::string,CGamePlayer*>*>,const int>
{
  __int8 gap0[1];
};

/* 268 */
struct __cppobj std::_Select1st<std::pair<const int,std::map<std::string,CGamePlayer*>*> > : std::unary_function<std::pair<const int,std::map<std::string,CGamePlayer*>*>,const int>
{
};

/* 269 */
struct std::unary_function<std::pair<const std::string,CGamePlayer*>,const std::string >
{
  __int8 gap0[1];
};

/* 270 */
struct __cppobj std::_Select1st<std::pair<const std::string,CGamePlayer*> > : std::unary_function<std::pair<const std::string,CGamePlayer*>,const std::string >
{
};

/* 271 */
struct std::unary_function<std::pair<const std::string,long unsigned int>,const std::string >
{
  __int8 gap0[1];
};

/* 272 */
struct __cppobj std::_Select1st<std::pair<const std::string,long unsigned int> > : std::unary_function<std::pair<const std::string,long unsigned int>,const std::string >
{
};

/* 273 */
struct __gnu_cxx::__normal_iterator<ICommand**,std::vector<ICommand*> >
{
  ICommand **_M_current;
};

/* 274 */
struct __gnu_cxx::__normal_iterator<ICommand* const*,std::vector<ICommand*> >
{
  ICommand *const *_M_current;
};

/* 275 */
struct __attribute__((packed)) __attribute__((aligned(2))) KAccountHead
{
  WORD Size;
  WORD Version;
  WORD Type;
  DWORD Operate;
};

/* 276 */
struct __cppobj KAccountUser : KAccountHead
{
  char Account[32];
};

/* 277 */
struct __cppobj KAccountUserPassword : KAccountUser
{
  char Password[64];
};

/* 278 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) KAccountUserLoginInfo : KAccountUserPassword
{
  DWORD UserIP;
  DWORD UserPort;
  BYTE nLogout;
};

/* 279 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) KAccountUserReturn : KAccountUser
{
  int nReturn;
};

/* 280 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) KAccountUserReturnExt : KAccountUserReturn
{
  int nExtPoint;
  int nExtPoint1;
  int nExtPoint2;
  int nExtPoint3;
  int nExtPoint4;
  int nExtPoint5;
  int nExtPoint6;
  int nExtPoint7;
  DWORD nLeftTime;
  DWORD nLeftTimeOfPoint;
  DWORD dwLimitTimeFlag;
  DWORD dwLimitTimeOnlineSeconds;
  DWORD dwLimitTimeOfflineSeconds;
};

/* 281 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) KAccountUserLogout : KAccountUser
{
  int nExtPoint;
};

/* 282 */
struct KAutoThreadLock
{
  IKLock *m_pLock;
};

/* 285 */
struct std::map<long unsigned int,IGoddessMsgHandler*>::value_compare;

/* 287 */
union tagProtocolHeader::$7018D87010F9056583A42BE4D549A0FD
{
  BYTE cProtocol;
  BYTE ProtocolType;
};

/* 286 */
struct tagProtocolHeader
{
  tagProtocolHeader::$7018D87010F9056583A42BE4D549A0FD _anon_0;
};

/* 288 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagProtocolHeader2 : tagProtocolHeader
{
  unsigned int ulIdentity;
};

/* 289 */
struct __attribute__((packed)) __attribute__((aligned(1))) TRoleBaseInfo
{
  DWORD dwId;
  char szName[32];
  bool bSex;
  char szReserved[16];
  DWORD dwLastExchangeTime;
  DWORD dwLastLoginTime;
  DWORD dwLastOfflineTime;
  DWORD dwEnergy;
  char caccname[32];
  BYTE nLastSect;
  BYTE nSect;
  BYTE cFightMode;
  BYTE cUseRevive;
  BYTE cIsExchange;
  BYTE cPkStatus;
  int ijoincount;
  int isectrole;
  int igroupcode;
  int igrouprole;
  int irevivalid;
  int irevivalx;
  int irevivaly;
  int ientergameid;
  int ientergamex;
  int ientergamey;
  char cpartnercode[32];
  int isavemoney;
  int imoney;
  int ifiveprop;
  int iteam;
  WORD ifightlevel;
  __int16 fightexpHigh;
  int fightexp;
  int ileadlevel;
  int ileadexp;
  int iliveexp;
  int ipower;
  int iagility;
  int iouter;
  int iinside;
  int iluck;
  int imaxlife;
  int imaxstamina;
  int imaxinner;
  int icurlife;
  int icurstamina;
  int icurinner;
  int ipkvalue;
  int ileftprop;
  int ileftfight;
  int ileftlife;
  int ifinishgame;
  __int16 iarmorres;
  __int16 iweaponres;
  __int16 iheadimage;
  int nSectStat;
  int nWorldStat;
  int nKillPeopleNumber;
  int nBitFlag;
  DWORD dwTongID;
  int nRepute;
  int nVotePoint;
  DWORD dwLastLogoutTime;
  __int16 shPhysicsRes;
  __int16 shColdRes;
  __int16 shPoisonRes;
  __int16 shLightingRes;
  __int16 shFireRes;
  __int16 shReLiveTime;
  BYTE byExtBox;
  BYTE byBoxPasswordParam;
  BYTE byReserve13;
  BYTE byReserve14;
  DWORD dwBoxPassword;
  unsigned int uCatchTimeForAntiBot;
  BYTE byRefuseLoginCount;
  BYTE byHaveRefuseLogin;
  BYTE byIsExchangeServer;
  BYTE byRefuseLoginRe2;
  int nMapCopyIndex;
  DWORD dwCreateTime;
  BYTE byDataTransMark;
  BYTE byRenascenceCount;
  BYTE bySkillMaxLevelAddons;
  BYTE byMaxFireResist;
  BYTE byMaxColdResist;
  BYTE byMaxPoisResist;
  BYTE byMaxLighResist;
  BYTE byMaxPhysResist;
  unsigned int uReserve9;
  unsigned int uReserve0;
};

/* 290 */
struct TRoleData
{
  DWORD dwVersion;
  TRoleBaseInfo BaseInfo;
  bool bBaseNeedUpdate;
  __int16 nFightSkillCount;
  __int16 nLiveSkillCount;
  BYTE nTaskCount;
  __attribute__((packed)) __attribute__((aligned(1))) __int16 nItemCount;
  __attribute__((packed)) __attribute__((aligned(1))) __int16 nStateCount;
  __attribute__((packed)) __attribute__((aligned(1))) int dwTaskOffset;
  __attribute__((packed)) __attribute__((aligned(1))) int dwLSkillOffset;
  __attribute__((packed)) __attribute__((aligned(1))) int dwFSkillOffset;
  __attribute__((packed)) __attribute__((aligned(1))) int dwItemOffset;
  __attribute__((packed)) __attribute__((aligned(1))) int dwStateOffset;
  __attribute__((packed)) __attribute__((aligned(1))) int dwDataLen;
  BYTE pBuffer[1];
};

/* 291 */
struct $39DAACE7A22B353EE010CC077B1937F4
{
  char szName[32];
  BYTE Sex;
  BYTE Series;
  BYTE Level;
};

/* 292 */
struct tagKSG_PASSWORD
{
  char szPassword[64];
};

/* 293 */
typedef tagKSG_PASSWORD KSG_PASSWORD;

/* 295 */
union tagDBSelPlayer::$3B2FF73F7354B290292D32FFBB5643F8
{
  char szRoleName[32];
  BYTE cFailReason;
};

/* 294 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagDBSelPlayer : tagProtocolHeader2
{
  int nNameLen;
  tagDBSelPlayer::$3B2FF73F7354B290292D32FFBB5643F8 _anon_0;
};

/* 296 */
struct __cppobj tagDBDelPlayer : tagProtocolHeader
{
  char szAccountName[32];
  KSG_PASSWORD Password;
  char szRoleName[32];
};

/* 297 */
struct __cppobj tagNewDelRoleResponse : tagDBSelPlayer
{
  bool bSucceeded;
};

/* 298 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) tagPermitPlayerLogin : tagProtocolHeader2
{
  GUID guid;
  char szRoleName[32];
  bool bPermit;
};

/* 299 */
struct __cppobj tagNotifyPlayerLogin : tagPermitPlayerLogin
{
  __attribute__((packed)) __attribute__((aligned(1))) UINT nIPAddr;
  unsigned __int16 nPort;
};

/* 300 */
struct __attribute__((packed)) __attribute__((aligned(1))) TProcessData
{
  unsigned __int8 nProtoId;
  size_t nDataLen;
  unsigned int ulIdentity;
  bool bLeave;
  char pDataBuffer[1];
};

/* 301 */
struct NEW_PLAYER_COMMAND_BASE
{
  BYTE m_btRoleNo;
  BYTE m_btSeries;
  unsigned __int16 m_NativePlaceId;
};

/* 302 */
struct __cppobj NEW_PLAYER_COMMAND : NEW_PLAYER_COMMAND_BASE
{
  char m_szName[32];
};

/* 303 */
struct __cppobj NEW_PLAYER_COMMAND2 : NEW_PLAYER_COMMAND
{
  DWORD dwMapId;
  DWORD dwPosX;
  DWORD dwPosY;
  BOOL bCheckName;
};

/* 304 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagOfflineKickoutAsk : tagProtocolHeader2
{
  char szAccount[32];
  int nIndex;
};

/* 305 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) tagOfflineKickoutRes : tagProtocolHeader2
{
  char szAccount[32];
  BYTE nResult;
  int nExtPoint;
};

/* 310 */
enum enumAccountStatus : __int32
{
  enumInNone = 0x0,
  enumInGateway = 0x1,
  enumInGameServer = 0x2,
  enumOfflineLive = 0x3,
  enumOfflineLock_1 = 0x4,
  enumOfflineLock_2 = 0x5,
};

/* 311 */
struct Exchange
{
  int nSource;
  int nTarget;
  DWORD dwMapId;
  BOOL bExchanging;
};

/* 312 */
struct Account
{
  enumAccountStatus nStatus;
  DWORD dwLoginTime;
  int nIndex;
  DWORD dwMapId;
  Exchange sExchange;
  int nGameServer;
  int nEnterGameCount;
  char szRole[32];
  char szPassword[64];
};

/* 316 */
struct CGameServer::KBUFFER
{
  const void *pData;
  size_t nLen;
};

/* 317 */
struct std::map<std::string,void*,string_icmp,std::allocator<std::pair<const std::string,void*> > >::value_compare;

/* 318 */
typedef std::map<unsigned int,CGamePlayer*> PLAYER_MAP;

/* 320 */
struct std::map<std::string,long unsigned int>::value_compare;

/* 321 */
struct __attribute__((packed)) __attribute__((aligned(2))) KLoginStructHead
{
  unsigned __int16 Size;
  int Param;
};

/* 323 */
union KLoginAccountInfo::$993CF42953C8E87484165CB633AFD65D
{
  unsigned int nLeftTimeOfPoint;
  BOOL bKickOutOldAcc;
};

/* 322 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) KLoginAccountInfo : KLoginStructHead
{
  char Account[32];
  KSG_PASSWORD Password;
  unsigned int nLeftTime;
  unsigned int ProtocolVersion;
  KLoginAccountInfo::$993CF42953C8E87484165CB633AFD65D _anon_0;
  DWORD dwLimitTimeFlag;
  DWORD dwLimitTimeOnlineSeconds;
  DWORD dwLimitTimeOfflineSeconds;
};

/* 324 */
enum KE_TRIPCPROTO_B2R : __int32
{
  emTRIPCPROTO_B2R_LOCKACCOUNT_OK = 0x1,
  emTRIPCPROTO_B2R_UNLOCKACCOUNT_OK = 0x2,
};

/* 328 */
struct std::map<std::string,CGamePlayer*>::value_compare;

/* 329 */
struct std::map<unsigned int,CGamePlayer*>::value_compare;

/* 330 */
struct std::map<int,std::map<std::string,CGamePlayer*>*,std::less<int>,std::allocator<std::pair<const int,std::map<std::string,CGamePlayer*>*> > >::value_compare;

/* 331 */
typedef __int64 INT64;

/* 332 */
typedef LONG HRESULT;

/* 333 */
struct std::unary_function<std::pair<const std::string,void*>,const std::string >
{
  __int8 gap0[1];
};

/* 334 */
struct __cppobj std::_Select1st<std::pair<const std::string,void*> > : std::unary_function<std::pair<const std::string,void*>,const std::string >
{
};

/* 335 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) KAccountUserReturnExt2 : KAccountUserReturn
{
  unsigned int uFlag;
  int nPlayerDataIndex;
};

/* 337 */
union KAccountUserChangeExtPoint::$C786420EA9288E877AA9CFCAA6896CA4
{
  unsigned int uExtPointIndex;
  unsigned int uSilverType;
};

/* 336 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) KAccountUserChangeExtPoint : KAccountUser
{
  KAccountUserChangeExtPoint::$C786420EA9288E877AA9CFCAA6896CA4 _anon_0;
  int nChangeValue;
  int nPlayerDataIndex;
  unsigned int uFlag;
};

/* 338 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) TRYOUT_TIMEOUT_ASK : KAccountHead
{
  UINT nIdentity;
  BYTE nReason;
  int nPlayerIndex;
  char szAccount[32];
};

/* 339 */
struct __cppobj TAccountCDKEY : KAccountHead
{
  char username[32];
  __int16 flag;
  int changevalue;
  char changestr[64];
  int resevrval;
  char resevrstr[64];
};

/* 340 */
struct __cppobj TAccountCDKEYRet : KAccountHead
{
  char username[32];
  __int16 flag;
  int ret;
  char resevrstr[64];
  int resevrval;
};

/* 341 */
struct tagKIB_ItemBuyInfo
{
  char szAccount[32];
  int nPlayerDataIndex;
  int nGoodsIndex;
  int nItemTypeID;
  int nItemLevel;
  int nUseType;
  int nPrice;
  DWORD dwOverdueTime;
};

/* 342 */
struct tagKIB_ItemUseInfo
{
  char szAccount[32];
  int nPlayerDataIndex;
  int nItemTypeID;
  int nItemLevel;
  INT64 nItemGUID;
};

/* 343 */
struct tagKIB_ItemBuyResult
{
  char szAccount[32];
  int nPlayerDataIndex;
  int nGoodsIndex;
  int nItemTypeID;
  int nItemLevel;
  int nPrice;
  INT64 nItemGUID;
  int nRetCode;
};

/* 344 */
struct tagKIB_ItemUseResult
{
  char szAccount[32];
  int nPlayerDataIndex;
  int nItemTypeID;
  int nItemLevel;
  INT64 nItemGUID;
  int nRetCode;
};

/* 345 */
struct tagKAccountProtocol
{
  BYTE cProtocol;
};

/* 346 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) tagKAccountIB_BuyItem : KAccountHead
{
  tagKIB_ItemBuyInfo info;
};

/* 347 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagKAccountIB_BuyItemProtocol : tagKAccountProtocol, tagKAccountIB_BuyItem
{
};

/* 348 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) tagKAccountIB_UseItem : KAccountHead
{
  tagKIB_ItemUseInfo info;
};

/* 349 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagKAccountIB_UseItemProtocol : tagKAccountProtocol, tagKAccountIB_UseItem
{
};

/* 350 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) tagKAccountIB_BuyItemResult : KAccountHead
{
  tagKIB_ItemBuyResult result;
};

/* 351 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) tagKAccountIB_UseItemResult : KAccountHead
{
  tagKIB_ItemUseResult result;
};

/* 352 */
struct __cppobj tagProtoHeader : tagProtocolHeader
{
};

/* 353 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagGameSvrLoginRelay : tagProtocolHeader2
{
  unsigned int uGameSvrIp;
  int nMapCount;
  int nMapIdList[];
};

/* 354 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagGatewayBroadCast : tagProtocolHeader2
{
  UINT uCmdType;
  char szData[260];
};

/* 355 */
struct __cppobj KACCOUNT_INFO2 : tagAccountUserInfo
{
  tagExtPointInfo ChangePointInfo;
};

/* 356 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagGuidableInfo : tagProtocolHeader2
{
  GUID guid;
  KACCOUNT_INFO2 sAccountInfo;
  size_t datalength;
  char szData[];
};

/* 357 */
struct TTm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

/* 358 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagQueryMapInfo : tagProtocolHeader
{
  unsigned int uTime;
  int nClientID;
  TTm timeTm;
};

/* 359 */
struct __cppobj tagQueryGameSvrInfo : tagProtocolHeader
{
};

/* 360 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) tagUpdateMapID : tagProtocolHeader
{
  bool bRelay;
  int nMapCount;
  int snMapID[];
};

/* 361 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagDynamicUpdateMapID : tagProtocolHeader
{
  int nMapId;
  int nAction;
};

/* 362 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagGameSvrInfo : tagProtocolHeader
{
  UINT nIPAddr_Intraner;
  UINT nIPAddr_Internet;
  unsigned __int16 nPort;
  WORD wCapability;
};

/* 363 */
struct __cppobj tagLogicLogout : tagProtocolHeader
{
  BYTE szRoleName[32];
};

/* 364 */
struct __cppobj tagNotifyToDelRole : tagProtocolHeader2
{
  char szRoleName[32];
};

/* 365 */
struct __cppobj tagRegisterAccount : tagProtocolHeader
{
  BYTE szAccountName[32];
};

/* 366 */
struct __cppobj tagEnterGame : tagProtocolHeader
{
  BYTE szAccountName[32];
};

/* 368 */
union tagChangeExtPoint::$C786420EA9288E877AA9CFCAA6896CA4
{
  unsigned int uExtPointIndex;
  unsigned int uSilverType;
};

/* 367 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagChangeExtPoint : tagProtocolHeader
{
  BYTE szAccountName[32];
  tagChangeExtPoint::$C786420EA9288E877AA9CFCAA6896CA4 _anon_0;
  int nChangeValue;
  unsigned int uFlag;
  int nPlayerDataIndex;
};

/* 369 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagReturnChangeExtPoint : tagProtocolHeader
{
  int nResult;
  BYTE szAccountName[32];
  unsigned int uFlag;
  int nPlayerDataIndex;
};

/* 370 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagUseSpreaderCDKey : tagProtoHeader
{
  char szAccountName[32];
  char szCDKey[32];
  int nPlayerDataIndex;
};

/* 371 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagUseSpreaderCDKeyRet : tagProtoHeader
{
  char szAccountName[32];
  int nPlayerDataIndex;
  int nResult;
};

/* 372 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagGateWayKickOut : tagProtocolHeader
{
  int nKickOutFlag;
  BYTE szAccountName[32];
};

/* 373 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) tagLeaveGame : tagProtocolHeader
{
  BYTE cCmdType;
  int nExtPoint;
  char szAccountName[32];
};

/* 374 */
struct __cppobj tagOfflineRequestReq : tagProtocolHeader
{
  char szAccount[32];
  char szRole[32];
};

/* 375 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagOfflineRequestAsk : tagOfflineRequestReq
{
  DWORD dwMapId;
  int nIndex;
};

/* 376 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagOfflineTimeoutRes : tagOfflineRequestReq
{
  int nExtPoint;
};

/* 377 */
struct __attribute__((packed)) __attribute__((aligned(2))) TRYOUT_TIMEOUT_REQ
{
  BYTE ProtocolType;
  BYTE nReason;
  char szAccount[32];
  int nPlayerIndex;
};

/* 378 */
struct __cppobj TUnlockAccount : tagProtocolHeader
{
  char szAccount[32];
};

/* 379 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) TLockAccount : TUnlockAccount
{
  DWORD dwTimeout;
};

/* 380 */
struct __cppobj TLockAccountResult : tagProtocolHeader
{
  char szAccount[32];
  BYTE nLockResult;
};

/* 381 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) THostExchange : tagProtocolHeader
{
  char szAccount[32];
  BYTE nCommand;
  DWORD dwMapId;
};

/* 382 */
struct __cppobj TGS2B_ReadyState : tagProtocolHeader
{
  BYTE bReady;
};

/* 383 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagKIB_BuyItemProtocol : tagProtoHeader
{
  tagKIB_ItemBuyInfo info;
};

/* 384 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagKIB_UseItemProtocol : tagProtoHeader
{
  tagKIB_ItemUseInfo info;
};

/* 385 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagKIB_BuyItemResultProtocol : tagProtoHeader
{
  tagKIB_ItemBuyResult result;
};

/* 386 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagKIB_UseItemResultProtocol : tagProtoHeader
{
  tagKIB_ItemUseResult result;
};

/* 387 */
struct __cppobj KPROTO_B2R_BISHOPINFO : tagProtocolHeader
{
  BYTE byProtocolVersion;
  __attribute__((packed)) __attribute__((aligned(1))) DWORD dwIp;
  unsigned __int16 nPort;
};

/* 388 */
struct CTransfer
{
  CIntercessor *m_pIntercessor;
  CGameServer *m_pGameServer;
  const int m_lIdentity;
};

/* 389 */
typedef int __suseconds_t;

/* 390 */
struct timeval
{
  __time_t tv_sec;
  __suseconds_t tv_usec;
};

/* 391 */
struct std::input_iterator_tag
{
  __int8 gap0[1];
};

/* 392 */
struct __cppobj std::forward_iterator_tag : std::input_iterator_tag
{
};

/* 393 */
struct __cppobj std::bidirectional_iterator_tag : std::forward_iterator_tag
{
};

/* 394 */
struct std::__copy<true,std::random_access_iterator_tag>
{
  __int8 gap0[1];
};

/* 395 */
struct std::__copy_normal<true,true>
{
  __int8 gap0[1];
};

/* 396 */
struct std::__copy_backward<true,std::random_access_iterator_tag>
{
  __int8 gap0[1];
};

/* 397 */
struct std::__copy_backward_normal<true,true>
{
  __int8 gap0[1];
};

/* 400 */
struct __gnu_cxx::new_allocator<CGamePlayer*>
{
  __int8 gap0[1];
};

/* 399 */
struct __cppobj std::allocator<CGamePlayer*> : __gnu_cxx::new_allocator<CGamePlayer*>
{
};

/* 404 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<CGamePlayer* const*,std::vector<CGamePlayer*> > >;

/* 405 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<CGamePlayer**,std::vector<CGamePlayer*> > >;

/* 406 */
struct std::pair<long unsigned int,CGamePlayer*>
{
  unsigned int first;
  CGamePlayer *second;
};

/* 408 */
struct __gnu_cxx::new_allocator<std::pair<long unsigned int,CGamePlayer*> >
{
  __int8 gap0[1];
};

/* 407 */
struct __cppobj std::allocator<std::pair<long unsigned int,CGamePlayer*> > : __gnu_cxx::new_allocator<std::pair<long unsigned int,CGamePlayer*> >
{
};

/* 410 */
struct __gnu_cxx::new_allocator<std::_List_node<std::pair<long unsigned int,CGamePlayer*> > >
{
  __int8 gap0[1];
};

/* 409 */
struct __cppobj std::allocator<std::_List_node<std::pair<long unsigned int,CGamePlayer*> > > : __gnu_cxx::new_allocator<std::_List_node<std::pair<long unsigned int,CGamePlayer*> > >
{
};

/* 411 */
struct __cppobj std::_List_node<std::pair<long unsigned int,CGamePlayer*> > : std::_List_node_base
{
  std::pair<long unsigned int,CGamePlayer*> _M_data;
};

/* 415 */
struct std::_List_iterator<std::pair<long unsigned int,CGamePlayer*> >
{
  std::_List_node_base *_M_node;
};

/* 416 */
struct std::_List_const_iterator<std::pair<long unsigned int,CGamePlayer*> >
{
  const std::_List_node_base *_M_node;
};

/* 417 */
struct std::reverse_iterator<std::_List_const_iterator<std::pair<long unsigned int,CGamePlayer*> > >;

/* 418 */
struct std::reverse_iterator<std::_List_iterator<std::pair<long unsigned int,CGamePlayer*> > >;

/* 422 */
struct std::_List_base<CGameServer*>::_List_impl
{
  std::_List_node_base _M_node;
};

/* 421 */
struct std::_List_base<CGameServer*>
{
  std::_List_base<CGameServer*>::_List_impl _M_impl;
};

/* 420 */
struct __cppobj std::list<CGameServer*> : std::_List_base<CGameServer*>
{
};

/* 419 */
struct std::pair<const unsigned int,std::list<CGameServer*> >
{
  const unsigned int first;
  std::list<CGameServer*> second;
};

/* 424 */
struct __gnu_cxx::new_allocator<std::_List_node<CGameServer*> >
{
  __int8 gap0[1];
};

/* 423 */
struct __cppobj std::allocator<std::_List_node<CGameServer*> > : __gnu_cxx::new_allocator<std::_List_node<CGameServer*> >
{
};

/* 426 */
struct __gnu_cxx::new_allocator<std::pair<const unsigned int,std::list<CGameServer*> > >
{
  __int8 gap0[1];
};

/* 425 */
struct __cppobj std::allocator<std::pair<const unsigned int,std::list<CGameServer*> > > : __gnu_cxx::new_allocator<std::pair<const unsigned int,std::list<CGameServer*> > >
{
};

/* 428 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const unsigned int,std::list<CGameServer*> > > >
{
  __int8 gap0[1];
};

/* 427 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const unsigned int,std::list<CGameServer*> > > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const unsigned int,std::list<CGameServer*> > > >
{
};

/* 429 */
struct __cppobj std::_Rb_tree_node<std::pair<const unsigned int,std::list<CGameServer*> > > : std::_Rb_tree_node_base
{
  std::pair<const unsigned int,std::list<CGameServer*> > _M_value_field;
};

/* 432 */
struct std::_Rb_tree_iterator<std::pair<const unsigned int,std::list<CGameServer*> > >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 433 */
struct std::_Rb_tree_const_iterator<std::pair<const unsigned int,std::list<CGameServer*> > >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 434 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const unsigned int,std::list<CGameServer*> > > >;

/* 435 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const unsigned int,std::list<CGameServer*> > > >;

/* 436 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const unsigned int,std::list<CGameServer*> > >,bool>
{
  std::_Rb_tree_iterator<std::pair<const unsigned int,std::list<CGameServer*> > > first;
  bool second;
};

/* 437 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const unsigned int,std::list<CGameServer*> > >,std::_Rb_tree_iterator<std::pair<const unsigned int,std::list<CGameServer*> > > >;

/* 438 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const unsigned int,std::list<CGameServer*> > >,std::_Rb_tree_const_iterator<std::pair<const unsigned int,std::list<CGameServer*> > > >;

/* 440 */
struct std::pair<const unsigned int,CGameServer*>
{
  const unsigned int first;
  CGameServer *second;
};

/* 448 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const unsigned int,CGameServer*> > >
{
  __int8 gap0[1];
};

/* 447 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const unsigned int,CGameServer*> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const unsigned int,CGameServer*> > >
{
};

/* 450 */
struct __gnu_cxx::new_allocator<std::pair<const unsigned int,CGameServer*> >
{
  __int8 gap0[1];
};

/* 449 */
struct __cppobj std::allocator<std::pair<const unsigned int,CGameServer*> > : __gnu_cxx::new_allocator<std::pair<const unsigned int,CGameServer*> >
{
};

/* 451 */
struct __cppobj std::_Rb_tree_node<std::pair<const unsigned int,CGameServer*> > : std::_Rb_tree_node_base
{
  std::pair<const unsigned int,CGameServer*> _M_value_field;
};

/* 452 */
struct std::_Rb_tree_iterator<std::pair<const unsigned int,CGameServer*> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 453 */
struct std::_Rb_tree_const_iterator<std::pair<const unsigned int,CGameServer*> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 454 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const unsigned int,CGameServer*> > >;

/* 455 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const unsigned int,CGameServer*> > >;

/* 456 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const unsigned int,CGameServer*> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const unsigned int,CGameServer*> > first;
  bool second;
};

/* 457 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const unsigned int,CGameServer*> >,std::_Rb_tree_iterator<std::pair<const unsigned int,CGameServer*> > >;

/* 458 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const unsigned int,CGameServer*> >,std::_Rb_tree_const_iterator<std::pair<const unsigned int,CGameServer*> > >;

/* 460 */
struct __gnu_cxx::new_allocator<CGameServer*>
{
  __int8 gap0[1];
};

/* 459 */
struct __cppobj std::allocator<CGameServer*> : __gnu_cxx::new_allocator<CGameServer*>
{
};

/* 461 */
struct __cppobj std::_List_node<CGameServer*> : std::_List_node_base
{
  CGameServer *_M_data;
};

/* 462 */
struct std::_List_iterator<CGameServer*>
{
  std::_List_node_base *_M_node;
};

/* 463 */
struct std::_List_const_iterator<CGameServer*>
{
  const std::_List_node_base *_M_node;
};

/* 464 */
struct std::reverse_iterator<std::_List_const_iterator<CGameServer*> >;

/* 465 */
struct std::reverse_iterator<std::_List_iterator<CGameServer*> >;

/* 466 */
struct std::unary_function<std::pair<const unsigned int,CGameServer*>,const unsigned int>
{
  __int8 gap0[1];
};

/* 467 */
struct __cppobj std::_Select1st<std::pair<const unsigned int,CGameServer*> > : std::unary_function<std::pair<const unsigned int,CGameServer*>,const unsigned int>
{
};

/* 468 */
struct std::unary_function<std::pair<const unsigned int,std::list<CGameServer*> >,const unsigned int>
{
  __int8 gap0[1];
};

/* 469 */
struct __cppobj std::_Select1st<std::pair<const unsigned int,std::list<CGameServer*> > > : std::unary_function<std::pair<const unsigned int,std::list<CGameServer*> >,const unsigned int>
{
};

/* 470 */
struct std::__copy_normal<false,false>
{
  __int8 gap0[1];
};

/* 471 */
struct __gnu_cxx::__normal_iterator<CGamePlayer**,std::vector<CGamePlayer*> >
{
  CGamePlayer **_M_current;
};

/* 472 */
struct __gnu_cxx::__normal_iterator<CGamePlayer* const*,std::vector<CGamePlayer*> >
{
  CGamePlayer *const *_M_current;
};

/* 473 */
struct __true_type
{
  __int8 gap0[1];
};

/* 474 */
struct __false_type
{
  __int8 gap0[1];
};

/* 475 */
enum c2s_PROTOCOL : __int32
{
  c2s_roleserver_saveroleinfo = 0xA,
  c2s_roleserver_createroleinfo = 0xB,
  c2s_gmsvr2gateway_saverole = 0xC,
  c2s_micropackbegin = 0x1F,
  c2s_accountbegin = 0x20,
  c2s_accountlogin = 0x21,
  c2s_gamelogin = 0x22,
  c2s_accountlogout = 0x23,
  c2s_gatewayverify = 0x24,
  c2s_gatewayverifyagain = 0x25,
  c2s_gatewayinfo = 0x26,
  c2s_gatewayclose = 0x27,
  c2s_account_change_extpoint = 0x28,
  c2s_gateway_kickout = 0x29,
  c2s_tryout_timeout_req = 0x2A,
  c2s_statinfo = 0x2B,
  c2s_cdkey = 0x2C,
  c2s_change_account_state = 0x2D,
  c2s_paysys_ib_item_buy = 0x2E,
  c2s_paysys_ib_item_use = 0x2F,
  c2s_paysys_end = 0x30,
  c2s_multiserverbegin = 0x30,
  c2s_permitplayerlogin = 0x31,
  c2s_updatemapinfo = 0x32,
  c2s_updategameserverinfo = 0x33,
  c2s_entergame = 0x34,
  c2s_leavegame = 0x35,
  c2s_registeraccount = 0x36,
  c2s_requestsvrip = 0x37,
  c2s_roleserver_getrolelist = 0x38,
  c2s_roleserver_getroleinfo = 0x39,
  c2s_roleserver_deleteplayer = 0x3A,
  c2s_transfer_role = 0x3B,
  c2s_gamestatistic = 0x3C,
  c2s_roleserver_lock = 0x3D,
  c2s_change_extpoint = 0x3E,
  c2s_use_spreader_cdkey = 0x3F,
  c2s_dynamicupdatemapinfo = 0x40,
  c2s_gs_ib_item_buy = 0x41,
  c2s_gs_ib_item_use = 0x42,
  c2s_ready_state = 0x43,
  c2s_gameserverbegin = 0x40,
  c2s_login = 0x41,
  c2s_logiclogin = 0x42,
  c2s_syncend = 0x43,
  c2s_loadplayer = 0x44,
  c2s_newplayer = 0x45,
  c2s_removeplayer = 0x46,
  c2s_requestworld = 0x47,
  c2s_requestplayer = 0x48,
  c2s_requestnpc = 0x49,
  c2s_requestobj = 0x4A,
  c2s_npcwalk = 0x4B,
  c2s_npcrun = 0x4C,
  c2s_npcskill = 0x4D,
  c2s_npcjump = 0x4E,
  c2s_npctalk = 0x4F,
  c2s_npchurt = 0x50,
  c2s_npcdeath = 0x51,
  c2s_playertalk = 0x52,
  c2s_team = 0x53,
  c2s_playerapplysetpk = 0x54,
  c2s_playerapplyfactiondata = 0x55,
  c2s_playersendchat = 0x56,
  c2s_playeraddbaseattribute = 0x57,
  c2s_playerapplyaddskillpoint = 0x58,
  c2s_playereatitem = 0x59,
  c2s_playerpickupitem = 0x5A,
  c2s_playermoveitem = 0x5B,
  c2s_playersellitem = 0x5C,
  c2s_playerbuyitem = 0x5D,
  c2s_playerthrowawayitem = 0x5E,
  c2s_playerselui = 0x5F,
  c2s_chatsetchannel = 0x60,
  c2s_chatapplyaddfriend = 0x61,
  c2s_chataddfriend = 0x62,
  c2s_chatrefusefriend = 0x63,
  c2s_dbplayerselect = 0x64,
  c2s_chatapplyresendallfriendname = 0x65,
  c2s_chatapplysendonefriendname = 0x66,
  c2s_chatdeletefriend = 0x67,
  c2s_chatredeletefriend = 0x68,
  c2s_tradeapplystateopen = 0x69,
  c2s_tradeapplystateclose = 0x6A,
  c2s_tradeapplystart = 0x6B,
  c2s_trademovemoney = 0x6C,
  c2s_tradedecision = 0x6D,
  c2s_dialognpc = 0x6E,
  c2s_changeauraskill = 0x6F,
  c2s_ping = 0x70,
  c2s_npcsit = 0x71,
  c2s_objmouseclick = 0x72,
  c2s_storemoney = 0x73,
  c2s_playerrevive = 0x74,
  c2s_tradereplystart = 0x75,
  c2s_pkapplychangenormalflag = 0x76,
  c2s_pkapplyenmity = 0x77,
  c2s_viewequip = 0x78,
  c2s_ladderquery = 0x79,
  c2s_repairitem = 0x7A,
  c2s_itemmask = 0x7B,
  c2s_stallextend = 0x7C,
  c2s_give = 0x7D,
  c2s_notify = 0x7E,
  c2s_enchaseritem = 0x7F,
  c2s_killerextend = 0x80,
  c2s_auctionextend = 0x81,
  c2s_citywarextend = 0x82,
  c2s_throwawayallmedicine = 0x83,
  c2s_boxoperate = 0x84,
  c2s_playerdivideitem = 0x85,
  c2s_bulletincontentquery = 0x86,
  c2s_autoattacknpc = 0x87,
  c2s_requestcityowner = 0x88,
  c2s_giveitemuiresult = 0x89,
  c2s_bot_clientcheck_result = 0x8A,
  c2s_welcome2server = 0x8B,
  c2s_offline_request_req = 0x8C,
  c2s_offline_request_ask = 0x8D,
  c2s_offline_kickout_res = 0x8E,
  c2s_offline_timeout_res = 0x8F,
  c2s_daytime_req = 0x90,
  c2s_requestnpcfeature = 0x91,
  c2s_foundry_request = 0x92,
  c2s_bishop_shutdowngodess = 0x93,
  c2s_replyroleinfo = 0x94,
  c2s_iambishop = 0x95,
  c2s_request_statdata = 0x96,
  c2s_getroledata_request = 0x97,
  c2s_hostexchange = 0x98,
  c2s_spectator = 0x99,
  c2s_partnerextend = 0x9A,
  c2s_closeconnection = 0x9B,
  c2s_queryrolename = 0x9C,
  c2s_lockaccount = 0x9D,
  c2s_unlockaccount = 0x9E,
  c2s_changerolename = 0x9F,
  c2s_changerolename_finish = 0xA0,
  c2s_querytongname = 0xA1,
  c2s_changetongname = 0xA2,
  c2s_sendtextcmd = 0xA3,
  c2s_chatroom = 0xA4,
  c2s_tongexextend = 0xA5,
  c2s_request_npcstate = 0xA6,
  c2s_apply_syncfile = 0xA7,
  c2s_setplayertaskvalue = 0xA8,
  c2s_nationalwar = 0xA9,
  c2s_select_diceitem = 0xAA,
  c2s_script_protocol = 0xAB,
  c2s_stores_change_shop = 0xAC,
  c2s_tripserver = 0xAD,
  c2s_tripclient = 0xAE,
  c2s_get_tags_request = 0xAF,
  c2s_add_tag = 0xB0,
  c2s_del_tag = 0xB1,
  c2s_set_friend_publish_flag = 0xB2,
  c2s_num_check = 0xB3,
  _c2s_begin_relay = 0xFA,
  c2s_extend = 0xFA,
  c2s_extendchat = 0xFB,
  c2s_extendfriend = 0xFC,
  _c2s_end_relay = 0xFC,
  c2s_extendtong = 0xFD,
  c2s_end = 0xFE,
};

/* 476 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) TRYOUT_TIMEOUT_ANS : KAccountHead
{
  UINT nIdentity;
  BYTE nResult;
  int nPlayerIndex;
  char szAccount[32];
};

/* 477 */
struct __cppobj NEW_PLAYER_COMMAND3 : NEW_PLAYER_COMMAND2
{
  char szAccount[32];
};

/* 478 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) tagPING_COMMAND : tagProtocolHeader
{
  DWORD m_dwTime;
};

/* 479 */
typedef tagPING_COMMAND PING_COMMAND;

/* 480 */
struct __attribute__((packed)) __attribute__((aligned(2))) TRYOUT_TIMEOUT_RES
{
  BYTE ProtocolType;
  BYTE nResult;
  char szAccount[32];
  int nPlayerIndex;
};

/* 481 */
struct TStatRequest
{
  WORD wType;
};

/* 482 */
struct KGAMESVR_STATUS
{
  unsigned int nID;
  char szIp[32];
  BOOL bIsRelay;
  BOOL bReady;
  int nPlayerCount;
};

/* 483 */
struct std::map<unsigned int,std::list<CGameServer*>>::value_compare;

/* 484 */
struct std::map<unsigned int,CGameServer*>::value_compare;

/* 485 */
typedef int __ssize_t;

/* 486 */
typedef __ssize_t ssize_t;

/* 487 */
typedef int __fd_mask;

/* 488 */
struct $7B8E1C2BE0F4100AF694763ED03D3D72
{
  __fd_mask fds_bits[32];
};

/* 489 */
struct std::pair<const std::string,void (KConsole::*)(const CHAR*)>
{
  const std::string first;
  $DBE2C87402ECD8AE6ACE3192AA0CE987 second;
};

/* 494 */
struct std::_Rb_tree<std::string,std::pair<const std::string,void (KConsole::*)(const CHAR*)>,std::_Select1st<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >,std::less<std::string >,std::allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >::_Rb_tree_impl<std::less<std::string >,false>
{
  std::less<std::string > _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 493 */
struct std::_Rb_tree<std::string,std::pair<const std::string,void (KConsole::*)(const CHAR*)>,std::_Select1st<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >,std::less<std::string >,std::allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,void (KConsole::*)(const CHAR*)>,std::_Select1st<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >,std::less<std::string >,std::allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >::_Rb_tree_impl<std::less<std::string >,false> _M_impl;
};

/* 492 */
struct std::map<std::string,void (KConsole::*)(const CHAR*),std::less<std::string >,std::allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,void (KConsole::*)(const CHAR*)>,std::_Select1st<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >,std::less<std::string >,std::allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > > _M_t;
};

/* 490 */
struct __cppobj KConsole : IMainUi
{
  BOOL m_bStop;
  IService *m_pService;
  std::map<std::string,void (KConsole::*)(const CHAR*),std::less<std::string >,std::allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > > m_mapHandler;
};

/* 496 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >
{
  __int8 gap0[1];
};

/* 495 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >
{
};

/* 498 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >
{
  __int8 gap0[1];
};

/* 497 */
struct __cppobj std::allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > : __gnu_cxx::new_allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >
{
};

/* 499 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,void (KConsole::*)(const CHAR*)> _M_value_field;
};

/* 500 */
struct std::_Rb_tree_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 501 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 502 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >;

/* 503 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >;

/* 504 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > first;
  bool second;
};

/* 505 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >,std::_Rb_tree_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >;

/* 506 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> >,std::_Rb_tree_const_iterator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >;

/* 507 */
struct std::unary_function<std::pair<const std::string,void (KConsole::*)(const CHAR*)>,const std::string >
{
  __int8 gap0[1];
};

/* 508 */
struct __cppobj std::_Select1st<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > : std::unary_function<std::pair<const std::string,void (KConsole::*)(const CHAR*)>,const std::string >
{
};

/* 509 */
struct std::map<std::string,void (KConsole::*)(const CHAR*),std::less<std::string >,std::allocator<std::pair<const std::string,void (KConsole::*)(const CHAR*)> > >::value_compare;

/* 510 */
struct KSimpleThread_0
{
  __int8 gap0[12];
};

/* 511 */
struct std::unary_function<std::pair<const std::string,KE_LOGLEVEL>,const std::string >
{
  __int8 gap0[1];
};

/* 512 */
struct __cppobj std::_Select1st<std::pair<const std::string,KE_LOGLEVEL> > : std::unary_function<std::pair<const std::string,KE_LOGLEVEL>,const std::string >
{
};

/* 513 */
struct std::unary_function<std::pair<const long unsigned int,IGoddessMsgHandler*>,const long unsigned int>
{
  __int8 gap0[1];
};

/* 514 */
struct __cppobj std::_Select1st<std::pair<const long unsigned int,IGoddessMsgHandler*> > : std::unary_function<std::pair<const long unsigned int,IGoddessMsgHandler*>,const long unsigned int>
{
};

/* 515 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) TRoleInstance : tagProtocolHeader
{
  char szRole[32];
  int nIndex;
  unsigned int nIdentity;
};

/* 518 */
struct std::_Rb_tree<std::string,std::pair<const std::string,Account>,std::_Select1st<std::pair<const std::string,Account> >,string_icmp,std::allocator<std::pair<const std::string,Account> > >::_Rb_tree_impl<string_icmp,false>
{
  string_icmp _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 517 */
struct std::_Rb_tree<std::string,std::pair<const std::string,Account>,std::_Select1st<std::pair<const std::string,Account> >,string_icmp,std::allocator<std::pair<const std::string,Account> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,Account>,std::_Select1st<std::pair<const std::string,Account> >,string_icmp,std::allocator<std::pair<const std::string,Account> > >::_Rb_tree_impl<string_icmp,false> _M_impl;
};

/* 516 */
struct std::map<std::string,Account,string_icmp,std::allocator<std::pair<const std::string,Account> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,Account>,std::_Select1st<std::pair<const std::string,Account> >,string_icmp,std::allocator<std::pair<const std::string,Account> > > _M_t;
};

/* 520 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,Account> > >
{
  __int8 gap0[1];
};

/* 519 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,Account> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,Account> > >
{
};

/* 521 */
struct std::pair<const std::string,Account>
{
  const std::string first;
  Account second;
};

/* 523 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,Account> >
{
  __int8 gap0[1];
};

/* 522 */
struct __cppobj std::allocator<std::pair<const std::string,Account> > : __gnu_cxx::new_allocator<std::pair<const std::string,Account> >
{
};

/* 524 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,Account> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,Account> _M_value_field;
};

/* 525 */
struct std::_Rb_tree_iterator<std::pair<const std::string,Account> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 526 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,Account> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 527 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,Account> > >;

/* 528 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,Account> > >;

/* 529 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,Account> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,Account> > first;
  bool second;
};

/* 530 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,Account> >,std::_Rb_tree_iterator<std::pair<const std::string,Account> > >;

/* 531 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,Account> >,std::_Rb_tree_const_iterator<std::pair<const std::string,Account> > >;

/* 532 */
struct std::unary_function<std::pair<const std::string,Account>,const std::string >
{
  __int8 gap0[1];
};

/* 533 */
struct __cppobj std::_Select1st<std::pair<const std::string,Account> > : std::unary_function<std::pair<const std::string,Account>,const std::string >
{
};

/* 535 */
struct KTASKSTATUS
{
  int nProcessCount;
  int nDenialCount;
  int nWaitingCount;
  int nWorkingCount;
};

/* 536 */
typedef std::map<std::string,Account,string_icmp,std::allocator<std::pair<const std::string,Account> > > stdAccountGlobal;

/* 541 */
struct std::map<std::string,Account,string_icmp,std::allocator<std::pair<const std::string,Account> > >::value_compare;

/* 544 */
struct std::_Rb_tree<std::string,std::pair<const std::string,int>,std::_Select1st<std::pair<const std::string,int> >,string_icmp,std::allocator<std::pair<const std::string,int> > >::_Rb_tree_impl<string_icmp,false>
{
  string_icmp _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 543 */
struct std::_Rb_tree<std::string,std::pair<const std::string,int>,std::_Select1st<std::pair<const std::string,int> >,string_icmp,std::allocator<std::pair<const std::string,int> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,int>,std::_Select1st<std::pair<const std::string,int> >,string_icmp,std::allocator<std::pair<const std::string,int> > >::_Rb_tree_impl<string_icmp,false> _M_impl;
};

/* 542 */
struct std::map<std::string,int,string_icmp,std::allocator<std::pair<const std::string,int> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,int>,std::_Select1st<std::pair<const std::string,int> >,string_icmp,std::allocator<std::pair<const std::string,int> > > _M_t;
};

/* 546 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,int> > >
{
  __int8 gap0[1];
};

/* 545 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,int> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,int> > >
{
};

/* 547 */
struct std::pair<const std::string,int>
{
  const std::string first;
  int second;
};

/* 549 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,int> >
{
  __int8 gap0[1];
};

/* 548 */
struct __cppobj std::allocator<std::pair<const std::string,int> > : __gnu_cxx::new_allocator<std::pair<const std::string,int> >
{
};

/* 550 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,int> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,int> _M_value_field;
};

/* 551 */
struct std::_Rb_tree_iterator<std::pair<const std::string,int> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 552 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,int> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 553 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,int> > >;

/* 554 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,int> > >;

/* 555 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,int> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,int> > first;
  bool second;
};

/* 556 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,int> >,std::_Rb_tree_iterator<std::pair<const std::string,int> > >;

/* 557 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,int> >,std::_Rb_tree_const_iterator<std::pair<const std::string,int> > >;

/* 558 */
struct std::pair<const CHAR*,Account>
{
  const CHAR *first;
  Account second;
};

/* 559 */
struct std::unary_function<std::pair<const std::string,int>,const std::string >
{
  __int8 gap0[1];
};

/* 560 */
struct __cppobj std::_Select1st<std::pair<const std::string,int> > : std::unary_function<std::pair<const std::string,int>,const std::string >
{
};

/* 561 */
struct __cppobj KServerAccountUserLoginInfo : KAccountUserPassword
{
  __attribute__((packed)) __attribute__((aligned(1))) DWORD Address;
  BYTE MacAddress[6];
  DWORD nLastTime;
  DWORD nPlayerCount;
};

/* 562 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) KAccountUserReturnVerify : KAccountUserReturn
{
  int dwVerifyTime;
};

/* 563 */
typedef std::map<std::string,int,string_icmp,std::allocator<std::pair<const std::string,int> > > stdAccountTimeOut;

/* 564 */
struct std::map<std::string,int,string_icmp,std::allocator<std::pair<const std::string,int> > >::value_compare;

/* 565 */
struct std::unary_function<std::string,std::string >
{
  __int8 gap0[1];
};

/* 566 */
struct __cppobj std::_Identity<std::string > : std::unary_function<std::string,std::string >
{
};

/* 567 */
struct KTRIPPROTO_HEAD
{
  BYTE byProtocol;
};

/* 568 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) KTRIPPROTO_SESSION : KTRIPPROTO_HEAD
{
  GUID sGuid;
};

/* 569 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) KTRIPCPROTO_B2R_CONFIRM : KTRIPPROTO_SESSION
{
  BOOL bResult;
  char szPassword[64];
};

/* 570 */
struct __cppobj KTRIPCPROTO_R2B : KTRIPPROTO_SESSION
{
  char szAccount[32];
};

/* 572 */
struct trip_s::KTripManager::KINFO
{
  DWORD dwIdentity;
  trip_s::KTripSession *pSession;
};

/* 571 */
struct std::pair<const std::string,trip_s::KTripManager::KINFO>
{
  const std::string first;
  trip_s::KTripManager::KINFO second;
};

/* 573 */
struct trip_s::KTripSession
{
  CIntercessor *m_pIntercessor;
  trip_s::KTripManager *m_pManager;
  char m_szAccount[32];
  char m_szRole[32];
  BYTE m_byWaiting;
  size_t m_nGameServer;
  GUID m_sGuid;
  char *m_pData;
  size_t m_nDataLen;
  DWORD m_dwMapId;
  DWORD m_dwPosX;
  DWORD m_dwPosY;
};

/* 578 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,trip_s::KTripManager::KINFO> > >
{
  __int8 gap0[1];
};

/* 577 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,trip_s::KTripManager::KINFO> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,trip_s::KTripManager::KINFO> > >
{
};

/* 583 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const long unsigned int,trip_s::KTripSession*> > >
{
  __int8 gap0[1];
};

/* 582 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const long unsigned int,trip_s::KTripSession*> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const long unsigned int,trip_s::KTripSession*> > >
{
};

/* 585 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,trip_s::KTripManager::KINFO> >
{
  __int8 gap0[1];
};

/* 584 */
struct __cppobj std::allocator<std::pair<const std::string,trip_s::KTripManager::KINFO> > : __gnu_cxx::new_allocator<std::pair<const std::string,trip_s::KTripManager::KINFO> >
{
};

/* 586 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,trip_s::KTripManager::KINFO> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,trip_s::KTripManager::KINFO> _M_value_field;
};

/* 587 */
struct std::_Rb_tree_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 588 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 589 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> > >;

/* 590 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> > >;

/* 591 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> > first;
  bool second;
};

/* 592 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> >,std::_Rb_tree_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> > >;

/* 593 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> >,std::_Rb_tree_const_iterator<std::pair<const std::string,trip_s::KTripManager::KINFO> > >;

/* 594 */
struct std::pair<const long unsigned int,trip_s::KTripSession*>
{
  const unsigned int first;
  trip_s::KTripSession *second;
};

/* 596 */
struct __gnu_cxx::new_allocator<std::pair<const long unsigned int,trip_s::KTripSession*> >
{
  __int8 gap0[1];
};

/* 595 */
struct __cppobj std::allocator<std::pair<const long unsigned int,trip_s::KTripSession*> > : __gnu_cxx::new_allocator<std::pair<const long unsigned int,trip_s::KTripSession*> >
{
};

/* 597 */
struct __cppobj std::_Rb_tree_node<std::pair<const long unsigned int,trip_s::KTripSession*> > : std::_Rb_tree_node_base
{
  std::pair<const long unsigned int,trip_s::KTripSession*> _M_value_field;
};

/* 598 */
struct std::_Rb_tree_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 599 */
struct std::_Rb_tree_const_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 600 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> > >;

/* 601 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> > >;

/* 602 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> > first;
  bool second;
};

/* 603 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> >,std::_Rb_tree_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> > >
{
  std::_Rb_tree_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> > first;
  std::_Rb_tree_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> > second;
};

/* 604 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> >,std::_Rb_tree_const_iterator<std::pair<const long unsigned int,trip_s::KTripSession*> > >;

/* 605 */
struct std::unary_function<std::pair<const std::string,trip_s::KTripManager::KINFO>,const std::string >
{
  __int8 gap0[1];
};

/* 606 */
struct __cppobj std::_Select1st<std::pair<const std::string,trip_s::KTripManager::KINFO> > : std::unary_function<std::pair<const std::string,trip_s::KTripManager::KINFO>,const std::string >
{
};

/* 607 */
struct std::unary_function<std::pair<const long unsigned int,trip_s::KTripSession*>,const long unsigned int>
{
  __int8 gap0[1];
};

/* 608 */
struct __cppobj std::_Select1st<std::pair<const long unsigned int,trip_s::KTripSession*> > : std::unary_function<std::pair<const long unsigned int,trip_s::KTripSession*>,const long unsigned int>
{
};

/* 609 */
struct std::map<std::string,trip_s::KTripManager::KINFO>::value_compare;

/* 610 */
struct std::map<long unsigned int,trip_s::KTripSession*>::value_compare;

/* 611 */
struct __attribute__((packed)) __attribute__((aligned(2))) KTRIPDATA_LOGINGS
{
  GUID sLoginGuid;
  DWORD dwLoginIp;
  WORD wLoginPort;
};

/* 612 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) KTRIPSPROTO_B2R_ROLERESULT : KTRIPPROTO_SESSION
{
  int nResult;
  DWORD dwGsIp;
  KTRIPDATA_LOGINGS sLoginParam;
};

/* 613 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) KTRIPSPROTO_R2B_CREATEROLE_WITHDATA : KTRIPPROTO_HEAD
{
  GUID sGuid;
  NEW_PLAYER_COMMAND3 sParam;
  DWORD dwDataLen;
};

/* 614 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) KTRIPSPROTO_R2B_LOADROLE_WITHDATA : KTRIPPROTO_HEAD
{
  GUID sGuid;
  char szAccount[32];
  char szRole[32];
  DWORD dwMapId;
  DWORD dwPosX;
  DWORD dwPosY;
  DWORD dwDataLen;
};

/* 615 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) KTRIPSPROTO_R2B_LOADROLE : KTRIPPROTO_HEAD
{
  GUID sGuid;
  char szAccount[32];
  char szRole[32];
};

/* 616 */
struct __cppobj __attribute__((packed)) __attribute__((aligned(1))) KTRIPSPROTO_B2GS_ROLEDATA : KTRIPPROTO_HEAD
{
  char szAccount[32];
  char szRole[32];
  GUID sLoginGuid;
  DWORD dwRoleDataLen;
  DWORD dwTripDataLen;
};

/* 617 */
struct __cppobj CTaskCommand<CGamePlayer> : ICommand
{
  $DBE2C87402ECD8AE6ACE3192AA0CE987 m_pfunAction;
  CGamePlayer *m_pReceiver;
};

/* 618 */
typedef unsigned __int64 __u_quad_t;

/* 619 */
typedef __u_quad_t __dev_t;

/* 620 */
typedef unsigned int __uid_t;

/* 621 */
typedef unsigned int __gid_t;

/* 622 */
typedef unsigned int __ino_t;

/* 623 */
typedef unsigned int __mode_t;

/* 624 */
typedef unsigned int __nlink_t;

/* 625 */
typedef int __blksize_t;

/* 626 */
typedef int __blkcnt_t;

/* 627 */
struct timespec
{
  __time_t tv_sec;
  int tv_nsec;
};

/* 628 */
struct stat
{
  __dev_t st_dev;
  unsigned __int16 __pad1;
  __ino_t st_ino;
  __mode_t st_mode;
  __nlink_t st_nlink;
  __uid_t st_uid;
  __gid_t st_gid;
  __dev_t st_rdev;
  unsigned __int16 __pad2;
  __off_t st_size;
  __blksize_t st_blksize;
  __blkcnt_t st_blocks;
  timespec st_atim;
  timespec st_mtim;
  timespec st_ctim;
  unsigned int __unused4;
  unsigned int __unused5;
};

/* 629 */
typedef LPSTR LPTSTR;

/* 630 */
typedef LPCSTR LPCTSTR;

/* 631 */
struct $3B56E64613065163E58763561CF03F3A
{
  BYTE Comment[4];
  WORD Width;
  WORD Height;
  WORD CenterX;
  WORD CenterY;
  WORD Frames;
  WORD Colors;
  WORD Directions;
  WORD Interval;
  WORD wPakIdx;
  BYTE bRenderStyle;
  BYTE bReserved1;
  int nNodeIdx;
  BYTE bExchangeColorCount;
  BYTE bReserved2;
  WORD wReserved;
};

/* 632 */
struct $8AF7355A0B219D468BEEBA87D683F623
{
  DWORD Offset;
  DWORD Length;
};

/* 633 */
struct __attribute__((aligned(2))) $FE94C6D27132B4914B56476658CAFF25
{
  WORD Width;
  WORD Height;
  WORD OffsetX;
  WORD OffsetY;
  BYTE Sprite[1];
};

/* 634 */
struct XPackIndexInfo
{
  unsigned int uId;
  unsigned int uOffset;
  unsigned int uSize;
  unsigned int uCompressSizeFlag;
};

/* 635 */
struct XPackFileFragmentInfo
{
  unsigned int uOffset;
  unsigned int uSize;
  unsigned int uCompressSizeFlag;
};

/* 636 */
struct KFile
{
  FILE *m_hFile;
  DWORD m_dwLen;
  DWORD m_dwPos;
  void *m_pContentBuffer;
};

/* 637 */
struct XPackFile
{
  KFile m_File;
  unsigned int m_uFileSize;
  unsigned int m_uElemFileCount;
  int m_nSelfIndex;
  unsigned int m_uPackTime;
  XPackIndexInfo *m_pIndexList;
};

/* 638 */
struct XPackFile::$A708F7F75F60F2D9C3572765D2448569
{
  unsigned int NameId;
  __int16 PakFileIndex;
  unsigned int ElemFileIndex;
  int CacheIndex;
  int Offset;
  unsigned int Size;
};

/* 639 */
struct XPackFile::XPackElemFileCache
{
  void *pBuffer;
  unsigned int uId;
  unsigned int lSize;
  int nPackIndex;
  int nElemIndex;
  unsigned int uRefFlag;
  int nNumFragment;
  XPackFileFragmentInfo *pFragmentInfo;
};

/* 640 */
struct XPackList
{
  XPackFile *m_PakFilePtrList[32];
  int m_nPakNumber;
  char m_szPacksPath[260];
};

/* 641 */
typedef BYTE *LPBYTE;

/* 642 */
struct KMemFile
{
  DWORD m_dwLen;
  DWORD m_dwPos;
  LPBYTE m_pDataRef;
};

/* 643 */
struct XPackFile_0
{
  __int8 gap0[36];
};

/* 644 */
struct XPackFile_0::$A708F7F75F60F2D9C3572765D2448569
{
  unsigned int NameId;
  __int16 PakFileIndex;
  unsigned int ElemFileIndex;
  int CacheIndex;
  int Offset;
  unsigned int Size;
};

/* 645 */
struct KPakFile
{
  KFile m_File;
  KMemFile m_MemFile;
  XPackFile_0::$A708F7F75F60F2D9C3572765D2448569 m_PackRef;
  void *m_pContentBuffer;
};

/* 646 */
struct $BFC4220914E4F7BCC90366403C1A56DC
{
  PBYTE lpData;
  DWORD dwDataLen;
  PBYTE lpPack;
  DWORD dwPackLen;
};

/* 647 */
typedef tagKeyNode KEYNODE;

/* 648 */
struct $6EC4DE85D66C20C0ED03CF1313D029AF
{
  DWORD Id;
  DWORD DataLen;
  DWORD PackLen;
  DWORD Method;
};

/* 649 */
struct KCodec
{
  int (**_vptr_KCodec)(...);
};

/* 650 */
struct KList
{
  KNode *m_pHead;
  KNode *m_pTail;
  int m_nCount;
};

/* 651 */
struct KNode
{
  int (**_vptr_KNode)(...);
  KNode *m_pNext;
  KNode *m_pPrev;
  KList *m_pList;
};

/* 652 */
struct __cppobj KMemList : KList
{
};

/* 653 */
struct KMemNode;

/* 654 */
struct KPakFile_0
{
  KFile m_File;
  KMemFile m_MemFile;
  XPackFile::$A708F7F75F60F2D9C3572765D2448569 m_PackRef;
  void *m_pContentBuffer;
};

/* 657 */
struct KMemClass
{
  PVOID m_lpMemPtr;
  DWORD m_lpMemLen;
};

/* 655 */
struct __cppobj KCodecLzo : KCodec
{
  KMemClass m_WorkMem;
};

/* 656 */
typedef DWORD *PDWORD;

/* 658 */
typedef unsigned __int8 md5_byte_t;

/* 659 */
typedef unsigned int md5_word_t;

/* 660 */
struct md5_state_s
{
  md5_word_t count[2];
  md5_word_t abcd[4];
  md5_byte_t buf[64];
};

/* 661 */
typedef md5_state_s md5_state_t;

/* 662 */
struct std::char_traits<char>
{
  __int8 gap0[1];
};

/* 664 */
struct std::_Vector_base<std::string>::_Vector_impl
{
  std::string *_M_start;
  std::string *_M_finish;
  std::string *_M_end_of_storage;
};

/* 663 */
struct std::_Vector_base<std::string>
{
  std::_Vector_base<std::string>::_Vector_impl _M_impl;
};

/* 665 */
struct __cppobj std::vector<std::string> : std::_Vector_base<std::string>
{
};

/* 666 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<const std::string*,std::vector<std::string> > >;

/* 667 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<std::string*,std::vector<std::string> > >;

/* 668 */
struct std::__copy_backward<false,std::random_access_iterator_tag>
{
  __int8 gap0[1];
};

/* 669 */
struct __gnu_cxx::__normal_iterator<std::string*,std::vector<std::string> >
{
  std::string *_M_current;
};

/* 670 */
struct __gnu_cxx::__normal_iterator<const std::string*,std::vector<std::string> >
{
  const std::string *_M_current;
};

/* 714 */
struct KMemFileManager::tagKMemFileData
{
  DWORD dwDataSize;
  LPBYTE pDataBuf;
};

/* 671 */
struct std::pair<const std::string,KMemFileManager::tagKMemFileData>
{
  const std::string first;
  KMemFileManager::tagKMemFileData second;
};

/* 673 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,KMemFileManager::tagKMemFileData> >
{
  __int8 gap0[1];
};

/* 672 */
struct __cppobj std::allocator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > : __gnu_cxx::new_allocator<std::pair<const std::string,KMemFileManager::tagKMemFileData> >
{
};

/* 675 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,KMemFileManager::tagKMemFileData> > >
{
  __int8 gap0[1];
};

/* 674 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,KMemFileManager::tagKMemFileData> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,KMemFileManager::tagKMemFileData> > >
{
};

/* 676 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,KMemFileManager::tagKMemFileData> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,KMemFileManager::tagKMemFileData> _M_value_field;
};

/* 678 */
struct std::_Rb_tree<std::string,std::pair<const std::string,KMemFileManager::tagKMemFileData>,std::_Select1st<std::pair<const std::string,KMemFileManager::tagKMemFileData> >,std::less<std::string >,std::allocator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > >::_Rb_tree_impl<std::less<std::string >,false>
{
  std::less<std::string > _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 677 */
struct std::_Rb_tree<std::string,std::pair<const std::string,KMemFileManager::tagKMemFileData>,std::_Select1st<std::pair<const std::string,KMemFileManager::tagKMemFileData> >,std::less<std::string >,std::allocator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,KMemFileManager::tagKMemFileData>,std::_Select1st<std::pair<const std::string,KMemFileManager::tagKMemFileData> >,std::less<std::string >,std::allocator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > >::_Rb_tree_impl<std::less<std::string >,false> _M_impl;
};

/* 679 */
struct std::_Rb_tree_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 680 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 681 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > >;

/* 682 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > >;

/* 683 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > first;
  bool second;
};

/* 684 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> >,std::_Rb_tree_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > >;

/* 685 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> >,std::_Rb_tree_const_iterator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > >;

/* 686 */
struct std::map<std::string,KMemFileManager::tagKMemFileData>
{
  std::_Rb_tree<std::string,std::pair<const std::string,KMemFileManager::tagKMemFileData>,std::_Select1st<std::pair<const std::string,KMemFileManager::tagKMemFileData> >,std::less<std::string >,std::allocator<std::pair<const std::string,KMemFileManager::tagKMemFileData> > > _M_t;
};

/* 687 */
struct std::pair<const std::string,IKMemFileNotify*>
{
  const std::string first;
  IKMemFileNotify *second;
};

/* 688 */
struct IKMemFileNotify
{
  int (**_vptr_IKMemFileNotify)(...);
};

/* 690 */
struct __gnu_cxx::new_allocator<std::pair<const std::string,IKMemFileNotify*> >
{
  __int8 gap0[1];
};

/* 689 */
struct __cppobj std::allocator<std::pair<const std::string,IKMemFileNotify*> > : __gnu_cxx::new_allocator<std::pair<const std::string,IKMemFileNotify*> >
{
};

/* 692 */
struct __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,IKMemFileNotify*> > >
{
  __int8 gap0[1];
};

/* 691 */
struct __cppobj std::allocator<std::_Rb_tree_node<std::pair<const std::string,IKMemFileNotify*> > > : __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<const std::string,IKMemFileNotify*> > >
{
};

/* 693 */
struct __cppobj std::_Rb_tree_node<std::pair<const std::string,IKMemFileNotify*> > : std::_Rb_tree_node_base
{
  std::pair<const std::string,IKMemFileNotify*> _M_value_field;
};

/* 695 */
struct std::_Rb_tree<std::string,std::pair<const std::string,IKMemFileNotify*>,std::_Select1st<std::pair<const std::string,IKMemFileNotify*> >,std::less<std::string >,std::allocator<std::pair<const std::string,IKMemFileNotify*> > >::_Rb_tree_impl<std::less<std::string >,false>
{
  std::less<std::string > _M_key_compare;
  std::_Rb_tree_node_base _M_header;
  size_t _M_node_count;
};

/* 694 */
struct std::_Rb_tree<std::string,std::pair<const std::string,IKMemFileNotify*>,std::_Select1st<std::pair<const std::string,IKMemFileNotify*> >,std::less<std::string >,std::allocator<std::pair<const std::string,IKMemFileNotify*> > >
{
  std::_Rb_tree<std::string,std::pair<const std::string,IKMemFileNotify*>,std::_Select1st<std::pair<const std::string,IKMemFileNotify*> >,std::less<std::string >,std::allocator<std::pair<const std::string,IKMemFileNotify*> > >::_Rb_tree_impl<std::less<std::string >,false> _M_impl;
};

/* 696 */
struct std::_Rb_tree_iterator<std::pair<const std::string,IKMemFileNotify*> >
{
  std::_Rb_tree_node_base *_M_node;
};

/* 697 */
struct std::_Rb_tree_const_iterator<std::pair<const std::string,IKMemFileNotify*> >
{
  const std::_Rb_tree_node_base *_M_node;
};

/* 698 */
struct std::reverse_iterator<std::_Rb_tree_iterator<std::pair<const std::string,IKMemFileNotify*> > >;

/* 699 */
struct std::reverse_iterator<std::_Rb_tree_const_iterator<std::pair<const std::string,IKMemFileNotify*> > >;

/* 700 */
struct __attribute__((aligned(4))) std::pair<std::_Rb_tree_iterator<std::pair<const std::string,IKMemFileNotify*> >,bool>
{
  std::_Rb_tree_iterator<std::pair<const std::string,IKMemFileNotify*> > first;
  bool second;
};

/* 701 */
struct std::pair<std::_Rb_tree_iterator<std::pair<const std::string,IKMemFileNotify*> >,std::_Rb_tree_iterator<std::pair<const std::string,IKMemFileNotify*> > >;

/* 702 */
struct std::pair<std::_Rb_tree_const_iterator<std::pair<const std::string,IKMemFileNotify*> >,std::_Rb_tree_const_iterator<std::pair<const std::string,IKMemFileNotify*> > >;

/* 703 */
struct std::map<std::string,IKMemFileNotify*>
{
  std::_Rb_tree<std::string,std::pair<const std::string,IKMemFileNotify*>,std::_Select1st<std::pair<const std::string,IKMemFileNotify*> >,std::less<std::string >,std::allocator<std::pair<const std::string,IKMemFileNotify*> > > _M_t;
};

/* 705 */
struct __gnu_cxx::new_allocator<IKMemFileNotify*>
{
  __int8 gap0[1];
};

/* 704 */
struct __cppobj std::allocator<IKMemFileNotify*> : __gnu_cxx::new_allocator<IKMemFileNotify*>
{
};

/* 707 */
struct std::_Vector_base<IKMemFileNotify*>::_Vector_impl
{
  IKMemFileNotify **_M_start;
  IKMemFileNotify **_M_finish;
  IKMemFileNotify **_M_end_of_storage;
};

/* 706 */
struct std::_Vector_base<IKMemFileNotify*>
{
  std::_Vector_base<IKMemFileNotify*>::_Vector_impl _M_impl;
};

/* 708 */
struct __cppobj std::vector<IKMemFileNotify*> : std::_Vector_base<IKMemFileNotify*>
{
};

/* 709 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<IKMemFileNotify* const*,std::vector<IKMemFileNotify*> > >;

/* 710 */
struct std::reverse_iterator<__gnu_cxx::__normal_iterator<IKMemFileNotify**,std::vector<IKMemFileNotify*> > >;

/* 711 */
struct __gnu_cxx::__normal_iterator<IKMemFileNotify**,std::vector<IKMemFileNotify*> >
{
  IKMemFileNotify **_M_current;
};

/* 712 */
struct __gnu_cxx::__normal_iterator<IKMemFileNotify* const*,std::vector<IKMemFileNotify*> >
{
  IKMemFileNotify *const *_M_current;
};

/* 713 */
struct KMemFileManager
{
  std::map<std::string,KMemFileManager::tagKMemFileData> m_mapFiles;
  std::map<std::string,IKMemFileNotify*> m_mapFileNotifys;
  std::vector<IKMemFileNotify*> m_arFileListNotifys;
};

/* 715 */
struct std::map<std::string,KMemFileManager::tagKMemFileData>::value_compare;

/* 716 */
struct std::map<std::string,IKMemFileNotify*>::value_compare;

/* 717 */
struct std::unary_function<std::pair<const std::string,IKMemFileNotify*>,const std::string >
{
  __int8 gap0[1];
};

/* 718 */
struct __cppobj std::_Select1st<std::pair<const std::string,IKMemFileNotify*> > : std::unary_function<std::pair<const std::string,IKMemFileNotify*>,const std::string >
{
};

/* 719 */
struct std::unary_function<std::pair<const std::string,KMemFileManager::tagKMemFileData>,const std::string >
{
  __int8 gap0[1];
};

/* 720 */
struct __cppobj std::_Select1st<std::pair<const std::string,KMemFileManager::tagKMemFileData> > : std::unary_function<std::pair<const std::string,KMemFileManager::tagKMemFileData>,const std::string >
{
};

/* 721 */
struct XPackFileHeader
{
  unsigned __int8 cSignature[4];
  unsigned int uCount;
  unsigned int uIndexTableOffset;
  unsigned int uDataOffset;
  unsigned int uCrc32;
  unsigned int uPakTime;
  unsigned __int8 cReserved[8];
};

/* 722 */
struct XPackFileFragmentElemHeader
{
  int nNumFragment;
  int nFragmentInfoOffest;
};

/* 723 */
struct XPackSprFrameInfo
{
  int lCompressSize;
  int lSize;
};

/* 724 */
typedef unsigned int ucl_uint32;

/* 725 */
typedef unsigned int ucl_uint;

/* 726 */
struct __attribute__((packed)) __attribute__((aligned(2))) KLARGEPACK_HEAD
{
  BYTE byProtocol;
  BYTE byFlag;
  unsigned int nData;
};

/* 727 */
struct KEnpack
{
  char *m_pBuf;
  size_t m_nMaxLen;
  size_t m_nLen;
  BOOL m_bComplete;
  BYTE m_byProtocol;
  KThreadLock *m_pLock;
};

/* 728 */
struct __attribute__((aligned(4))) KDopack
{
  char m_szBuf[65536];
  const char *m_pData;
  size_t m_nLen;
  size_t m_nOffset;
  size_t m_nPackLen;
  unsigned int m_nData;
  BYTE m_byProtocol;
};

/* 729 */
struct KDepack
{
  KDopack m_cDoPack;
  char *m_pBuf;
  size_t m_nMaxLen;
  size_t m_nLen;
  KThreadLock *m_pLock;
};

/* 730 */
typedef unsigned int CRC_UINT32;

/* 731 */
typedef unsigned __int8 CRC_BYTE;

/* 732 */
struct std::string;

