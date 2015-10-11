/*Chullipparambil, Chinnu Padman
Class Account: masc1445
CS 570, Summer 2014
Assignment #1, Program that manages multiple threads writing to a shared file. 
Filename: sem.cpp */

// Include header file.
#include "sem.h"

using namespace std;

// Preprocessor macro
#define NUMBER_OF_THREADS 6

// Function declaration
void print_pid();
void create_threads();
void* print_threadid(void*);
void join_threads();
void initialize_sem();
void destroy_sem();
 
// Global variables
sem_t SEM;
pthread_t t[NUMBER_OF_THREADS]; 


// The main program prints its pid and creates 6 threads.   
int main()
{
    // Function call to print the pid to SHARED.txt.
    print_pid();

    // Function call to initialize semaphore.
    initialize_sem();
   
    // Function call to create threads.
    create_threads();
 
    // Function call to wait till the created threads are terminated.
    join_threads();
   
    // Function  call to destroy semaphore. 
    destroy_sem();
    
    //Console message to indicate the end of program.
    cout << "End of program.\n"; 

    return 0; 
}

// This function creates SHARED.txt, writes pid in the file and closes the file. 
void print_pid()
{
    // Creates an instance of ofstream (stream class to write on fles). 
    ofstream outputFile;
    
    // Open SHARED.txt file.
    // In default mode for opening file, the constructor creates the file if it does not exist or delete everything in it if something exist in it.   
    outputFile.open("SHARED.txt");
    if(!(outputFile.is_open()))
    {
        cout << "Error opening file!";
    }
  
    // Write pid to the file
    outputFile << getpid() << '\r' << '\n';
    
    // Close the file.
    outputFile.close();    
}

// This function creates 6 threads.   
void create_threads()
{  
    int thread_create_status; 
    
    // Creates 6 threads 
    for(int i=0; i<NUMBER_OF_THREADS; i++)
    {
        thread_create_status = pthread_create(&t[i], NULL, &print_threadid, (void*)i);
    
        if(thread_create_status != 0)
        {
            cout << "pthread_create failed. Error number: " << thread_create_status;
            exit(-1); 
        } 	  
    }
}
  
// This function make the caller thread(here the main thread) to wait till all 6 threads are terminated. 
void join_threads()
{    
    int thread_join_status;
    for(int i=0; i<6; i++)
    {
	thread_join_status = pthread_join(t[i], NULL);
        if(thread_join_status != 0)
        {
            cout << "pthread_join failed. Error number: " << thread_join_status;
            exit(-1); 
        } 	  
    }
}

// This functon initializes semaphore.
void initialize_sem()
{
    // Initialize semaphore to 1 - binary semaphore, second param = 0 - semaphore is local.
    int sem_init_status = sem_init(&SEM, 0, 1);
    if(sem_init_status == -1)
    {
        cout << "sem_init failed. Error number: " << strerror(errno);
        exit(-1);  
    }
}

// This functon destroys semaphore.
void destroy_sem()
{
    // Destroy semaphore.
    int sem_destroy_status = sem_destroy(&SEM);
    if(sem_destroy_status == -1)
    {
        cout << "sem_destroy failed. Error number: " << strerror(errno);
        exit(-1);  
    }
}

// This function is called upon thread creation.
void* print_threadid(void* i)
{
    pthread_t tid;
    int delayTime; 
    ofstream outputFile;
    int sem_wait_status;
    int sem_post_status;

    // Assign a periodic wait of 2 seconds for even numbered threads and 3 seconds for odd numbered threads. 
    if((int)i%2==0)
    {
        delayTime = 2;
    }
    else
    {
        delayTime = 3;
    }
   
    // Each thread performs the steps inside this for loop 10 times.    
    for(int i=0; i<10; i++)
    {
    // Call the periodic wait before getting the semaphore.  
    sleep(delayTime); 
    
    //Wait for semaphore.
    sem_wait_status = sem_wait(&SEM);
    if(sem_wait_status == -1)
    {
        cout << "sem_wait failed. Error number: " << strerror(errno);
        exit(-1);  
    }
    
    // Open SHARED.txt file in append mode.	 
    outputFile.open("SHARED.txt", std::fstream::app);
    if(!(outputFile.is_open()))
    {
        cout << "Error opening file!";
    }
    
    // Get thread id.
    tid = pthread_self();
    
    // Write thread id to the file.
    outputFile << tid << '\n';
    
    // Write to the console which thread is running.
    cout << "Thread " << tid << " is running\n";
    
    // Close the file.
    outputFile.close();
   
    //Release the semaphore.  
    sem_post_status = sem_post(&SEM);
    if(sem_post_status == -1)
    {
        cout << "sem_post failed. Error number: " << strerror(errno);
        exit(-1);  
    }
    
    }
    
    // Terminate thread.
    pthread_exit(NULL);    
}

