#include <fstream>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

constexpr auto TABLE_SIZE = 5;

struct Board
{
public:

	Board(int maxValue)
	{
		Positions = new pair<int, int>[maxValue + 1];
		for (int i = 0; i <= maxValue; i++)
			Positions[i] = make_pair(-1, -1);
		memset(Configuration, 0, sizeof(int) * TABLE_SIZE * TABLE_SIZE);
		memset(Marked, 0, sizeof(bool) * TABLE_SIZE * TABLE_SIZE);
		memset(MarkedLines, 0, sizeof(int) * TABLE_SIZE);
		memset(MarkedColumns, 0, sizeof(int) * TABLE_SIZE);
	}

	pair<int, int> GetScore(std::vector<int>& moves)
	{
		int lastMove = -1;
		int time = 0;
		for (auto& move : moves)
		{
			time++;
			auto position = Positions[move];
			if (position.first == -1 || position.second == -1)
				continue;

			Marked[position.first][position.second] = true;
			MarkedLines[position.first]++;
			MarkedColumns[position.second]++;
			if (Bingo(position.first, position.second))
			{
				lastMove = move;
				break;
			}
		}

		if (lastMove == -1)
			return make_pair(-1, -1);

		int sum = 0;

		for (int i = 0; i < TABLE_SIZE; i++)	
			for (int j = 0; j < TABLE_SIZE; j++)
				if (!Marked[i][j])
					sum += Configuration[i][j];
	
		return make_pair(sum * lastMove, time);
	}

private:

	bool Bingo(int line, int column)
	{
		return MarkedLines[line] >= TABLE_SIZE || MarkedColumns[column] >= TABLE_SIZE;
	}

public:

	pair<int, int>* Positions;
	int             Configuration[TABLE_SIZE][TABLE_SIZE];
	bool            Marked[TABLE_SIZE][TABLE_SIZE];
	int             MarkedLines[TABLE_SIZE];
	int             MarkedColumns[TABLE_SIZE];
};

int main()
{
	ifstream fin("input.txt");

	string numbersLine;
	fin >> numbersLine;

	vector<int> inputNumbers;
	string separator = ",";
	size_t pos;
	string token;
	while ((pos = numbersLine.find(separator)) != string::npos)
	{
		token = numbersLine.substr(0, pos);
		inputNumbers.push_back(atoi(token.c_str()));
		numbersLine.erase(0, pos + separator.length());
	}
	inputNumbers.push_back(atoi(numbersLine.c_str()));

	int biggestVal = inputNumbers[0];

	for (int i = 0; i < inputNumbers.size(); i++)
		biggestVal = max(biggestVal, inputNumbers[i]);
	
	int nr;
	vector<Board> boards;

	while (fin >> nr)
	{
		Board board(biggestVal + 1);

		for (int i = 0; i < TABLE_SIZE; i++)
		{
			for (int j = 0; j < TABLE_SIZE; j++)
			{
				int number;
				if (i == 0 && j == 0)
					number = nr;
				else
					fin >> number;

				board.Positions[number] = make_pair(i, j);
				board.Configuration[i][j] = number;
			}
		}

		boards.push_back(board);
	}

	fin.close();
	vector<pair<int, int>> scores;

	for (auto& board : boards)
	{
		auto score = board.GetScore(inputNumbers);
		scores.push_back(score);
	}

	int bestScore = 0;
	int worstScore = 0;
	for (int i = 0; i < scores.size(); i++)
	{
		auto score = scores[i];
		if (score.second < scores[bestScore].second)
			bestScore = i;
		if (score.second > scores[worstScore].second)
			worstScore = i;
	}

	cout << scores[bestScore].first << " " << scores[worstScore].first << endl;

	for (auto& board : boards)
	{
		if (board.Positions)
		{
			delete board.Positions;
			board.Positions = nullptr;
		}
	}

	return 0;
}