/*Chullipparambil, Chinnu Padman
Class Account: masc1445
CS 570, Summer 2014
Assignment #1, Program that manages multiple threads writing to a shared file. 
Filename: sem.h */

#include <iostream>
#include <fstream>
#include <pthread.h>    /* Required for pthreads */
#include <sys/types.h>
#include <semaphore.h>  /* Required for semaphores */
#include <unistd.h>
#include <errno.h>      /* For system error nmbers */

