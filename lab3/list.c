//**************************************************
// Linked list implementation
// Matt Volpe
// CST 240 Linux Programming
// January 28, 2021

#include <stdlib.h>

#include "list.h"

// A node structure
typedef struct node_s
{
    int data;
    struct node_s * next;
    struct node_s * prev;
} node_t;

// A containing list structure
typedef struct list_s
{
    node_t * head;
    node_t * tail;
    int count;
} list_t;

// Create and initialize a node.
// Return pointer to node. Return NULL on failure.
node_t * Init_Node(int data, node_t * next, node_t * prev)
{
    // Dynamically allocate a node
    node_t * n = malloc(sizeof(node_t));

    // If successful, assign what is given
    if (n)
    {
        n -> data = data;
        n -> next = next;
        n -> prev = prev;
    }

    return n;
}

// Create and initialize a list. 
// Return pointer to list. Return NULL on failure.
linked_list_t Init_List()
{
    // Dynamically allocate a list
    list_t * l = (list_t *)malloc(sizeof(list_t));

    // If successful, assign default values
    if (l)
    {
        l->head = NULL;
        l->tail = NULL;
        l->count = 0;
    }

    return (linked_list_t *)l;
}

// Delete a list and free all memory used by the list
// It is erroneous to use the list pointer after caling this routine.
// Return zero on success
// Return one if list is NULL
int Delete_List(linked_list_t list)
{
    short ret = 1;

    if (list)
    {
        ret = 0;

        list_t * l = (list_t *)list;
        node_t * curr = l -> head;
        node_t * prev = NULL; 

        while (curr != NULL)
        {
            prev = curr;
            curr = curr -> next;
            free(prev);
        }

        if (curr != NULL)
            free(curr);

        free(list);
    }

    return ret;
}

// Returns the number of items in the list
// Returns -1 if list is NULL
int Count(linked_list_t list)
{
    short ret = -1;

    if (list)
        ret = ((list_t *)list) -> count;

    return ret; 
}

// Insert an item at the beginning of the list
// Return zero on success
// Return one if list is NULL
// Return two if malloc failed
// Params:
//    list: list to add item to
//    data: Data to be added to the list
int Insert_At_Beginning(linked_list_t list, int data)
{
    short ret = 1;
    
    if (list)
    {
        ret = 2;

        list_t * l = (list_t *)list; 
        node_t * n = Init_Node(data, l -> head, NULL);
        
        if (n)
        {
            ret = 0;

            l -> head = n;
            ++(l -> count);

            if (l -> count == 1)
                l -> tail = n;
            else
                l -> head -> next -> prev = n;
        }
    }

    return ret;
}

// Insert an item at the end of the list
// Return zero on success
// Return one if list is NULL
// Return two if a malloc fails
// Params:
//    list: list to add item to
//    data: Data to be added to the list
int Insert_At_End(linked_list_t list, int data)
{
    short ret = 1;

    if (list)
    {
        ret = 2;

        list_t * l = (list_t *)list;
        node_t * n = Init_Node(data, NULL, l -> tail);
        
        if (n)
        {
            ret = 0;

            l -> tail = n;
            ++(l -> count);

            if (l -> count == 1)
                l -> head = n;
            else
                l -> tail -> prev -> next = n;
        }
    }

    return ret;
}

// Return true (non-zero) if the list is empty
// Return -1 if the list is NULL
//    list: list to examine
int Empty(linked_list_t list)
{
    short ret = -1;

    if (list)
    {
        list_t * l = (list_t *)list;

        ret = l -> count;
    }

    return ret;
}

// Remove an item from the beginning of the list 
// Return zero on success
// Return one if the list is NULL
// Return two if list is empty
// Return three if data is NULL
// Params:
//    list: list to remove item from
//    data: pointer to location to store data of removed item
//          if data is NULL, data is not returned
int Remove_From_Beginning(linked_list_t list, int* data)
{
    short ret = 1;

    if (list)
    {
        ret = 2;

        list_t * l = (list_t *)list;

        if (l -> head != NULL)
        {
            ret = 3;

            if (data)
            {
                ret = 0;
                
                *data = l -> head -> data;
            }

            if (l -> head -> next != NULL)
                l -> head -> next -> prev = NULL;
            
            node_t * h = l -> head -> next;

            free(l -> head);

            l -> head = h;
        }
    }

    return ret;
}

// Remove an item from the end of the list 
// Return zero on success
// Return one if list is NULL
// Return two if the list is empty
// Return three if data is NULL
// Params:
//    list: list to remove item from
//    data: pointer to location to store data of removed item
//          if data is NULL, data is not returned
int Remove_From_End(linked_list_t list, int* data)
{
    short ret = 1;

    if (list)
    {
        ret = 2;

        list_t * l = (list_t *)list;

        if (l -> tail != NULL)
        {
            ret = 3;

            if (data)
            {
                ret = 0;

                *data = l -> tail -> data;
            }

            if (l -> tail -> prev != NULL)
                l -> tail -> prev -> next = NULL;

            node_t * t = l -> tail -> prev;

            free(l -> tail);

            l -> tail = t;
        }
    }

    return ret;
}

// Execute a function on the data of each node in the list
// Return zero on success
// Return one if list is NULL
// Return two if the list is empty
// Return three if the function pointer is NULL
// Params:
//    list: list to traverse
//    void (*action)(int data): the function to execute
int Traverse(linked_list_t list, void (*action)(int data))
{
    short ret = 1;

    if (list)
    {
        ret = 2;

        list_t * l = (list_t *)list;
        
        if (l -> head)
        {
            ret = 3;

            if (action)
            {
                ret = 0;

                node_t * n = l -> head;

                while (n)
                {
                    action(n -> data);

                    n = n -> next;
                }
            }
        }
    }
    return ret;
}

// Insert a value in order into an ordered list
// Return zero on success
// Return one if list is NULL
// Params:
//    list: list to insert into
//    value: value to create a node for and place ordered in the list
int Insert_In_Order(linked_list_t list, int value)
{
    short ret = 1;

    if (list)
    {
        ret = 0;

        list_t * l = (list_t *)list;
        node_t * n = Init_Node(value, NULL, NULL);

        if (l -> head == NULL)
        {
            l -> head = n;
            l -> tail = n;
        }
        else
        {
            node_t * c = l -> head;
            node_t * p = NULL;

            while (c != NULL && c -> data < value)
            {
                p = c;
                c = c -> next;
            }

            n -> next = c;
            n -> prev = p;

            if (p)
                p -> next = n;
            else if (!p)
                l -> head = n;
            else if (c)
                c -> prev = n;
            else if (!c)
                l -> tail = n;
        }
    }

    return ret;
}
