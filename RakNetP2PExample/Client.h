//Class CCleint
#ifndef CLIENT_H
#define CLIENT_H

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "GameObject.h"
#include "NetworkIDManager.h"
#include <map>

class CClient
{
public:

	enum CustomPacket
	{
		CP_1 = ID_USER_PACKET_ENUM+1,
		CP_GameObject,
		ID_CREATE_GAMEOBJECT,
	};

	CClient();
	~CClient();

	void Init();
	void Destroy();

	//CREATE AN UPDATE FUNCTION
	//LISTENS FOR PACKETS
	void Update();

	void SendGameObject() const;
	void CreateGameObject(RakNet::Packet* packet);

	bool Connect(const char* host, unsigned short remotePort);

private:
	RakNet::RakPeerInterface* m_Peer;
	
	std::map<RakNet::NetworkID,GameObject*> m_GameObjects;

	RakNet::NetworkIDManager m_NetworkIDManager;

	RakNet::SystemAddress m_ServerAddress;

};

#endif