/* Nap cuong buc khi dung Core (/FI). Engine khai bao g_strcpy va g_strcpyLen
   voi chu 's' thuong; Core goi chung voi chu 'S' hoa. Moi ten g_Str* con lai
   thi khop, nen chi bac cau dung hai ten nay. */
#ifndef MSVC_COMPAT_H
#define MSVC_COMPAT_H
/* KProtocolProcess.cpp dung std::string va std::vector ma khong include; truoc day
   chung den qua <windows.h> hoac qua header khac da doi. */
#include <string>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#define g_StrCpy    g_strcpy
#define g_StrCpyLen g_strcpyLen
#endif
