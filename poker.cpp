#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <map>
#include <Windows.h>
#include <MMSystem.h>
//#include "SoundSystem.h"
//#include "Desktop\moje\pliki_pokera\RiskAnalyst.cpp"

using namespace std;
struct Card
{
public:
	string name;
	string symbol;
};

class Deck
{
public:
	vector<string>names;
	vector<string>symbols;
	map<int, string>hands;
	Deck()
	{
		names = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };
		symbols = { string(1, char(259)), string(1, char(260)), string(1, char(261)), string(1, char(262)) };
		/*hands = { { 0, "High Card" }, { 1, "Pair" }, { 2, "Two pairs" }, { 3, "Three Of A Kind" }, { 4, "Straight" },
		{ 5, "Flush" }, { 6, "Full House" }, { 7, "Four Of A Kind" }, { 8, "Straight Flush" }, { 9, "Royal Flush" } };*/
	}
};

struct Player
{
	Card card1;
	Card card2;
	unsigned int chips;
	unsigned int chipsIn;
};

vector<string>myHand, compHand;
vector<string>mySymbols, myNames, compSymbols, compNames;
Card flop1, flop2, flop3, turnCard, riverCard;
vector<int>myNamesIdxs, compNamesIdxs;
Player player1, player2;
bool folded = false, folded2 = false, folded3 = false;
unsigned int pot = 0;
const int bigBlind = 400, smallBlind = 200;
int raiseValue = 0;
bool raised = false, raised2Round = false, raised3Round = false,
compDecidedRound2 = false, compDecidedRound3 = false,
flopped = false, called = false, turned = false, rivered = false;
char decision;
vector<string>usedCards;
int allInedR2 = 0, allInedR3 = 0;
bool a = false, b = false, c = false;

bool raisedB4Flopped, foldedB4Flopped;
bool z = false;
int allInedR1 = 0;
bool compDecidedRound1 = false;

bool raisedEnd, foldedEnd;
bool x = false;
int allInedEnd = 0;
bool compDecidedEnd;

bool tabled = false;

class Game : public Deck
{
public:
	Game();

	class RiskAnalyst:public Deck
	{
	public:
		bool hasHighCard();
		bool checkIfWorthPlayingAtAll();
		bool checkIfWorthPlayingFlop();
		bool checkIfWorthPlayingTurn();
		bool checkIfWorthPlayingRiver();
		bool checkIfWorthPlayingFinish();
	};

	RiskAnalyst riskAnalyst;
//	SoundSystem soundSystem;

	void dealFirstCard(vector<string>&, vector<string>&, vector<string>&);
	void dealSecondCard(vector<string>&, vector<string>&, vector<string>&);
	void dealFirstCardComp(vector<string>&, vector<string>&, vector<string>&);
	void dealSecondCardComp(vector<string>&, vector<string>&, vector<string>&);
	void deal();
	void showHands();
	void getBlinds();
	void dealFirstFlop(vector<string>&, vector<string>&, vector<string>&);
	void dealSecondFlop(vector<string>&, vector<string>&, vector<string>&);
	void dealThirdFlop(vector<string>&, vector<string>&, vector<string>&);
	void turn(vector<string>&, vector<string>&, vector<string>&);                       //odkrycie czwartej karty
	void river(vector<string>&, vector<string>&, vector<string>&);                          //odkrycie piatej karty
	void flop();
	void turn();
	void river();
	void showFlop();
	void fold();
	void raise();
	void call();
	void showTurn();
	void showRiver();
	void decide();
	void compCall();
	void compFold();
	void compCheck();
	void compBet(int);
	int compDecideRound1(int&);
	int compDecideRound2(int&);
	int compDecideRound3(int&);
	int compDecideEnd(int&);
	void writePlayersInfo();
	void showPot();
	bool b4flop(bool&);
	bool flopRound(bool&);
	bool turnRound(bool&);         //mozna zrobic zeby ta funkcja byla boolem, wtedy przyjmuje boola i nie trzeba robic zmiennych round2started
	bool riverRound(bool&);
	bool afterRiver(bool&);
	bool lastRound(bool&);
	bool checkIfEnd();
	void round();
	void resetGame();
};

