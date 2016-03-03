#include <iostream>
using namespace std;

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <boost/tokenizer.hpp>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <cstring>
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
	  bool starttest(string& com);
	  bool testP(string& com);  //for parsing test
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
//

bool Commands::testP(string& com)
{
    int rmtest = com.find("test");
    string t = "test";
    if (rmtest != string::npos)     //remove test
    {
        //	 cout<< "WORD: " << com << endl;
        //	 cout << "Test works" << endl;    
   	 com.erase(rmtest, t.length());
        //  cout<< "WORD: " << com << endl;
    }
    int rmbracket1 = com.find("[");
    string first = "[";
    if (rmbracket1 != string::npos)  //remove brackets
    {
        //	cout <<"CHECKING BRACKETS: " << com << endl;
        //	cout << "test with [  " << endl;
        com.erase(rmbracket1, first.length());
        //	cout <<"CHECKING BRACKETS: " << com << endl;

	    int rmbracket2 = com.find("]");
        string second = "]";
        if (rmbracket2 != string::npos)
        {
            com.erase(rmbracket2, second.length());
        }
        else 
        {
            perror("Error: command not found."); //missing second bracket
            return false;
        }
        //	cout <<"CHECKING second BRACKETS: " << com << endl;

    }
    return starttest(com);
    
}
bool Commands::starttest(string& com)
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
    struct stat sb;
	
    if (finde != string::npos)     //remove -e and set flage true
    {
        com.erase(finde, e.length());
	    flage = true;
    }
    else if (findf != string::npos) //remove -f and set flagf true
    {
	    com.erase(findf, f.length());
	    flagf = true;
    }
    else if (findd != string::npos) //remove -d and set flagd true
    {
   	    com.erase(findd, d.length());
	    flagd = true;
    }
    else			//if no flags then -e is set by default 
    {
	    flage = true;
//#include "boost/filesystem.hpp"
    }
    if ((flage && flagf) || (flage && flagd) || (flagf && flagd)) //gives error if too many flags
    {
    	perror("Too many flags");
    	return false;
    }
    
    char* c = new char[com.length() + 1];
    strcpy(c, com.c_str());
    c = strtok(&com[0], " \t");
    if (flage)
    {
	    if (stat(c,&sb) == 0 )
	    {
	        cout << "(True)" << endl; 
	        return true; 
	    }
	    else {
	        cout << "(False)" << endl; 
	        return false;
	    }
    	//do the stuff for flag e
    }
    else if (flagf)
    {
	    if (stat(c, &sb) == 0 && S_ISREG(sb.st_mode)) 
	    {
	        cout << "(True)" << endl; 
	        return true;
	    }
	    else 
	    {
	        cout << "(False)" << endl; 
	        return false;   
	    }
	    //do the stuff for flad f
    	
    }
    else if(flagd)
    {
	    if (stat(c, &sb) == 0 && S_ISDIR(sb.st_mode)) 
	    {
	        cout << "(True)" << endl; 
	        return true; 
	    }
	    else 
	    {
	        cout << "(False)" << endl; 
	        return false;
	    }
	    //do the stuff for flag d
    }
    else 
    {
    	perror("flag not recognized"); //i dont think we need this
    	return false;
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
//to be inserted in main
int main() {
	
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
		c->testP(commLine);
	  }
	  else if(commLine == "exit")	//EXIT program if user types "exit"
	  {
	      exit(0);
	  }
	  else
	  {
	      c->commandP(commLine);
	  }
	 }	
    // Commands * c = new Commands();
    // string commLine;
    // vector <string> com;
    // vector <bool> returnv;
    
    // while(1)
    // {
    //     prompt();
    //     getline(cin, commLine);
    //     while(commLine != "") //while not empty string
    //     {
    //         int endP = commLine.find(")");
    //         if (commLine.find(")") != string::npos)
    //         {
    //             if (endP > commLine.find("||") && endP > commLine.find("&&") && endP > commLine.find(";"))
    //             {
    //                 if (commLine.find("(") != string::npos)
    //                 {
    //                     int beginP = commLine.find("(");
    //                     com.push_back(commLine.substr(beginP, endP)); //so that it gets rid of the parentheses
    //                     commLine.erase(beginP, endP); //erase that second from the string
    //                 }
    //                 else 
    //                 {
    //                     perror("Invalid command.");
    //                 }
    //             }
    //             else 
    //             {
    //                 int beginP = commLine.find("(");
    //                 if (beginP != string::npos)
    //                 {
    //                     int findAND = commLine.find("&");
    //                     int findOR = commLine.find("|");
    //                     int findSEMI = commLine.find(";");
    //                     if (findAND != string::npos && findAND - 3 <= 0)
    //                     {
    //                         com.push_back("&&");
    //                         commLine.erase(0, 2);
    //                     }
    //                     else if (findOR != string::npos && findOR - 3 <= 0)
    //                     {
    //                         com.push_back("||");
    //                         commLine.erase(0, 2);
    //                     }
    //                     else if (findSEMI != string::npos && findSEMI - 3 <= 0)
    //                     {
    //                         com.push_back(";");
    //                         commLine.erase(0, 2);
    //                     }
    //                     com.push_back(commLine.substr(0, beginP - 1));
    //                     commLine.erase(0, beginP - 1);
    //                 }
    //                 else 
    //                 {
    //                     perror("Invalid command");
    //                 }
                    
    //             }
    //         }
    //         else
    //         {
    //             size_t foundTest = commLine.find("test");	//this will check "test" in the string
	   //         size_t foundFirstBracket = commLine.find("[");	//this will check "[" in the string
	   //         size_t foundSecBracket = commLine.find("]");	// this will check "]" in the string 
	   //         // check if the string has "test" or "[]" in the string	
 	  //          if(foundTest != string::npos || (foundFirstBracket != string::npos && foundSecBracket != string:: npos))
	   //         {
		  //          c->testP(commLine);
		  //          commLine.erase(0, commLine.size() - 1);
	   //         }
	   //         else if(commLine == "exit")	//EXIT program if user types "exit"
	   //         {
	   //             exit(0);
	   //         }
	   //         else
	   //         {
	   //              c->commandP(commLine);
	   //              commLine.erase(0, commLine.size() - 1);
	   //         }
	   //        //okay so pretty much, this else is just to run it like normal
	   //        //so using the old main, i'm just commenting it out for testing
    //         }
    //     }
    //     for (int i = 0; i < com.size(); ++i)
    //     {
    //         if (com.at(i) == "&&" || com.at(i) == ";")
    //         {
    //             if(i > 0)
    //             {
    //                 size_t foundTest = commLine.find("test");	//this will check "test" in the string
	   //             size_t foundFirstBracket = commLine.find("[");	//this will check "[" in the string
	   //             size_t foundSecBracket = commLine.find("]");	// this will check "]" in the string
                    
    //                 if(foundTest != string::npos || (foundFirstBracket != string::npos && foundSecBracket != string:: npos))
	   //             {
		  //              ++i;
		  //              returnv.push_back(c->testP(com.at(i))); //assuming it returns a bool
		                
	   //             }
	   //             else if(commLine == "exit")	//EXIT program if user types "exit"
	   //             {
	   //                 exit(0);
	   //             }
	   //             else
	   //             {
	   //                 ++i;
	   //                 //returnv.push_back(c->commandP(com.at(i))); //assuming it returns a bool
	   //             }
    //             }
    //             else 
    //             {
    //                 perror("Invaid command."); //I'm not sure if thats the right error for this
    //                 //its for having a connector with nothing infront of it 
    //             }
    //         } 
    //         else if(com.at(i) == "||")
    //         {
    //             if(i > 0 && returnv.at(i - 1) == false)
    //             {
    //                 size_t foundTest = commLine.find("test");	//this will check "test" in the string
	   //             size_t foundFirstBracket = commLine.find("[");	//this will check "[" in the string
	   //             size_t foundSecBracket = commLine.find("]");	// this will check "]" in the string
                    
    //                 if(foundTest != string::npos || (foundFirstBracket != string::npos && foundSecBracket != string:: npos))
	   //             {
		  //              ++i;
		  //              returnv.push_back(c->testP(com.at(i)));
		                
	   //             }
	   //             else if(commLine == "exit")	//EXIT program if user types "exit"
	   //             {
	   //                 exit(0);
	   //             }
	   //             else
	   //             {
	   //                 ++i;
	   //                 //returnv.push_back(c->commandP(com.at(i)));
	   //             }
    //             }
    //             else 
    //             {
    //                 perror("Invaid command."); //I'm not sure if thats the right error for this
    //                 //its for having a connector with nothing infront of it 
    //             }
    //         }
    //         else 
    //         {
    //             size_t foundTest = commLine.find("test");	//this will check "test" in the string
	   //         size_t foundFirstBracket = commLine.find("[");	//this will check "[" in the string
	   //         size_t foundSecBracket = commLine.find("]");	// this will check "]" in the string
    //             if(foundTest != string::npos || (foundFirstBracket != string::npos && foundSecBracket != string:: npos))
	   //         {
		  //          returnv.push_back(c->testP(com.at(i)));
	   //         }
	   //         else if(commLine == "exit")	//EXIT program if user types "exit"
	   //         {
	   //             exit(0);
	   //         }
	   //         else
	   //         {
	   //             //returnv.push_back(c->commandP(com.at(i)));
	   //         }
    //         }
    //     }
    // }
    // return 0;
}
