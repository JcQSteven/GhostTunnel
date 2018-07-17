#pragma once
#define _CRT_SECURE_NO_DEPRECATE


#include <windows.h>
#include <wlanapi.h>
#include <stdio.h>
#pragma comment(lib, "wlanapi.lib")	//表示链接wlanapi.lib这个库
#pragma comment(lib, "ole32.lib")
#include "string"
#include "fstream"


#define __DEBUG__
#ifdef  __DEBUG__
#define HLOG(format,...) printf(format,##__VA_ARGS__)
#else
#define HLOG(format,...)
#endif


struct ie_data	//3个字节
{
	unsigned char id;
	unsigned char len;
	unsigned char val[1];
};

