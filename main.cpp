#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

//Prompt which will print [USERNAME]@[HOSTNAME]$
void prompt()
{
	string username = getlogin();		
	char hostname[1024];		//Created a 1024 elemetn hostname
	hostname[1023] = '\0';		
	
	//Checks if getlogin() returns 0 when user is NULL else prints USERNAME
	getlogin() == 0 ? cout << "user" : cout << username;
	
	//Checks if gethosname() returns -1 when host is NULL else prints HOSTNAME	
	gethostname(hostname, 1023) < 0 ? cout << "@host" : cout << "@" << hostname;
	cout << "$ ";
}

int main()
{
	string line;
	
	while(1)
	 {
	  prompt();		//prints [USERNAME]@[HOSTNAME]$	
	  getline(cin, line);	
	  
	  if(line == "exit")	//EXIT program if user types "exit"
	    {
	      exit(0);
	    }
	 }	
}

