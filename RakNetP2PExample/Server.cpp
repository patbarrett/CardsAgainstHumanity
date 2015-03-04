#include "Server.h"

#include "RakNetTypes.h"  // MessageID
#include "BitStream.h"
#include "GameObject.h"

CServer::CServer() : m_Peer(0)
{
}

CServer::~CServer()
{
	Destroy();
}

void CServer::Init()
{
	m_Peer = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd(SERVER_PORT,0);
	m_Peer->Startup(MAX_CLIENTS, &sd, 1);
	m_Peer->SetMaximumIncomingConnections(MAX_CLIENTS);
}

void CServer::Destroy()
{
	if(m_Peer)
		RakNet::RakPeerInterface::DestroyInstance(m_Peer);
}

void CServer::Update()
{
	RakNet::Packet *packet;
	for (packet=m_Peer->Receive(); packet; m_Peer->DeallocatePacket(packet), packet=m_Peer->Receive())
	{
		switch (packet->data[0])
		{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				CreateGameObject();
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				CreateGameObject();
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
					printf("A client has disconnected.\n");
				break;
			case ID_CONNECTION_LOST:
				printf("A client lost the connection.\n");
				break;
			case CP_GameObject:
				HandleGameObject(packet);
			break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
		}
	}
}

void CServer::HandleGameObject(RakNet::Packet* packet) const
{
	RakNet::BitStream bs(packet->data,packet->length,false);
	bs.IgnoreBytes(sizeof(RakNet::MessageID));
	const int GameObjectSize = sizeof(GameObject);
	unsigned char *gameObjectChar = new unsigned char[GameObjectSize]; 
	bs.Read(gameObjectChar);
	if(gameObjectChar)
	{
		GameObject* gameObject = reinterpret_cast<GameObject*>( gameObjectChar );
		if(gameObject)
		{
			gameObject->PrintPosition();
		}
	}
}

void CServer::CreateGameObject()
{
	GameObject *gameObject = new GameObject();
	gameObject->SetNetworkIDManager(&m_NetworkIDManager);
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_CREATE_GAMEOBJECT);
	bsOut.Write(gameObject->GetNetworkID());
	//broadcasting a message
	m_Peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,RakNet::UNASSIGNED_SYSTEM_ADDRESS,true);
	m_GameObjects.push_back(gameObject);
}