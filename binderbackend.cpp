#include <string>
#include "codearray.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>

using namespace std;

int main()
{
	
	/* The child process id */
	pid_t childProcId = -1;
		
	/* Go through the binaries */
	for(int progCount = 0; 	progCount < NUM_BINARIES; ++progCount)
	{
			
		// Create a temporary file
		// E.g. fileName = tmpnam(NULL)
		string fileName = tmpnam(NULL);
		
		// Open the file and write the bytes of the first program to the file.
		//These bytes are found in codeArray[progCount]
		FILE* fp = fopen(fileName.c_str(), "wb");
		
		if (fwrite(codeArray[progCount], sizeof(char), programLengths[progCount], fp) < 0)
		{
			perror("fwrite");
			exit(-1);	
		}
		fclose(fp);
		// Make the file executable
		chmod(fileName.c_str(), 0777);
		
		// Create a child process using fork
		pid_t childProcId;
		childProcId = fork();
		/* I am a child process; I will turn into an executable */
		if(childProcId == 0)
		{
			
			// use execlp() in order to turn the child process into the process
			//running the program in the above file.	
			if (execlp( fileName.c_str(), ("./" + fileName).c_str() , (char *) NULL ) < 0)
			{
				perror("execlp");
				exit(-1);
			}
		}
	}
	
	/* Wait for all programs to finish */
	for(int progCount = 0; progCount < NUM_BINARIES; ++progCount)
	{
		/* Wait for one of the programs to finish */
		if(wait(NULL) < 0)
		{
			perror("wait");
			exit(-1);
		}	
	}
}
