#include <fstream>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

typedef unsigned long long ull;

string GetBinaryString(string hexaString)
{
	string result = "";
	for (auto& chr : hexaString)
	{
		int number = 0;
		if (chr >= '0' && chr <= '9')
			number = chr - '0';
		else
			number = 10 + (chr - 'A');

		char bits[5] = {};
		bits[4] = '\0';

		for (int i = 0; i < 4; i++)
		{
			bits[3 - i] = number % 2 == 0 ? '0' : '1';
			number /= 2;
		}

		result.append(bits);
	}

	return result;
}

ull Sum(ull a, ull b)
{
	return a + b;
}

ull Product(ull a, ull b)
{
	return a * b;
}

ull Minimum(ull a, ull b)
{
	if (a < b)
		return a;
	return b;
}

ull Maximum(ull a, ull b)
{
	if (a > b)
		return a;
	return b;
}

ull GreaterThan(ull a, ull b)
{
	if (a > b)
		return 1;
	return 0;
}

ull LessThan(ull a, ull b)
{
	if (a < b)
		return 1;
	return 0;
}

ull EqualTo(ull a, ull b)
{
	return a == b;
}

class Packet
{
public:

	Packet() : 
		_version(-1),
		_id(-1),
		_packetValue(-1)
	{
	}

	~Packet() 
	{
		for (int i = 0; i < _childPackets.size(); i++)
		{
			if (_childPackets[i])
			{
				delete _childPackets[i];
				_childPackets[i] = nullptr;
			}
		}

		_childPackets.clear();
	}

	int Create(const string& binaryString, int startIndex)
	{
		_version = BinaryStringToInt(binaryString, startIndex, 3);
		_id = BinaryStringToInt(binaryString, startIndex + 3, 3);
		if (_id == 4)
		{
			int startLiteral = startIndex + 6;
			bool finishedLiteral = false;
			ull result = 0;
			while (startLiteral < binaryString.size() && !finishedLiteral)
			{
				if (binaryString[startLiteral] == '0')
					finishedLiteral = true;

				result = (result << 4) + BinaryStringToInt(binaryString, startLiteral + 1, 4);

				startLiteral += 5;
			}
			_packetValue = result;
			
			return startLiteral;
		}
		else
		{
			startIndex += 6;
			
			function<ull(ull, ull)> currentOperation = Sum;

			switch (_id)
			{
			case 0:
				currentOperation = Sum;
				break;
			case 1:
				currentOperation = Product;
				break;
			case 2:
				currentOperation = Minimum;
				break;
			case 3:
				currentOperation = Maximum;
				break;
			case 5:
				currentOperation = GreaterThan;
				break;
			case 6:
				currentOperation = LessThan;
				break;
			case 7:
				currentOperation = EqualTo;
				break;
			}

			char lengthTypeId = binaryString[startIndex];
			startIndex++;
			if (lengthTypeId == '0')
			{
				int packagesLength = 0;
				for (int i = 0; i < 15; i++)
					packagesLength = (packagesLength << 1) + (binaryString[startIndex + i] == '1' ? 1 : 0);

				startIndex += 15;
				int startedChildrenPos = startIndex;
				int stepIx = 0;
				while (startIndex < startedChildrenPos + packagesLength)
				{
					Packet* child = new Packet();
					startIndex = child->Create(binaryString, startIndex);
					if (stepIx == 0)
						_packetValue = child->_packetValue;
					else
						_packetValue = currentOperation(_packetValue, child->_packetValue);
					stepIx++;
					_childPackets.push_back(child);
				}

				return startedChildrenPos + packagesLength;
			}
			else
			{
				int packagesCount = 0;
				for (int i = 0; i < 11; i++)
					packagesCount = (packagesCount << 1) + (binaryString[startIndex + i] == '1' ? 1 : 0);

				startIndex += 11;
				int stepIx = 0;
				for (int i = 0; i < packagesCount; i++)
				{
					Packet* child = new Packet();
					startIndex = child->Create(binaryString, startIndex);
					if (stepIx == 0)
						_packetValue = child->_packetValue;
					else
						_packetValue = currentOperation(_packetValue, child->_packetValue);
					stepIx++;
					_childPackets.push_back(child);
				}

				return startIndex;
			}
		}
		return binaryString.size();
	}

	int VersionSum()
	{
		int result = _version;
		for (auto& child : _childPackets)
			result += child->VersionSum();

		return result;
	}

	ull GetValue() const
	{
		return _packetValue;
	}

private:

	ull BinaryStringToInt(const string& binaryString, int startIndex, int length)
	{
		ull result = 0;
		for (int i = 0; i < length; i++)
			result = (result << 1) + (binaryString[i + startIndex] == '1' ? 1 : 0);
		return result;
	}

private:

	int             _version;
	int             _id;
	ull             _packetValue;
	vector<Packet*> _childPackets;
};

int main()
{
	ifstream fin("input.txt");
	
	string hexaString;
	fin >> hexaString;
	
	fin.close();

	string binaryString = GetBinaryString(hexaString);
	Packet* mainPacket = new Packet();
	mainPacket->Create(binaryString, 0);

	cout << mainPacket->VersionSum() << endl;
	cout << mainPacket->GetValue();

	if (mainPacket)
	{
		delete mainPacket;
		mainPacket = nullptr;
	}

	return 0;
}