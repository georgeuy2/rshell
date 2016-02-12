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
	gethostname(hostname, 1023) < 0 ? cout << "@host" : cout << "@" << hostname << " ";
	
	char path[1024];
	char* checkPath = getcwd(path, 1023);
	if(checkPath == NULL)
	{
		perror("Error in getcwd");
  	}
	cout << path;
	cout << "$ ";
}

class Commands{
	public:
	  Commands(){};
	  void commandP(string& com);	//for parsing command	
	  bool execforcommand(string& com); //for execute commmand
	  int connectP(string com, int andCheck, int orCheck); // for parsing connectors
	  void getarg (string& com, char* arrayargs[]);
	  static void sigHandle(int signal){}
};

void Commands::commandP(string& com)
{
	int hashfound = com.find("#");		// for comments
	char * temp;

	if(hashfound > -1)
	{
			 com.erase(hashfound);
	}
	temp = strtok(&com[0], ";");
	while(temp)
	{
	  string temptemp(temp);
	  if(temptemp.find("exit") != string::npos)
	  {
		 exit(0);
	  }
	  connectP(temptemp, 0, 0);
	  temp = strtok(0, ";");
	}
}

int Commands::connectP(string com, int andCheck, int orCheck) // for parsing connectors
{
	const char * locOr = strstr(com.c_str(), "&&");
	const char * locAnd = strstr(com.c_str(), "||");
	
	if(locOr < locAnd && locAnd && locOr)
	{
		//cout << "Test 0" << locAnd << " " << locOr << endl;
		locAnd = 0;
	}
	
	if(locAnd != 0) //when user types ||
	{
		// cout << "Test 1" << locAnd << " " locOr << endl;
		orCheck = connectP(com.substr(0, locAnd - com.c_str()), orCheck, andCheck); //command succeeded
		orCheck != 1 ? connectP(locAnd + 2, 2, 0) : connectP(locAnd + 2, 1, 0);
	}
	else if(locOr != 0) //when user types &&
	{
		//cout << "Test 2" << locAnd << " " << locOr << endl;
		andCheck = connectP(com.substr(0, locOr - com.c_str()), orCheck, andCheck);
		andCheck != 1 ? connectP(locOr + 2, 0, 1) : connectP(locOr + 2, 0, 2);
	}
	else
	{
		// cout << "Test 3" << locAnd << " " << locOr << endl; 
		if(orCheck == 2)
		{
			//cout << "Test d" << locAnd << " " << locOr << endl;
			return 0;
		}
		if(andCheck == 2)
		{
			//cout << "Test e " << locAnd << " " << locAnd << endl;
			return 0;
		}
		return execforcommand(com);
	}
	return 0;	
}

bool Commands::execforcommand(string& com)
{
	if (com.compare("exit") == 0) //check for exit	
	{
		exit(0);
	}
	int location = com.find("cd");
	
	if (location > -1 ) //thats a negatory for cd	
	{
		string cdir;
		char* strings;
		char path[2048];
		const char* args;
		string homebase;
		string temps = "";
		if (com.size() > 2) //so not ls, cd, etc
		{
			temps = com.substr(location + 3);
			args = temps.c_str();
		}
		if (temps.size() == 0) //could be ls	
		{
			char * cdir = getenv("HOME");
			homebase = "";
			if(cdir != NULL)
			{
				homebase += cdir;
			}
			else	
			{
			    perror("error in getenv");
			}
			char * check = getcwd(path, 2047);
			if(check != '\0')	
			{
				int checkcheck = setenv("OLDPWD", path, 1);
				if (checkcheck <= -1)	
				{
					perror("error in setenv");
				}
			}
			else 
			{
				perror("Error in getcwd()");
			}		
			int tricheck = chdir(homebase.c_str());
			if(tricheck <= -1)	
			{
				perror("error in chdir");
			}
			return true;
		}
		else if (strcmp(args, "-") == 0) //checking args
		{
			cdir = "";
			strings = getenv("OLDPWD");
			if (strings != '\0')
			{
				cdir += strings;
			}
			else
			{
				perror("error in getenv");
			}
			char* check = getcwd(path, 2047);
			if(check != '\0')	
			{
				int checkcheck = setenv("OLDPWD", path, 1);
				if (checkcheck <= -1)	
				{
					perror("error in setenv");
				}
			}
			else 
			{
				perror("Error in getcwd()");
			}		
			int tricheck = chdir(cdir.c_str());
			if (tricheck <= -1)	
			{
				perror("error in chdir");
			}
			return true;
		
		}

		else if (strcmp(args, "~") == 0)	
		{
			char* cdir = getenv("HOME");
			homebase = "";
			if (cdir != NULL)	
			{
				homebase += cdir;
			}
			else 
			{
				perror("error in getenv");
			}
			char* check = getcwd(path, 2047);
			if (check != '\0')	
			{
				int checkcheck = setenv("OLDPWD", path, 1);
				if (checkcheck <= -1)	
				{
					perror("error in setenv");
				}
			}
			else 
			{
		        perror("Error in getcwd()");
			}		
			int tricheck = chdir(homebase.c_str());
			if(tricheck <= -1)	
			{
				perror("error in chdir");
			}
			return true;
	
		}
		char* check = getcwd(path, 2047);
		if (check != '\0')	
		{
			int checkcheck = setenv("OLDPWD", path, 1);
			if (checkcheck <= -1) 
			{
				perror("error in setenv");
			}
		}
		else 
		{
			perror("Error in getcwd()");
		}
		int tricheck = chdir(args); //problems with args
		if (tricheck <= -1) 
		{
			perror("error in chdir");
		}
		
		return true;
	}
	int pro = fork(); //time to fork
	if (pro <= -1)	//-1 is error
	{
		perror("ERROR IN CREATING FORK");
		return false;
	}
	else if (pro == 0) //in child process	
	{
		char * arg[2048];
		getarg(com, arg);

		int res = execvp(*arg, arg);
		if (res <= -1)	
		{
			perror("ERROR IN EXECUTING COMMAND LINE 202");
			exit(1);
		}
	}
	else //in parent process
	{
		int childWait = waitpid(pro, &childWait, 0);
		if (childWait <= -1)
		{
			perror("WAITING FOR CHILD FAILED");
		}
		if(childWait == 0)
		{
			return true;
		}
	}
	return false;
}

void Commands::getarg(string& com, char* arrayargs[])
{
    char * WOwhitespace = strtok(&com[0], " \t"); //removes whitespace
    vector<string> temps;
    while (WOwhitespace)
    {
        temps.push_back(WOwhitespace); //pushing back without whitespace
        WOwhitespace = strtok(0, " \t");
    }
    for (unsigned i = 0; i < temps.size(); ++i)
    {
        string& toswap = temps.at(i);
        arrayargs[i] = &toswap[0];
    }
    arrayargs[temps.size()] = '\0';
}

int main(int argc, char * argv[])
{
	string commLine;
	Commands * c = new Commands();	
	while(1) //wamt jos tp lee[ going because exit will end it here or in functions
	 {
	  prompt();		//prints [USERNAME]@[HOSTNAME]$	
	  getline(cin, commLine);	

	  if(commLine == "exit")	//EXIT program if user types "exit"
	    {
	      exit(0);
	    }
	    c->commandP(commLine);
	 }	
	return 0;
}

