/*Chullipparambil, Chinnu Padman
Class Account: masc1445
CS 570, Summer 2014
Assignment #2,  Analysis of Page Replacement Algorithms.
Filename: a2_functions.cpp */


#include "a2.h"

using namespace std;

struct node;
struct clock_node; 

// Node for OPT, LRU and FIFO
typedef struct node
{
	int page_no;
	struct node *next;
}node_type;

// Node for CLOCK
typedef struct clock_node
{
	int page_no;
	int use_bit;
	struct clock_node *next;
}clock_node_type;

node_type *head = NULL;
node_type *tail = NULL;
clock_node_type *clock_head = NULL;
clock_node_type *clock_tail = NULL;
clock_node_type *clock_pointer = NULL;

// Function for adding page to frame in OPT/LRU/FIFO.
// Always add the page to head.
void insert_to_list(int page_no)
{
	node_type *new_node;
	node_type *temp = NULL;

	new_node = (node_type*)malloc(sizeof(node_type));
	new_node->page_no = page_no;

	if(new_node == NULL)
	{
	cout << "Error!";
	return;
	}	
	
    	if(head != NULL)
	{
		temp = head;	
	}
	

	head = new_node;

	new_node->next = temp;

	if(temp == NULL)
	{
		tail = new_node;
	}
}

// Function to check the page is already loaded one of the frames.
node_type * find_in_list(int page_no)
{
	node_type *temp = NULL;	
    	for(temp=head ; temp!=NULL; temp=temp->next)
   	{
		if(temp->page_no == page_no)
		return temp;
    	}

	return NULL;
}

//Function to remove a page from frame. Always removes tail.
void delete_from_list()
{
	node_type *temp = NULL;

        if(head==tail)
        {
		free(tail);
        	head = NULL;
        	tail = NULL;
        	return;
        }
   	
        for(temp=head ; temp!=NULL && temp->next!=tail; temp=temp->next);
	free(tail);
	temp->next = NULL;
	tail = temp;
}

//Function to move recently used page to top/head.
void move_to_head(node_type *item)
{
	node_type *temp;
	for(temp=head; (temp!=NULL) && (temp->next!=item); temp=temp->next);
	if(item == tail)
	{
		tail = temp;
	}	

	if(temp)
	{
		temp->next = item->next;
		item->next = head;
		head=item;
	}
		
}

//Prints list of currently loaded pages.
void print_list()
{
	node_type *temp;	
    	for(temp=head ; temp!=NULL; temp=temp->next)
    	{
		cout << temp->page_no << " ";
    	}
    	cout << "\n";

}

// Function to create a clock node.
void create_clock_node()
{
	clock_node_type *clock_new_node;
	clock_node_type *temp = NULL;

	clock_new_node = (clock_node_type*)	malloc(sizeof(clock_node_type));
	clock_new_node->page_no = -1;
	clock_new_node->use_bit = 0;

	i
:(clock_new_node == NULL)
	{
	cout << "Error!";
	return;
	}	
	
    	if(clock_head != NULL)
	{
		temp = clock_head;	
	}
	
	clock_head = clock_new_node;

	clock_new_node->next = temp;

	if(temp == NULL)
	{
		clock_tail = clock_new_node;
	}

	clock_tail->next = clock_head;
}

//Function to create clock. Number of nodes equals the frame count.
void create_clock()
{
	for(int i=0; i<frame_count; i++)
    	{
		create_clock_node();
	}
	
	clock_pointer = clock_head;
}

//Check if a page is already loaded in frame in clock structure.
clock_node_type *find_in_clock(int page_no)
{
	clock_node_type *temp;	
    	for(temp=clock_head ; temp!=NULL && temp->next!=clock_head; temp=temp->next)
   	{
		if(temp->page_no == page_no)
		{
			temp->use_bit = 1;
			clock_pointer = temp->next;	
			return temp;
		}
    	}

	if(temp->page_no == page_no)
	{
			temp->use_bit = 1;
			clock_pointer = temp->next;	
			return temp;

	}
	
	return NULL;
}

//Get slot in clock for replacement.
clock_node_type *get_free_slot_clock()
{
	clock_node_type *temp;	
	for(temp=clock_pointer; temp->use_bit != 0; temp=temp->next)
   	{
		temp->use_bit = 0;
		
	}

	return temp;

}