class SetupFinder : public Deck
{
public:
	SetupFinder() {}
	Game *g;
	void getCardsInfo();
	void makeVecs(vector<string>&, vector<string>&);
	int FindNameSetups(vector<string>&);
	int FindSymbolSetups(vector<string>&);
	bool checkWinner();
	void finish(Game*);
};

class Play : public Game		//chyba nie musi byc dziedziczenia z Game
{
public:
	SetupFinder *s;
	void play(Game*, SetupFinder*);
};

int main()
{
	srand(time(0));
	Play p;
	Game g;
	SetupFinder sf;
//	PlaySound(TEXT("bass_czas_ataku.wav"), NULL, SND_SYNC);     //PLAY SOUND
	p.play(&g, &sf);
	return 0;
}


void Play::play(Game *game, SetupFinder *setFin)
{
	do
	{
		game->deal();
		game->showHands();
		game->flop();
		game->turn();
		game->river();
		game->round();
		setFin->getCardsInfo();
		setFin->finish(*&game);
		game->resetGame();
	} while (!game->checkIfEnd());
}

void Game::dealFirstCard(vector<string>&n, vector<string>&s, vector<string>&u)
{
	player1.card1.name = n[rand() % names.size()];
	player1.card1.symbol = s[rand() % symbols.size()];
	string stringCard = player1.card1.name + player1.card1.symbol;
	u.push_back(stringCard);
}

void Game::dealSecondCard(vector<string>&n, vector<string>&s, vector<string>&u)
{
	player1.card2.name = n[rand() % names.size()];
	player1.card2.symbol = s[rand() % symbols.size()];
	string stringCard = player1.card2.name + player1.card2.symbol;
	for (int i = 0; i<usedCards.size(); i++)
	{
		if (stringCard == u[i])
		{
			dealSecondCard(names, symbols, usedCards);
		}
	}
	u.push_back(stringCard);
}

void Game::dealFirstCardComp(vector<string>&n, vector<string>&s, vector<string>&u)
{
	player2.card1.name = n[rand() % names.size()];
	player2.card1.symbol = s[rand() % symbols.size()];
	string stringCard = player2.card1.name + player2.card1.symbol;
	for (int i = 0; i<usedCards.size(); i++)
	{
		if (stringCard == u[i])
		{
			dealFirstCardComp(names, symbols, usedCards);
		}
	}
	u.push_back(stringCard);
}
void Game::dealSecondCardComp(vector<string>&n, vector<string>&s, vector<string>&u)
{
	player2.card2.name = n[rand() % names.size()];
	player2.card2.symbol = s[rand() % symbols.size()];
	string stringCard = player2.card2.name + player2.card2.symbol;
	for (int i = 0; i<usedCards.size(); i++)
	{
		if (stringCard == u[i])
		{
			dealSecondCardComp(names, symbols, usedCards);
		}
	}
	u.push_back(stringCard);
}

void Game::deal()
{
	dealFirstCard(names, symbols, usedCards);
	dealFirstCardComp(names, symbols, usedCards);
	dealSecondCard(names, symbols, usedCards);
	dealSecondCardComp(names, symbols, usedCards);
}
void Game::showHands()
{
	if (!tabled)
	{
		cout << "YOUR HAND: " << endl << endl;
//		Sleep(500);
		cout << player1.card1.name << player1.card1.symbol;
//		soundSystem.playCardSound();
//		Sleep(500);
		cout << "           " << "XX" << endl;

//		Sleep(500);
		cout << player1.card2.name << player1.card2.symbol;
//		Sleep(500);
		cout << "           " << "XX" << endl;

		cout << endl;
	}
	else
	{
		cout << player1.card1.name << player1.card1.symbol << endl;
		cout << player1.card2.name << player1.card2.symbol << endl << endl;

		cout << player2.card1.name << player2.card1.symbol << endl;
		cout << player2.card2.name << player2.card2.symbol << endl << endl;
	}

}

void Game::getBlinds()
{
	player1.chips -= smallBlind;
	player2.chips -= bigBlind;
	pot += smallBlind;
	pot += bigBlind;
	player2.chipsIn = bigBlind;
	player1.chipsIn = smallBlind;
	cout << "COMP: BIG BLIND" << endl;
	cout << "YOU: SMALL BLIND" << endl << endl;
	cout << "BLINDS TAKEN!" << endl << endl;
}

