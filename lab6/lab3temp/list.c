//**************************************************
// Linked list implementation
//

#include <stdlib.h>
#include <stdio.h>
//#ifdef __unix__
//#elif defined(_WIN32) || defined(WIN32)
//#endif

#include "list.h"

typedef struct item_s
{
    int data;
    struct item_s *next;
    struct item_s *prev;
} item_t;

typedef struct list_s
{
    item_t* head;
    item_t* tail;
    int     count;
} list_t;

void Fatal_Error(const char* msg) {
    perror(msg);
    exit(1);
}

// Create and initialize a list. 
// Return pointer to list. Return NULL on failure.
linked_list_t Init_List()
{
    list_t* pList = (list_t*)malloc(sizeof(list_t));
    if(pList==NULL) {
        Fatal_Error("Cannot allocate memory for list");
    }
    pList->count = 0;
    pList->head = NULL;
    pList->tail = NULL;
    return (linked_list_t)pList;
}

// Delete a list are free all memory used by the list
// It is erroneous to use the list pointer after caling this routine.
// Return zero on success
int Delete_List(linked_list_t list)
{
    list_t* pList = (list_t*)list;
    item_t* pItem = pList==NULL ? NULL : pList->head;
    while(pItem!=NULL) {
        item_t *p = pItem->next;
        free(pItem);
        pItem = p;
    }
	free(pList);
    return 0;
}

// Returns the number of items in the list
int Count(linked_list_t list)
{
    list_t* pList = (list_t*)list;
    return pList==NULL ? 0 : pList->count;
}

// Insert an item at the beginning of the list
// Return zero on success
// Params:
//    list: list to add item to
//    data: Data to be added to the list
int Insert_At_Beginning(linked_list_t list, int data)
{
    int err = 0;
    if(list==NULL)
    {
        err = 1;
    }
    else
    {
    	list_t* pList = (list_t*)list;
        // Setup the item
        item_t *pItem = (item_t*)malloc(sizeof(item_t));
        pItem->data = data;
        pItem->prev = NULL;
        pItem->next = pList->head;

        // Handle the tail case
        if(pList->tail) // There is a tail
        {
            pItem->next = pList->head;
            pList->head->prev = pItem;
            pList->head = pItem;
        }
        else // There is no tail, yet
        {
            pList->head = pItem;
            pList->tail = pItem;
        }
        ++pList->count;
    }
    return err;
}

// Insert an item at the end of the list
// Return zero on success
// Params:
//    list: list to add item to
//    data: Data to be added to the list
int Insert_At_End(linked_list_t list, int data)
{
    int err = 0;
    if(list==NULL)
    {
        err = 1;
    }
    else
    {
    	list_t* pList = (list_t*)list;
        // Setup the item
        item_t *pItem = (item_t*)malloc(sizeof(item_t));
        pItem->data = data;
        pItem->next = NULL;
        pItem->prev = pList->tail;

        // Handle the tail case
        if(pList->count) // There is at least one item
        {
            pList->tail->next = pItem;
            pList->tail = pItem;
        }
        else // There is no tail, yet
        {
            pList->head = pItem;
            pList->tail = pItem;
        }
        ++pList->count;
    }
    return err;
}

// Return true (non-zero) if the list is empty
//    list: list to examine
int Empty(linked_list_t list)
{
    list_t* pList = (list_t*)list;
    return pList->count==0;
}

// Remove an item from the beginning of the list 
// Return zero on success
// Params:
//    list: list to remove item from
//    data: pointer to location to store data of removed item
//          if data is NULL, data is not returned
int Remove_From_Beginning(linked_list_t list, int* data)
{
    int err = 0;
   	list_t* pList = (list_t*)list;
    if(pList==NULL || pList->count==0)
    {
        err = 1;
    }
    else
    {
        item_t* pItem = pList->head;
        *data = pItem->data;
        --pList->count;
        if(pList->count==0)
        {
            pList->head = NULL;
            pList->tail = NULL;
        }
        else
        {
            pList->head = pItem->next;
            if(pItem->next)
            {
                pItem->next->prev = NULL;
            }
        }
        free(pItem);
    }

    return err;
}

// Remove an item from the end of the list 
// Return zero on success
// Params:
//    list: list to remove item from
//    data: pointer to location to store data of removed item
//          if data is NULL, data is not returned
int Remove_From_End(linked_list_t list, int* data)
{
    int err = 0;
   	list_t* pList = (list_t*)list;
    if(pList==NULL || pList->count==0)
    {
        err = 1;
    }
    else
    {
        item_t* pItem = pList->tail;
        *data = pItem->data;
        --pList->count;
        if(pList->count==0)
        {
            pList->head = NULL;
            pList->tail = NULL;
        }
        else
        {
            pList->tail = pItem->prev;
            if(pItem->prev)
            {
                pItem->prev->next = NULL;
            }
        }
        free(pItem);
    }

    return err;
}

// Traverse the list calling action for each item
// Return zero on success
// Params:
//    list: list to traverse
//    action: function to call with each data item
int Traverse(linked_list_t list, void (*action)(int data))
{
	int err = 0;
	if(list==NULL || action==NULL)
	{
		err = 1;
	}
	else
	{
    	list_t* pList = (list_t*)list;
		item_t* pItem = pList->head;
		while(pItem)
		{
			action(pItem->data);
			pItem = pItem->next;
		}
	}
	return err;
}

// Insert an item in sorted order of the list
// List must be sorted when this is called
// Params:
//    list: list to insert into
//    data: Data to be added to the list
int Insert_In_Order(linked_list_t list, int data)
{
    int err = 0;
   	list_t* pList = (list_t*)list;
    if(pList==NULL)
    {
        err = 1;
    }
	else if(pList->count==0)
	{
		err = Insert_At_Beginning(list, data);
	}
    else
    {
		// Find location to insert data
		item_t* p = pList->head;
		while(p && data >= p->data)
		{
			p = p->next;
		}
		if(p == NULL) // If end of list, Insert_At_End
		{
			err = Insert_At_End(list, data);
		}
		else if(p == pList->head) // If head, call Insert_At_Beginning
		{
			err = Insert_At_Beginning(list, data);
		}
		else // Otherwise, insert before p
		{
			item_t* n = (item_t*)malloc(sizeof(item_t));
			n->data = data;
			n->next = p;
			n->prev = p->prev;
			p->prev = n;
			n->prev->next = n;
		}
	}
	return err;
}