// Add a new page to clock node.
void insert_to_clock(int page_no)
{
	clock_pointer = get_free_slot_clock();
	clock_pointer->page_no = page_no;
	clock_pointer->use_bit = 1;
	clock_pointer = clock_pointer->next;
}

// Display pages loaded in clock.
void print_clock()
{
	clock_node_type *temp;	
    	for(temp=clock_head ; temp!=NULL && temp->next!=clock_head; temp=temp->next)
   	{
		cout << temp->page_no << " ";
    	}
	cout << temp->page_no << " ";
    	cout << "\n";
}

// Delete page for replacement in OPT.
void delete_item(int page_no)
{
	node_type *temp;
	node_type *prev = NULL;
	for(temp=head ; temp!=NULL && temp->page_no!=page_no; temp=temp->next) 
	{
		prev = temp;
	}
	if(temp != NULL)
	{
		if(prev != NULL)
		{
			
			prev->next= temp->next;
		}
		else
		{
			head = temp->next;	
		}

		if(temp == tail)
		{
			tail = prev;
		}

		free(temp);
	}
}

// Find the when the page is showing p in future.
int find_distance(int page_no, int page_index)
{
	for(int i=page_index; i<page_count; i++)
	{
		if(page[i]==page_no)
		{
			return i-page_index+1;
		}
	}

	return page_count-page_index+2;
}

// Function to find the farthest item.
int find_farthest_item(int page_index)
{
	node_type *temp;
	int distance = 0;
	int temp_distance = 0;
	int farthest_page_no;
	for(temp=head ; temp!=NULL; temp=temp->next)
	{
		temp_distance = find_distance(temp->page_no, page_index);

		if(distance < temp_distance)
		{
			distance = temp_distance;
			farthest_page_no = temp->page_no;
		}
	}
	return farthest_page_no;
}

//LRU alogorithm
void lru_algo()
{
	int frames_used = 0;
        int fault = 0;
	head = NULL;
	tail = NULL;

    	for(int i=0; i<page_count; i++)
    	{
    	node_type *found = find_in_list(page[i]);

	if(found == NULL)
	{
		if(frames_used==frame_count)
		{
			delete_from_list();
			frames_used--;
		}

		insert_to_list(page[i]);
		frames_used++;
		fault++;
	}

	else
	{
		move_to_head(found);
        }

	//cout << "After inserting element " << page[i] << ":" << "\n";	
	//print_list();
    }

    cout << "\nTotal no. of page faults in lru algorithm: " << fault << "\n";
}
//Clock algorithm
void clock_algo()
{
	int frames_used = 0;
        int fault = 0;
        create_clock();
    	for(int i=0; i<page_count; i++)
    	{
    		clock_node_type *found = find_in_clock(page[i]);

		if(found == NULL)
		{
			insert_to_clock(page[i]);
			fault++;
		}

		//cout << "After inserting element " << page[i] << ":" << "\n";	
		//print_clock();
    }

    cout << "\nTotal no. of page faults in clock algorithm: " << fault << "\n";
}

//Optimal algorithm
void opt_algo()
{
	int frames_used = 0;
        int fault = 0;
	int temp_page_no;
	head = NULL;
	tail = NULL;

    	for(int i=0; i<page_count; i++)
    	{
    	node_type *found = find_in_list(page[i]);

	if(found == NULL)
	{
		if(frames_used==frame_count)
		{
			temp_page_no = find_farthest_item(i+1);

			delete_item(temp_page_no);
			frames_used--;
		}

		insert_to_list(page[i]);
		frames_used++;
		fault++;
	}

	//cout << "After inserting element " << page[i] << ":" << "\n";	
	//print_list();
    }

    cout << "\nTotal no. of page faults in optimal algorithm: " << fault << "\n";
}


// Fifo algorithm
void fifo_algo()
{
	int frames_used = 0;
        int fault = 0;
	head = NULL;
	tail = NULL;

    	for(int i=0; i<page_count; i++)
    	{
    	node_type *found = find_in_list(page[i]);

	if(found == NULL)
	{
		if(frames_used==frame_count)
		{
			delete_from_list();
			frames_used--;
		}

		insert_to_list(page[i]);
		frames_used++;
		fault++;
	}

	//cout << "After inserting element " << page[i] << ":" << "\n";	
	//print_list();
    }

    cout << "\nTotal no. of page faults in fifo algorithm: " << fault << "\n";
}


