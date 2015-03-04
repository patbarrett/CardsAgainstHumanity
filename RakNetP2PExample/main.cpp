#include "Client.h"


int main()
{
	CClient* myClient = new CClient();
	myClient->Init();
	myClient->Connect("127.0.0.1",60000);

	while(1)
	{
		//.. do stuff
		myClient->Update();
	}

	myClient->Destroy();
	delete myClient;
	
	return 0;
}