void Game::dealFirstFlop(vector<string>&n, vector<string>&s, vector<string>&u)
{
	flop1.name = n[rand() % names.size()];
	flop1.symbol = s[rand() % symbols.size()];
	string stringCard = flop1.name + flop1.symbol;
	for (int i = 0; i<usedCards.size(); i++)
	{
		if (stringCard == u[i])
		{
			dealFirstFlop(names, symbols, usedCards);
		}
	}
	u.push_back(stringCard);
}
void Game::dealSecondFlop(vector<string>&n, vector<string>&s, vector<string>&u)
{
	flop2.name = n[rand() % names.size()];
	flop2.symbol = s[rand() % symbols.size()];
	string stringCard = flop2.name + flop2.symbol;
	for (int i = 0; i<usedCards.size(); i++)
	{
		if (stringCard == u[i])
		{
			dealSecondFlop(names, symbols, usedCards);
		}
	}
	u.push_back(stringCard);
}
void Game::dealThirdFlop(vector<string>&n, vector<string>&s, vector<string>&u)
{
	flop3.name = n[rand() % names.size()];
	flop3.symbol = s[rand() % symbols.size()];
	string stringCard = flop3.name + flop3.symbol;
	for (int i = 0; i<usedCards.size(); i++)
	{
		if (stringCard == u[i])
		{
			dealThirdFlop(names, symbols, usedCards);
		}
	}
	u.push_back(stringCard);
}
void Game::turn(vector<string>&n, vector<string>&s, vector<string>&u)                             //odkrycie czwartej karty
{
	turnCard.name = n[rand() % names.size()];
	turnCard.symbol = s[rand() % symbols.size()];
	string stringCard = turnCard.name + turnCard.symbol;
	for (int i = 0; i<usedCards.size(); i++)
	{
		if (stringCard == u[i])
		{
			turn(names, symbols, usedCards);
		}
	}
	u.push_back(stringCard);
}
void Game::river(vector<string>&n, vector<string>&s, vector<string>&u)                             //odkrycie czwartej karty
{
	riverCard.name = n[rand() % names.size()];
	riverCard.symbol = s[rand() % symbols.size()];
	string stringCard = riverCard.name + riverCard.symbol;
	for (int i = 0; i<usedCards.size(); i++)
	{
		if (stringCard == u[i])
		{
			river(names, symbols, usedCards);
		}
	}
	u.push_back(stringCard);
}

void Game::flop()
{
	dealFirstFlop(names, symbols, usedCards);
	dealSecondFlop(names, symbols, usedCards);
	dealThirdFlop(names, symbols, usedCards);
}
void Game::turn()
{
	turn(names, symbols, usedCards);
}
void Game::river()
{
	river(names, symbols, usedCards);
	////TESTOWANIE KART
	////vector<string>symbols = { string(1,char(259)),string(1,char(260)),string(1,char(261)),string(1,char(262)) };
	//player1.card1.name = "A";
	//player1.card1.symbol = string(1, char(259));
	//player1.card2.name = "Q";
	//player1.card2.symbol =
	//player2.card1.name =
	//player2.card1.symbol =
	//player2.card2.name =
	//player2.card2.symbol =
	//flop1.name= "A";
	//flop1.symbol = string(1, char(260));
	//flop2.name = "A";
	//flop2.symbol = string(1, char(261));
	//flop3.name = "A";
	//flop3.symbol = string(1, char(261));
	//turnCard.name = "K";
	//turnCard.symbol =
	//riverCard.name=
	//riverCard.symbol = "K";
}
void Game::showFlop()
{
	cout << "ON TABLE: " << endl;
	cout << flop1.name << flop1.symbol << endl;
	cout << flop2.name << flop2.symbol << endl;
	cout << flop3.name << flop3.symbol << endl;
	flopped = true;
}
void Game::fold()
{
	if (z)
	{
		foldedB4Flopped = true;
	}
	else if (a)
	{
		folded = true;
	}
	else if (b)
	{
		folded2 = true;
	}
	else if (c)
	{
		folded3 = true;
	}
	else if (x)
	{
		foldedEnd = true;
	}
}

