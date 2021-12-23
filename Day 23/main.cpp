#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <sstream>

using namespace std;

struct Configuration
{
public:

	Configuration(int bucketSize) :
		Cost(0),
		BucketSize(bucketSize),
		Estimation(-1),
		ConfigKey(""),
		Parent(nullptr)
	{
		Hallway.resize(11, '.');
		Rooms.resize(4, vector<char>());
	}

	bool IsFinal()
	{
		char currentTargetSymbol = 'A';
		int roomIndex = 0;

		for (auto& room : Rooms)
		{
			if (room.size() < BucketSize)
				return false;

			for (auto& symbol : room)
				if (symbol != currentTargetSymbol)
					return false;

			currentTargetSymbol++;
			roomIndex++;
		}

		return true;
	}

	int EstimateRemaining()
	{
		int result = 0;

		char currentTargetSymbol = 'A';
		int roomPositionInHall = 2;
		int roomIndex = 0;
		for (auto& room : Rooms)
		{
			int chrIndex = 0;
			for (auto& chr : room)
			{
				if (chr != currentTargetSymbol)
				{
					result += GetMovementCostRoomToHallway(roomIndex, BucketSize, roomPositionInHall, chr);
					int targetRoom = chr - 'A';
					result += GetMovementCostHallwayToRoom(targetRoom, BucketSize - 1, roomPositionInHall, chr);
				}
				chrIndex++;
			}

			currentTargetSymbol++;
			roomPositionInHall += 2;
			roomIndex++;
		}

		for (int hallwayPos = 0; hallwayPos < Hallway.size(); hallwayPos++)
		{
			if (Hallway[hallwayPos] != '.')
			{
				char currentChr = Hallway[hallwayPos];
				int targetRoom = currentChr - 'A';
				int targetRoomPos = 2 + targetRoom * 2;
				result += GetMovementCostHallwayToRoom(targetRoom, BucketSize - 1, hallwayPos, currentChr);
			}
		}

		Estimation = result;

		return result;
	}

	void GetNeighbours(vector<Configuration>& result)
	{
		char currentTargetSymbol = 'A';
		int roomPositionInHall = 2;
		int roomIndex = 0;

		for (auto& room : Rooms)
		{
			if (MustGetOutRoom(room, currentTargetSymbol))
				GenNeighboursOutRoom(result, *this, roomIndex, roomPositionInHall);
			currentTargetSymbol++;
			roomPositionInHall += 2;
			roomIndex++;
		}

		for (int hallwayPos = 0; hallwayPos < Hallway.size(); hallwayPos++)
		{
			if (Hallway[hallwayPos] != '.')
			{
				char currentCharacter = Hallway[hallwayPos];
				int targetRoomIndex = currentCharacter - 'A';
				int targetRoomPosition = 2 + targetRoomIndex * 2;
				int leftSide = min(hallwayPos, targetRoomPosition);
				int rightSide = max(hallwayPos, targetRoomPosition);
				bool canGoToRoom = true;
				for (int pos = leftSide; pos <= rightSide; pos++)
					if (Hallway[pos] != '.' && pos != hallwayPos)
						canGoToRoom = false;
				if (canGoToRoom)
				{
					if (CanAppendToRoom(Rooms[targetRoomIndex], currentCharacter))
					{
						Configuration newConfiguration(*this);
						newConfiguration.Hallway[hallwayPos] = '.';
						newConfiguration.Cost += GetMovementCostHallwayToRoom(targetRoomIndex, Rooms[targetRoomIndex].size(), hallwayPos, currentCharacter);
						newConfiguration.Rooms[targetRoomIndex].push_back(currentCharacter);
						newConfiguration.EstimateRemaining();
						newConfiguration.ComputeKey();
						newConfiguration.Parent = this;

						result.push_back(newConfiguration);
					}
				}
			}
		}
	}

	string ComputeKey()
	{
		string result(Hallway.begin(), Hallway.end());
		result.append("#");
		for (auto& room : Rooms)
		{
			result.append(string(room.begin(), room.end()));
			result.append("#");
		}

		ConfigKey = result;

		return result;
	}

private:

	void GenNeighboursOutRoom(vector<Configuration>& result, const Configuration& startConfig, int roomIndex, int roomPosition)
	{
		for (int i = 0; i < Hallway.size(); i++)
		{
			if (i != roomPosition && i != 2 && i != 4 && i != 6 && i != 8)
			{
				int leftSide = min(i, roomPosition);
				int rightSize = max(i, roomPosition);
				bool validPlacePos = true;
				for (int pos = leftSide; pos <= rightSize; pos++)
					if (startConfig.Hallway[pos] != '.')
						validPlacePos = false;

				if (validPlacePos)
				{
					Configuration newConfig(startConfig);
					char chr = newConfig.Rooms[roomIndex].back();
					newConfig.Cost += GetMovementCostRoomToHallway(roomIndex, newConfig.Rooms[roomIndex].size(), i, chr);
					newConfig.Rooms[roomIndex].pop_back();
					newConfig.Hallway[i] = chr;
					newConfig.EstimateRemaining();
					newConfig.ComputeKey();
					newConfig.Parent = this;

					result.push_back(newConfig);
				}
			}
		}
	}

