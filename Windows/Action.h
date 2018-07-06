#pragma once
#include "Action_Abs.h"

class Action_1:public Action_Abs
{
public:
	Action_1();
	~Action_1();
	bool ResolutionCMD(char *pp, int total_size);
	void set_sendInfo(char* infomation);
};

