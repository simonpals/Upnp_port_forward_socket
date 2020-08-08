#pragma once
#include "includes.h"
class SServer
{
public:
	SServer();
	~SServer();
	bool startServer();
	void closeServer();
	void handle();
	unsigned short port;
private:
	SOCKET this_s;
	WSAData wData;
};