	int GetMovementCostRoomToHallway(int roomIndex, int roomSize, int hallwayPos, char chr)
	{
		int steps = 1;
		steps += BucketSize - roomSize;

		int roomPosition = 2 + roomIndex * 2;

		steps += abs(roomPosition - hallwayPos);

		return LetterCost(chr) * steps;
	}

	int GetMovementCostHallwayToRoom(int roomIndex, int roomSize, int hallwayPos, char chr)
	{
		int steps = BucketSize;
		steps -= roomSize;

		int roomPosition = 2 + roomIndex * 2;

		steps += abs(roomPosition - hallwayPos);

		return LetterCost(chr) * steps;
	}

	int LetterCost(char letter)
	{
		int result = 1;
		for (char l = 'A'; l < letter; l++)
			result *= 10;

		return result;
	}

	bool MustGetOutRoom(const vector<char>& room, char targetRoomSymbol)
	{
		for (auto& chr : room)
			if (chr != targetRoomSymbol)
				return true;
		
		return false;
	}

	bool CanAppendToRoom(const vector<char>& room, char targetSymbol)
	{
		if (room.size() >= BucketSize)
			return false;

		for (int i = 0; i < room.size(); i++)
			if (room[i] != targetSymbol)
				return false;

		return true;
	}

	friend bool operator<(Configuration const& a, Configuration const& b)
	{
		return std::strcmp(a.ConfigKey.c_str(), b.ConfigKey.c_str()) < 0;
	}

public:

	vector<char>         Hallway;

	vector<vector<char>> Rooms;
	
	int                  Cost;
	int                  BucketSize;
	int                  Estimation;
	string               ConfigKey;
	Configuration*       Parent;
};

struct CompareCost 
{
public:

	bool operator() (Configuration& c1, Configuration& c2)
	{
		return c1.Cost + c1.Estimation > c2.Cost + c2.Estimation;
	}
};

int BestSol(const Configuration& initialConfiguration, int bucketSize)
{
	priority_queue<Configuration, vector<Configuration>, CompareCost> configurationQueue;
	configurationQueue.push(initialConfiguration);
	set<Configuration> inQueue;
	inQueue.insert(initialConfiguration);

	while (!configurationQueue.empty())
	{
		Configuration* ptr = (Configuration*)(& (*inQueue.find(configurationQueue.top())));
		auto& currentConfig = *ptr;
		configurationQueue.pop();

		if (currentConfig.IsFinal())
		{
			int result = currentConfig.Cost;
			// Fordebugging purposes only
			/*
			Configuration* ptr = &currentConfig;
			while (ptr)
			{
				cout << ptr->ConfigKey << endl;
				ptr = ptr->Parent;
			}
			*/
			return result;
		}
		vector<Configuration> neighbours;
		currentConfig.GetNeighbours(neighbours);
		for (auto& neighbour : neighbours)
		{
			if (inQueue.find(neighbour) == inQueue.end())
			{
				configurationQueue.push(neighbour);
				inQueue.insert(neighbour);
			}
			else
			{
				if (inQueue.find(neighbour)->Cost > neighbour.Cost)
				{
					Configuration* foundPtr = (Configuration*)(& (*inQueue.find(neighbour)));
					foundPtr->Cost = neighbour.Cost;
					configurationQueue.push(neighbour);
				}
			}
		}
	}

	return 0;
}

int main()
{
	ifstream fin("input.txt");

	string line;
	string configLine1;
	string configLine2;
	fin >> line >> line >> configLine1 >> configLine2;

	fin.close();

	Configuration initialConfiguration(2);
	initialConfiguration.Rooms[0].push_back(configLine2[1]);
	initialConfiguration.Rooms[0].push_back(configLine1[3]);

	initialConfiguration.Rooms[1].push_back(configLine2[3]);
	initialConfiguration.Rooms[1].push_back(configLine1[5]);

	initialConfiguration.Rooms[2].push_back(configLine2[5]);
	initialConfiguration.Rooms[2].push_back(configLine1[7]);

	initialConfiguration.Rooms[3].push_back(configLine2[7]);
	initialConfiguration.Rooms[3].push_back(configLine1[9]);

	initialConfiguration.ComputeKey();

	cout << BestSol(initialConfiguration, 2) << endl;

	initialConfiguration = Configuration(4);
	initialConfiguration.Rooms[0].push_back(configLine2[1]);
	initialConfiguration.Rooms[0].push_back('D');
	initialConfiguration.Rooms[0].push_back('D');
	initialConfiguration.Rooms[0].push_back(configLine1[3]);

	initialConfiguration.Rooms[1].push_back(configLine2[3]);
	initialConfiguration.Rooms[1].push_back('B');
	initialConfiguration.Rooms[1].push_back('C');
	initialConfiguration.Rooms[1].push_back(configLine1[5]);

	initialConfiguration.Rooms[2].push_back(configLine2[5]);
	initialConfiguration.Rooms[2].push_back('A');
	initialConfiguration.Rooms[2].push_back('B');
	initialConfiguration.Rooms[2].push_back(configLine1[7]);

	initialConfiguration.Rooms[3].push_back(configLine2[7]);
	initialConfiguration.Rooms[3].push_back('C');
	initialConfiguration.Rooms[3].push_back('A');
	initialConfiguration.Rooms[3].push_back(configLine1[9]);

	initialConfiguration.ComputeKey();

	cout << BestSol(initialConfiguration, 4);

	return 0;
}