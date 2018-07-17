#pragma once
#include"stdafx.h"

class mainProcess
{
private:
	HANDLE hClient;//指定wlan客户端在会话中使用的句柄，这个句柄会贯穿整个会话被其他函数使用
	PWLAN_INTERFACE_INFO_LIST pIfList;//指向存储 本地计算机启用的无线LAN接口 的指针
	PWLAN_INTERFACE_INFO pIfInfo;//网卡信息
	PWLAN_AVAILABLE_NETWORK_LIST pBssList; //附近网络信息列表
	WLAN_AVAILABLE_NETWORK* pBssEntry;
	char *sendInfo;
	char *hash;
public:
	mainProcess();
	~mainProcess();
	DWORD get_Handle(HANDLE *hClient, DWORD dwMaxClient, DWORD dwCurVersion);
	DWORD get_WlanList(HANDLE *hClient, PWLAN_INTERFACE_INFO_LIST *pIfList);
	PWLAN_INTERFACE_INFO get_Wlan(PWLAN_INTERFACE_INFO_LIST *pIfList);
	DWORD sendRequest(HANDLE *hClient, PWLAN_INTERFACE_INFO *pIfInfo, PWLAN_RAW_DATA pwlan_data);
	boolean getssid(HANDLE *hClient, PWLAN_INTERFACE_INFO *pIfInfo, char *ssid);
	PWLAN_RAW_DATA get_payload(char *buf);
	void run();
	char *get_sendInfo();
	void set_sendInfo(char* infomation);
	char *get_hash();
	void set_hash(char* hash);
};

