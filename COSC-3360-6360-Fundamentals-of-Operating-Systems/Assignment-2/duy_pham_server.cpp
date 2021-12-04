// Second COSC 3360 assignment for Fall 2021
// Server part
// Duy Pham (2085253)
// J.-F. Paris
// November 2021

#include <errno.h>       /* obligatory includes */
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

// Taken from http://www2.cs.uh.edu/~paris/3360/Sockets.html as noted in assignment2.pdf
int establish(unsigned short portnum)
{
	int MAXHOSTNAME = 9; //localhost

	char myname[MAXHOSTNAME+1];
	int s;
	struct sockaddr_in sa;
	struct hostent *hp;

	memset(&sa, 0, sizeof(struct sockaddr_in));
	gethostname(myname, MAXHOSTNAME);
	hp = gethostbyname("localhost"); //changed myname to localhost.
	if (hp == NULL)
	{
		return(-1);
	}
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons(portnum);
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		return(-1);
	}

	if (bind(s,(struct sockaddr*)&sa, sizeof(sa)) < 0)
	{
		close(s);
        return(-1); 
	}

	listen(s,3);
	return(s);
}

// Taken from http://www2.cs.uh.edu/~paris/3360/Sockets.html as noted in assignment2.pdf
int get_connection(int s)
{
	int t;
	if ((t = accept(s,NULL,NULL)) < 0)
	{
		return(-1);
	}
	return(t);
}



int main(int argc, char* argv[])
{
	int s, t;
	int portNumber;
	std::string inputFile;
	std::unordered_map<std::string, std::string> data;


	// Read in text file.
	std::ifstream file;
	cout << "Enter the file name: ";
	cin  >> inputFile;

	file.open(inputFile);
	string line;
	while(!file.eof())
	{
		getline(file, line);
		std::istringstream iss(line);
		std::string value;
		std::vector<std::string> values;
		while (getline(iss, value, ' ')) values.push_back(value);
		std::string key = values[0] + " " + values[1];
		data[key] = values[2];
	}


	// Prompt for the port number and reject if the port number is between 2000-65535 according to https://www.linuxhowtos.org/C_C++/socket.htm
	cout << "Enter the server port number: ";
	cin  >> portNumber;
	while (portNumber < 2000 || portNumber > 65535)
	{
		cout << "The server port number must be between 2000 and 65535. Enter the server port number again." << endl;
		cout << "Enter the port number: ";
		cin  >> portNumber;
	}
	
	// Taken from http://www2.cs.uh.edu/~paris/3360/Sockets.html as noted in assignment2.pdf
	if ((s = establish(portNumber)) < 0)
	{
		perror("Establish");
		exit(1);
	}
	else 
	{
		cout << "Server is established. Use Ctrl-C to terminate the server." << endl;
	}
	
	
	
	
	// Infinite loop. Control-C to break
	while(1)
	{
		// Taken from http://www2.cs.uh.edu/~paris/3360/Sockets.html as noted in assignment2.pdf
		if ((t = get_connection(s)) < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			perror("Accept");
			exit(1);
		}
		
		// Read message from client. Code for read() was taken from https://www.linuxhowtos.org/C_C++/socket.htm
		char buf[256];
		bzero(buf, 256);
		int n = read(t, buf, 255);
		if (n < 0)
		{
			cout << "Error reading message from client. Terminate the server." << endl;
		}
		else {
			
			std::string message(buf);
			
			// For debugging purposes
			std::vector<std::string> v;
			std::stringstream ss(message);
			std::string item;
			while (getline(ss, item, ' ')) 
			{
				v.push_back(item);
			}
			
			cout << endl << "Test code: " << v[0] << endl;
			cout << "Birthdate: " << v[1] << endl;
			if (data.find(message) != data.end()) {
				cout << "Test result: " << data[message] << endl;
			} 
			else 
			{
				cout << "Test result: " << "X" << endl;
			}
			
			// Send results back to client. Code for write() was taken from https://www.linuxhowtos.org/C_C++/socket.htm
			if (data.find(message) == data.end()) 
			{
				n = write(t, "X", 1);
				if (n < 0)
				{
					cout << "Error sending message back to client. Terminate the server." << endl;
				}
			}
			else 
			{
				string result = data[message];
				char testResult[result.size()+1];
				strcpy(testResult, result.c_str());
				n = write(t, testResult, strlen(testResult));
				if (n < 0)
				{
					cout << "Error sending message back to client. Terminate the server." << endl;
				}
			}
		}
	}

	close(s);
	close(t);
	return 0;
}