#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class MoveType
{
	Forward,
	Up,
	Down,
	None
};

int main()
{
	ifstream fin("input.txt");
	string word;
	int number;

	vector<pair<MoveType, int>> moves;

	while (fin >> word >> number)
	{
		MoveType moveType = MoveType::None;
		
		if (word == "forward")
			moveType = MoveType::Forward;
		else if (word == "down")
			moveType = MoveType::Down;
		else if (word == "up")
			moveType = MoveType::Up;

		if (moveType != MoveType::None)
			moves.push_back(make_pair(moveType, number));
	}

	fin.close();

	int horizontal = 0;
	int depth = 0;
	
	for (auto& move : moves)
	{
		switch (move.first)
		{
		case MoveType::Forward:
			horizontal += move.second;
			break;
		case MoveType::Down:
			depth += move.second;
			break;
		case MoveType::Up:
			depth -= move.second;
			break;
		}
	}

	cout << horizontal * depth << endl;

	horizontal = 0;
	depth = 0;
	int aim = 0;

	for (auto& move : moves)
	{
		switch (move.first)
		{
		case MoveType::Forward:
			horizontal += move.second;
			depth += aim * move.second;
			break;
		case MoveType::Down:
			aim += move.second;
			break;
		case MoveType::Up:
			aim -= move.second;
			break;
		}
	}

	cout << horizontal * depth;

	return 0;
}