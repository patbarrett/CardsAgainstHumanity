#include "CardsAH.h"


CardsAH::CardsAH()
{
}


CardsAH::~CardsAH()
{
}

void CardsAH::readAnswers()
{
	std::ifstream inFile;
	inFile.open("CAH_answers.txt");

	//reading till no more;

	for (int i = 0; i < 1048; i++)
	{
		std::getline(inFile, answers[i]);
		
		//inFile >> answers[i];
	}

	inFile.close();
	

}
void CardsAH::printAnswers()
{
	for (int i = 0; i < 1048; i++){
		std::cout << answers[i] << std::endl;
	}
}

void CardsAH::readQuestions()
{
	std::ifstream inFile;
	inFile.open("CAH_questions.txt");

	for (int i = 0; i < 275; i++)
	{
		std::getline(inFile, questions[i]);

		//inFile >> answers[i];
	}

	inFile.close();


}
void CardsAH::printQuestions()
{

	/*for (int i = 0; i < 275; i++){
		std::cout << questions[i] << std::endl;
	}*/
	
}

void CardsAH::displayInstructions()
{
	printf("Welcome to Cards against Humanity...LAN edition\n");
	printf("-----------------------------\n");
	printf("If you would like to host a game and play with friends please type in 'host'\n");
	printf("If you would like to join a game and play with friends please type in 'join'\n");
}


void CardsAH::SetUpQuestion()
{
	srand(time(NULL));
	char * p2;

	const char* mychar = questions[rand() % 275 + 1].c_str();
	p2 = const_cast<char *>(mychar);


	m_NetHelper->SendBroadcastPacket(m_NetHelper->MIQUESTION, p2);

	m_NetHelper->Update();
	
	
}


void CardsAH::setUpGame()
{
	
	readAnswers();
	readQuestions();

	char userOption[255];
	gets_s(userOption);

	if (strcmp(userOption, "host") == 0)
	{
		//connectedUsers++;
		setUpHost();
		
	}
	else if (strcmp(userOption, "join") == 0)
	{
		//connectedUsers++;
		setUpJoin();
	}
	else if (strcmp(userOption, "ready") == 0 && m_NetHelper->IsHost() == true &&m_NetHelper->connectedUsers >= maxPlayers)
	{
	
		playerID = m_NetHelper->connectedUsers;
		//m_NetHelper->isgameStart = true;
		
		m_NetHelper->SetEvent(NetworkHelper::RE_START, true);
		std::cout << playerID;
		
	}
	else if (strcmp(userOption, "ready") == 0 && m_NetHelper->IsHost() == false)
	{
		playerID = m_NetHelper->connectedUsers;
		m_NetHelper->SetEvent(NetworkHelper::RE_START, true);
		std::cout << playerID;

		//isGameStarted = true;
		//printf("%i\n",connectedUsers);
	}
}

