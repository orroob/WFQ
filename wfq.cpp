#define _CRT_SECURE_NO_WARNINGS
#define BUFF_SIZE 1024

#include <stdio.h>
#include <string.h>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

typedef class Package
{

public:
	char Sadd[15], Dadd[15];
	int time, Sport, Dport, len, weight-1;

public:
	Package() = default;
	void Set_time(int time) {this->time = time;}
	void Set_Sport(int Sport) { this->Sport = Sport; }
	void Set_Dport(int Dport) {this->Dport = Dport;}
	void Set_len(int len) { this->len = len; }
	void Set_weight(int weight) { this->weight = weight; }
	void Set_Sadd(char *Sadd) { strcpy(this->Sadd, Sadd); }
	void Set_Sport(char *Dadd) { strcpy(this->Dadd, Dadd); }

	int Get_time() { return this->time ; }
	int Get_Sport() { return this->Sport; }
	int Get_Dport() { return this->Dport; }
	int Get_len() { return this->len; }
	int Get_weight() { return this->weight; }
	char* Get_Sadd() { return this->Sadd; }
	char* Get_Dadd() { return this->Dadd; }

	friend ostream& operator<<(ostream& os, const Package& p);
}pck;

ostream& operator<<(ostream& os, const Package& p)
{
	os << p.time << ' ' << p.Sadd << ' ' << p.Sport << ' ' << p.Dadd << ' ' << p.Dport << ' ' << p.len << ' ' << p.weight;
	return os;
}

template<typename T>
void print_queue(T q) { // NB: pass by value so the print uses a copy
	while (!q.empty()) {
		std::cout << q.top() << '\n';
		q.pop();
	}
	std::cout << '\n';
}

void Split(string buffer, vector <string> *data)
{
	stringstream input_stringstream(buffer);
	string intermediate;


	// Tokenizing w.r.t. space ' '
	while (getline(input_stringstream, intermediate, ' '))
	{
		(*data).push_back(intermediate);
	}

}
void Parse(string buffer, pck *p)
{
	/*vector <string> data;

	Split(buffer, &data);

	for (int i = 0; i < data.size(); i++)
		cout << data[i] << '\n';
	*/
	
	char *temp, *new_buff = (char*)buffer.c_str();
	temp = strchr(new_buff, ' ');
	*temp = '\0';
	(*p).time = atoi(new_buff);

	new_buff = temp + 1;
	temp = strchr(new_buff, ' ');
	*temp = '\0';
	strcpy((*p).Sadd, new_buff);

	new_buff = temp + 1;
	temp = strchr(new_buff, ' ');
	*temp = '\0';
	(*p).Sport= atoi(new_buff);
	
	new_buff = temp + 1;
	temp = strchr(new_buff, ' ');
	*temp = '\0';
	strcpy((*p).Dadd, new_buff);

	new_buff = temp + 1;
	temp = strchr(new_buff, ' ');
	*temp = '\0';
	(*p).Dport = atoi(new_buff);

	new_buff = temp + 1;
	temp = strchr(new_buff, ' ');
	
	if (temp == NULL)
	{
		(*p).len = atoi(new_buff);
		return;
	}

	*temp = '\0';
	(*p).len = atoi(new_buff);
	
	new_buff = temp + 1;
	(*p).weight = atoi(new_buff);
	
	return;
}

int main()
{
	//std::priority_queue<pck> q;

//	char buffer[BUFF_SIZE];
	char buffer[BUFF_SIZE];
	cin >> buffer;

	string line;

	ifstream f;
	f.open(buffer);
	
	cout << "Reading from the file" << endl;
	//f >> line;
	getline(f, line);

	char a[200] = { 0 }, b[20] = { 0 };
	strcpy(a, "10 1.1.1.1 2 2.2.2.2 4 10 2");
	
	// Using lambda to compare elements.
	auto cmp = [](pck left, pck right) { return left.Get_weight() > right.Get_weight(); };
	std::priority_queue<pck, std::vector<pck>, decltype(cmp)> pq(cmp);

	pck p1;
	//void(*pck_methods[5])(int); //pck_methods is an array that holds pointers to functions
	//pck_methods[0] = &(p1.Set_time);
	//pck_methods[1] = &(p1.Set_len);

	Parse(line, &p1);
	pq.push(p1);	
	
	/*for (pck n : data)
		q3.push(n);
	print_queue(q3);
	*/
	return 0;
}