void Game::raise()
{
	cout << "RAISE FOR: ";
	cin >> raiseValue;
	if (raiseValue <= player1.chips)
	{
		if (raised == false)
		{
			unsigned int diffrence = player2.chipsIn - player1.chipsIn;
			if (raiseValue>diffrence)
			{
				player1.chipsIn += raiseValue;
				player1.chips -= raiseValue;
				pot += raiseValue;
				if (z)
				{
					raisedB4Flopped = true;
				}
				else if (a)
				{
					raised = true;
				}
				else if (b)
				{
					raised2Round = true;
				}
				else if (c)
				{
					raised3Round = true;
				}
				else if (x)
				{
					raisedEnd = true;
				}
			}
			else
			{
				cout << "WRONG VALUE! " << endl;
				raise();
			}
			diffrence = 0;
		}
		else
		{
			cout << "YOU'VE ALREADY RAISED!" << endl;
			return;
		}
	}
	else
	{
		cout << "U DONT HAVE AS MUCH!" << endl;
		raise();
	}
}

void Game::call()
{
	if (player2.chipsIn>player1.chipsIn)
	{
		unsigned int diffrence = player2.chipsIn - player1.chipsIn;
		player1.chips -= diffrence;
		player1.chipsIn += diffrence;
		pot += diffrence;
		called = true;
		cout << "U'VE CALLED! " << endl;
		system("pause");
	}
	else
	{
		cout << "NOTHING TO CALL!" << endl;
	}
}
void Game::showTurn()
{
	turned = true;
	cout << turnCard.name << turnCard.symbol << endl;
}
void Game::showRiver()
{
	rivered = true;
	cout << riverCard.name << riverCard.symbol << endl;
}
void Game::decide()
{
	if (player2.chipsIn>player1.chipsIn)
	{
		cout << "U GOT: " << player2.chipsIn - player1.chipsIn << " TO CALL " << endl << endl;
	}
	cout << "Pot: " << pot << endl;
	cout << endl;
	cout << "f-fold, r-raise, c-call\\check" << endl;
	cin >> decision;
	if ((decision == 'f') || (decision == 'r') || (decision == 'c'))
	{
		if (decision == 'f')
		{
			fold();
		}
		else if (decision == 'r')
		{
			raise();
		}
		else if (decision == 'c')
		{
			call();
		}
	}
	else
	{
		cout << "WRONG LETTER! " << endl;
		cout << endl;
		decide();
	}
}

void Game::compCall()
{
	unsigned int diffrence = player1.chipsIn - player2.chipsIn;
	player2.chips -= diffrence;
	player2.chipsIn += diffrence;
	pot += diffrence;
	cout << "COMP CALLS!" << endl;                 //tutaj trzeba zrobic jakies 3s wait zeby gracz przeczytal ze komp wszedl
	cout << endl;
	system("pause");
	//	diffrence = 0;
}

void Game::compFold()
{
	cout << "COMP FOLDS! U'VE EARNED " << pot - player1.chipsIn << endl;
}

void Game::compCheck()
{
	cout << "COMP: CHECK" << endl;
	//	return true;
}

void Game::compBet(int a)
{
	player2.chipsIn += a;
	player2.chips -= a;
	pot += a;
	cout << endl;
	cout << "COMP BETS: " << a << endl;
	cout << endl;
}

int Game::compDecideRound1(int&z)
{
	compDecidedRound1 = true;
	if ((player2.card1.name == player2.card2.name)
		|| (player2.card1.symbol == player2.card2.symbol))          //jezeli po flopie chce podbic
	{
		int money;
		if (player2.chips >= 500)
		{
			if (player2.chips <= 2000)
			{
				money = player2.chips;
			}
			else
			{
				money = 500;
			}
		}
		else money = player2.chips;
		compBet(money);
	}
	else if (player1.chips == 0)			//jezeli gracz zagral all_in
	{
		system("CLS");
		showHands();
		showFlop();
		showTurn();
		showRiver();
		return z = 1;
	}
	else                   //jezeli nie chce podbijac po preflopie
	{
		compCheck();
	}
}

