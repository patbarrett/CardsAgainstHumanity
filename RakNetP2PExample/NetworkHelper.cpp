#include "NetworkHelper.h"
#include "BitStream.h"
#include <string>

NetworkHelper::NetworkHelper() : m_RakPeer(0), m_IsHost(false), m_IsMaster(false), m_IsInGame(false), connectedUsers(0), isgameStart(false), askerID(0), winnerID(10)
{
}

void NetworkHelper::Init(bool isHost, const char* customMsg /*= 0*/)
{
	question = " ";

	m_RakPeer = RakPeerInterface::GetInstance();
	m_IsHost = isHost;
	
	m_RakPeer->AttachPlugin(&m_ReadyEventPlugin);
	m_RakPeer->AttachPlugin(&m_FCM2);
	m_RakPeer->AttachPlugin(&m_ConnectedGraph2);

	m_RakPeer->SetMaximumIncomingConnections(MAX_CONNECTIONS);
	
	m_FCM2.SetAutoparticipateConnections(true);
	m_FCM2.SetConnectOnNewRemoteConnection(true, "");
	m_ConnectedGraph2.SetAutoProcessNewConnections(true);

	SocketDescriptor sd(PORT,0);
	if(isHost)
	{
		std::string hostMsg("Welcome to ");
		
		if(customMsg)
		{
			/*hostMsg2 = strcat(hostMsg,customMsg);
			hostMsg2 = strcat(hostMsg," server");*/
			hostMsg += customMsg;
			hostMsg += " server";
		}
		else
		{
			/*hostMsg2 = strcat(hostMsg,"some random server");*/
			hostMsg += "some random server";
		}
		//size_t len = sizeof(hostMsg2);
		m_RakPeer->SetOfflinePingResponse(hostMsg.c_str(),hostMsg.length());
		while (IRNS2_Berkley::IsPortInUse(sd.port,sd.hostAddress,sd.socketFamily,SOCK_DGRAM)==true)
		sd.port++;

		StartupResult sr = m_RakPeer->Startup(MAX_CONNECTIONS, &sd, 1);
		RakAssert(sr==RAKNET_STARTED);
	}
	else
	{
		sd.socketFamily = AF_INET;
		while (IRNS2_Berkley::IsPortInUse(sd.port,sd.hostAddress,sd.socketFamily,SOCK_DGRAM)==true)
		sd.port++;
		m_RakPeer->Startup(MAX_CONNECTIONS, &sd, 1);
		m_RakPeer->Ping("255.255.255.255", PORT, true);
		printf("Pinging\n");
	}
}

const char* NetworkHelper::GetIPAddress()
{
	return m_RakPeer->GetLocalIP(0);
}

void NetworkHelper::Destroy()
{
	if(m_RakPeer)
		RakPeerInterface::DestroyInstance(m_RakPeer);
}

bool NetworkHelper::EstablishConnection(const char* hostIP)
{
	ConnectionAttemptResult car = m_RakPeer->Connect(hostIP, PORT, 0, 0, 0);
	RakAssert(car==CONNECTION_ATTEMPT_STARTED);
	return true;
}

void NetworkHelper::SendBroadcastPacket(MessageID header, char *msg)
{
	RakNet::BitStream bs;
	bs.Write(header);
	bs.Write(msg);
	m_RakPeer->Send(&bs,PacketPriority::HIGH_PRIORITY,PacketReliability::RELIABLE_ORDERED,0,UNASSIGNED_SYSTEM_ADDRESS,true);
}
void NetworkHelper::SendBroadcastPacket(MessageID header, int num)
{
	RakNet::BitStream bs;
	bs.Write(header);
	bs.Write(num);
	m_RakPeer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}
