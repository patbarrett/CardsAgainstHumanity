#pragma once

#include "..\RakNetP2PExample\NetworkHelper.h"
#include <thread>
#include "NetworkBridge.h"

class NumberGuesser : INetworkBridge 
{
public:
	NumberGuesser(void);
	~NumberGuesser(void);

	bool PlayTurn();

	void DisplayInstructions();
	void SetMaxNumber(int maxNumber);


	int GenerateRandomNum();
	
	void setSecretNumber(int secretNum);
	

	bool CheckForRightAnswer(int guess);
	bool CheckForRightAnswer(char* guess);

	int GetTotalPlayers();

	void SetupHost();
	void SetupJoin();

	void NetworkUpdate();

	virtual void OnConnectionAccepted();
	virtual void OnWaitingForHostReady();
	virtual void OnReadyEventAllSet();

private:
	int m_MaxNumber;
	char m_Guess[255];
	bool m_IsGameStarted;
	int m_SecretNumber;
	int m_CurrentTry;
	int m_MaxTries;

	bool m_IsWaitingForReady;

	NetworkHelper *m_NetHelper;
	std::thread* m_NetworkUpdateThread;

};