int Game::compDecideRound2(int&a)
{
	compDecidedRound2 = true;
	if (false)                   //jezeli flop kompowi nie siadl
	{
		compCheck();
	}
	else if (player1.chips == 0)
	{
		system("CLS");
		showHands();
		showFlop();
		showTurn();
		showRiver();
		return a = 1;
	}
	else if (true)          //jezeli po flopie chce podbic
	{
		int money;
		if (player2.chips >= 500)
		{
			money = 500;
		}
		else money = player2.chips;
		compBet(money);
	}
}
int Game::compDecideRound3(int&b)
{
	compDecidedRound3 = true;
	if (false)                   //jezeli flop kompowi nie siadl
	{
		compCheck();
	}
	else if (player1.chips == 0)
	{
		system("CLS");
		showHands();
		showFlop();
		showTurn();
		showRiver();
		return b = 1;
	}
	else if (true)          //jezeli po flopie chce podbic
	{
		int money;
		if (player2.chips >= 500)
		{
			money = 500;
		}
		else money = player2.chips;
		compBet(money);
	}
}

int Game::compDecideEnd(int&c)
{
	compDecidedEnd = true;
	if (false)                   //jezeli flop kompowi nie siadl
	{
		compCheck();
	}
	else if (player1.chips == 0)
	{
		system("CLS");
		showHands();
		showFlop();
		showTurn();
		showRiver();
		return c = 1;
	}
	else if (true)          //jezeli po flopie chce podbic
	{
		int money;
		if (player2.chips >= 500)
		{
			money = 500;
		}
		else money = player2.chips;
		compBet(money);
	}
}

void Game::writePlayersInfo()
{
	cout << "Ur chips: " << player1.chips << endl;
	cout << "Comp chips: " << player2.chips << endl;
	cout << endl;
}

void Game::showPot()
{
	cout << "Pot: " << pot << endl;
	cout << endl;
}

bool Game::b4flop(bool&z1)      //trzeba dodac opcje ze przed flopem, jak przyjmie mojego raisa, to już nie ma ruchu wtedy komp.
{
	z1 = true;
	writePlayersInfo();
	if (!raisedB4Flopped)
	{
		decide();
	}
	//writePlayersInfo();
	if (foldedB4Flopped)
	{
		player2.chips += pot;
		cout << "U'VE FOLDED!" << endl;
		cout << endl;
		pot = 0;
		return z1 = false;
	}
	if (player1.chipsIn == player2.chipsIn)
	{
		return z1 = false;
	}
	else if (player1.chipsIn>player2.chipsIn)
	{
		compCall();                 //tutaj trzeba dac warunek kiedy call
		showPot();
		b4flop(z1);
	}
	writePlayersInfo();
}

bool Game::flopRound(bool&a1)
{
	a1 = true;
	if (!foldedB4Flopped)
	{
		system("CLS");
		showHands();
		writePlayersInfo();
		if (!raised)
		{
			if (!compDecidedRound1)
			{
				compDecideRound1(allInedR1);             //tutaj komp podbija zawsze na razie o 500
				if (allInedR1 == 1)
				{
					return a1 = false;
				}
			}
			decide();
		}
		if (folded)
		{
			player2.chips += pot;
			cout << "U'VE FOLDED!" << endl;
			cout << endl;
			pot = 0;
			return a1 = false;
		}
		if (player1.chipsIn == player2.chipsIn)
		{
			if (!flopped)
			{
				showFlop();
			}
			return a1 = false;
		}
		else if (player1.chipsIn > player2.chipsIn)
		{
			compCall();                 //tutaj trzeba dac warunek kiedy call
			system("pause");
			showPot();
			flopRound(a1);
		}
		writePlayersInfo();
	}
}

bool Game::turnRound(bool&b1)           //mozna zrobic zeby ta funkcja byla boolem, wtedy przyjmuje boola i nie trzeba robic zmiennych round2started
{
	b1 = true;
	if ((!folded) && (!foldedB4Flopped))          //jak jest folded to funkcja nie ma sensu
	{
		system("CLS");
		showHands();
		writePlayersInfo();
		showFlop();
		if (!raised2Round)          //po to zeby 2x nie moc podbic. okazuje sie ze nie bedzie potrzebne, bo po kazdej rundzie przechodzi automatycznie do kolejnej
		{
			if (!compDecidedRound2)
			{
				compDecideRound2(allInedR2);             //tutaj komp podbija zawsze na razie o 500
				if (allInedR2 == 1)
				{
					return b1 = false;
				}
			}
			decide();
		}
		if (folded2)
		{
			player2.chips += pot;
			cout << "U'VE FOLDED!" << endl;
			cout << endl;
			pot = 0;
			return b1 = false;
		}
		if (player1.chipsIn == player2.chipsIn)
		{
			if (!turned)
			{
				showTurn();
			}
			return b1 = false;
		}
		else if (player1.chipsIn>player2.chipsIn)
		{
			compCall();                  //tutaj trzeba dac warunek kiedy call
			system("pause");
			showPot();
			showTurn();
			turnRound(b1);
		}
		//            writePlayersInfo();
	}
	return b1 = false;
}

