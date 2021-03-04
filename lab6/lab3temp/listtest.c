#include <stdio.h>
#include <stdlib.h>

#include "list.h"

void print_action(int data)
{
	printf("> %d\n", data);
}

//******************************************
int main()
{
    linked_list_t list;
    int ii;
    int value;

	 printf("Checking stack behavior\n");
    list = Init_List();
    if (list == NULL)
    {
        printf("Unable to initialize list\n");
        return 1;
    }

    printf("Insert At Beginning\n");
    for (ii=0; ii<10; ii++)
    {
        if (Insert_At_Beginning(list, ii) != 0)
        {
            printf("Error inserting %d at beginning\n", ii);
        }
    }

    if (Count(list) != 10) 
    {
        printf("Count is not correct after inserts at beginning: %d\n", Count(list));
    }

    printf("Remove From Beginning\n");
    for (ii=9; ii>=0; ii--)
    {
        if (Remove_From_Beginning(list, &value) != 0)
        {
            printf("Failed to remove item from beginning\n");
        }
        if (ii != value)
        {
            printf("Error removing %d %d from beginning\n", ii, value);
        }
    }

    printf("Insert At End\n");
    for (ii=0; ii<10; ii++)
    {
        if (Insert_At_End(list, ii) != 0)
        {
            printf("Error inserting %d at end\n", ii);
        }
    }

    if (Count(list) != 10)
    {
        printf("Count is not correct after inserts at end: %d\n", Count(list));
    }

    printf("Remove From End\n");
    for (ii=9; ii>=0; ii--)
    {
        if (Remove_From_End(list, &value) != 0)
        {
            printf("Failed to remove item from end\n");
        }
        if (ii != value)
        {
            printf("Error removing %d %d from end\n", ii, value);
        }
    }

    if (!Empty(list))
    {
        printf("List is not empty after removes\n");
    }

	printf("Traverse function\n");
	for(ii=1; ii<10; ii++)
	{
		Insert_At_End(list, ii);
	}
	if(Traverse(list, print_action) != 0)
	{
        printf("Traverse did not process correctly\n");
	}

	Delete_List(list);

    printf("Insert_In_Order\n");

    list = Init_List();

	for(ii=0; ii<10; ii++)
	{
		Insert_In_Order(list, rand() % 100);
	}

	Traverse(list, print_action);

	Delete_List(list);

    printf("Tests complete\n");

    return 0;
}

