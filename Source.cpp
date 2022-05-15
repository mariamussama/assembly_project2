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

vector<block> read_mem(ifstream& file_name, vector<block>Memory)
{
	if (!file_name.is_open())
		cout << "Can't open file" << endl;
	else
	{
		string address;
		cout << "successfully opened file" << endl;
		while (getline(file_name, address))
		{

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
	cout << "Please enter the following data";
	cout << "Total cache size ";
	cin >> S;
	cout << "The cache line size ";
	cin >> L;
	cout << "The cache line size ";
	cin >> CC;
	Cache.resize(S);
	Memory = read_mem(file_name, Memory);
	return 0;
}