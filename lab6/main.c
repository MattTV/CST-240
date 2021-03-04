#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <pthread.h>

#include "list.h"
#include "sequence.h"

//*************************************************
void Check_Item(int value)
{
    static int last_item = INT_MIN;

    if (last_item != INT_MIN && last_item+1 != value)
    {
        printf("Items not in sequence: %d %d\n", last_item, value);
    }
    last_item = value;

    // Uncomment the following if you want to see the data
    printf("Item: %d\n", value);
}
//*************************************************
void * thread(void * data)
{
    int val = *(int *)data;

    printf("Thread ID: %ld, Value: %d\n", pthread_self(), val);

    for (short ii = 0; ii < val; ++ii)
    {
        int n = Next_In_Sequence();
        printf("Next in sequence: %d\n", n);
    }

    return NULL;
}
//*************************************************
int main(int argc, char** argv)
{
    linked_list_t *list;
    int opt;                    // command line arg
    int ii;                     // for index
    int data;               // value to insert into list
    int num_to_insert = 20;
    int num_threads = 1;

    while ((opt = getopt(argc, argv, "hn:t:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                printf("%s -h -n <inserts/thread> -t <num threads>\n", argv[0]);
                break;
            case 'n':
                num_to_insert = atol(optarg);
                if (num_to_insert < 0) 
                    Fatal_Error("-n must specify a value > 0");
                break;
            case 't':
                num_threads = atoi(optarg);
                if (num_threads < 1)
                    Fatal_Error("-t must specify a value > 0");
                break;
        }
    }

    pthread_t * threads = malloc(sizeof(pthread_t) * num_threads);

    for (short ii = 0; ii < num_threads; ++ii)
    {
        pthread_create(threads + ii, NULL, thread, &num_to_insert);
    }

    for (short ii = 0; ii < num_threads; ++ii)
    {
        pthread_join(threads[ii], NULL);
    }

    printf("Next expected: %i\n", num_to_insert * num_threads + 1);

    printf("Creating sorted list with %d elements\n", num_to_insert);
    list = Init_List();

    /*for (ii=0; ii<num_to_insert; ii++)
    {
        data = Next_In_Sequence();
        Insert_In_Order(list, data);
    }*/

    printf("Checking sorted list\n");
    Traverse(list, Check_Item);

    if (Count(list) != num_to_insert) printf("List is not tracking size\n");

    Delete_List(list);

    return 0;
}

