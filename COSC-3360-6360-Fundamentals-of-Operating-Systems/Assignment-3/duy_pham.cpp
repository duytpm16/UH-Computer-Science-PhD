// Third COSC 3360 -6310 assignment for Fall 2021
// This program attempts to solve the Observation Platform using
//  POSIX threads and POSIX advanced synchronization feature.
// 	Outputs are printed to the console.
//
// Compile: g++ -fpermissive duy_pham.cpp -lpthread -o a3
// Run: ./a3
//
// Name: Duy Pham (2085253)
// Due: December 3rd, 2021

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

static int nLadder    = 0;
static int nVisitors  = 0;
static int nHadToWait = 0;
static int nVisitorsOnPlatform = 0;

static pthread_mutex_t mylock;
static pthread_cond_t ok = PTHREAD_COND_INITIALIZER;

// Struct to store value from input file.
struct visit
{
	std::string nickName;
	int arrivalTime   = 0;
	int platformTime  = 0;
	int climbUpTime   = 0;
	int climbDownTime = 0;
};

void climbUpLadder(string nickName, int climbUpTime)
{
	// Visitor has to wait until nVisitorOnPlatform < 3 and nLadder != 1. Otherwise climb the ladder onto the platform
	pthread_mutex_lock(&mylock);
		int i = 0;
		while ((nVisitorsOnPlatform == 3) || (nLadder == 1))
		{
			if (i == 0)
			{	
				if (nVisitorsOnPlatform == 3)
				{
					cout << "-- " << nickName << " has to wait." << endl;
					nHadToWait++;
				}
			}
			i++;
			pthread_cond_wait(&ok, &mylock);
		}


		cout << nickName << " climbs the ladder." << endl;
		nLadder++;
	pthread_cond_signal(&ok);
	pthread_mutex_unlock(&mylock);

	// Sleep the number of minutes it takes the visitor to go up the ladder
	sleep(climbUpTime);

	// Update values after visitor reached the platform.
	pthread_mutex_lock(&mylock);
		cout << nickName << " is on the platform." << endl;
		nLadder--;
		nVisitors++;
		nVisitorsOnPlatform++;
	pthread_cond_signal(&ok);
	pthread_mutex_unlock(&mylock);
}

void climbDownLadder(string nickName, int climbDownTime)
{
	// Visitor has to wait until the ladder is free to climb down. Otherwise climb down
	pthread_mutex_lock(&mylock);
		while (nLadder == 1)
		{
			pthread_cond_wait(&ok, &mylock);
		}

		cout << nickName << " goes down the ladder." << endl;
		nLadder++;
	pthread_cond_signal(&ok);
	pthread_mutex_unlock(&mylock);

	// Sleep the number of minutes it takes the visitor to goes down the ladder
	sleep(climbDownTime);

	// Update values after visitor reaches the bottom
	pthread_mutex_lock(&mylock);
		nVisitorsOnPlatform--;
		nLadder--;
	pthread_cond_signal(&ok);
	pthread_mutex_unlock(&mylock);

	cout << nickName << " leaves." << endl;
}

void *visitor(void *arg)
{
	struct visit vData = *((visit*)arg);

	// Climb up ladder
	cout << vData.nickName << " arrives at the platform." << endl;
	
	climbUpLadder(vData.nickName, vData.climbUpTime);

	// Sleep for number of minutes on platform
	sleep(vData.platformTime);

	// Climb down ladder
	cout << vData.nickName << " decides to leave the platform." << endl;
	climbDownLadder(vData.nickName, vData.climbDownTime);

	pthread_exit((void*) 0);
}

int main(int argc, char* argv[])
{
	// Prompt for file name
	std::string fileName;
	cout << "Enter the file name: ";
	cin  >> fileName;
	cout << endl;

	// Read input file
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open()) 
	{
		cerr << "\nERROR: Cannot open the file: " << fileName << "\n\n";
		exit(1);
	}
	
	// Read in data from file and store values to visit struct. Save each struct to vector
	std::string line;
	std::vector<pthread_t> children;
	std::vector<struct visit> visitVector;
	while (getline(file, line))
	{
		struct visit cData;
		
		std::istringstream iss(line);
		
		iss >> cData.nickName;
		iss >> cData.arrivalTime;
		iss >> cData.climbUpTime;
		iss >> cData.platformTime;
		iss >> cData.climbDownTime;

		visitVector.push_back(cData);
	}
	
	
	// Initialize pthread mutex
	pthread_mutex_init(&mylock, NULL);

	// Start thread for each visitor and store then into children vector
	for (size_t i = 0; i < visitVector.size(); i++)
	{
		sleep(visitVector[i].arrivalTime);
		
		pthread_t tid;
		pthread_create(&tid, NULL, visitor, (void *) &visitVector[i]);
		children.push_back(tid);
	}

	// Join threads
	for (size_t n = 0; n < children.size(); n++)
	{
		pthread_join(children[n], NULL);
	}


	// Print number of visitors and number of vistors that had to wait.
	cout << "\n" << nVisitors << " visitor(s) came on the platform." << endl; 
	cout << nHadToWait << " of them had to wait.\n" << endl; 

	return 0;
}