bool Game::riverRound(bool&c1)
{
	c1 = true;
	if ((!folded) && (!folded2) && (!foldedB4Flopped))
	{
		system("CLS");
		showHands();
		writePlayersInfo();
		showFlop();
		showTurn();
		if (!raised3Round)          //po to zeby 2x nie moc podbic
		{
			if (!compDecidedRound3)
			{
				compDecideRound3(allInedR3);             //tutaj komp podbija zawsze na razie o 500
				if (allInedR3 == 1)
				{
					return c1 = false;
				}
			}
			decide();
		}
		if (folded3)
		{
			player2.chips += pot;
			cout << "U'VE FOLDED!" << endl;
			cout << endl;
			pot = 0;
			return c1 = false;
			system("pause");
		}
		if (player1.chipsIn == player2.chipsIn)
		{
			if (!rivered)
			{
				showRiver();
			}
			return c1 = false;
			system("pause");
		}
		else if (player1.chipsIn>player2.chipsIn)
		{
			compCall();                 //tutaj trzeba dac warunek kiedy call
			system("pause");
			showPot();
			showRiver();
			return c1 = false;
			riverRound(c1);
		}
		//            writePlayersInfo();
	}
	return c1 = false;
}

bool Game::lastRound(bool&d1)
{
	d1 = true;
	if ((!folded) && (!folded2) && (!foldedB4Flopped) && (!folded3))
	{
		system("CLS");
		showHands();
		writePlayersInfo();
		showFlop();
		showTurn();
		showRiver();
		if (!raisedEnd)          //po to zeby 2x nie moc podbic
		{
			if (!compDecidedEnd)
			{
				compDecideEnd(allInedEnd);             //tutaj komp podbija zawsze na razie o 500
				if (allInedEnd == 1)
				{
					return d1 = false;
				}
			}
			decide();
		}
		if (foldedEnd)
		{
			player2.chips += pot;
			cout << "U'VE FOLDED!" << endl;
			cout << endl;
			pot = 0;
			return d1 = false;
			system("pause");
		}
		if (player1.chipsIn == player2.chipsIn)
		{
			//			if (!rivered)
			//			{
			//				showRiver();
			//			}
			return d1 = false;
			system("pause");
		}
		else if (player1.chipsIn>player2.chipsIn)
		{
			compCall();                 //tutaj trzeba dac warunek kiedy call
			system("pause");
			showPot();
			showRiver();
			return d1 = false;
			lastRound(d1);
		}
		//            writePlayersInfo();
	}
	return d1 = false;
}

bool Game::checkIfEnd()
{
	if ((player1.chips == 0) || (player2.chips == 0))
	{
		return true;
	}
	else return false;
}

void SetupFinder::getCardsInfo()
{
	myNames.push_back(player1.card1.name);
	myNames.push_back(player1.card2.name);
	myNames.push_back(flop1.name);
	myNames.push_back(flop2.name);
	myNames.push_back(flop3.name);
	myNames.push_back(turnCard.name);
	myNames.push_back(riverCard.name);

	compNames.push_back(player2.card1.name);
	compNames.push_back(player2.card2.name);
	compNames.push_back(flop1.name);
	compNames.push_back(flop2.name);
	compNames.push_back(flop3.name);
	compNames.push_back(turnCard.name);
	compNames.push_back(riverCard.name);

	mySymbols.push_back(player1.card1.symbol);
	mySymbols.push_back(player1.card2.symbol);
	mySymbols.push_back(flop1.symbol);
	mySymbols.push_back(flop2.symbol);
	mySymbols.push_back(flop3.symbol);
	mySymbols.push_back(turnCard.symbol);
	mySymbols.push_back(riverCard.symbol);

	compSymbols.push_back(player2.card1.symbol);
	compSymbols.push_back(player2.card2.symbol);
	compSymbols.push_back(flop1.symbol);
	compSymbols.push_back(flop2.symbol);
	compSymbols.push_back(flop3.symbol);
	compSymbols.push_back(turnCard.symbol);
	compSymbols.push_back(riverCard.symbol);

	//rece. wektory skladaja sie tylko z names, bo to tylko bylo potrzebne na razie (do porownywania par i.im)
	myHand.push_back(player1.card1.name);
	myHand.push_back(player1.card2.name);
	compHand.push_back(player2.card1.name);
	compHand.push_back(player2.card2.name);
}

