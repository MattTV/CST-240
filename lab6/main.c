#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#include "list.h"
#include "sequence.h"

//*************************************************
// Timing of Step 1c.
//*************************************************
// ./threads -t 1 -n 30000
// real    0m0.091s
// user    0m0.018s
// sys     0m0.071s
//
// ./threads -t 3 -n 10000
//real    0m0.165s
//user    0m0.074s
//sys     0m0.202s
//
//I tested with about the highest number I could using int instead of int64. I notice that much more kernel
//  time is needed when using threads. I don't think that multithreading helps in this case because each
//  thread relies on the same one resource.
//*************************************************
// Timing of Step 2b.
//*************************************************
//
//*************************************************

typedef struct tinfo_s
{
    linked_list_t * l;
    int v;
} tinfo_t;

pthread_mutex_t lock;

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
    tinfo_t * s = (tinfo_t *)data;
    linked_list_t * list = s->l;
    int val = s->v;

    printf("Thread ID: %ld, Value: %d\n", pthread_self(), val);

    int d = 0;
    for (short ii = 0; ii < val; ii++)
    {
        pthread_mutex_lock(&lock);
        d = Next_In_Sequence();
        pthread_mutex_unlock(&lock);
        Insert_In_Order(list, d);
    }

    return NULL;
}
//*************************************************
int main(int argc, char** argv)
{
    linked_list_t *list;
    int opt;                    // command line arg
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

    pthread_mutex_init(&lock, NULL);

    list = Init_List();

    tinfo_t info;
    info.l = list;
    info.v = num_to_insert;

    for (short ii = 0; ii < num_threads; ++ii)
    {
        pthread_create(threads + ii, NULL, thread, &info);
    }

    for (short ii = 0; ii < num_threads; ++ii)
    {
        pthread_join(threads[ii], NULL);
    }

    free(threads);

    printf("Next expected: %i\n", num_to_insert * num_threads + 1);

    printf("Creating sorted list with %d elements\n", num_to_insert);

    printf("Checking sorted list\n");
    Traverse(list, Check_Item);

    if (Count(list) != num_to_insert) printf("List is not tracking size\n");

    Delete_List(list);
    pthread_mutex_destroy(&lock);

    return 0;
}