void CardsAH::SetUpTurn()
{
	srand(time(NULL));

	if (isGameStarted && hasQuestionBeenRead == false )
	{
			
		if (playerID == m_NetHelper->askerID){
			system("cls");
			std::cout << "YOU ARE THE JUDGE HERE IS THE QUESTION: " << std::endl;
			std::cout << m_NetHelper->question << std::endl;
			std::cout << m_NetHelper->questionanswer << std::endl;
			
			if (m_NetHelper->questionanswer != "")
			{
				hasQuestionBeenRead = true;
			}
				
		}
		else 
		{

			//system("cls");
			std::cout << " YOU MUST SUBMIT YOUR ANSWER TO THE JUDGE FOR THIS QUESTION: " << std::endl;
			std::cout << m_NetHelper->question << std::endl;
			for (int i = 0; i < 6; i++)
			{
				playerAnswers[i] = answers[rand() % 1048 + 1];
				std::cout << playerAnswers[i] << std::endl;
			}
			
			std::cout << " PLEASE CHOOSE AN ANSWER 1 - 5"<< std::endl;
			std::string userOption;
			std::cin >> userOption;
			if (userOption == "1")
			{
				sendAnswer(playerAnswers[0]);
				
			}else if (userOption == "2")
			{
				sendAnswer(playerAnswers[1]);

			}
			else if (userOption == "3")
			{
				sendAnswer(playerAnswers[2]);

			}
			else if (userOption == "4")
			{
				sendAnswer(playerAnswers[3]);

			}
			else if (userOption == "5")
			{
				sendAnswer(playerAnswers[4]);

			}
			
			hasQuestionBeenRead = true;
	
		}
		
	}else if (isGameStarted == true && hasQuestionBeenRead == true){
		
		if (playerID == m_NetHelper->askerID && hasQuestionBeenRead == true)
		{

		std::cout << "CHOOSE A WINNER 1-4" << std::endl;
		std::string userOption;
		std::cin >> userOption;
		if (userOption == "1")
		{
			sendWinner(0);
		}
		hasQuestionBeenRead = false;
		SetUpQuestion();
		//m_NetHelper->Update();

		}else if (playerID == m_NetHelper->winnerID && hasQuestionBeenRead == true)
		{
		points++;
		std::cout << "YOU A WINNER" << std::endl;
		std::cout << " you have : "<<points << std::endl;
		//sendWinner(10);
		hasQuestionBeenRead = false;
		}
		/*else if (hasQuestionBeenRead == true)
		{
			hasQuestionBeenRead = false;
		}*/
	}
	else{

		setUpGame();
	 }


	
	
}


void CardsAH::sendWinner(int winnerId)
{
	int temp = winnerId;

	//m_NetHelper->SendBroadcastPacket(m_NetHelper->QUESTIONANSWER, p2);
	m_NetHelper->SendBroadcastPacket(m_NetHelper->WINNERID, temp);
	m_NetHelper->Update();

}

void CardsAH::sendAnswer(std::string answer)
{

	char * p2;
	const char* mychar = answer.c_str();
	p2 = const_cast<char *>(mychar);

	m_NetHelper->SendBroadcastPacket(m_NetHelper->QUESTIONANSWER, p2);
	m_NetHelper->Update();

}

void CardsAH::chooseQuestionAsker()
{
	int rnd = rand() % 1 + 0;

	std::cout << rnd << std::endl;

	m_NetHelper->SendBroadcastPacket(m_NetHelper->ASKERID, rnd);
	
}


void CardsAH::setUpHost()
{
	printf("Thanks for hosting, please enter your name:\n");
	char hostName[255];
	gets_s(hostName);
	m_NetHelper = new NetworkHelper();
	m_NetHelper->Init(true, hostName);
	m_NetHelper->AddNetworkBridgeListener(this);
	m_NetworkUpdateThread = new std::thread(&CardsAH::NetworkUpdate, this);
	printf("Your IP:%s\n", m_NetHelper->GetIPAddress());
	printf("waiting for players to join...");
}
void CardsAH::setUpJoin()
{
	//enter an ip address 
	printf("scanning to find hosts on this network\n");
	m_NetHelper = new NetworkHelper();
	m_NetHelper->Init(false);
	m_NetHelper->AddNetworkBridgeListener(this);
	m_NetworkUpdateThread = new std::thread(&CardsAH::NetworkUpdate, this);

	printf("Please enter the IP you would like to join\n");
	char ipAddy[255];
	gets_s(ipAddy);


	m_NetHelper->EstablishConnection(ipAddy);
	m_NetHelper->SendBroadcastPacket(m_NetHelper->ADDUSER, true);

}
void CardsAH::NetworkUpdate()
{
	while (1)
	{
		if (m_NetHelper)
		{
			m_NetHelper->Update();
		}		

	}
}

void CardsAH::OnConnectionAccepted()
{
	printf("Welcome to the lobby for cards agains humanity\n");
	printf("type ready when you are ready to play\n");
	m_IsWaitingForReady = true;
}

void CardsAH::OnWaitingForHostReady()
{
	m_IsWaitingForReady = true;
}

void CardsAH::OnReadyEventAllSet()
{
	isGameStarted = true;
	SetUpQuestion();	

}