vector<string>myPair;
vector<string>compPair;
void SetupFinder::makeVecs(vector<string>&names, vector<string>&whom)
{
	for (int i = 0; i < names.size(); i++)
	{
		for (int j = 0; j < names.size(); j++)
		{
			if (i != j)
			{
				if (names[i] == names[j])
					whom.push_back(names[i]);       //to jest ok. bo niżej, wywołuję ta funkcje po sprawdzeniu ze score==1;
			}
		}
	}
}

int SetupFinder::FindNameSetups(vector<string>&names)
{
	//getCardsInfo();
	int score = 0;
	int sameNames = 0;
	for (int i = 0; i < names.size(); i++)                       //trzeba zmienic bo jak sa 3 takie same to wyswietla 6
	{
		for (int j = 0; j < names.size(); j++)
		{
			if (i != j)
			{
				if (names[i] == names[j])
					sameNames++;
			}
		}
	}
	/*map<int, string>hands = { { 0,"High Card" },{ 1,"Pair" },{ 2,"Two pairs" },{ 3,"Three Of A Kind" },{ 4,"Straight" },
	{ 5,"Flush" },{ 6,"Full House" },{ 7,"Four Of A Kind" },{ 8,"Straight Flush" },{ 9,"Royal Flush" } };*/
	if (sameNames == 0)score = 0;                                //NIE MOZE BYC SWITCH CASEM BO TUTAJ JEST MOZLIWOSC NADPISYWANIA                   //high card
	else if (sameNames == 2)score = 1;                    //para
	else if (sameNames == 4)score = 2;                    //2 pary
	else if (sameNames == 6)score = 3;                    //trojka
	else if ((sameNames == 8) || (sameNames == 10))	score = 6;  					//jak sa dwie trójki to tez sameNames=12, trzeba to rozwiazac. i jak sa 3 pary to tez                  //full
	else if ((sameNames == 12) || (sameNames == 14) || (sameNames == 18)) score = 7;					//4 of a kind
	//else if ()
	//{
	//			//straight
	//}
	return score;
}


int SetupFinder::FindSymbolSetups(vector<string>&symbols)
{
	int score1 = 0;
	int sameSymbols = 0;
	for (int i = 0; i<symbols.size(); i++)
	{
		for (int j = 0; j<symbols.size(); j++)
		{
			if (i != j)
			{
				if (symbols[i] == symbols[j])
				{
					sameSymbols++;
				}
			}
		}
	}
	if ((sameSymbols == 20) || (sameSymbols == 22))
	{
		score1 = 5;					//flush
	}
	return score1;
}

