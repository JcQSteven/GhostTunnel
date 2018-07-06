#pragma once
//payload结构体
struct ie_data	//3个字节
{
	unsigned char id;
	unsigned char len;
	unsigned char val[1];
};

struct STATUS_WLAN {
	DWORD STATUS_handle;
	DWORD STATUS_wlanList;
	DWORD STATUS_sendRequest;
};