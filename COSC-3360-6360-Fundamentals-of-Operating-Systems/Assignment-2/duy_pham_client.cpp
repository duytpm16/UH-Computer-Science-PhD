// Second COSC 3360 assignment for Fall 2021
// Client part
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

using namespace std;

// Taken from http://www2.cs.uh.edu/~paris/3360/Sockets.html as noted in assignment2.pdf
int call_socket(char *hostname, unsigned short portnum)
{
	struct sockaddr_in sa;
	struct hostent *hp;
	int a, s;
    
	if ((hp = gethostbyname(hostname)) == NULL)
	{
		errno = ECONNREFUSED;
		return(-1);
	}

	memset(&sa, 0, sizeof(sa));
	memcpy((char*) &sa.sin_addr, hp->h_addr, hp->h_length);
	sa.sin_family = hp->h_addrtype;
	sa.sin_port = htons((u_short)portnum);

	if ((s=socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0)
	{
		return(-1);
	}
	if (connect(s, (struct sockaddr*)&sa, sizeof (sa)) < 0)
	{
		close(s);
		return(-1);
	}
	return(s);
}


int main(int argc, char* argv[])
{
	int b;
	int portNumber;
	std::string hostName;
	std::string testResult;
	std::string dob;
	
	
	// Prompt for the server host name and reject if the host name is not "localhost" according to assigment2.pdf
	cout << "Enter the server host name: ";
	cin  >> hostName;
	while (hostName != "localhost")
	{
		cout << "Invalid server host name. Enter the host name again." << endl;
		cout << "Enter the server host name: ";
		cin  >> hostName;
	}
	
	
	// Prompt for the port number and reject if the port number between 2000-65535 according to https://www.linuxhowtos.org/C_C++/socket.htm
	cout << "Enter the server port number: ";
	cin  >> portNumber;
	while (portNumber < 2000 || portNumber > 65535)
	{
		cout << "The server port number must be between 2000 and 65535. Enter the server port number again." << endl;
		cout << "Enter the port number: ";
		cin  >> portNumber;
	}
	
	
	// Connect socket
	char *hostn[hostName.size()+1];
	strcpy(*hostn, hostName.c_str());
	if ((b = call_socket(*hostn, portNumber)) < 0)
	{
		perror("Call Socket");
		exit(1);
	}
	
	
	// Prompt for test ID and a birthday
	cout << "Enter your test code: ";
	cin  >> testResult;
	
	cout << "Enter your birthday in MMDDYYYY format: ";
	cin  >> dob;
	cout << endl;
	std::string message = testResult + " " + dob;
	
	
	// Send message to the server. Code for write() was taken from https://www.linuxhowtos.org/C_C++/socket.htm
	char buf[256];
	strcpy(buf, message.c_str());
	int n = write(b, buf, strlen(buf));
	if (n < 0)
	{
		cout << "Error sending message to server." << endl;
		close(b);
		return(-1);
	}
	
	
	// Receive message from server. Code for read() was taken from https://www.linuxhowtos.org/C_C++/socket.htm
	bzero(buf, 256);
	n = read(b,buf,255);
	if (n < 0)
	{
		cout << "Error reading reply from server." << endl;
		close(b);
		return(-1);
	}
	
	
	// Output results based on the test result.
	switch(buf[0])
	{
		case 'N':
			cout << "Your test result was NEGATIVE." << endl << endl;
			break;

		case 'P':
			cout << "Your test result was POSITIVE." << endl << endl;
			break;
		
		case 'X':
			cout << "The test result you requested is not in our database." << endl << endl;
			break;

		default:
			cout << "Invalid test result." << endl << endl;
			break;

	}

	close(b);
	return 0;
}