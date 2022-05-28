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
	string check = ""; // "0" means instruction memory address  "1" means data memory address
	string tag = "";
	string offset = "";
	string line_idx = "";
	string data = "";
	int index;
	int address;
};

struct line
{
	string valid_bit = "0";
	string tag = "";
	string data = "";

};

struct Set
{
	int size;
	vector<line> S;
};

vector<block> read_mem(ifstream& file_name, vector<block>& Memory, int offset, int index, int tag, int numset, int choice)
{
	block B;
	if (!file_name.is_open())
		cout << "Can't open file!!" << endl;
	else
	{
		string address, text;
		cout << "File opend successfully!!" << endl;
		cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
		while (getline(file_name, text))
		{
			B.check = text.substr(0, 1);
			address = text.substr(2, 32);
			B.data = address;
			B.tag = address.substr(0, tag);
			B.line_idx = address.substr(tag, index);
			B.offset = address.substr(tag + index, offset);
			string temp = B.line_idx;
			bitset<32> b(temp);
			B.index = b.to_ulong();
			bitset<32> k(B.data);
			int addr = k.to_ulong();
			//cout <<endl<< addr << endl;
			int set = abs(addr) % numset;
			B.address = set;
			//cout << B.data << " " << B.address << endl;
			Memory.push_back(B);
		}
	}

	return Memory;
}



void direct_mapping(block temp, vector<block>Memory, vector<line>& Cache, double CC, double& hit, double& miss)
{
	
	
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
		double miss_rate = (miss / (miss + hit));
		double hit_rate = (hit / (miss + hit));
		double miss_penalty = 100;
		double AMAT = CC + miss_rate * 100;

		cout << "Total Hits: " << hit << endl << "Total Misses: " << miss << endl;
		cout << "Hit Time: " << CC << endl;
		cout << "Miss Rate: " << miss_rate << endl;
		cout << "Hit Rate: " << hit_rate << endl;
		cout << "Miss Penalty: " << miss_penalty << endl;
		cout << "AMAT Value: " << AMAT << endl << endl;
		cout << "(VB)\t(Tag)\t\t\t\t(Data)" << endl;
		for (int j = 0; j < Cache.size(); j++)
		{
			line c = Cache[j];
			cout << c.valid_bit << "\t" << c.tag << "\t" << c.data << endl;
		}
		cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	

}

void Set_Full(block temp ,vector<block>Memory, vector<Set>& Setcache, double CC, int numset, double&hit, double&miss)
{
	
		bool check = false;
		for (int m = 0; m < Setcache[temp.address].S.size(); m++)
		{
			if (Setcache[temp.address].S[m].valid_bit == "1")
			{
				if (Setcache[temp.address].S[m].tag == temp.tag)
				{
					hit++; check = true;
				}
			}
		}
		if (!check)
		{
			miss++;
			line l;
			l.data = temp.data;
			l.tag = temp.tag;
			l.valid_bit = "1";
			if (Setcache[temp.address].S.size() < Setcache[temp.address].size)
			{
				Setcache[temp.address].S.push_back(l);
			}
			else
			{
				int k = rand() % Setcache[temp.address].S.size();
				Setcache[temp.address].S[k] = l;
			}
		}

		double miss_rate = (miss / (miss + hit));
		double hit_rate = (hit / (miss + hit));
		double miss_penalty = 100;
		double AMAT = CC + miss_rate * 100;

		cout << "Total Hits: " << hit << endl << "Total Misses: " << miss << endl;
		cout << "Hit Time: " << CC << endl;
		cout << "Miss Rate: " << miss_rate << endl;
		cout << "Hit Rate: " << hit_rate << endl;
		cout << "Miss Penalty: " << miss_penalty << endl;
		cout << "AMAT Value: " << AMAT << endl << endl;
		cout << "(VB)\t(Tag)\t\t\t\t(Data)" << endl;
		for (int j = 0; j < Setcache.size(); j++)
		{
			if (Setcache[j].size != 1)
				cout << "Set" << j << endl;
			for (int k = 0; k < Setcache[j].S.size(); k++)
			{
				line c = Setcache[j].S[k];
				cout << c.valid_bit << "\t" << c.tag << "\t" << c.data << endl;
			}

		}
		cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	
}


