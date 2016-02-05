#include <iostream>
#include <stdlib.h>

using namespace std;

void prompt()
{
	cout << "$ ";
}

int main()
{
	string line;
	while(1)
	{
		prompt();
	 	getline(cin, line);
		if(line == "exit")
		 {
			exit(0);
		 }
	}	
}

