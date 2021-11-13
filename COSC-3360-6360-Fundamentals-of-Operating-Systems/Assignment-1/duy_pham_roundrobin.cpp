// Duy Pham - 2085253
// COSC 3360/6310 - Fundamentals of Operatin Systems
// ASSIGNMENT - 1: ROUND ROBIN SCHEDULING


#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <utility>
#include <algorithm>
#include <numeric>

using namespace std;


enum processStatus { ready = 0, running = 1, blocked = 2, terminated = 3, notStarted = 4};


//A struct containing the variables for the information of every process
struct processes 
{
	int startTime;
	int processNumber;
	int completionTime;
	int processID;
	queue<string> processType;
	queue<int> processTime;
	processStatus currentStatus;
};

void checkstatus(processes temp)
{
	cout << "Process " << temp.processID << " is ";
	switch(temp.currentStatus) {
		case 0:
			cout << "READY." << endl;
			break;
		case 1:
			cout << "RUNNING." << endl;
			break;
		case 2:
			cout << "BLOCKED." << endl;
			break;
		case 3:
			cout << "TERMINATED." << endl;
			break;
		default:
			NULL;
	}

}

//Global variables so I can access them from anywhere.
int cores;
int timeSlice;
int processIndex = -1;
int processDone = -1;


int main(int argc, char* argv[])
{
	//The variables, and the queue's/deque's i'm going to be using for reading in the input and for the round robin.
	string instruction;
	int time = 0;
	int currentTime = 0;	
	deque<processes> readyQueue;
	deque<processes> blockedQueue;
	deque<processes> terminatedQueue;
	queue<processes> disk;
	queue<processes> theProcesses;
	vector<processes> processor;
	vector<int> pn;
	vector<int> st;

	// Read in data
	if (cin.fail())
	{
		cout << "Input file failed." << endl;
	}

	// Read in NCORES AND SLICES
	cin >> instruction;
	if (instruction == "NCORES")
	{
		cin >> cores;
	}

	cin >> instruction;
	if (instruction == "SLICE")
	{
		cin >> timeSlice;
	}



	//Read in the rest of the input files and store the values
	while (!cin.eof()) 
	{
		cin >> instruction;
		cin >> time;
		if (instruction == "PROCESS") //initalizing all the new processes and setting their start time, current status, their process number, and their completion time.
		{
			// Save process ID
			int id = time;

			// Read in the START time
			cin >> instruction;
		    cin >> time;
			
			processIndex++;
			theProcesses.push(processes());
			theProcesses.back().startTime = time;
			theProcesses.back().completionTime = time;
			theProcesses.back().currentStatus = notStarted;
			theProcesses.back().processNumber = processIndex;
			theProcesses.back().processID = id;
			pn.push_back(processIndex);
			st.push_back(time);
		}
		else
		{
			theProcesses.back().processType.push(instruction);
			theProcesses.back().processTime.push(time);
		}
	}


	// Rearranging based on start time
	sort(st.begin(), st.end());
	queue<processes> theProcesses2 = theProcesses;
	queue<processes> theProcesses3;

	theProcesses2.push(theProcesses.front());
	int counter = 0;
	for (int j = 0; j < theProcesses.size(); j++) {
		queue<processes> theProcesses2 = theProcesses;
		while (!theProcesses2.empty()) {

			if (theProcesses2.front().startTime == st[j]) {
				theProcesses3.push(processes());
				theProcesses3.back().startTime = theProcesses2.front().startTime;
				theProcesses3.back().completionTime = theProcesses2.front().completionTime;
				theProcesses3.back().currentStatus = notStarted;
				theProcesses3.back().processNumber = theProcesses2.front().processNumber;
				theProcesses3.back().processID = theProcesses2.front().processID;
				theProcesses3.back().processType = theProcesses2.front().processType;
				theProcesses3.back().processTime = theProcesses2.front().processTime;
				theProcesses2.pop();
				counter++;
			}
			if (!theProcesses2.empty()) {
				theProcesses2.pop();
			}
		}
		if (counter == theProcesses.size()) {
			break;
		}
	}
	theProcesses = theProcesses3;
	
	
	int tmp = 0;
	// Loop until the process is completed
	while (processDone != processIndex)
	{
		while (!theProcesses.empty())
		{
			//Putting all the new processes in the right queue depending on the process type
			if (currentTime == theProcesses.front().startTime)
			{
				if (theProcesses.front().processType.front() == "CORE")
				{
					theProcesses.front().currentStatus = ready;
					readyQueue.push_back(theProcesses.front());
					theProcesses.pop();
					cout << "Process " << readyQueue.at(readyQueue.size()-1).processID << " starts at t = " << currentTime << endl;
					cout << "Current number of busy cores: " << processor.size() << endl;
					cout << "Core queue contains process(es): " << readyQueue.size() + processor.size() << endl;
					int numStatus = terminatedQueue.size() + processor.size() + readyQueue.size() + blockedQueue.size();
					{
					for (int k = 0; k < pn.size(); k++) {
							int tmp2 = pn[k];
							if (!terminatedQueue.empty())
							{
								for (int j = 0; j < terminatedQueue.size(); j++) {
									if (terminatedQueue.at(j).processNumber == tmp2) {
										checkstatus(terminatedQueue.at(j));
									}
								}
							}
							if (!processor.empty())
							{
								for (int j = 0; j < processor.size(); j++) {
									if (processor.at(j).processNumber == tmp2) {
										checkstatus(processor.at(j));
									}
								}
							}
							if (!readyQueue.empty())
							{
								for (int j = 0; j < readyQueue.size(); j++) {
									if (readyQueue.at(j).processNumber == tmp2) {
										checkstatus(readyQueue.at(j));
									}
								}
							}
							if (!blockedQueue.empty())
							{
								for (int j = 0; j < blockedQueue.size(); j++) {
									if (blockedQueue.at(j).processNumber == tmp2) {
										checkstatus(blockedQueue.at(j));
									}
								}
							}
						}
					}
					cout << endl;
				}
				else if (theProcesses.front().processType.front() == "OUTPUT" || theProcesses.front().processType.front() == "INPUT")
				{
					theProcesses.front().currentStatus = blocked;
					theProcesses.front().completionTime = currentTime + theProcesses.front().processTime.front();
					blockedQueue.push_back(theProcesses.front());
					theProcesses.pop();
				}
			}
			// Have the processes run anyway in case the current time does not match with the start time.
			else 
			{
				currentTime = min(currentTime, theProcesses.front().startTime);
				break;
			}
		}

		// While the ready queue is not empty and the processor is not full, change the status of the process and fill the processor, as well as updating the completion time, depending on the process time of the process.
		while (!readyQueue.empty() && processor.size() != cores)
		{
			readyQueue.front().currentStatus = running;
			if (readyQueue.front().processTime.front() > timeSlice)
			{
				readyQueue.front().completionTime = currentTime + timeSlice;
			}
			else
			{
				readyQueue.front().completionTime = currentTime + readyQueue.front().processTime.front();
			}
			processor.push_back(readyQueue.front());
			readyQueue.pop_front();
		}

		// When a process terminates, print the required messages
		if (!terminatedQueue.empty()) 
		{
			{

				int numStatus = terminatedQueue.size() + processor.size() + readyQueue.size() + blockedQueue.size();
				cout << "Process " << terminatedQueue.at(0).processID << " terminates at t = " << currentTime << endl;
				cout << "Current number of busy cores: " << processor.size() << endl;

				if (readyQueue.empty())
				{
					cout << "Ready queue is empty" << endl;
				}
				else
				{
					cout << "Current number in READY queue: " << readyQueue.size() << endl;
				}
				
				int popThis;
				{
					for (int k = 0; k < pn.size(); k++) {
						int tmp2 = pn[k];
						if (!processor.empty())
						{
							for (int j = 0; j < processor.size(); j++) {
								if (processor.at(j).processNumber == tmp2) {
									checkstatus(processor.at(j));
								}
							}
						}
						if (!readyQueue.empty())
						{
							for (int j = 0; j < readyQueue.size(); j++) {
								if (readyQueue.at(j).processNumber == tmp2) {
									checkstatus(readyQueue.at(j));
								}
							}
						}
						if (!blockedQueue.empty())
						{
							for (int j = 0; j < blockedQueue.size(); j++) {
								if (blockedQueue.at(j).processNumber == tmp2) {
									checkstatus(blockedQueue.at(j));
								}
							}
						}
						if (!terminatedQueue.empty())
						{
							for (int j = 0; j < terminatedQueue.size(); j++) {
								if (terminatedQueue.at(j).processNumber == tmp2) {
									checkstatus(terminatedQueue.at(j));
								}
							}
						}
					}
				}
			}
			cout << endl;
			terminatedQueue.pop_front();
			processDone++;
		}


		//Set current time to a large number in case it is smaller than the smallest completion time so the program doesn't get stuck.
		currentTime = 0x7FFFFFFF; 
		if (!theProcesses.empty())
		{
			currentTime = min(currentTime, theProcesses.front().completionTime);
		}
		if (!processor.empty())
		{
			for (int i = 0; i < processor.size(); i++)
			{
			currentTime = min(currentTime, processor.at(i).completionTime);	
			}
		}
		if (!blockedQueue.empty())
		{
			for (int i = 0; i < blockedQueue.size(); i++)
			{
			currentTime = min(currentTime, blockedQueue.at(i).completionTime);
			}
		}
	
		// Here we pop the processtime and move onto the next queue or terminated queue.
		if (!processor.empty())
		{
			for (int i = 0; i<processor.size(); i++)
			{
				if (processor.at(i).processTime.front() > timeSlice)
				{
					if (processor.at(i).completionTime == currentTime)
					{
						processor.at(i).processTime.front() -= timeSlice;
						processor.at(i).currentStatus = ready;
						readyQueue.push_back(processor.at(i));
						processor.erase(processor.begin() + i);
						i--;
					}
				}
				else
				{
					if (processor.at(i).completionTime == currentTime)
					{
						processor.at(i).processType.pop();
						processor.at(i).processTime.pop();
						if (processor.at(i).processType.empty())
						{
							processor.at(i).currentStatus = terminated;
							terminatedQueue.push_back(processor.at(i));
							processor.erase(processor.begin() + i);
							i--;
						}
						else if (processor.at(i).processType.front() == "INPUT" || processor.at(i).processType.front() == "OUTPUT")
						{
							processor.at(i).currentStatus = blocked;
							processor.at(i).completionTime = currentTime + processor.at(i).processTime.front();
							blockedQueue.push_back(processor.at(i));
							processor.erase(processor.begin() + i);
							i--;
						}
						else if (processor.at(i).processType.front() == "CORE")
						{
							processor.at(i).currentStatus = ready;
							readyQueue.push_back(processor.at(i));
							processor.erase(processor.begin() + i);
							i--;
						}
					}
				}
			}
		}

		// Update block queue if it's not empty
		if (!blockedQueue.empty()) 
		{
			for (int i = 0; i < blockedQueue.size(); i++)
			{
				if (blockedQueue.at(i).completionTime == currentTime)
				{	
						blockedQueue.at(i).processType.pop();
						blockedQueue.at(i).processTime.pop();
					if (blockedQueue.at(i).processType.empty())
					{
						blockedQueue.at(i).currentStatus = terminated;
						terminatedQueue.push_back(blockedQueue.at(i));
						blockedQueue.erase(blockedQueue.begin()+i);
						i--;
					}
					else if (blockedQueue.at(i).processType.front() == "CORE")
					{
						blockedQueue.at(i).currentStatus = ready;
						readyQueue.push_back(blockedQueue.at(i));
						blockedQueue.erase(blockedQueue.begin()+i);
						i--;
					}
					else if (blockedQueue.at(i).processType.front() == "OUTPUT" || blockedQueue.at(i).processType.front() == "INPUT")
					{
						blockedQueue.at(i).completionTime = currentTime + blockedQueue.at(i).processTime.front();
						blockedQueue.push_back(blockedQueue.at(i));
						blockedQueue.erase(blockedQueue.begin()+i);
						i--;
					}
				}
			}
		}
	}
	return 0;
}