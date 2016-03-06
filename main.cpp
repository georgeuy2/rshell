#include <cstddef>
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
	  bool parenVal;
	  Commands(){parenVal = true;};
	  void setparenVal(bool val);
	  bool getparenVal();
	  void parse(string commLine);
	  void commandP(string& com);	//for parsing command	
	  bool execforcommand(string& com); //for execute commmand
	  int connectP(string com, int andCheck, int orCheck); // for parsing connectors
	  void getarg (string& com, char* arrayargs[]);
	  void starttest(string& com);
	  void testP(string& com);  //for parsing test
	  void operatorP(char* tempCom, int& numParen, string& comLine, vector<string> list);
	  void removeChar(char* tempCom, int x, string& comLine, string ch);
	  void print(char* x);
	  void updateTempCom(char* tempCom, int x, int y, string comLine);
};


void Commands::parse(string commLine)
{
		int numberOfParentheses = 0;
		vector <string> list;
		int posFirstP = 0;
		int posSecP = 0;
	//	int numFirstP = 0;
		size_t foundTest = commLine.find("test"); //this will check "test in the string
		size_t foundFirstBracket = commLine.find("["); //this will check "[" in the string
		size_t foundSecBracket = commLine.find("]"); //this will check "]" in the string
		//check if the string as "test" or "[]" in it
		size_t foundParen = commLine.find("(");
		char * tempCon = new char[commLine.length() + 1];
		strcpy(tempCon, commLine.c_str());
		if (foundParen == string::npos)
		{
				numberOfParentheses++;
				cout << "number of (): " << numberOfParentheses << endl;
				commandP(commLine);
				setparenVal(true); //need to reset all these for the next round
		}
		else
		{
				operatorP(tempCon,posFirstP, commLine, list);
				setparenVal(true);
		}
		if (foundTest != string::npos || (foundFirstBracket != string::npos && foundSecBracket != string::npos))
		{
				testP(commLine);
				setparenVal(true);
		}
		if (commLine == "exit") //Exit program if user types "exit"
		{
				exit(0);
		}
}
void Commands::setparenVal(bool val)
{
		parenVal = val;
}
bool Commands::getparenVal()
{
		return parenVal;
}
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
void Commands::removeChar(char* tempCom, int x, string& comLine, string ch)
{
	comLine.erase(x,ch.size());	//since tempCom updates then it can remove ( properly
//	cout<< x << "comeLine:" << comLine << endl;	
		
	for(x; tempCom[x] != '\0'; x++)
	{
		if(tempCom[x+1] != '\0')
		{
			tempCom[x] = tempCom[x+1];
		}
		else if(tempCom[x+1] == '\0')
		{
			tempCom[x] = '\0';
		}
	}	

//	print(tempCom);		//just to print the result

}
void Commands::updateTempCom(char* tempCom, int x, int y, string comLine)
{

	for(int i = 0; i != comLine.length(); i++)
	{
	
		if(tempCom[x+1] != '\0')
		{
			tempCom[y] = tempCom[x+1];
			y++;
		}
		x++;
	}
	tempCom[y] = '\0';	
}

