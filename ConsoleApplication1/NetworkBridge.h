#pragma once
class INetworkBridge
{
public:
	INetworkBridge(void);
	~INetworkBridge(void);

	virtual void OnConnectionAccepted() = 0;
	virtual void OnWaitingForHostReady() = 0;
	virtual void OnReadyEventAllSet() = 0;
};