void NetworkHelper::SendBroadcastPacket(MessageID header, std::string msg)
{
	RakNet::BitStream bs;
	bs.Write(header);
	bs.Write(msg);
	m_RakPeer->Send(&bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void NetworkHelper::SetEvent(READYEVENTIDs eventId, bool isReady)
{
	m_ReadyEventPlugin.SetEvent(eventId, isReady);

}

void NetworkHelper::HandleEventAllSet(int eventId)
{
	if(eventId == RE_START)
	{
		system("cls");
		printf("game\n");
		printf("press 'f' to finish game\n");
		m_IsInGame = true;

	}
	if(eventId == RE_FINISH)
	{
		system("cls");
		printf("over\n");
		printf("new highscore, congrats\n");
	}
}

void NetworkHelper::Update()
{
	Packet *p; 
	for(p = m_RakPeer->Receive();p != NULL; m_RakPeer->DeallocatePacket(p), p=m_RakPeer->Receive())
	{
		switch (p->data[0])
		{
			//server side
		case ID_NEW_INCOMING_CONNECTION:
			//printf("ID_NEW_INCOMING_CONNECTION %s \n", p->guid.ToString());
			system("cls");
			m_ReadyEventPlugin.AddToWaitList(RE_START, p->guid);
			break;
			//client side
		case ID_CONNECTION_REQUEST_ACCEPTED:
			//printf("ID_CONNECTION_REQUEST_ACCEPTED\n");
			system("cls");
			//adding peers that you are connected to, to your event list
			m_ReadyEventPlugin.AddToWaitList(RE_START, p->guid);
			for(std::vector<INetworkBridge*>::iterator start = m_NetBridgeListeners.begin(); start != m_NetBridgeListeners.end(); ++start)
			{
				(*start)->OnConnectionAccepted();
			}
			break;
			//clients recieve this
		case ID_READY_EVENT_ALL_SET:
			//isgameStart = true;
			printf("Got ID_READY_EVENT_ALL_SET from %s\n", p->guid.ToString());
			{
			
				

				/*int eventId = 0;
				RakNet::BitStream bs(p->data,p->length,false);
				bs.IgnoreBytes(sizeof(MessageID));
				bs.Read(eventId);
				HandleEventAllSet(eventId);
				
				if(eventId == RE_START)
					m_ReadyEventPlugin.AddToWaitList(RE_FINISH, p->guid);*/
				for(std::vector<INetworkBridge*>::iterator start = m_NetBridgeListeners.begin(); start != m_NetBridgeListeners.end(); ++start)
				{
					
					(*start)->OnReadyEventAllSet();
				}
			}
			break;
			//clients recieve
		case ID_READY_EVENT_SET:
			
			connectedUsers++;

			if(m_IsHost)
			{
				printf("Got ID_READY_EVENT_SET from %s\n", p->guid.ToString());
				int waitSize = m_ReadyEventPlugin.GetRemoteWaitListSize(RE_START);
				bool isReady = true;
				for(int i = 0; i < waitSize; ++i)
				{
					RakNet::RakNetGUID guid = m_ReadyEventPlugin.GetFromWaitListAtIndex(RE_START,i);
					if(m_RakPeer->GetMyGUID() != guid)
					{
						RakNet::ReadyEventSystemStatus status = m_ReadyEventPlugin.GetReadyStatus(RE_START,guid);
						if(status != RakNet::RES_READY)
						{
							isReady = false;
							break;
						}
					}
				}//end of for loop
				if(isReady)
				{
					for(std::vector<INetworkBridge*>::iterator start = m_NetBridgeListeners.begin(); start != m_NetBridgeListeners.end(); ++start)
					{
						(*start)->OnWaitingForHostReady();
					}
				}
			}
			break;
		case ID_READY_EVENT_UNSET:
			printf("Got ID_READY_EVENT_UNSET from %s\n", p->guid.ToString());
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION\n");
			break;
		case ID_ALREADY_CONNECTED:
			// Connection lost normally
			printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", p->guid);
			break;
		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
			break;
		case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
			printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n"); 
			break;
		case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
			printf("ID_REMOTE_CONNECTION_LOST\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
			printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
			break;
		case ID_CONNECTION_BANNED: // Banned from this server
			printf("We are banned from this server.\n");
			break;			
		case ID_CONNECTION_ATTEMPT_FAILED:
			printf("Connection attempt failed\n");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			// Sorry, the server is full.  I don't do anything here but
			// A real app should tell the user
			printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
			break;
		case ID_INVALID_PASSWORD:
			printf("ID_INVALID_PASSWORD\n");
			break;
		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf("ID_CONNECTION_LOST\n");
			break;
		case ID_CONNECTED_PING:
			printf("Connected Ping from %s\n", p->systemAddress.ToString(true));
			break;
		case ID_UNCONNECTED_PING:
			printf("Unconnected Ping from %s\n", p->systemAddress.ToString(true));
			break;
		case ID_UNCONNECTED_PONG:
			printf("Unconnected Pong from %s\n", p->systemAddress.ToString(true));
			{
				unsigned char* data = p->data + sizeof( unsigned char ) + sizeof( RakNet::TimeMS );
				printf("%s\n",data);
			}
			break;
		case MI_CHAT:
			{
				RakNet::BitStream bs(p->data,p->length,false);
				bs.IgnoreBytes(sizeof(MessageID));
				RakString msg;
				bs.Read(msg);
				printf("%s\n",msg.C_String());
			}
		case MI_SECRETNUMBER:
		{
			RakNet::BitStream bs(p->data, p->length, false);
			bs.IgnoreBytes(sizeof(MessageID));
			int num;
			m_RandomNumber = bs.Read(num);
					
		}
		case MIQUESTION:
		{
			RakNet::BitStream bs(p->data, p->length, false);
			bs.IgnoreBytes(sizeof(MessageID));
			RakString msg;
			bs.Read(msg);
			//printf("%s\n", msg.C_String());
			const char *c = msg.C_String();
			std::string q(c);
			question = q;
			
		}
		case ADDUSER:
		{
			connectedUsers++;
		}
		case ASKERID:
		{
			RakNet::BitStream bs(p->data, p->length, false);
			bs.IgnoreBytes(sizeof(MessageID));
			int num;
			askerID = bs.Read(num);

		}
		break;
		case QUESTIONANSWER:
		{
			RakNet::BitStream bs(p->data, p->length, false);
			bs.IgnoreBytes(sizeof(MessageID));
			RakString msg;
			bs.Read(msg);
			//printf("%s\n", msg.C_String());
			const char *c = msg.C_String();
			std::string q(c);
			questionanswer = q;
			printf("%s", questionanswer);

		}break;
		case WINNERID:
		{
			


			RakNet::BitStream bs(p->data, p->length, false);
			bs.IgnoreBytes(sizeof(MessageID));
			int num;
			bs.Read(num);
			winnerID = num;
			//winnerID = bs.Read(num);
			//printf("%i", winnerID);

		}
			break;
		}
	}		
}