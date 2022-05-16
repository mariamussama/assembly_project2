#include<iostream>
#include<map>
#include<string>
#include<fstream>
#include <cstdlib>
#include <cstdint>
#include<iomanip>
#include<bitset>
#include<vector>
#include<array>
#include<cmath>
using namespace std;

struct block
{
	string tag;
	string offset;
	string line_idx;
};

struct line
{
	string valid_bit;
	string tag;
	string data;

};

vector<block> read_mem(ifstream& file_name, vector<block>Memory, int offset, int index, int tag)
{
	block B;
	if (!file_name.is_open())
		cout << "Can't open file" << endl;
	else
	{
		string address;
		cout << "successfully opened file" << endl;
		while (getline(file_name, address))
		{
			B.tag = address.substr(0, tag);
			B.line_idx = address.substr(tag - 1, index);
			B.offset = address.substr(tag + index - 1, offset);
			Memory.push_back(B);
		}
	}

	return Memory;
}


int main()
{
	ifstream file_name("memory.txt");
	vector<block>Memory;
	vector<line> Cache;
	int S, L, CC;
	cout << "Please enter the following data" << endl;
	cout << "Total cache size ";
	cin >> S;
	cout << "The cache line size ";
	cin >> L;
	cout << "The number of cycles needed to access the cache ";
	cin >> CC;
	int C = S / L;
	Cache.resize(C);
	int offset = log2(L);
	int index = log2(C);
	int tag = 32 - index - offset;
	Memory = read_mem(file_name, Memory, offset, index, tag);
	return 0;
}