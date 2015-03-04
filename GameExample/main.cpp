#include <iostream>
#include "../RakNetP2PExample/NetworkHelper.h"
#include <string>
#include <thread>
#include "..\ConsoleApplication1\NumberGuesser.h"
#include "..\ConsoleApplication1\CardsAH.h"

NumberGuesser *numGuesser = 0;
CardsAH *cardsAH = 0;
//NetworkHelper *netHelp = 0;

void ProcessInput();
 
int main()
{
	

	cardsAH = new CardsAH();
	cardsAH->displayInstructions();
	//netHelp = new NetworkHelper();
	
	cardsAH->isGameStarted = false;
	cardsAH->hasQuestionBeenRead = false;
	cardsAH->turnSetup = true;

	//numGuesser = new NumberGuesser();
	//numGuesser->DisplayInstructions();
	int i = 0;


	bool isGameOver = false;
	while(!isGameOver)
	{
		if(cardsAH)
		{
			 cardsAH->maxPlayers = 1;
			 cardsAH->SetUpTurn();
		
			}
				
		}
		
	

	//char command[255];
	//gets_s(command);
	

	return 0;
}

/*
void ProcessInput()
{
	do
	{
		//strcmp(command,"r") == 0
		gets_s(command);
		if(strlen(command) == 1 && command[0] == 'r')
		{
			if(netHelp)
			{
				netHelp->SetEvent(NetworkHelper::RE_START,true);
			}
		}
		else if(strcmp(command,"f") == 0)
		{
			if(netHelp && netHelp->IsInGame())
			{
				netHelp->SetEvent(NetworkHelper::RE_FINISH,true);
			}
		}
		else
		{
			if(netHelp)
			{
				netHelp->SendBroadcastPacket(NetworkHelper::MI_CHAT,command);
			}
		}

	}
	while(command[0] != 'q');

	printf("thanks for playing\n");
}
*/