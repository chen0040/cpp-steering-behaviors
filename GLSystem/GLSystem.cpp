#include "GLSystem.h"
#include <atlbase.h>
#include <windows.h>
#include <tchar.h>

GLSystem::GLSystem()
{

}

GLSystem::~GLSystem()
{

}

GLSystem* GLSystem::Instance()
{
	static GLSystem theInstance;
	return &theInstance;
}

void GLSystem::execute(const std::string &cmd_exe)
{
	//ShellExecute(NULL, _T("open"), _T("espeak.exe"), CA2CT(oss.str().c_str()), _T(""), SW_HIDE);
	STARTUPINFOW siStartupInfo;
     PROCESS_INFORMATION piProcessInfo;
     memset(&siStartupInfo, 0, sizeof(siStartupInfo));
     memset(&piProcessInfo, 0, sizeof(piProcessInfo));
     siStartupInfo.cb = sizeof(siStartupInfo);

	 //const_cast<LPCWSTR>
     if (CreateProcess(NULL, CA2CT(cmd_exe.c_str()),
                                  NULL, NULL, false,
                                  NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL,
                                  &siStartupInfo, &piProcessInfo) != false)
     {
          /* Watch the process. */
          WaitForSingleObject(piProcessInfo.hProcess, 200); //INFINITE
     }

     /* Release handles */
     CloseHandle(piProcessInfo.hProcess);
     CloseHandle(piProcessInfo.hThread);
}
