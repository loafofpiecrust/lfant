/******************************************************************************
 *
 * ShadowFox Engine Source
 * Copyright (C) 2012-2013 by ShadowFox Studios
 * Created: 2012-09-15 by Taylor Snead
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "SystemInfo.hpp"

// External

// Internal
#include "Console.hpp"
#include "String.hpp"

namespace sfs
{

SystemInfo::SystemInfo()
{
}

SystemInfo::~SystemInfo()
{
}

#if WINDOWS

void SystemInfo::Init()
{
	SYSTEM_INFO info;
	GetNativeSystemInfo(&info);
	/// @todo Change rest to use this

	// Computer name
	{
		TCHAR name[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD size = sizeof(name) / sizeof(name[0]);
		GetComputerName(name, &size);
		computerName = name;
	}

	// Username
	{
		TCHAR name[60];
		DWORD size = sizeof(name) / sizeof(name[0]);
		GetUserName(name, &size);
		username = name;
	}

	// Monitor information
	monitor.resolution.x = GetSystemMetrics(SM_CXSCREEN);
	monitor.resolution.y = GetSystemMetrics(SM_CYSCREEN);

	// Operating System
	OSVERSIONINFO verInfo = { sizeof(OSVERSIONINFO) };
	GetVersionEx(&verInfo);
	if(verInfo.dwMajorVersion == 5)
	{
		switch(verInfo.dwMinorVersion)
		{
			case 0:
				OS = "Windows 2000 " + string(verInfo.szCSDVersion);
				break;
			case 1:
				OS = "Windows XP " + string(verInfo.szCSDVersion);
				break;
			case 2:
				OS = "Windows 2003 " + string(verInfo.szCSDVersion);
				break;
		}
	}
	else if(verInfo.dwMajorVersion == 6)
	{
		switch(verInfo.dwMinorVersion)
		{
			case 0:
				OS = "Windows Vista " + string(verInfo.szCSDVersion);
				break;
			case 1:
				OS = "Windows 7 " + string(verInfo.szCSDVersion);
				break;
			case 2:
				OS = "Windows 8 " + string(verInfo.szCSDVersion);
				break;
		}
	}

	// Processor/CPU
	{
		int CPUInfo[4] = { -1};
		__cpuid(CPUInfo, 0x80000000);
		unsigned int nExIds = CPUInfo[0];
		char CPUBrandString[0x40] = { 0 };
		for(uint i = 0x80000000; i < nExIds; ++i)
		{
			__cpuid(CPUInfo, i);

			if(i == 0x80000002)
			{
				memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
			}
			else if(i == 0x80000003)
			{
				memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
			}
			else if(i == 0x80000004)
			{
				memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
			}
		}
		cpu.cores = (typeof(cpu.cores))info.dwNumberOfProcessors;     //(t >> 16) & 0xff;
		cpu.name = CPUBrandString;//info.dwProcessorType;
	}
	/*
	 HKEY hKey, tempKey;
	 if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,"HARDWARE\\DESCRIPTION\\System\\CentralProcessor",0,KEY_READ,&hKey) == ERROR_SUCCESS)
	 {
	 uint i;
	 while(RegEnumKey(hKey,++i,subKeyName,40) != ERROR_NO_MORE_ITEMS)
	 {
	 if(RegOpenKeyEx(hKey,subKeyName,0,KEY_READ,&tempKey) == ERROR_SUCCESS)
	 {
	 size = 100;
	 if(RegQueryValueEx(tempKey,_T("ProcessorNameString"),NULL,NULL,valBuf,&size) == ERROR_SUCCESS)
	 cpu.name = valBuf;
	 size = 100;
	 if(RegQueryValueEx(tempKey,_T("Identifier"),NULL,NULL,valBuf,&size) == ERROR_SUCCESS)
	 _tprintf(_T(" %s"),valBuf);
	 RegCloseKey(tempKey);
	 }
	 }
	 RegCloseKey(hKey);
	 }*/
	/*
	// Memory/RAM
	MEMORYSTATUSEX ms =
	{ sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&ms);
	memory = ms.ullTotalPhys / (1024 * 1024.0);

	// Hard Drives
	int drives;
	__int64 nFree, nTotal;
	TCHAR dName[40], volName[40];

	uint i = 0;

	drives = GetLogicalDrives();
	while (drives != 0)
	{
		if ((drives & 1) == 1)
		{
			wsprintf(dName, "%c:\\", 'A' + i);
			if (GetDriveType(dName) == DRIVE_FIXED)
			{
				if (GetDiskFreeSpaceEx(dName, (PULARGE_INTEGER)&nFree, (PULARGE_INTEGER)&nTotal, 0) != 0)
				{
					HardDrive hdd;
					hdd.name = dName;
					hdd.free = nFree / (1024 * 1024 * 1024.0);
					hdd.total = nTotal / (1024 * 1024 * 1024.0);
					GetVolumeInformation(dName, 0, 0, 0, 0, 0, volName, 10);
					sprintf("%s", volName);
				}
			}
		}
		drives >>= 1;
		++i;
	}*/
}

#elif OSX

void SystemInfo::Init()
{

}

#elif LINUX
void SystemInfo::Init()
{
	FILE* cmd;
	int status;
	char result[250];

	cmd = popen("/bin/uname -a", "r");
	while(fgets(result, sizeof(result) - 1, cmd) != 0)
	{
		printf("%s", result);
	}
	pclose(cmd);

	vector<string> result_spl = Split(string(result), " \n");
	OS = result_spl[result_spl.size() - 1] + " " + result_spl[2];
	computerName = result_spl[1];

}
#endif

}
