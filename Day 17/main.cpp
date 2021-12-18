#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_set>

using namespace std;

typedef long long ll;

ll HitsTarget(ll velocityY, int minX, int maxX, int minY, int maxY)
{
	ll result = 0;
	unordered_set<ll> usedXVels;
	for (ll startAreaY = minY; startAreaY <= maxY; startAreaY++)
	{
		ll b = startAreaY;
		ll c = velocityY;
		ll delta2 = 4 * c * c + 4 * c - 8 * b + 1;
		ll sq2 = sqrtl(delta2);
		if (sq2 * sq2 == delta2)
		{
			ll n2 = -(-2 * c + 1 - sq2) / 2;
			n2++;
			for (ll startVelocityX = 0; startVelocityX <= maxX; startVelocityX++)
			{
				ll xPos = (startVelocityX * (startVelocityX + 1)) / 2;
				ll cutX = max(startVelocityX - n2, 0LL);
				xPos -= (cutX * (cutX + 1)) / 2;
				if (xPos >= minX && xPos <= maxX)
					usedXVels.insert(startVelocityX);
			}
		}
	}

	return usedXVels.size();
}

int main()
{
	ifstream fin("input.txt");

	string word;
	char chr;
	int minX, maxX;
	int minY, maxY;

	fin >> word >> word >> chr >> chr >> minX >> chr >> chr >> maxX >> chr >> chr >> chr >> minY >> chr >> chr >> maxY;

	fin.close();

	
	ll searchVal = 1LL << 61;
	ll currentY = 0;

	ll solution = 0;
	for (ll i = 0; i < 10000; i++)
		if (HitsTarget(i, minX, maxX, minY, maxY))
			solution = i;

	cout << (solution * (solution + 1)) / 2 << endl;

	ll maxVelY = solution;

	ll solution2 = 0;
	for (ll velY = minY; velY <= solution; velY++)
		solution2 += HitsTarget(velY, minX, maxX, minY, maxY);

	cout << solution2;

	return 0;
}