void Commands::print(char* x)
{
	cout << "array:";
	for(int i=0; x[i] !='\0'; i++)
	{
		cout << x[i];
	}
	cout << endl;
}
void Commands::operatorP(char* tempCom, int& numParen, string& comLine, vector<string> list)
{
	string line;
	string first = "(";
	string second = ")";
        string andC = "&";
	string orC= "|";
	string semiC= ";";
	int size =0;
	bool foundboth = false;
	size_t posBackP = comLine.find_last_of(")");
	setparenVal(false);	
	//gets the size of the string
	for(int j =0; tempCom[j] != '\0'; j++)
	{
		size++;
	}    
	cout << "position of  ) is element: " << posBackP << endl;

	for(int i = 0; tempCom[i] != '\0' ; i++ )
    {
        //when ( is matched
	    if(tempCom[i] == '(')
        {
            // when first element is ( command or argument)
			removeChar(tempCom, i, comLine, first); //This needs to remove the ( and update the tempCom
			numParen++;
			cout << " ( detected " << comLine << " " << numParen << endl; 
		}

		else if (tempCom[i] == ')')
		{
			numParen--;
			comLine.erase( comLine.find(")"), second.length());
			foundboth = true;
			cout << " ) detected " << comLine << " " << numParen << endl;
		}
		//when & is match and i+1 does not go out of bound
		else if( tempCom[i] == '&' && foundboth == true)
		{
				cout << "& detected" << endl;
				removeChar(tempCom, i, comLine, andC); //This needs to remove the ( and update the tempCom
				if(tempCom[i] == '&')
				{
			//		i--;
					cout << "&& detected" << endl;
					removeChar(tempCom, i, comLine, andC);	
					line = comLine.substr(0, i);
					cout << "line:" << line << endl;
				
					cout << "b4 comLine: " << comLine << endl;
					comLine.erase(0,line.size()+1);
					
					cout<< "comeLINE:" << comLine << " " << comLine.length() << endl;
					updateTempCom(tempCom,i,0,comLine);
					commandP(line);
					cout << "comline: " << comLine << endl;
				}
				else
				{
					setparenVal(false);
					cout << " error " << endl;
				}
		}
		else if ( tempCom[i] == '|' || foundboth == true) 
		{
				cout << "| detected" << endl;
				removeChar(tempCom, i, comLine, orC); //this removes ( and updates the tempCon
				cout << getparenVal() << endl;
				if (tempCom[i] == '|')
				{
						cout << "|| dectected" << endl;
						removeChar(tempCom, i, comLine, orC);
						line = comLine.substr(0, i);
						cout << "line: " << line << endl;

						cout << "b4 comLine: " << comLine << endl;
						comLine.erase(0, line.size() + 1);

						cout << "comLine: " << comLine << " " << comLine.length() << endl;
						//commandP(line);
						//updateTempCom(tempCom, i, 0, comLine);
						if (getparenVal() == 0)
						{
								commandP(line);
								//cout << "comLine: " << comLine << endl;
						}
				}
				else 
				{
						setparenVal(false);
						cout << "error" << endl;
				}
		}

		if( numParen == 0 && getparenVal() == 0)
		{	
			cout << "no more ()" << endl;
			commandP(comLine);
			break;	
		}
    }
}