bool SetupFinder::checkWinner()
{
	int a, b, a_, b_;
	a = FindNameSetups(myNames);
	b = FindNameSetups(compNames);
	a_ = FindSymbolSetups(mySymbols);						//za³atwia sprawe nadpisywania punktow
	b_ = FindSymbolSetups(compSymbols);
	int thescoreme = a > a_ ? a : a_;
	int thescorecomp = b > b_ ? b : b_;
	cout << thescoreme << thescorecomp << endl;
	if (thescoreme>thescorecomp)
	{
		return true;
	}
	else if (thescoreme<thescorecomp)
	{
		return false;
	}
	else if (thescoreme == thescorecomp)				//jezeli mamy to samo
	{
		if (thescoreme == 0)
		{
			int myMaxNameCard = 0, compMaxNameCard = 0;
			for (int i = 0; i < myHand.size(); i++)
			{
				for (int j = 0; j < names.size(); j++)
				{
					if (myHand[i] == names[j])
					{
						if (j >= myMaxNameCard)
						{
							myMaxNameCard = j;
						}
					}
				}
			}
			for (int i = 0; i < compHand.size(); i++)
			{
				for (int j = 0; j < names.size(); j++)
				{
					if (compHand[i] == names[j])
					{
						if (j >= compMaxNameCard)
						{
							compMaxNameCard = j;
						}
					}
				}
			}
			if (myMaxNameCard > compMaxNameCard) return true;
			else return false;
		}
		else if ((thescoreme == 1) || (thescoreme == 2) || (thescoreme == 3) || (thescoreme == 5)||(thescoreme==7))//jezeli kazdy ma pare /lub trojke (chyba tez bedzie dzialac?) czyli karete tez
		{
			makeVecs(myNames, myPair);
			makeVecs(compNames, compPair);
			int myPairScore = -1;
			int compPairScore = -1;
			for (int i = 0; i < myPair.size(); i++)
			{
				for (int j = 0; j < names.size(); j++)
				{
					if (myPair[0] == names[j])
					{
						myPairScore = j;
					}
					if (compPair[0] == names[j])
					{
						compPairScore = j;
					}
				}
			}
			if (myPairScore != compPairScore)
				return myPairScore>compPairScore ? true : false;
			else if (myPairScore == compPairScore)
			{
				int myMaxNameCard = 0, compMaxNameCard = 0;
				for (int i = 0; i < myNames.size(); i++)
				{
					for (int j = 0; j < names.size(); j++)
					{
						if (myNames[i] == names[j])
						{
							if (j >= myMaxNameCard)
							{
								myMaxNameCard = j;
							}
						}
					}
				}
				for (int i = 0; i < compNames.size(); i++)
				{
					for (int j = 0; j < names.size(); j++)
					{
						if (compNames[i] == names[j])
						{
							if (j >= compMaxNameCard)
							{
								compMaxNameCard = j;
							}
						}
					}
				}
				if (myMaxNameCard > compMaxNameCard) return true;
				else return false;
			}
		}

		/*hands = { { 0, "High Card" }, { 1, "Pair" }, { 2, "Two pairs" }, { 3, "Three Of A Kind" }, { 4, "Straight" },
		{ 5, "Flush" }, { 6, "Full House" }, { 7, "Four Of A Kind" }, { 8, "Straight Flush" }, { 9, "Royal Flush" } };*/

	}
}
void Game::round()
{
	getBlinds();
	b4flop(z);
	if (!raisedB4Flopped) flopRound(a);
	turnRound(b);
	riverRound(c);
	lastRound(x);
	tabled = true;
}
void Game::resetGame()
{
	foldedB4Flopped = false;
	folded = false;
	folded2 = false;
	folded3 = false;
	foldedEnd = false;
	compDecidedRound1 = false;
	compDecidedRound2 = false;
	compDecidedRound3 = false;
	compDecidedEnd = false;
	raisedB4Flopped = false;
	raised = false;
	raised2Round = false;
	raised3Round = false;
	raisedEnd = false;
	usedCards.clear();
	myNames.clear();
	mySymbols.clear();
	compNames.clear();
	compSymbols.clear();
	myHand.clear();
	compHand.clear();
	player1.chipsIn = 0;
	player1.chipsIn = 0;
	raiseValue = 0;
	pot = 0;
	system("pause");
	system("CLS");
	tabled = false;
}
void SetupFinder::finish(Game *game)
{
	if ((!foldedB4Flopped) && (!folded) && (!folded2) && (!folded3) && (!foldedEnd))
	{
		system("CLS");
		game->showHands();
		game->writePlayersInfo();
		game->showFlop();
		game->showTurn();
		game->showRiver();
		bool d = checkWinner();
		if (d == true)
		{
			cout << "U'VE WON " << pot - player1.chipsIn << "! " << endl;
			player1.chips += pot;
		}
		else if (d == false)
		{
			cout << "COMP WINS! " << endl;
			player2.chips += pot;
		}
	}
}

Game::Game()
{
	player1.chips = 10000;
	player2.chips = 10000;
}
