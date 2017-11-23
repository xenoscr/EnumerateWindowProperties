// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <iostream>
#include <map>
#include <string>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <psapi.h>
#include <strsafe.h>
#include <sstream>

namespace std {
#if defined _UNICODE || defined UNICODE
	typedef wstring tstring;
#else
	typedef string tstring;
#endif
}

// TODO: reference additional headers your program requires here
