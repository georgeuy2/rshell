#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <boost/tokenizer.hpp>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <string>
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
	char * check = strrchr(getcwd(path, 1023), '/');
	if(check == NULL)
	{
		perror("Error in getcwd");
  	}
	if (check)
	{
			++check; //remove /
	}
	else 
	{
			check = path;
	}
	cout << check;
	cout << "$ ";
}

class Commands{
	public:
	  Commands(){};
	  void commandP(string& com);	//for parsing command	
	  bool execforcommand(string& com); //for execute commmand
	  int connectP(string com, int andCheck, int orCheck); // for parsing connectors
	  void getarg (string& com, char* arrayargs[]);
	  void starttest(string& com);
	  void testP(string& com);  //for parsing test
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

//BEGIN NEW STUFF
void Commands::testP(string& com)
{
    int rmtest = com.find("test");
    string t = "test";
    if (rmtest != string::npos)     //remove test
    {
	 cout << "Test works" << endl;    
   	 com.erase(rmtest, t.length());
    }
    int rmbracket1 = com.find("[");
    string first = "[";
    if (rmbracket1 != string::npos)  //remove brackets
    {
	cout << "test with [ ] " << endl;
        com.erase(rmbracket1, first.length());
        int rmbracket2 = com.find("]");
        string second = "]";
        if (rmbracket2 != string::npos)
        {
            com.erase(rmbracket2, second.length());
        }
        else 
        {
            perror("Error: command not found."); //missing second bracket
        }
    }
    starttest(com);
    
}
void Commands::starttest(string& com)
{
    int finde = com.find("-e");
    string e = "-e";
    int findf = com.find("-f");
    string f = "-f";
    int findd = com.find("-d");
    string d = "-d";
    bool flage = false;
    bool flagf = false;
    bool flagd = false;
    if (finde != string::npos)     //remove test
    {
        com.erase(finde, e.length());
        flage = true;
    }
    else if (findf != string::npos)
    {
    	com.erase(findf, f.length());
    	flagf = true;
    }
    else if (findd != string::npos)
    {
    	com.erase(findd, d.length());
    	flagd = true;
    }
    else 
    {
    	flage = true;
    }
    if ((flage && flagf) || (flage && flagd) || (flagf && flagd))
    {
    	perror("Too many flags");
    }
    if (flage)
    {
    	//do the stuff for flag e
    	
    }
    else if (flagf)
    {
    	//do the stuff for flad f
    	
    }
    else if(flagd)
    {
    	//do the stuff for flag d
    	
    }
    else 
    {
    	perror("flag not recognized");
    }
    
    // do we need to get rid of whitespace now?
    //now we need to figure out how to use that stat() and the other stuff I mentioned
}

//END NEW STUFF

int Commands::connectP(string com, int andCheck, int orCheck) // for parsing connectors, 0 for fail, 1 for success, 2 for skip
{
	const char * locOr = strstr(com.c_str(), "||");
	const char * locAnd = strstr(com.c_str(), "&&");
	
	if(locOr > locAnd && locAnd && locOr)
	{
		//cout << "Test 0" << locAnd << " " << locOr << endl;
		locOr = 0;
	}
	
	if(locOr != 0) //when user types ||
	{
		// cout << "Test 1" << locAnd << " " locOr << endl;
		orCheck = connectP(com.substr(0, locOr - com.c_str()), orCheck++, andCheck);
		//command succeeded
		if (orCheck != 1)
		{
				connectP(locOr + 2, 2, 0);
		}
		//else if (orCheck != 1 && andCheck == 0)
		//{
		//		connectP(locOr + 2, 1, 1);
		//}
		else //Command failed
		{
				connectP(locOr + 2, 1, 0);
		}
	}
	else if(locAnd != 0) //when user types &&
	{
		//cout << "Test 2" << locAnd << " " << locOr << endl;
		andCheck = connectP(com.substr(0, locAnd - com.c_str()), orCheck, andCheck);
		andCheck != 1 ? connectP(locAnd + 2, 0, 1) : connectP(locAnd + 2, 0, 2);
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
	
	if (location > -1 ) //thats a not a negatory for cd	
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

	  size_t foundTest = commLine.find("test");	//this will check "test" in the string
	  size_t foundFirstBracket = commLine.find("[");	//this will check "[" in the string
	  size_t foundSecBracket = commLine.find("]");	// this will check "]" in the string 
	  // check if the string has "test" or "[]" in the string	
 	  if(foundTest != string::npos || (foundFirstBracket != string::npos && foundSecBracket != string:: npos))
	  {
	//	cout << "test has been passed" << endl;
		c->testP(commLine);
	  }
	
	  if(commLine == "exit")	//EXIT program if user types "exit"
	  {
	      exit(0);
	  }
	  
	  c->commandP(commLine);
	
	 }	
	return 0;
}
