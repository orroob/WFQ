#define _CRT_SECURE_NO_WARNINGS
#define BUFF_SIZE 1024

#include <stdio.h>
#include <string.h>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

typedef class Package
{

public:
	char Sadd[16], Dadd[16];
	int time, Sport, Dport, len, end_t;
	double weight = 1;
	
public:
	Package() = default;

	friend bool operator==(const Package& p1, const Package& p2);
	friend ostream& operator<<(ostream& os, const Package& p);
}pck;

ostream& operator<<(ostream& os, const Package& p)
{
	if (p.weight != 1)
		os << p.time << ' ' << p.Sadd << ' ' << p.Sport << ' ' << p.Dadd << ' ' << p.Dport << ' ' << p.len << ' ' << fixed << setprecision(2) << p.weight;
	else
		os << p.time << ' ' << p.Sadd << ' ' << p.Sport << ' ' << p.Dadd << ' ' << p.Dport << ' ' << p.len;
	return os;
}

bool operator==(const Package& p1, const Package& p2)
{
	return (!strcmp(p1.Sadd, p2.Sadd) && !strcmp(p1.Dadd, p2.Dadd) && p1.Sport == p1.Sport && p1.Dport == p2.Dport);
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
	vector <string> data;

	Split(buffer, &data);

	for (int i = 0; i < data.size(); i++)
	{
		switch (i)
		{
		case 0:
			(*p).time = stoi(data[i]);
			break;
		case 1:
			strcpy((*p).Sadd, data[i].c_str());
			break;
		case 2:
			(*p).Sport = stoi(data[i]);
			break;
		case 3:
			strcpy((*p).Dadd, data[i].c_str());
			break;
		case 4:
			(*p).Dport = stoi(data[i]);
			break;
		case 5:
			(*p).len = stoi(data[i]);
			break;
		case 6:
			(*p).weight = stod(data[i]);
		default:
			break;
		}
	}
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
			times[j] = (*pcktList)[i].time + (times[j] - (*pcktList)[i].time) / (double)((*pcktList)[j].weight / counter);
		}
		if (counter == 0) counter = 1;
		times[i] = (*pcktList)[i].time + (*pcktList)[i].len  / (double)((*pcktList)[i].weight / counter);
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
	char buffer[BUFF_SIZE];
	cin >> buffer;

	string line;
	ifstream f;
	f.open(buffer);
	
	queue <pck> pq;
	vector <pck> pcktList;

	int time = 0, endTime=0;

	while (getline(f, line)) {
		pck p1;
		Parse(line, &p1);

		if (endTime == 0)
			endTime = p1.time + p1.len;
		
		if (p1.time > endTime) {//sending packet
			
			endTime = sendPacket(&pcktList, &pq);
		}
		pcktList.push_back(p1);
	}

	//finish sending rest of the buffer
	while(pcktList.size() != 0) sendPacket(&pcktList, &pq);

	int sTime = pq.front().time;

	//print the queue
	while (pq.size() > 0) {
		if (sTime < pq.front().time)
			sTime = pq.front().time;
		cout << sTime << ": " << pq.front() << "\n";
		
		sTime += pq.front().len;
		pq.pop();

	}
	return 0;
}
