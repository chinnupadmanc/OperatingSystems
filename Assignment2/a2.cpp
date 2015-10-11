/*Chullipparambil, Chinnu Padman
Class Account: masc1445
CS 570, Summer 2014
Assignment #2,  Analysis of Page Replacement Algorithms.
Filename: a2.cpp */


#include "a2.h"
#include "a2_functions.h"

using namespace std;

int frame_count;
int page_count;
int *page = NULL;

// Function to read pages.txt.
int read_page_numbers()
{
	string inputdata;
	char delimiter [] = " ";
  	int count = 0;
        int len;	
        char *result;
        char *input;
  
	ifstream inputfile("pages.txt");
  	if (inputfile.is_open())
  	{
		getline (inputfile,inputdata);
		len = strlen(inputdata.c_str());
	        input = (char *)malloc(len+1);
	        page = (int *)malloc(((len/2)+1)*sizeof(int));	
                strcpy(input, inputdata.c_str());
    		result = strtok(input,delimiter);
    		while (result != NULL) 
		{
        	    page[count++] = atoi(result);
        	    //cout << page[count-1] << " ";
                    result = strtok(NULL, delimiter);
        	}
        	//cout << "\n";

    	inputfile.close();
	return count;
	}

  	else 
	{
		cout << "Unable to open file"; 
		return -1;
	}
}

// main function
int main()

{
	// User inputs the frame size.
	cout << "Enter the number of frames:";
	cin >> frame_count;

	// Read pages.txt
        page_count = read_page_numbers();

	if(page_count == -1)
	{
 		return 0;
	}

	// Run all four algorithms on the data set
	opt_algo();
	lru_algo();
	clock_algo();
	fifo_algo();
}



