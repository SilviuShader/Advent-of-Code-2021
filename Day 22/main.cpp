#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <map>

using namespace std;

struct Action
{
public:

	enum class ActionType
	{
		On,
		Off
	};

public:

	ActionType     ActionType = ActionType::Off;
	pair<int, int> RangeX;
	pair<int, int> RangeY;
	pair<int, int> RangeZ;
};

struct ActionsBatch
{
public:

	ActionsBatch(const vector<Action>& defaultActions)
	{
		int minX = defaultActions[0].RangeX.first;
		int maxX = defaultActions[0].RangeX.second;
		for (auto& action : defaultActions)
		{
			minX = min(minX, action.RangeX.first);
			maxX = max(maxX, action.RangeX.second);
		}

		int difference = maxX - minX;
		int step = sqrt(difference);
		if (step < 1)
			step = 1;

		int startX = minX;
		while (startX <= maxX)
		{
			int nextX = startX + step - 1;
			vector<Action> currentBatch;
			for (auto& action : defaultActions)
			{
				Action newAction = action;
				newAction.RangeX = make_pair(max(startX, newAction.RangeX.first), min(nextX, newAction.RangeX.second));
				if (newAction.RangeX.first <= newAction.RangeX.second)
					currentBatch.push_back(newAction);
			}
			Batches.push_back(currentBatch);
			startX += step;
		}
	}

public:

	vector<vector<Action>> Batches;
};

int Solve1(const vector<Action>& actions)
{
	set<pair<int, pair<int, int>>> usedPositions;

	for (auto& action : actions)
	{
		for (int x = max(action.RangeX.first, -50); x <= min(action.RangeX.second, 50); x++)
		{
			for (int y = max(action.RangeY.first, -50); y <= min(action.RangeY.second, 50); y++)
			{
				for (int z = max(action.RangeZ.first, -50); z <= min(action.RangeZ.second, 50); z++)
				{
					auto currentPosition = make_pair(x, make_pair(y, z));
					if (action.ActionType == Action::ActionType::On)
					{
						if (usedPositions.find(currentPosition) == usedPositions.end())
							usedPositions.insert(currentPosition);
					}
					else
					{
						if (usedPositions.find(currentPosition) != usedPositions.end())
							usedPositions.erase(currentPosition);
					}
				}
			}
		}
	}

	return usedPositions.size();
}

int main()
{
	string action;
	char tmpChr;
	int startX, endX;
	int startY, endY;
	int startZ, endZ;

	ifstream fin("input.txt");

	vector<Action> actions;

	while (fin >> action >> tmpChr >> tmpChr >> startX >> tmpChr >> tmpChr >> endX >> 
		tmpChr >> tmpChr >> tmpChr >> startY >> tmpChr >> tmpChr >> endY >> tmpChr >>
		tmpChr >> tmpChr >> startZ >> tmpChr >> tmpChr >> endZ)
	{
		Action currentAction;
		currentAction.ActionType = action == "on" ? Action::ActionType::On : Action::ActionType::Off;
		currentAction.RangeX = make_pair(startX, endX);
		currentAction.RangeY = make_pair(startY, endY);
		currentAction.RangeZ = make_pair(startZ, endZ);

		actions.push_back(currentAction);
	}

	fin.close();

	cout << Solve1(actions) << endl;

	ActionsBatch actionsBatch(actions);

	set<int> realXValues;
	set<int> realYValues;
	set<int> realZValues;

	for (auto& batch : actionsBatch.Batches)
	{
		for (auto& action : batch)
		{
			realXValues.insert(action.RangeX.first);
			realXValues.insert(action.RangeX.second + 1);

			realYValues.insert(action.RangeY.first);
			realYValues.insert(action.RangeY.second + 1);

			realZValues.insert(action.RangeZ.first);
			realZValues.insert(action.RangeZ.second + 1);
		}
	}

	vector<int> orderToValueX;
	map<int, int> valueToOrderX;

	for (auto& x : realXValues)
	{
		valueToOrderX[x] = orderToValueX.size();
		orderToValueX.push_back(x);
	}

	vector<int> orderToValueY;
	map<int, int> valueToOrderY;

	for (auto& y : realYValues)
	{
		valueToOrderY[y] = orderToValueY.size();
		orderToValueY.push_back(y);
	}

	vector<int> orderToValueZ;
	map<int, int> valueToOrderZ;

	for (auto& z : realZValues)
	{
		valueToOrderZ[z] = orderToValueZ.size();
		orderToValueZ.push_back(z);
	}

	unsigned long long result = 0;
	
	for (auto& batch : actionsBatch.Batches)
	{
		set<pair<int, pair<int, int>>> usedPositions;

		for (auto& action : batch)
		{
			for (int x = valueToOrderX[action.RangeX.first]; x < valueToOrderX[action.RangeX.second + 1]; x++)
			{
				for (int y = valueToOrderY[action.RangeY.first]; y < valueToOrderY[action.RangeY.second + 1]; y++)
				{
					for (int z = valueToOrderZ[action.RangeZ.first]; z < valueToOrderZ[action.RangeZ.second + 1]; z++)
					{
						auto currentPosition = make_pair(x, make_pair(y, z));
						if (action.ActionType == Action::ActionType::On)
						{
							if (usedPositions.find(currentPosition) == usedPositions.end())
								usedPositions.insert(currentPosition);
						}
						else
						{
							if (usedPositions.find(currentPosition) != usedPositions.end())
								usedPositions.erase(currentPosition);
						}
					}
				}
			}
		}

		for (auto& usedPosition : usedPositions)
		{
			long long currentX = orderToValueX[usedPosition.first];
			long long nextX = orderToValueX[usedPosition.first + 1];

			long long currentY = orderToValueY[usedPosition.second.first];
			long long nextY = orderToValueY[usedPosition.second.first + 1];

			long long currentZ = orderToValueZ[usedPosition.second.second];
			long long nextZ = orderToValueZ[usedPosition.second.second + 1];

			result += (nextX - currentX) * (nextY - currentY) * (nextZ - currentZ);
		}
	}

	cout << result << endl;

	return 0;
}