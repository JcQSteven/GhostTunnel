#pragma once

class Action_Abs {

protected:
	char *sendInfo;
	char *hash;
public:
	Action_Abs();
	~Action_Abs();
	char *get_sendInfo();
	void set_sendInfo(char* infomation);
	char *get_hash();
	void set_hash(char* hash);
	virtual  bool ResolutionCMD(char* pp, int total_size) = 0;
};

