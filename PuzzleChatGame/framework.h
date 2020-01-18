// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

// 使用ライブラリ
#pragma comment(lib,"wsock32.lib")

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <Windows.h>
#include <WinSock.h>
#include <string>
#include <stdio.h>
#include <ctime>