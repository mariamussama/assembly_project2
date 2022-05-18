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
	string tag = "";
	string offset = "";
	string line_idx = "";
	string data = "";
	int index;
};

struct line
{
	string valid_bit = "0";
	string tag = "";
	string data = "";

};

vector<block> read_mem(ifstream& file_name, vector<block>& Memory, int offset, int index, int tag)
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
			B.data = address;
			B.tag = address.substr(0, tag);
			B.line_idx = address.substr(tag - 1, index);
			B.offset = address.substr(tag + index - 1, offset);
			string temp = B.line_idx;
			bitset<32> b(temp);
			B.index = b.to_ulong();
			//cout << B.line_idx << " " << B.index << endl;
			Memory.push_back(B);
		}
	}

	return Memory;
}
void direct_mapping(vector<block>Memory, vector<line>& Cache, int CC)
{
	int hit = 0, miss = 0;
	int AMAT;
	int hit_time = 1;
	int miss_penalty = 20;
	for (int i = 0; i < 10; i++)
	{
		int idx = rand() % Memory.size();
		block temp = Memory[idx];
		cout << endl;
		cout << "processor: " << temp.tag << " " << temp.index << " " << temp.data << endl;
		if (Cache[temp.index].valid_bit == "0")
		{
			miss++;
			Cache[temp.index].data = temp.data;
			Cache[temp.index].tag = temp.tag;
			Cache[temp.index].valid_bit = "1";
		}
		else
		{
			if (Cache[temp.index].tag == temp.tag)
			{
				hit++;
			}
			else
			{
				miss++;
				Cache[temp.index].data = temp.data;
				Cache[temp.index].tag = temp.tag;
			}
		}
		cout << "hit: " << hit << " miss: " << miss << endl;
		AMAT = hit_time + (miss / (miss + hit)) * miss_penalty;
		cout << "AMAT: " << AMAT;
		for (int j = 0; j < Cache.size(); j++)
		{
			line c = Cache[j];
			cout << c.valid_bit << "\t" << c.tag << "\t" << c.data << endl;
		}
	}

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
	int C = S / L;//number of lines
	Cache.resize(C);
	int offset = log2(L);//disp
	int index = log2(C);//index
	int tag = 32 - index - offset;//tag
	//cout << tag;
	Memory = read_mem(file_name, Memory, offset, index, tag);
	direct_mapping(Memory, Cache, CC);
	return 0;
}