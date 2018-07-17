#include "Action_Sendfile.h"

long hex2int(const std::string& hexStr)
{
	char *offset;
	if (hexStr.length() > 2) {
		if (hexStr[0] == '0' && hexStr[1] == 'x') {
			return strtol(hexStr.c_str(), &offset, 0);
		}
	}
	return strtol(hexStr.c_str(), &offset, 16);
}

void Action_Sendfile::ExcuteAction(ie_data * ie, char * magic){
	char* filenamelen = (char *)malloc(2);
	strncpy(filenamelen, magic + 1, 2);
	char* filelen = (char *)malloc(2);
	strncpy(filelen, magic + 11, 2);
	char* CurrentIndex = (char *)malloc(2);
	strncpy(CurrentIndex, magic + 13, 2);
	char* fileName = (char *)malloc(hex2int(filenamelen) + 1);
	strncpy(fileName, magic + 15, hex2int(filenamelen));
	strncpy(fileName + hex2int(filenamelen), "\0", 1);

	char command[221] = { 0 };
	strncpy_s(command, magic + 15 + hex2int(filenamelen), ie->len - 15 - hex2int(filenamelen));
	HLOG("收到的信息：\n%s\n", command);//执行命令
	HLOG("合计：%s  : filenamelen [%d] , buflen [%d] , CurrentIndex [%d] , filelen[%d] \n", fileName, hex2int(filenamelen), ie->len - 15 - hex2int(filenamelen), hex2int(CurrentIndex), hex2int(filelen));
	std::ofstream outfile;
	if (hex2int(CurrentIndex) == 1) {//第一帧
		outfile.open(fileName, std::ios::out | std::ios::binary);
	}
	else {
		outfile.open(fileName, std::ios::app | std::ios::binary);
	}
	outfile << command;
	outfile.close();

	if (hex2int(CurrentIndex) == hex2int(filelen)) {//最后一帧
	}

}
