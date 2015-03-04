#include "Client.h"
#include <iostream>
#include "Bitstream.h"
#include "RakNetTypes.h"

CClient::CClient() 
{
	//m_GameObject = new GameObject();
}

CClient::~CClient()
{
	//delete m_GameObject;
}

void CClient::Init()
{
	m_Peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd;
	m_Peer->Startup(1,&sd, 1);
	
}

void CClient::Update()
{
	RakNet::Packet *packet;
	for (packet=m_Peer->Receive(); packet; m_Peer->DeallocatePacket(packet), packet=m_Peer->Receive())
	{
		switch (packet->data[0])
		{
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
				m_ServerAddress = packet->systemAddress;
				SendGameObject();
				break;
			case ID_CONNECTION_LOST:
				printf("ID_CONNECTION_LOST\n");
				break;
			case ID_CREATE_GAMEOBJECT:
				CreateGameObject(packet);
				break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
		}
	}
}

void CClient::Destroy()
{
	if(m_Peer)
		RakNet::RakPeerInterface::DestroyInstance(m_Peer);
}

bool CClient::Connect(const char* host, unsigned short remotePort)
{
	RakNet::ConnectionAttemptResult result;
	result = m_Peer->Connect(host, remotePort,0,0);
	return result == RakNet::CONNECTION_ATTEMPT_STARTED;
}

void CClient::SendGameObject() const
{
	/*RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)CP_GameObject);
	bs.Write((unsigned char*)m_GameObject);
	m_Peer->Send(&bs,HIGH_PRIORITY,RELIABLE_ORDERED,0,m_ServerAddress,false);*/
}

void CClient::CreateGameObject(RakNet::Packet* packet)
{
	RakNet::BitStream bsIn(packet->data, packet->length, false);
	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
	RakNet::NetworkID gameObjectNetworkID;
	bsIn.Read(gameObjectNetworkID);
	GameObject* gameObject = new GameObject();
	gameObject->SetNetworkIDManager(&m_NetworkIDManager);
	gameObject->SetNetworkID(gameObjectNetworkID); 
	m_GameObjects.insert(std::pair<RakNet::NetworkID,GameObject*>(gameObjectNetworkID,gameObject));
}