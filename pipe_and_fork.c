#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
	// twop file descriptors for two pipes p1 and p2 
	int fd1[2]; // used to store two ends of first pipe
	int fd2[2]; // used to store two ends of second pipe

	// parent sends a string to the child process
	char fixed_str[] = "Kumar Paluri";
	// create a pid
	pid_t p;
	char concat_str[100]; // final output string
	// pipe creation
	if(pipe(fd1)==-1)
	{
		fprintf(stderr, "Pipe-1 creation failed");
		return -1;
	}

	if(pipe(fd2)==-1)
	{
		fprintf(stderr, "Pipe-2 creation failed");
		return -1;
	}

	char input_str[100];
	scanf("%s", input_str);

	// forking once creates a child process 
	// total of 2: one child and one parent process
	p = fork();

	if(p<0)
	{
		fprintf(stderr, "fork failed");
		return -1;
	}

	else if(p>0) // parent process
	{
		
		// role of parent process is to send the string to the child process
		close(fd1[0]); // close the read end of Pipe-1

		// write input string to the pipe-1
		write(fd1[1], input_str, strlen(input_str)+1);
		// after writing, close the write end of the pipe
		close(fd1[1]);

		// Now wait for child process to send the string 
		wait(NULL);

		// If here: its time to read the sent string by child process
		// close the write end of second pipe
		close(fd2[1]);
		read(fd2[0], concat_str, 100);
		printf("Final String is: %s\n", concat_str);
	}

	else{ // pid == 0 // child process
		char c_str[100];
		close(fd1[1]); // child process needs to read the sent // so close the write end first
		// read the string 
		read(fd1[0], c_str, 100);
		int sizer = strlen(c_str);

		// now concatenate the c_str 
		for(int i=0; i<strlen(fixed_str);i++)
		{
			c_str[sizer]= fixed_str[i];
			sizer++;
		}
		// append a '\0' at the ned 
		c_str[strlen(c_str)]='\0';
		close(fd1[0]);
		// also close the read end of fd2 to write
		close(fd2[0]);
		write(fd2[1], c_str, strlen(c_str)+1);
		close(fd2[1]);
		_exit(0); // exit the child process
	}

	return 0;
}