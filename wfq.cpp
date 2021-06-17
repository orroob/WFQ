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

/*

	File name : wfq.cpp
	Title : Assembler Source Code
	version: 3.1
	Created On : 13 / 06 / 2021
	Comments :
	Assumptions :	 The first run of the assembler giving the list of the labels ( with the PC address).
	Limitations : none
	Known Errors : none
	Developers : Or Roob and Ido Datner
	Notes :	The code is working as exepted - getting the files from the CMD and writing the instructoin memory (already checked)

*/


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
			(*p).arrivalTime = stoi(data[i]);
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

int addConnection(Connection c, Connection *cList, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (cList[i] == c)
			return 0;
	}
	cList[size] = c;
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

void sendPacket(vector <pck> *sendBuffer, vector <pck> *pq, Connection *cList, int packetsCount) 
{
	
	if ((*sendBuffer).size() == 1) 
	{
		if ((*pq).size() != 0)
		{
			(*sendBuffer)[0].sendingTime = max((*sendBuffer)[0].arrivalTime + (*sendBuffer)[0].len, (*pq)[packetsCount-1].sendingTime + (*sendBuffer)[0].len);
			(*pq).push_back((*sendBuffer).front());
			(*sendBuffer).pop_back();
		}
		else
		{
			(*sendBuffer)[0].sendingTime = (*sendBuffer)[0].arrivalTime + (*sendBuffer)[0].len;
			(*pq).push_back((*sendBuffer).front());
			(*sendBuffer).pop_back();
		}
		return;
	}

	//array holding the sending times of the packets acording to GPS algorithm
	double *finishTimes;
	finishTimes = (double*)malloc((*sendBuffer).size() * sizeof(double));
	
	//counter of the packets in the bus
	int counter=0;
	int tempEnding;
	double *weights;
	//weights = (double*)malloc((*sendBuffer).size() * sizeof(double));
	weights = (double*)calloc((*sendBuffer).size(), sizeof(double));
	for (int i = 0; i < (*sendBuffer).size(); i++)
	{
		finishTimes[i] = (*sendBuffer)[i].arrivalTime + (*sendBuffer)[i].len;
	}

	for (int i = 0; i < (*sendBuffer).size(); i++)
	{
		for (int k = 0; k <= i; k++)
		{
			if (finishTimes[k] > (*sendBuffer)[i].arrivalTime)
			{
				weights[i] += (*sendBuffer)[k].weight;
			}
		}
		finishTimes[i] = (*sendBuffer)[i].arrivalTime + (*sendBuffer)[i].len * weights[i];

		for (int j = 0; j < i; j++)
		{
			if (finishTimes[j] > (*sendBuffer)[i].arrivalTime)
			{
				finishTimes[j] = (*sendBuffer)[i].arrivalTime + (finishTimes[j] - (*sendBuffer)[i].arrivalTime) / weights[j] * weights[i] / (*sendBuffer)[i].weight;
			}
		}
	}

	int j = 0;

	for (int i = 0; i < (*sendBuffer).size(); i++)
	{
		if (finishTimes[i] < finishTimes[j])
		{
			j = i;
		}
		else
		{
			if (finishTimes[i] == finishTimes[j])
			{
				if (isFirst((*sendBuffer)[i], (*sendBuffer)[j], cList))
					j = i;
			}
		}
	}

	(*sendBuffer)[j].sendingTime = max((*sendBuffer)[j].arrivalTime + (*sendBuffer)[j].len, (*pq)[packetsCount-1].sendingTime + (*sendBuffer)[j].len);

	(*pq).push_back((*sendBuffer)[j]);
	(*sendBuffer).erase((*sendBuffer).begin() + j);
	
	free(finishTimes);
	free(weights);
	return;
}

void printQueue(vector <pck> pq)
{
	int sTime = pq.front().arrivalTime;

	while (pq.size() > 0) {
		if (sTime < pq.front().arrivalTime)
			sTime = pq.front().arrivalTime;
		cout << sTime << ": " << pq.front() << "\n";

		sTime += pq.front().len;
		pq.erase(pq.begin());
	}
}

int main()
{
	char fName[F_NAME_SIZE];
	cin >> fName;

	string line;
	ifstream f;
	f.open(fName);
	
	vector <pck> pq, sendBuffer, waitBuffer;
	Connection *connectionsList;
	connectionsList = (Connection*)malloc(INIT_SIZE * sizeof(Connection));
	int connectionsCounter = 0, sentPacketsCount = 0, endTime = 0;
	//pck p1;
	bool sendEnable = false;

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

		//if it's the first packet arriving, set endTime
		if (endTime == 0)
		{
			endTime = p1.arrivalTime + p1.len;
		}

		if (sentPacketsCount >= 207)
		{
			int a=0;
		}
		while (p1.arrivalTime > endTime && !sendBuffer.empty())
		{//packet has finished being sent, send new packet
			sendPacket(&sendBuffer, &pq, connectionsList, sentPacketsCount);
			sentPacketsCount++;
			endTime = pq.back().sendingTime;
		}

		//push the new packet to the buffer
		sendBuffer.push_back(p1);
		
	}

	//all packets have arrived
	//finish sending rest of the buffer
	while (sendBuffer.size() != 0)
	{
		sendPacket(&sendBuffer, &pq, connectionsList, sentPacketsCount);
		sentPacketsCount++;
	}

	//print the queue
	printQueue(pq);

	f.close();
	free(connectionsList);
	return 0;
}


//
		//if (p1.arrivalTime < endTime || sendBuffer.empty())
		//{
		//	sendBuffer.push_back(p1);
		//	continue;
		//}
		//
		//sendEnable = true;
		//
		//waitBuffer.push_back(p1);
		//
		//if (sendEnable)
		//{
		//	sendPacket(&sendBuffer, &pq, connectionsList, sentPacketsCount);
		//	sentPacketsCount++;
		//	endTime = pq.back().sendingTime;
		//	sendEnable = false;
		//}
		/*
		if(p1.arrivalTime >= endTime)
		{//packet has finished being sent, send new packet
			sendPacket(&sendBuffer, &pq, connectionsList, sentPacketsCount);
			sentPacketsCount++;
			endTime = pq.back().sendingTime;
		}

		//push the new packet to the buffer
		sendBuffer.push_back(p1);
		*/
