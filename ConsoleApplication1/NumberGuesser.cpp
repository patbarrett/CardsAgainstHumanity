#include "NumberGuesser.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

NumberGuesser::NumberGuesser(void) : m_MaxNumber(10), m_IsGameStarted(false)
	, m_SecretNumber(0), m_CurrentTry(0), m_MaxTries(7), m_IsWaitingForReady(false)
{
}

NumberGuesser::~NumberGuesser(void)
{
}

void NumberGuesser::DisplayInstructions()
{
	printf("Welcome to Number Guesser 1.3\n");
	printf("-----------------------------\n");
	printf("First you type in the max number\n");
	printf("Our state of the art random number generator will generate a state of the art random number\n");
	printf("You have 7 tries to guess it. Goodluck.\n");
	printf("Press Enter to Start Single Player\n");
	printf("If you would like to host a game and play with friends please type in 'host'\n");
	printf("If you would like to join a game and play with friends please type in 'join'\n");
}

int NumberGuesser::GenerateRandomNum()
{
	/* initialize random seed: */
  srand (time(NULL));

  /* generate secret number between 1 and MaxNumber: */
  m_SecretNumber = rand() % m_MaxNumber + 1;

  m_NetHelper->SendBroadcastPacket(NetworkHelper::MI_SECRETNUMBER, m_SecretNumber);

  return m_SecretNumber;
}

bool NumberGuesser::CheckForRightAnswer(int guess)
{
	return guess == m_SecretNumber;
}

bool NumberGuesser::CheckForRightAnswer(char* guess)
{
	int iGuess = atoi(guess);
	return CheckForRightAnswer(iGuess);
}

bool NumberGuesser::PlayTurn()
{
	if(m_IsGameStarted)
	{
		printf("Please take a guess ( 1 - %i)\n",m_MaxNumber);
		gets_s(m_Guess);

		//USE THIS FOR SENDING PACKETS HOMIE
		//m_NetHelper->SendBroadcastPacket(NetworkHelper::MI_CHAT, m_Guess);


		if(CheckForRightAnswer(m_Guess))
		{
			printf("Congratulations, you win!! ( %i tries )\n",m_CurrentTry);
			return true;
		}
		else
		{
			m_CurrentTry++;
			if(m_CurrentTry >= m_MaxTries)
			{
				printf("Go home\n");
				return true;
			}
		}
	}//end of isgamestarted
	else
	{
		if(m_IsWaitingForReady)
		{
			if(m_NetHelper->IsHost())
			{
				printf("type in 'ready' to start the game\n");
			}
			else
			{
				printf("waiting for ready..\n");
			}
		}
		char userOption[255];
		gets_s(userOption);

		if(strcmp(userOption,"host") == 0)
		{
			SetupHost();
		}
		else if(strcmp(userOption,"join") == 0)
		{
			SetupJoin();
		}
		else if(strcmp(userOption,"ready") == 0)
		{
			m_NetHelper->SetEvent(NetworkHelper::RE_START,true);
		}
		else if(m_NetHelper == nullptr)
		{
			printf("Please enter your max number please\n");
			char maxnum[255];
			gets_s(maxnum);
			m_MaxNumber = atoi(maxnum);
			GenerateRandomNum();
			m_SecretNumber = m_NetHelper->m_RandomNumber;
			m_IsGameStarted = true;
		}
	}

	return false;
}

void NumberGuesser::SetupHost()
{
	printf("Thanks for hosting, please enter your name:\n");
	char hostName[255];
	gets_s(hostName);
	m_NetHelper = new NetworkHelper();
	m_NetHelper->Init(true,hostName);
	m_NetHelper->AddNetworkBridgeListener(this);
	m_NetworkUpdateThread = new std::thread(&NumberGuesser::NetworkUpdate,this);
	printf("Your IP:%s\n",m_NetHelper->GetIPAddress());
	printf("waiting for players to join...");
}

void NumberGuesser::SetupJoin()
{
	//enter an ip address 
	printf("scanning to find hosts on this network\n");
	m_NetHelper = new NetworkHelper();
	m_NetHelper->Init(false);
	m_NetHelper->AddNetworkBridgeListener(this);
	m_NetworkUpdateThread = new std::thread(&NumberGuesser::NetworkUpdate,this);

	printf("Please enter the IP you would like to join\n");
	char ipAddy[255];
	gets_s(ipAddy);

	
	m_NetHelper->EstablishConnection(ipAddy);
}

void NumberGuesser::NetworkUpdate()
{
	while(1)
	{
		if(m_NetHelper)
		{
			m_NetHelper->Update();
		}
	}
}

void NumberGuesser::OnConnectionAccepted()
{
	printf("Welcome to the networked game of Number Guessing\n");
	printf("enter 'ready' when you want to start the game\n");
	m_IsWaitingForReady = true;
}

void NumberGuesser::OnWaitingForHostReady()
{
	m_IsWaitingForReady = true;
}

void NumberGuesser::OnReadyEventAllSet()
{
	system("cls");
	m_MaxNumber = 10;
	GenerateRandomNum();
	m_IsGameStarted = true;
	printf("game is starting\n");
}

void NumberGuesser::setSecretNumber(int secretNum)
{
	m_SecretNumber = secretNum;
}