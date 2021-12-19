#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <queue>

using namespace std;

typedef pair<int, pair<int, int>> Position;

struct Scanner
{
public:

	Scanner() :
		ScannerCenter(make_pair(0, make_pair(0, 0))),
		Positions(vector<Position>()),
		RotatedPositions(vector<Position>()),
		WasFound(false)
	{
	}

public:

	Position         ScannerCenter;
	vector<Position> Positions;
	vector<Position> RotatedPositions;
	bool             WasFound;
};

Position ParsePosition(const string& str)
{
	stringstream ss(str);
	int x, y, z;
	char comma;
	ss >> x >> comma >> y >> comma >> z;
	return make_pair(x, make_pair(y, z));
}

Position RotatePosition(Position position, int xRot, int yRot, int zRot)
{
	float deg2rad = 0.0174532925;
	float xRadians = xRot * deg2rad;
	float yRadians = yRot * deg2rad;
	float zRadians = zRot * deg2rad;

	Position result = position;

	int x = result.first;
	int y = result.second.first;
	int z = result.second.second;
	result = make_pair(x, make_pair(round(y * cos(xRadians) - z * sin(xRadians)), round(y * sin(xRadians) + z * cos(xRadians))));

	x = result.first;
	y = result.second.first;
	z = result.second.second;
	result = make_pair(round(x * cos(yRadians) + z * sin(yRadians)), make_pair(y, round(z * cos(yRadians) - x * sin(yRadians))));

	x = result.first;
	y = result.second.first;
	z = result.second.second;
	result = make_pair(round(x * cos(zRadians) - y * sin(zRadians)), make_pair(round(x * sin(zRadians) + y * cos(zRadians)), z));
	
	return result;
}

// if a solution was found -> bool
// the center of the scannner -> Position
pair<bool, Position> AreMatchings(Scanner* baseScanner, const vector<Position>& rotatedPositions)
{
	set<Position> existingWorldPositions;
	for (auto& rotatedPos : baseScanner->RotatedPositions)
		existingWorldPositions.insert(make_pair(rotatedPos.first + baseScanner->ScannerCenter.first,
			make_pair(rotatedPos.second.first + baseScanner->ScannerCenter.second.first,
				rotatedPos.second.second + baseScanner->ScannerCenter.second.second)));

	for (int i = 0; i < baseScanner->RotatedPositions.size(); i++)
	{
		for (int j = 0; j < baseScanner->RotatedPositions.size(); j++)
		{
			if (j == i)
				continue;

			Position worldPos1 = make_pair(baseScanner->RotatedPositions[i].first + baseScanner->ScannerCenter.first,
				make_pair(baseScanner->RotatedPositions[i].second.first + baseScanner->ScannerCenter.second.first,
					baseScanner->RotatedPositions[i].second.second + baseScanner->ScannerCenter.second.second));

			Position worldPos2 = make_pair(baseScanner->RotatedPositions[j].first + baseScanner->ScannerCenter.first,
				make_pair(baseScanner->RotatedPositions[j].second.first + baseScanner->ScannerCenter.second.first,
					baseScanner->RotatedPositions[j].second.second + baseScanner->ScannerCenter.second.second));

			Position difference = make_pair(worldPos2.first - worldPos1.first,
				make_pair(worldPos2.second.first - worldPos1.second.first, worldPos2.second.second - worldPos1.second.second));

			for (int k = 0; k < rotatedPositions.size(); k++)
			{
				for (int l = 0; l < rotatedPositions.size(); l++)
				{
					if (k == l)
						continue;

					Position tryDiff = make_pair(rotatedPositions[l].first - rotatedPositions[k].first,
						make_pair(rotatedPositions[l].second.first - rotatedPositions[k].second.first,
							rotatedPositions[l].second.second - rotatedPositions[k].second.second));

					if (tryDiff == difference)
					{
						Position possibleCenter = make_pair(worldPos2.first - rotatedPositions[l].first,
							make_pair(worldPos2.second.first - rotatedPositions[l].second.first,
								worldPos2.second.second - rotatedPositions[l].second.second));

						int collisions = 0;

						for (int m = 0; m < rotatedPositions.size(); m++)
						{
							Position worldPosition = make_pair(possibleCenter.first + rotatedPositions[m].first,
								make_pair(possibleCenter.second.first + rotatedPositions[m].second.first,
									possibleCenter.second.second + rotatedPositions[m].second.second));

							if (existingWorldPositions.find(worldPosition) != existingWorldPositions.end())
								collisions++;
						}

						if (collisions >= 12)
							return make_pair(true, possibleCenter);
					}
				}
			}
		}
	}

	return make_pair(false, make_pair(0, make_pair(0, 0)));
}

