/*Chullipparambil, Chinnu Padman
Class Account: masc1445
CS 570, Summer 2014
Assignment #3, Design and implement a multiple process, time viewer.
Filename: a3.cpp */

// Include header file.
#include "a3.h"

using namespace std;

// Function declaration
void create_child_processes();
void first_child();
void second_child();
void third_child();
 
// Global variables
int run_time;
pid_t main_pid;
pid_t pid1 = -1;
pid_t pid2 = -1;
pid_t pid3 = -1;
int fd1[2];
int fd2[2];
int fd3[2];
bool signal_received = false;

// Signal handler.
void sig_handler(int signo)
{
	signal_received = true;
}

// main function
int main(int argc, char *argv[])
{
	
	/* Read the total runtime the user wants the program to run for. 
        If the user did not include it when lauching the program, take a default value of 10 seconds. */
	if (argv[1] == NULL)
	{
		run_time = 10;
	}
	else
	{
        	run_time = atoi(argv[1]);
        }
 	
	// Register signal
	if(signal(SIGUSR1, sig_handler) == SIG_ERR)
	{
		cout << "Could not register signal.";
		exit(-1);
	}

    	// Get pid of main process.
    	main_pid = getpid();
  
    	// Function call to create threads.
    	create_child_processes();
    
    	//Console message to indicate the end of program.
    	cout << "End of program.\n"; 

    	return 0; 
}

// Function to create pipes.
void create_pipes()
{
	if(pipe(fd1) == -1)
	{
		cout << "pipe failed\n";
		exit(-1);
	}

	if(pipe(fd2) == -1)
	{
		cout << "pipe failed\n";
		close(fd1[0]);
		close(fd1[1]);
		exit(-1);
	}

	if(pipe(fd3) == -1)
	{
		cout << "pipe failed\n";
		close(fd1[0]);
		close(fd1[1]);
		close(fd2[0]);
		close(fd2[1]);
		exit(-1);
	}
}

// This function creates 3 child processes.   
void create_child_processes()
{  
	char readbuffer[50];

	// Function call to create pipes.
	create_pipes();

	// Create first child process.
	pid1 = fork();
	if (pid1 < 0) 
	{
  		/* handle error */
  		cout << "Fork() for first child process returned error code: " << errno << "\n";
  		exit(1);
	} 
	else if (pid1 > 0) 
	{
  		/* parent code */
		// Create second child process.
		pid2 = fork();
		if (pid2 < 0) 
		{
  			/* handle error */
  			cout << "Fork() for second child process returned error code: " << errno << "\n";
  			exit(-1);
		} 
		else if (pid2 > 0) 
		{
			/* parent code */
			// Create third child process.
			pid3 = fork();
			if (pid3 < 0) 
			{
  				/* handle error */
  				cout << "Fork() for third child process returned error code: " << errno << "\n";
  				exit(-1);
			} 
			else if (pid3 > 0) 
			{
				/* parent code */
				
				int count;
  				fd_set rfds;
			     FD_ZERO(&rfds);
    				FD_SET(fd1[0], &rfds);
    				FD_SET(fd2[0], &rfds);	
    				FD_SET(fd3[0], &rfds);	
				int highestfd;	

			        highestfd = (fd1[1] > fd2[1]) && (fd1[1] > fd3[1]) ? fd1[1] : (fd2[1] > fd3[1]) ? fd2[1] : fd3[1];
			
				while(!signal_received)
			        {	
				// select() to monitor all the pipes until one or more become ready.
				if(select(highestfd + 1, &rfds, NULL, NULL, NULL) >= 0)
				{
					memset(readbuffer, 0 , sizeof(readbuffer));
			        	if (FD_ISSET(fd1[0], &rfds))
					{
						// Read from pipe and print.
						if(read(fd1[0], readbuffer, sizeof(readbuffer))>=0)
						{
							cout << readbuffer;
						}
						else
						{	
							cout << "Error while reading from pipe";	
						}	
					}
			        	
					if (FD_ISSET(fd2[0], &rfds))
					{
						
						// Read from pipe and print.
						if(read(fd2[0], readbuffer, sizeof(readbuffer))>=0)
						{
							cout << readbuffer;
						}
						else
						{	
							cout << "Error while reading from pipe";	
						}	
					}
				
					if (FD_ISSET(fd3[0], &rfds))
					{
						// Read from pipe and print.
						if(read(fd3[0], readbuffer, sizeof(readbuffer))>=0)
						{
							cout << readbuffer;
						}
						else
						{	
							cout << "Error while reading from pipe";	
						}	
					}
				}	
	
				// Reinitialize fd_set values before every select() system call.		
			       	FD_ZERO(&rfds);
    				FD_SET(fd1[0], &rfds);
    				FD_SET(fd2[0], &rfds);
    				FD_SET(fd3[0], &rfds);	
				}

				// Busy wait until all three child processes are complete.
				if(waitpid(pid1, NULL, 0) < 0) 
			        {
					cout << "waitpid() returned error code: "<< errno << "\n";
				}		

				if(waitpid(pid2, NULL, 0) < 0) 
			        {
					cout << "waitpid() returned error code: "<< errno << "\n";
				}		

				if(waitpid(pid3, NULL, 0) < 0) 
			        {
					cout << "waitpid() returned error code: "<< errno << "\n";
				}		
			}
			else
			{
				/* third child code */
				third_child();
			}	
		}
		else
		{
			/* second child code */
			second_child();
		}
	}	
	else
	{
		/* first child code */
		first_child();
	}	
}


