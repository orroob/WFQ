#define _CRT_SECURE_NO_WARNINGS
#define F_NAME_SIZE 1024
#define INIT_SIZE 500

#include <queue>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "Package.h"

using namespace std;

void Split(string buffer, vector <string> *data)
{
	stringstream input_stringstream(buffer);
	string intermediate;

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

int addConnection(Connection c, Connection *cList, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (cList[i] == c)
			return 0;
	}
	cList[n] = c;
	return 1;
}

bool isFirst(Connection c1, Connection c2, Connection *cList)
{
	while (cList != NULL)
	{
		if (*cList == c1)
			return true;
		if (*cList == c2)
			return false;
		cList++;
	}
}

int sendPacket(vector <pck> *pcktBuffer, queue <pck> *pq, Connection *cList) {
	
	if ((*pcktBuffer).size() == 1) 
	{
		int to_return = (*pcktBuffer)[0].time + (*pcktBuffer)[0].len;
		(*pq).push((*pcktBuffer).front());
		(*pcktBuffer).pop_back();
		return to_return;
	}

	//array holding the sending times of the packets acording to GPS algorithm
	int *times;
	times = (int*)malloc((*pcktBuffer).size() * sizeof(int)); 
	
	//counter of the packets in the bus
	int counter=0;
	int tempEnding;

	for (int i = 0; i < (*pcktBuffer).size()-1; i++) 
	{
		if (counter == 0) 
		{
			times[i] = (*pcktBuffer)[i].time + (*pcktBuffer)[i].len;
		}
		counter += (*pcktBuffer)[i].weight;

		for (int j = 0; j < i; j++)
		{
			if (times[j] < (*pcktBuffer)[i].time)
			{
				times[i] = -1;
				break;
			}
			times[j] = (*pcktBuffer)[i].time + (times[j] - (*pcktBuffer)[i].time) / (double)((*pcktBuffer)[j].weight / counter);
		}
		if (counter == 0) counter = 1;
		if(times[i] != -1)
			times[i] = (*pcktBuffer)[i].time + (*pcktBuffer)[i].len  / (double)((*pcktBuffer)[i].weight / counter);
	}
	
	int j = 0;

	for (int i = 0; i < (*pcktBuffer).size(); i++)
	{
		if (times[i] == -1)
			break;
		if (times[i] < times[j])
		{
			j = i;
		}
		else
		{
			if (times[i] == times[j])
			{
				if (isFirst((*pcktBuffer)[i], (*pcktBuffer)[j], cList))
					j = i;
			}
		}
	}
	int to_return = (*pcktBuffer)[j].time + (*pcktBuffer)[0].len;

	(*pq).push((*pcktBuffer)[j]);
	(*pcktBuffer).erase((*pcktBuffer).begin() + j);
	
	free(times);
	return to_return;
}


int main()
{
	char fName[F_NAME_SIZE];
	cin >> fName;

	string line;
	ifstream f;
	f.open(fName);
	
	queue <pck> pq;
	vector <pck> pcktBuffer;
	Connection *connectionsList;
	connectionsList = (Connection*)malloc(INIT_SIZE * sizeof(Connection));
	int connectionsCounter = 0;
	int time = 0, endTime=0;

	while (getline(f, line))
	{
		//packet has arrived
		pck p1;
		Parse(line, &p1);

		//check and update if it's a new connection
		connectionsCounter += addConnection(p1, connectionsList, connectionsCounter);
		if (connectionsCounter % 500 == 0)
		{
			connectionsList = (Connection*)realloc(connectionsList, connectionsCounter * 2 * sizeof(Connection));
		}

		
		if (endTime == 0)
		{//first packet to arrive
			endTime = p1.time + p1.len;
		}
		

		if (p1.time >= endTime) 
		{//packet has finished being sent, send new packet

			endTime = sendPacket(&pcktBuffer, &pq, connectionsList);
		}

		//add the new packet to the buffer
		pcktBuffer.push_back(p1);
	}
	
	//finish sending rest of the buffer
	while(pcktBuffer.size() != 0) sendPacket(&pcktBuffer, &pq, connectionsList);

	int sTime = pq.front().time;

	//print the queue
	while (pq.size() > 0) {
		if (sTime < pq.front().time)
			sTime = pq.front().time;
		cout << sTime << ": " << pq.front() << "\n";
		
		sTime += pq.front().len;
		pq.pop();

	}

	f.close();
	free(connectionsList);
	return 0;
}
