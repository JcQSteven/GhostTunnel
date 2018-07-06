#include "stdafx.h"
#include "Action_Abs.h"
#include <string.h>
#include <stdlib.h>
using namespace std;


Action_Abs::Action_Abs() {
	sendInfo = (char *)malloc(255);
	hash = (char *)malloc(9);
}

Action_Abs::~Action_Abs() {
	free(sendInfo);
	free(hash);
}

void Action_Abs::set_sendInfo(char* command) {
	//HLOG("========info===============设置前！！！%s", get_sendInfo());
	//HLOG("command！！！%s", command);
	memset(sendInfo, '\0', 255);
	strncpy(sendInfo, command, strlen(command));
	//HLOG("设置后！！！%s=====================", get_sendInfo());
}

char *Action_Abs::get_sendInfo() {
	return sendInfo;
}

void Action_Abs::set_hash(char* h) {
	//HLOG("设置前！！！%s", get_hash());
	memset(hash, '\0', 9);
	strncpy(hash, h, 8);
	//HLOG("设置后！！！%s", get_hash());
}

char *Action_Abs::get_hash() {
	return hash;
}