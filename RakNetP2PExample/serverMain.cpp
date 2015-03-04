#include "Server.h"

int main()
{
	CServer* myServer = new CServer();
	myServer->Init();

	//do stuff
	while(1)
	{
		myServer->Update();
	}

	myServer->Destroy();
	delete myServer;
}