// First child performs this function.
// It prints hour, minute and second once every second.
void first_child()
{
	time_t timer;
     	struct tm* tm_info;
	char arr[100];

	while(true)
	{
		time(&timer);
		tm_info = localtime(&timer);
		
		// Write to pipe.
		sprintf(arr, "%02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
		write(fd1[1], arr, strlen(arr)+1);
		//printf("%02d:%02d:%02d\n", tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
		sleep(1);
	}

}

// Second child performs this function.
// Runs uptime program once every 5 seconds.
void second_child()
{
	pid_t child_pid;

	// Close stdout.
	close(1);

	// Maps write end of pipe to stdout.
	dup(fd2[1]);

	// Close (unused) read end of pipe. 
	close(fd2[0]);

	while(true)
	{
                // Create a new child process which will run uptime program.	
		child_pid = fork();

		if (child_pid < 0) 
			{
  				/* handle error */
  				cout << "Fork() for Child of second child process returned error code: "<< errno;
  				exit(-1);
			} 
			else if (child_pid > 0) 
			{
		
  				/* parent code. Here child 2.*/
				waitpid(child_pid, NULL, 0); 
                		sleep(5);
			}
			else
			{
				/* child code. Child of child 2. */
				cout << "\n";
				execl("/usr/bin/uptime", "uptime", NULL);
			        exit(0);	
			}
	}

}

// Function to print timer for third child.
void print_time(int elapsed_time)
{

        int minutes_pending;
        int seconds_pending;
	char arr[100];

	minutes_pending = elapsed_time / 60;
        seconds_pending = elapsed_time % 60;

	// Write to pipe.
	sprintf(arr, "%02d:%02d\n", minutes_pending, seconds_pending);
	write(fd3[1], arr, strlen(arr)+1);
	//printf("%02d:%02d\n", minutes_pending, seconds_pending);
}


// Third child performs this function.
// Prints time remaining once every second.
// When time is up, notify the first and second child processes to terminate and then terminate itself.
void third_child()
{
	int elapsed_time = run_time;
	
	// Count down timer
	while (elapsed_time >= 0)
	{
        	if(elapsed_time == 0)
		{
			break;	
		}  
        
		// Call function to print remaining time.	
                print_time(elapsed_time);

        	sleep(1);	
        	elapsed_time--;
	}

        // To print 00:00. 
        print_time(elapsed_time);

	// Send signal to first and second child processes to terminate.
	kill(pid1, SIGTERM);
	kill(pid2, SIGTERM);

        // Send signal to main process that time is up. 
	kill(main_pid, SIGUSR1);
		
        // Third child terminates.	
	exit(0);
}


