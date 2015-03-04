#ifndef NETWORKHELPER
#define NETWORKHELPER

#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "ReadyEvent.h"
#include "SocketLayer.h"
#include "FullyConnectedMesh2.h"
#include "ConnectionGraph2.h"
#include "..\ConsoleApplication1\NetworkBridge.h"

#include <vector>

using namespace RakNet;

class NetworkHelper 
{
public:

	int m_RandomNumber;
	int connectedUsers;
	bool isgameStart;
	int askerID;
	int winnerID;
	

	std::string question;
	std::string questionanswer;

	enum READYEVENTIDs {RE_START = 0, RE_FINISH };

	enum MessageIdentifiers { MI_CHAT = ID_USER_PACKET_ENUM, MI_SECRETNUMBER,MIQUESTION, ADDUSER,ASKERID,QUESTIONANSWER,WINNERID};

	NetworkHelper();
	~NetworkHelper() {}

	void Init(bool isHost, const char *customMsg = 0);
	void Destroy();
	bool EstablishConnection(const char* hostIP);
	void Update();

	void SetIsMaster(bool isMaster) { m_IsMaster = isMaster; }
	bool IsMaster();

	void SendBroadcastPacket(MessageID header, char *msg);
	void SendBroadcastPacket(MessageID header, int num);
	void SendBroadcastPacket(MessageID header, std::string msg);

	void SetEvent(READYEVENTIDs eventId, bool isReady);

	void HandleEventAllSet(int eventId);

	bool IsInGame() const {return m_IsInGame;}

	const char* GetIPAddress();
	//ask clients to connect
	//wait for connections

	void AddNetworkBridgeListener(INetworkBridge* listener) 
	{
		m_NetBridgeListeners.push_back(listener); 
	}

	void RemoveNetworkBridgeListener(INetworkBridge* listener)
	{
		//TODO fill in the code to remove a listener
	}

	bool IsHost() const {return m_IsHost;}

private:

	static const unsigned int MAX_CONNECTIONS = 8;
	static const unsigned int PORT = 60000;

	bool m_IsMaster;
	bool m_IsHost;
	bool m_IsInGame;

	

	RakPeerInterface *m_RakPeer;
	ReadyEvent m_ReadyEventPlugin;

	// These two plugins are just to automatically create a fully connected mesh so I don't have to call connect more than once
	FullyConnectedMesh2 m_FCM2;
	ConnectionGraph2 m_ConnectedGraph2;

	std::vector<INetworkBridge*> m_NetBridgeListeners;

protected:
	//a container of connected clients

};


#endif