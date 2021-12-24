#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>

using namespace std;
typedef long long ll;

int euclidean_remainder(int a, int b)
{
	int r = a % b;
	return r >= 0 ? r : r + std::abs(b);
}

struct ALU
{
public:
	
	struct Instruction
	{
	public:

		enum InstructionType
		{
			inp,
			add,
			mul,
			div,
			mod,
			eql
		};


	public:

		Instruction(ALU* alu, string rawInstruction) :
			_allocSrc(false)
		{
			stringstream ss(rawInstruction);

			string op;
			ss >> op;
			string dest;
			ss >> dest;

			_dst = &alu->X;

			if (dest == "y")
				_dst = &alu->Y;
			else if (dest == "z")
				_dst = &alu->Z;
			else if (dest == "w")
				_dst = &alu->W;
			else if (dest != "x")
			{
				cout << "WTF" << endl;
			}

			if (op == "inp")
			{
				_instructionType = inp;
			}
			else
			{
				string src;
				ss >> src;
				if (src == "x")
					_src = &alu->X;
				else if (src == "y")
					_src = &alu->Y;
				else if (src == "z")
					_src = &alu->Z;
				else if (src == "w")
					_src = &alu->W;
				else
				{
					ll val = atoi(src.c_str());
					_src = new ll();
					*_src = val;
					_allocSrc = true;
				}

				if (op == "add")
					_instructionType = add;
				else if (op == "mul")
					_instructionType = mul;
				else if (op == "div")
					_instructionType = div;
				else if (op == "mod")
					_instructionType = mod;
				else if (op == "eql")
					_instructionType = eql;
				else
				{
					cout << "WTF" << endl;
				}
			}
		}

		~Instruction()
		{
			if (_allocSrc)
			{
				delete _src;
				_src = nullptr;
			}
		}

		void Execute(int* input, int& inputIndex)
		{
			switch (_instructionType)
			{
			case inp:
				*_dst = input[inputIndex++];
				break;
			case add:
				*_dst = *_dst + *_src;
				break;
			case mul:
				*_dst = *_dst * *_src;
				break;
			case div:
				*_dst = *_dst / *_src;
				break;
			case mod:
				*_dst = *_dst % *_src;
				break;
			case eql:
				*_dst = *_dst == *_src ? 1 : 0;
				break;
			}
		}

	private:

		InstructionType _instructionType;
		ll*             _dst;
		ll*             _src;

		bool            _allocSrc;
	};
public:

	ALU() :
		Instructions(vector<string>()),
		X(0),
		Y(0),
		Z(0),
		W(0)
	{
	}

	~ALU()
	{
		for (auto& compiledInstruction : CompiledInstructions)
		{
			if (compiledInstruction)
			{
				delete compiledInstruction;
				compiledInstruction = nullptr;
			}
		}
	}


	void Compile()
	{
		for (auto& instruction : Instructions)
		{
			Instruction* compiledInstruction = new Instruction(this, instruction);
			CompiledInstructions.push_back(compiledInstruction);
		}
	}

	void Run(int* input, int inputSize, ll startZ, int startInstruction, int instructionsCount)
	{
		X = Y = Z = W = 0;
		Z = startZ;
		int inputIndex = 0;

		for (int instructionIndex = startInstruction; instructionIndex < startInstruction + instructionsCount; instructionIndex++)
		{
			auto& instruction = CompiledInstructions[instructionIndex];
			instruction->Execute(input, inputIndex);
		}
	}

public:

	vector<string>                     Instructions;
	vector<Instruction*>               CompiledInstructions;
	ll                                 X, Y, Z, W;
};

void Backtr(int index, ALU& alu, int targetZ, int* solution, unordered_map<int, vector<pair<int, int>>>* pairsToObtainZ, ll& finalSolution, ll& finalSolution2)
{
	if (index < 0)
	{
		if (targetZ == 0)
		{
			ll crtSolution = 0;
			for (int i = 0; i < 14; i++)
				crtSolution = crtSolution * 10 + solution[i];
			finalSolution = max(crtSolution, finalSolution);
			if (finalSolution2 == -1)
				finalSolution2 = crtSolution;
			finalSolution2 = min(crtSolution, finalSolution2);
		}
	}
	else
	{
		for (auto& pr : pairsToObtainZ[index][targetZ])
		{
			int w = pr.first;
			ll previousZ = pr.second;
			solution[index] = w;
			Backtr(index - 1, alu, previousZ, solution, pairsToObtainZ, finalSolution, finalSolution2);
		}
	}
}

int main()
{
	ifstream fin("input.txt");

	string currentLine;
	ALU alu = ALU();
	while (getline(fin, currentLine))
		alu.Instructions.push_back(currentLine);

	alu.Compile();

	fin.close();

	unordered_map<int, vector<pair<int, int>>> pairsToObtainZ[14];
	unordered_set<int> targets;

	targets.insert(0);

	for (int i = 13; i >= 0; i--)
	{
		unordered_set<int> newTargets;

		int rangeStart = -1000000;
		int rangeEnd = 1000000;

		if (i == 13)
		{
			rangeStart = -10;
		}

		for (int w = 1; w <= 9; w++)
		{
			for (int previousZ = rangeStart; previousZ <= rangeEnd; previousZ++)
			{
				alu.Run(&w, 1, previousZ, i * 18, 18);
				if (targets.find(alu.Z) != targets.end())
				{
					newTargets.insert(previousZ);
					if (pairsToObtainZ[i].find(alu.Z) == pairsToObtainZ[i].end())
						pairsToObtainZ[i][alu.Z] = vector<pair<int, int>>();
					pairsToObtainZ[i][alu.Z].push_back(make_pair(w, previousZ));
				}
			}
		}

		targets = newTargets;
	}

	int solution[14];
	ll result1 = -1;
	ll result2 = -1;

	Backtr(13, alu, 0, solution, pairsToObtainZ, result1, result2);

	cout << result1 << endl;
	cout << result2;

	return 0;
}