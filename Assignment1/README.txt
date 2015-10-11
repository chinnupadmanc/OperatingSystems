Chullipparambil, Chinnu Padman
Class Account: masc1445
CS 570, Summer 2014 
Assignment #1, Program that manages multiple threads writing to a shared file. 
Filename: README 

Files Included:
Source File: sem.cpp
Include File: sem.h
Makefile
README.txt

Compile Instruction:
g++ -o sem sem.cpp -lrt

Need to compile and link with 'rt' library to link the libs needed for semaphores and pthreads.

No bugs found. 
Covers all requirement specifications. 
Modularized the problem into relevant functions and kept the main function simple and minimum nmber of lines.
Error handling done for all system calls used.

 
Lessons Learned:

1. Learned how to use semaphore to manage multiple threads executing their critical region.

For this, first mplemented the program without using semaphore. The output looked like below.

This clearly showed how managing processes/threads to enter their critical region (allowing only one at a time inside the cricial region)
becomes very important, else how it will result in undesirable outputs.

Thread Thread 2 is running
6 is running
Thread 4 is running
Thread 3 is running
Thread 5 is running
Thread 7 is running
Thread Thread 2 is running
Thread 4 is running
6 is running
Thread Thread 5 is running
3 is running
Thread 7 is running
Thread Thread 46 is running
Thread  is running
2 is running
Thread Thread 2 is running
Thread 4 is running
6 is running
Thread 5 is running
Thread Thread 3 is running
7 is running
Thread Thread 26 is running
Thread  is running
4 is running
Thread Thread Thread 73 is running
5 is running
 is running
Thread Thread Thread 62 is running
 is running
4 is running
Thread Thread Thread 62 is running
 is running
4 is running
Thread 3 is running
Thread Thread 5 is running
7 is running
Thread Thread Thread 64 is running
2 is running
 is running
Thread Thread Thread 753 is running
 is running
 is running
Thread Thread Thread 642 is running
 is running
 is running
Thread Thread 2 is running
Thread 4 is running
6 is running
Thread Thread 73 is running
 is running
Thread 5 is running
Thread Thread Thread 735 is running
 is running
 is running
Thread Thread 57 is running
 is running
Thread 3 is running
Thread Thread Thread 73 is running
5 is running
 is running

Next checked below cases.
1. Withot giving any delay time.
2. Giving same delay time for both odd and even numbered threads.
3. Finally different delay times for odd and even numbered threads as per the original problem.

Observed different patterns and understood the effect of delay.
Without any delay introdced, each thread entering the for loop will finish the printing 10 times in a fracton of second.
So we can see the messages clubbed for each thread. e.g. 10 messages from thread 3, 10 messages from, thread 5, 10 messages from thread 2 etc.

For same sleep time for all the threads, we can see all threads are jumbled up randomly, no same threads repeating adjacently.

Giving 2 sec for even nmbered and 3 sec for odd numbered, we introduce a pattern in the output and we can understand the flow better.
Even numbered threads prints at 2 sec, 4 sec, 6 sec, 8 sec, 10 sec, 12 sec etc. Odd numbered threads print at 3 sec, 6 sec, 9 sec, 12 sec etc. 
Even and odd threads are aternating except in time segments which are multiples of 6 (say in 6sec, 12 sec etc). 
In this case, both sets of threads are printed. 


2. Also learned usage of various system calls like pthread_create(), pthread_join, pthread_exit, sem_init(), sem_wait(), sem_post(), sem_destroy().
For instance, I did not use pthread_join initially. After creating 6 threads, the main thread immediately destroyed the semaphore and exited. 
So in a for loop, introduced pthread_join to make the main thread wait till all 6 threads  finished their work and are terminated.

3. I used pthread_self() to get the thread id. I could have used the thread count to print the message. 
System call has an overhead of user mode to kernel mode switch and reduction in performance thereby. 
When I first read the assignment, I was under the impression that the requirement is to print the system generated thread id.       
You want us to find out and learn one more system call for the same. 
But from the discussion board, I understood we can even use the thread count the programmer is using to print the thread id. 
Did not want to do a last minute code change. Also I think performance is not the key requirement here as far as the 
objective of the assignment is concerned (we have many sleep() within). 
