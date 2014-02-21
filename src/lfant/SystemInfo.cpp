/******************************************************************************
*
* LFANT Source
* Copyright (C) 2012-2013 by LazyFox Studios
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

#include <lfant/SystemInfo.h>

// Internal
#include <lfant/Console.h>
#include <lfant/util/String.h>

// External

namespace lfant
{

SystemInfo::SystemInfo()
{
}

SystemInfo::~SystemInfo()
{
}

void SystemInfo::Destroy()
{
	Subsystem::Destroy();
}

#if WINDOWS

void SystemInfo::Init()
{
	Subsystem::Init();

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
		cpu.cores = (typeof(cpu.cores))info.dwNumberOfProcessors;             //(t >> 16) & 0xff;
		cpu.name = CPUBrandString;        //info.dwProcessorType;
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

#elif MACOSX

void SystemInfo::Init()
{

}

#elif LINUX
void SystemInfo::Init()
{
	Subsystem::Init();

	FILE* cmd;
	int status;
	char result[250];

	cmd = popen("/bin/uname -a", "r");
	while(fgets(result, sizeof(result) - 1, cmd) != 0)
	{
		printf("%s", result);
	}
	pclose(cmd);

	std::deque<string> result_spl = Split(string(result), " \n");
	OS = result_spl[result_spl.size() - 1] + " " + result_spl[2];
	computerName = result_spl[1];

	// GPU currently in use
	/*
	cmd = popen("inxi -G", "r");
	uint cnt = 0;
	deque<string> line;
	while(fgets(result, sizeof(result) - 1, cmd) != 0)
	{
		Log("Count: ", cnt);
		if(cnt == 0)
		{
			++cnt;
			continue;
		}
		else if(cnt == 1)
		{
			line = Split(result, " ");
			for(uint i = 0; i < line.size(); ++i)
			{
				if(line[i] == "Resolution:")
				{
					deque<string> res = Split(line[i+1], "x");
					monitor.resolution.x = lexical_cast<uint16_t>(res[0]);
					monitor.resolution.y = lexical_cast<uint16_t>(res[2]);
					break;
				}
			}
		}
		else if(cnt == 2)
		{
			Log("Past split, res: ", result);
			line = Split(string(result), " \t");
			int begin = 0, diff = 0;
			for(uint i = 0; i < line.size(); ++i)
			{
				Log("Line portion: ", line[i]);
				if(line[i] == "Renderer:")
				{
					begin = i;
					Log("Begin ", begin);
				}
				else if(line[i] == "GLX" && line[i+1] == "Version:")
				{
					diff = i-begin;
					Log("Diff: ", diff);
				}
			}

			for(uint i = begin; i < diff; ++i)
			{
				gpu.append(line[i]);
				if(i+1 < diff)
				{
					gpu.append(" ");
				}
			}

			Log("Gpu: "+gpu);

			// OpenGL Version supported
			for(uint i = 0; i < line.size(); ++i)
			{
				if(line[i] == "Version:")
				{
					glVersion.major = lexical_cast<uint16_t>(line[i+1][0]);
					glVersion.minor = lexical_cast<uint16_t>(line[i+1][2]);
					break;
				}
			}

		}
		++cnt;
	}
	pclose(cmd);
	*/

}
#elif ANDROID

void SystemInfo::Init()
{

}

#endif

}
