//Class CServer
#ifndef SERVER_H
#define SERVER_H

#include "GameObject.h"
#include <vector>
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "NetworkIDManager.h"


class CServer
{
public:
	CServer();
	~CServer();

	void Init();
	void Destroy();
	void Update();
	static const unsigned short MAX_CLIENTS = 10;
	static const unsigned short SERVER_PORT = 60000;
private:
	RakNet::RakPeerInterface* m_Peer;

protected:

};


#endif
