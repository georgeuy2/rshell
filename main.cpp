#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <boost/tokenizer.hpp>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <vector>
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

class Commands{
	public:
	  Commands(){};
	  void commandP(string& com);	//for parsing command	
	  bool execforcommand(string& com); //for execute commmand
	  int connectP(string com, int andCheck, int orCheck); // for parsing connectors
};

void Commands::commandP(string& com)
{
	int hashfound = com.find("#");		// for comments

	char * temp;
	hashfound > -1 ? com.erase(hashfound): temp = strtok(&com[0], ";");
	while(temp)
	{
	  string temptemp(temp);
	  if(temptemp.find("exit") != string::npos)
	  {
		 exit(0);
	  }
	 // connectP(temptemp, 0, 0);
	  temp = strtok(0, ";");
	}
}

int Commands::connectP(string com, int andCheck, int orCheck) // for parsing connectors
{
	const char * locOr = strstr(com.c_str(), "||");
	const char * locAnd = strstr(com.c_str(), "&&");
	
	if(locOr < locAnd && locAnd && locOr)
	{
		locAnd = 0;
	}
	
	if(locOr != 0)
	{
		andCheck = connectP(com.substr(0, locOr - com.c_str()), orCheck, andCheck);
		andCheck != 1 ? connectP(locOr + 2, 0, 1) : connectP(locOr + 2, 0, 2);
	}
	else if (locAnd != 0)
	{
		orCheck = connectP(com.substr(0, locAnd - com.c_str()), orCheck, andCheck);
		orCheck != 1 ? connectP(locAnd + 2, 2, 0) : connectP(locAnd + 2, 1, 0);
	}
	else
	{
		if(orCheck == 2)
		{
			return 0;
		}
		if(andCheck == 2)
		{
			return 0;
		}
		// return execforcommand(com);
	}
	return 0;	
}

bool Commands::execforcommand(string& com)
{
	if(com.compare("exit") == 0)
	{
		exit(0);
	}
	
	int location = com.find("cd");

	if(location > -1)
	{
		string cdir;
		char * strings;
		string homebase;
		const char * args;
		char path[2048];
		string t = "";

		if(com.size() > 2)
		{
			t = com.substr(location + 3);
			args = t.c_str();
		}

		if(t.size() == 0)
		{
			char * directory = getenv("HOME");
			homebase = "";
			directory == NULL ? perror("Error in getenv()") : homebase = homebase + directory;
			char * check = (getcwd(path, 2057));

			if(check != '\0')
			{
				int checkcheck = setenv("OLDPWD", path, 1);
				if(checkcheck <= -1)
				{
					perror("Error in setenv()");
				}
			}
			else
			{
				perror("Error in getcwd()");
			}
			int tricheck = chdir(com.c_str());
			tricheck <= -1 ? perror("Error in chdir()") : return true;
		}
		else if(strcmp(args, "-") == 0)
		{
			cdit = "";
			string = getnv("OLDPWD");
		
			if(strings != '\0')
			{
				directory += strings;
			}
			char * check = getcwd(path, 2047);
			
			else
			{
				perror("Error in getenv()");
			}
			if(check != '\0');
			{
				int checkcheck = setenv("OLDPWD", path, 1);
			
				if(checkcheck <= -1)
				{
					perror("Error in setenv")l;
				}
			}
			else
			{
				perror("Error in getcwd()");
			}
			
			int tricheck = chdir(directory.c_str());
			tricheck <= -1 ? perror("Error in chdir()") : return true;
		}
		
		int pro1 = fork();	//Forking
		if(pro1 <= -1)
		{
			perror("Error in creating fork");
			return false;
		}
		if(signal(SIGINT, handle_SIGINT) == SIG_ERR)
		{
			perror("Error in signal");
		}
		else if(pro1 == 0)
		{
			char * arg[2048];
			getarg(com, arg);
			int res = execvp(*arg, arg);
			
			if(res <= -1)
			{
				perror("Error in executing command line 202");
				exit(1);
		}
		else
		{
			int childWait = waitpid(pro1, &childWait, 0);
			if(childWait <= -1)
			{
				perror("Waiting failed");
			}
			if(childWait == 0)
			{
				return true;
			}
		return false;
		}
		return false;
}
}
void handle_SIGINT(int signal)
{

}

int main(int argc, char * argv[])
{
	string line;

	Commands * c = new Commands();	
	while(1)
	 {
	  prompt();		//prints [USERNAME]@[HOSTNAME]$	
	  if(signal(SIGINT, handle_SIGINT) == SIG_ERR)
	  {
		perror("Error in signal");
	  }
	  getline(cin, line);	

	  if(line == "exit")	//EXIT program if user types "exit"
	    {
	      exit(0);
	    }
	    c->commandP(line);
	 }	
	return 0;
}

