#pragma once

#ifdef _WIN64
	#pragma comment(lib, "ServerCore_x64")
#else
	#pragma comment(lib, "ServerCore_x86")
#endif

#include "CorePch.h"