int main()
{
	ifstream file_name("memory.txt");
	vector<block>Memory;
	vector<line> Cache_inst;
	vector <Set> Setcache_inst;
	vector<line> Cache_data;
	vector <Set> Setcache_data;
	int S, L, M = 1;
	double CC;
	double I_hit = 0, I_miss = 0;
	double D_hit = 0, D_miss = 0;
	int choice, offset, index, tag;
	cout << "The American University in Cairo" << endl << "Assembly Language - Memory Hierarchy Simulator" << endl << "Mariam Abdelaziz 900196082" << endl << "Salma Abdelhalim 900193718" << endl << "Abdelhalim Ali 900193539" << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	cout << "Choose Cache Organization: " << endl;
	cout << "1. Direct Mapping" << endl;
	cout << "2. Set Associative" << endl;
	cout << "3. Fully Associative" << endl;
	cout << "Enter your choice  ";
	cin >> choice;
	cout << "Please enter the following data for the program to operate:- " << endl;
	cout << "Total Cache Size (S): ";
	cin >> S;
	cout << "Cache Line Size (L): ";
	cin >> L;
	if (choice == 2)
	{
		cout << "Enter number of sets: ";
		cin >> M;
	}
	cout << "Cycles / Cache (CC): ";
	cin >> CC;
	int C = S / L;//number of lines
	cout << "Total Cache Lines (C): " << C << endl;
	cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
	if (choice == 1)
	{
		Cache_inst.resize(C);
		Cache_data.resize(C);
		offset = log2(L);//disp
		index = log2(C);//index
		tag = 32 - index - offset;//tag
	}

	if (choice == 2)
	{
		offset = log2(L);//disp
		index = log2(C / M);//index
		tag = 32 - index - offset;//tag
	}

	if (choice == 3)
	{
		M = 1;
		offset = log2(L);//disp
		index = 0;//index
		tag = 32 - offset;//tag
	}

	Setcache_inst.resize(M);
	for (int i = 0; i < Setcache_inst.size(); i++)
		Setcache_inst[i].size = C / M;

	Setcache_data.resize(M);
	for (int i = 0; i < Setcache_data.size(); i++)
		Setcache_data[i].size = C / M;

	//cout << tag;
	Memory = read_mem(file_name, Memory, offset, index, tag, M, choice);
	if (choice == 1)
	{
		for (int i = 0; i < 20; i++)
		{
			int idx = rand() % Memory.size();
			block temp = Memory[idx];
			cout << "Processor: " << temp.tag << " " << temp.index << " " << temp.data << endl;
			if (temp.check == "0")
			{
				cout << "Instruction Cache accessed" << endl;
				direct_mapping(temp, Memory, Cache_inst, CC, I_hit, I_miss);
			}
			else
			{
				cout << "Data Cache accessed" << endl;
				direct_mapping(temp, Memory, Cache_data, CC, D_hit, D_miss);
				
			}
		}
	}
	else
	{
		for (int i = 0; i < 20; i++)
		{
			int idx = rand() % Memory.size();
			block temp = Memory[idx];
			cout << "Processor: " << temp.tag << "   " << temp.address << "   " << temp.data << endl;
			if (temp.check == "0")
			{
				cout << "Instruction Cache accessed" << endl;
				Set_Full(temp, Memory, Setcache_inst, CC, M, I_hit, I_miss);
				
			}
			else
			{
				cout << "Data Cache accessed" << endl;
				Set_Full(temp, Memory, Setcache_data, CC, M, D_hit, D_miss);
			}
		}
	}
	return 0;
}