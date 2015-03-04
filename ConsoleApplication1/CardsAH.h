#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include <thread>
#include "NetworkBridge.h"
#include "..\RakNetP2PExample\NetworkHelper.h"

class CardsAH:INetworkBridge
{
public:

	CardsAH();
	~CardsAH();

	void readQuestions();
	void readAnswers();
	void printQuestions();
	void printAnswers();

	void displayInstructions();

	void setUpGame();
	void setUpHost();
	void setUpJoin();

	void NetworkUpdate();


	virtual void OnConnectionAccepted();
	virtual void OnWaitingForHostReady();
	virtual void OnReadyEventAllSet();

	std::string playerAnswers[5];
	std::string playerQuestion;

	void SetUpTurn();
	void SetUpQuestion();

	void chooseQuestionAsker();

	bool isGameStarted;
	bool isQuestionAsker;

	int maxPlayers;
	int points;
	int playerID;
	NetworkHelper *m_NetHelper;
	bool hasQuestionBeenRead;
	bool turnSetup;

	void sendAnswer(std::string answer);
	void sendWinner(int winnerID);
	
private:


	
	bool m_IsGameStarted;
	bool m_IsWaitingForReady;

	std::string answers[1048];
	std::string questions[275];

	
	std::thread* m_NetworkUpdateThread;



};