void Commands::testP(string& com)
{
    int rmtest = com.find("test");
    string t = "test";
    if (rmtest != string::npos)     //remove test
    {
   	 com.erase(rmtest, t.length());
    }
    int rmbracket1 = com.find("[");
    string first = "[";
    if (rmbracket1 != string::npos)  //remove brackets
    {
        com.erase(rmbracket1, first.length());

	int rmbracket2 = com.find("]");
        string second = "]";
        if (rmbracket2 != string::npos)
        {
            com.erase(rmbracket2, second.length());
        }
        else 
        {
			setparenVal(false);
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
		setparenVal(false);
    	perror("Too many flags");
    }
    
    string s = " \t";
    char* c = new char[com.length() + 1];
    strcpy(c, com.c_str());

    c = strtok(&com[0], " \t");		//c will have the data path

 	if (flage)
    {
		if (stat(c,&sb) == 0)
		 {
				 setparenVal(true);
				 cout << "(True)" << endl;
		 }
		 else
		 {
				setparenVal(false);
				 cout << "(False)" << endl;
		 }
	}
    else if (flagf)
    {
		if (stat(c, &sb) == 0 && S_ISREG(sb.st_mode))
		{
				setparenVal(true);
				cout << "(True)" << endl;
		}
		else
		 {
				 setparenVal(false);
				 cout << "(False)" << endl;
		 }
		//do the stuff for flad f
    	
    }
    else if(flagd)
    {
		if (stat(c, &sb) == 0 && S_ISDIR(sb.st_mode)) 
		{
				setparenVal(true);
				cout << "(True)" << endl;
		}
		else 
		{
				setparenVal(false);
				cout << "(False)" << endl;
		}
	//do the stuff for flag d
    	
    }
    else 
    {
		setparenVal(false);
    	perror("flag not recognized"); //i dont think we need this
    }
	
	int findc = com.find(c);
	com.erase(findc, strlen(c) + 1);	// this will delete the data path
	com.erase(com.find(" "), s.length());		//also deletes the first space

	string orC = "||";
	string andC = "&&";
	string semiC = ";";

    //when there is connectors after test command
	if ( com.find("&&") != string::npos )
	{
		com.erase(com.find("&&"), andC.length());
	
	}
	else if( com.find("||") != string::npos)
	{
		com.erase(com.find("||"), orC.length());
	}
	else if( com.find(";") != string::npos)
	{
		com.erase(com.find(";"), semiC.length());
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
	// cout << "Test 1" <<  endl;
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
		bool temp =  execforcommand(com); //i changed this from return execforcommand(com), 
		//it will still work right?
		setparenVal(temp);
		return temp;
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
				setparenVal(false);
			    perror("error in getenv");
			}
			char * check = getcwd(path, 2047);
			if(check != '\0')	
			{
				int checkcheck = setenv("OLDPWD", path, 1);
				if (checkcheck <= -1)	
				{
					setparenVal(false);
					perror("error in setenv");
				}
			}
			else 
			{
				setparenVal(false);
				perror("Error in getcwd()");
			}		
			int tricheck = chdir(homebase.c_str());
			if(tricheck <= -1)	
			{
				setparenVal(false);
				perror("error in chdir");
			}
			setparenVal(true);
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
				setparenVal(false);
				perror("error in getenv");
			}
			char* check = getcwd(path, 2047);
			if(check != '\0')	
			{
				int checkcheck = setenv("OLDPWD", path, 1);
				if (checkcheck <= -1)	
				{
					setparenVal(false);
					perror("error in setenv");
				}
			}
			else 
			{
				setparenVal(false);
				perror("Error in getcwd()");
			}		
			int tricheck = chdir(cdir.c_str());
			if (tricheck <= -1)	
			{
				setparenVal(false);
				perror("error in chdir");
			}
			setparenVal(true);
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
				setparenVal(false);
				perror("error in getenv");
			}
			char* check = getcwd(path, 2047);
			if (check != '\0')	
			{
				int checkcheck = setenv("OLDPWD", path, 1);
				if (checkcheck <= -1)	
				{
					setparenVal(false);
					perror("error in setenv");
				}
			}
			else 
			{
				setparenVal(false);
		        perror("Error in getcwd()");
			}		
			int tricheck = chdir(homebase.c_str());
			if(tricheck <= -1)	
			{
				setparenVal(false);
				perror("error in chdir");
			}
			setparenVal(true);
			return true;
	
		}
		char* check = getcwd(path, 2047);
		if (check != '\0')	
		{
			int checkcheck = setenv("OLDPWD", path, 1);
			if (checkcheck <= -1) 
			{
				setparenVal(false);
				perror("error in setenv");
			}
		}
		else 
		{
			setparenVal(false);
			perror("Error in getcwd()");
		}
		int tricheck = chdir(args); //problems with args
		if (tricheck <= -1) 
		{
			setparenVal(false);
			perror("error in chdir");
		}
		setparenVal(true);
		return true;
	}
	int pro = fork(); //time to fork
	if (pro <= -1)	//-1 is error
	{
		setparenVal(false);
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
			setparenVal(false);
			perror("ERROR IN EXECUTING COMMAND LINE 202");
			exit(1);
		}
	}
	else //in parent process
	{
		int childWait = waitpid(pro, &childWait, 0);
		if (childWait <= -1)
		{
			setparenVal(false);
			perror("WAITING FOR CHILD FAILED");
		}
		if(childWait == 0)
		{
			setparenVal(true);
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
	
	vector<string> list;
	int posFirstP =0;
    	int posSecP = 0;
    	int numFirstP = 0;

	while(1) //wamt jos tp lee[ going because exit will end it here or in functions
	 {
	  prompt();		//prints [USERNAME]@[HOSTNAME]$	
	  getline(cin, commLine);	  
		c->parse(commLine);
	 }	
	return 0;
}
