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
	int time, Sport, Dport, len, weight=1, end_t;
	float sentPck = 0;
	bool sent = false;

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
	if (p.weight != 1)
		os << p.time << ' ' << p.Sadd << ' ' << p.Sport << ' ' << p.Dadd << ' ' << p.Dport << ' ' << p.len << ' ' << p.weight;
	else
		os << p.time << ' ' << p.Sadd << ' ' << p.Sport << ' ' << p.Dadd << ' ' << p.Dport << ' ' << p.len;
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

int sendPacket(vector <pck> *pcktList, queue <pck> *pq) {
	if ((*pcktList).size() == 1) 
	{
		int to_return = (*pcktList)[0].time + (*pcktList)[0].len;
		(*pq).push((*pcktList).front());
		(*pcktList).pop_back();
		return to_return;
	}

	//array holding the sending times of the packets acording to GPS algorithm
	int *times;
	times = (int*)malloc((*pcktList).size() * sizeof(int)); 
	
	//counter of the packets in the bus
	int counter=0;
	int tempEnding;

	for (int i = 0; i < (*pcktList).size()-1; i++) 
	{
		if (counter == 0) 
		{
			times[i] = (*pcktList)[i].time + (*pcktList)[i].len;
		}
		counter += (*pcktList)[i].weight;

		for (int j = 0; j < i; j++)
		{
			if (times[j] < (*pcktList)[i].time)
			{
				counter = -(*pcktList)[j].weight;
				continue;
			}
			times[j] = (*pcktList)[i].time + (times[j] - (*pcktList)[i].time) / (float)((*pcktList)[j].weight / counter);
		}
		if (counter == 0) counter = 1;
		times[i] = (*pcktList)[i].time + (*pcktList)[i].len  / (float)((*pcktList)[i].weight / counter);
	}
	
	int j = 0;

	for (int i = 0; i < (*pcktList).size(); i++)
	{
		if (times[i] < times[j])
		{
			j = i;
		}
	}
	int to_return = (*pcktList)[j].time + (*pcktList)[0].len;

	(*pq).push((*pcktList)[j]);
	(*pcktList).erase((*pcktList).begin() + j);
	
	free(times);
	return to_return;
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

	char a[200] = { 0 }, b[20] = { 0 };
	strcpy(a, "10 1.1.1.1 2 2.2.2.2 4 10 2");
	
	queue <pck> pq;

	//void(*pck_methods[5])(int); //pck_methods is an array that holds pointers to functions
	//pck_methods[0] = &(p1.Set_time);
	//pck_methods[1] = &(p1.Set_len);

	
	/*for (pck n : data)
		q3.push(n);
	print_queue(q3);
	*/
	vector <pck> pcktList;

	int time = 0, endTime=0;

	while (getline(f, line)) {
		pck p1;
		Parse(line, &p1);

		if (endTime == 0)
			endTime = p1.time + p1.len;
		//pq.push(p1);
		
		if (p1.time > endTime) {//sending packet
			
			endTime = sendPacket(&pcktList, &pq);
			//continue;
		}

		pcktList.push_back(p1);
	}

	while(pcktList.size() != 0) sendPacket(&pcktList, &pq);

	int sTime = pq.front().time;

	while (pq.size() > 0) {
		if (sTime < pq.front().time)
			sTime = pq.front().time;
		cout << sTime << ": " << pq.front() << "\n";
		
		sTime += pq.front().len;
		pq.pop();

	}
	return 0;
}