bool GetMatchings(Scanner* baseScanner, Scanner* tryScanner)
{
	vector<Position> possibleRotations;
	for (int z = 0; z < 4; z++)
	{
		int zRotation = z * 90;
		for (int y = 0; y < 4; y++)
		{
			int yRotation = y * 90;
			possibleRotations.push_back(make_pair(0, make_pair(yRotation, zRotation)));
		}
		for (int x = -1; x < 2; x += 2)
		{
			int xRotation = x * 90;
			possibleRotations.push_back(make_pair(xRotation, make_pair(0, zRotation)));
		}
	}

	for (auto& possibleRotation : possibleRotations)
	{
		vector<Position> rotatedPoints;
		for (auto& position : tryScanner->Positions)
		{
			auto rotated = RotatePosition(position, possibleRotation.first, possibleRotation.second.first, possibleRotation.second.second);
			rotatedPoints.push_back(rotated);
		}
		auto canMatch = AreMatchings(baseScanner, rotatedPoints);
		if (canMatch.first)
		{
			tryScanner->RotatedPositions = rotatedPoints;
			tryScanner->ScannerCenter = canMatch.second;
			tryScanner->WasFound = true;
			return true;
		}
	}

	return false;
}

int Manhattan(Position a, Position b)
{
	return abs(a.first - b.first) + abs(a.second.first - b.second.first) + abs(a.second.second - b.second.second);
}

int main()
{
	ifstream fin("input.txt");

	string currentLine;
	Scanner* currentScanner = nullptr;
	vector<Scanner*> scanners;
	while (fin >> currentLine)
	{
		if (currentLine[0] == '-' && currentLine[1] == '-')
		{
			fin >> currentLine >> currentLine >> currentLine;
			currentScanner = new Scanner();
			scanners.push_back(currentScanner);
		}
		else
			if (currentScanner)
				currentScanner->Positions.push_back(ParsePosition(currentLine));
	}

	fin.close();

	scanners[0]->RotatedPositions.resize(scanners[0]->Positions.size());
	copy(scanners[0]->Positions.begin(), scanners[0]->Positions.end(), scanners[0]->RotatedPositions.begin());
	scanners[0]->WasFound = true;
	set<Position> beaconPositions;
	for (auto& beaconPos : scanners[0]->RotatedPositions)
		beaconPositions.insert(beaconPos);
	queue<int> scannersToMatch;
	scannersToMatch.push(0);

	while (!scannersToMatch.empty())
	{
		int crtScanner = scannersToMatch.front();
		scannersToMatch.pop();

		for (int i = 0; i < scanners.size(); i++)
		{
			if (i != crtScanner && !scanners[i]->WasFound)
			{
				if (GetMatchings(scanners[crtScanner], scanners[i]))
				{
					scannersToMatch.push(i);
					for (auto& beaconPos : scanners[i]->RotatedPositions)
						beaconPositions.insert(make_pair(beaconPos.first + scanners[i]->ScannerCenter.first, 
							make_pair(beaconPos.second.first + scanners[i]->ScannerCenter.second.first,
								beaconPos.second.second + scanners[i]->ScannerCenter.second.second)));
				}
			}
		}
	}

	cout << beaconPositions.size() << endl;

	int maxDistance = 0;
	for (int i = 0; i < scanners.size(); i++)
	{
		for (int j = 0; j < scanners.size(); j++)
		{
			if (i == j)
				continue;

			int distance = Manhattan(scanners[i]->ScannerCenter, scanners[j]->ScannerCenter);
			maxDistance = max(distance, maxDistance);
		}
	}

	cout << maxDistance;

	for (int i = 0; i < scanners.size(); i++)
	{
		if (scanners[i])
		{
			delete scanners[i];
			scanners[i] = nullptr;
		}
	}

	return 0;
}