#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>

using namespace std;

typedef pair<pair<pair<int, int>, pair<int, int>>, bool> Configuration;
typedef unsigned long long ull;

struct Player
{
public:

	Player(int position) :
		Position(position),
		Score(0)
	{
	}

	void Advance(int toAdvance)
	{
		Position += toAdvance;
		Position %= 10;

		Score += Position + 1;
	}

public:

	int Position;
	int Score;
};

int RollDice(int& diceValue)
{
	int result = diceValue;
	diceValue++;
	if (diceValue >= 101)
		diceValue = 1;

	return result;
}

int Solve(int initialPlayer1, int initialPlayer2)
{
	initialPlayer1--;
	initialPlayer2--;

	Player* player1 = new Player(initialPlayer1);
	Player* player2 = new Player(initialPlayer2);

	Player* currentPlayer = player1;

	bool wasWon = false;
	int diceNumber = 1;
	int diceThrew = 0;

	while (!wasWon)
	{
		int roll1 = RollDice(diceNumber);
		int roll2 = RollDice(diceNumber);
		int roll3 = RollDice(diceNumber);

		diceThrew += 3;

		currentPlayer->Advance(roll1 + roll2 + roll3);

		if (currentPlayer->Score >= 1000)
			wasWon = true;

		if (currentPlayer == player1)
			currentPlayer = player2;
		else
			currentPlayer = player1;
	}

	int result = currentPlayer->Score * diceThrew;

	if (player2)
	{
		delete player2;
		player2 = nullptr;
	}

	if (player1)
	{
		delete player1;
		player1 = nullptr;
	}

	return result;
}

int main()
{
	ifstream fin("input.txt");

	string word;
	int player1Pos;
	int player2Pos;

	fin >> word >> word >> word >> word >> player1Pos;
	fin >> word >> word >> word >> word >> player2Pos;

	fin.close();

	cout << Solve(player1Pos, player2Pos)<< endl;

	ull distribution[10];
	memset(distribution, 0, sizeof(ull) * 10);
	for (int i = 1; i <= 3; i++)
		for (int j = 1; j <= 3; j++)
			for (int k = 1; k <= 3; k++)
				distribution[i + j + k]++;

	map<Configuration, ull> universesCount;
	queue<Configuration> configurationsQueue;
	set<Configuration> inQueue;

	Configuration initialConfig = make_pair(make_pair(make_pair(0, 0), make_pair(player1Pos, player2Pos)), false);
	configurationsQueue.push(initialConfig);
	universesCount[initialConfig] = 1;
	inQueue.insert(initialConfig);

	ull wonPlayer1 = 0;
	ull wonPlayer2 = 0;
	bool previousPlayerTurn = false;

	while (!configurationsQueue.empty())
	{
		auto currentConfig = configurationsQueue.front();
		configurationsQueue.pop();

		inQueue.erase(currentConfig);

		if (previousPlayerTurn != currentConfig.second)
			for (auto& keyVal : universesCount)
				if (keyVal.first.second == previousPlayerTurn)
					universesCount[keyVal.first] = 0;

		previousPlayerTurn = currentConfig.second;

		if (currentConfig.second)
		{
			if (currentConfig.first.first.first >= 21)
			{
				wonPlayer1 += universesCount[currentConfig];
				continue;
			}
		}
		else
		{
			if (currentConfig.first.first.second >= 21)
			{
				wonPlayer2 += universesCount[currentConfig];
				continue;
			}
		}

		ull univCount = universesCount[currentConfig];

		int currentPos = currentConfig.first.second.first;
		int currentScore = currentConfig.first.first.first;
		if (currentConfig.second)
		{
			currentPos = currentConfig.first.second.second;
			currentScore = currentConfig.first.first.second;
		}

		for (int i = 3; i <= 9; i++)
		{
			ull nextUnivCount = univCount * distribution[i];
			int nextPos = i + currentPos;
			nextPos--;
			nextPos %= 10;
			nextPos++;
			int nextScore = currentScore + nextPos;
			Configuration nextConfiguration = currentConfig;
			nextConfiguration.second = !nextConfiguration.second;
			if (!currentConfig.second)
			{
				nextConfiguration.first.first.first = nextScore;
				nextConfiguration.first.second.first = nextPos;
			}
			else
			{
				nextConfiguration.first.first.second = nextScore;
				nextConfiguration.first.second.second = nextPos;
			}
			if (universesCount.find(nextConfiguration) == universesCount.end())
				universesCount[nextConfiguration] = 0;
			universesCount[nextConfiguration] += nextUnivCount;

			if (inQueue.find(nextConfiguration) == inQueue.end())
			{
				configurationsQueue.push(nextConfiguration);
				inQueue.insert(nextConfiguration);
			}
		}
	}

	cout << max(wonPlayer1, wonPlayer2);
		
	return 0;
}