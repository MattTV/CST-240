#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#include "list.h"
#include "sequence.h"

//*************************************************
void Check_Item(int64_t value)
{
    static int64_t last_item = INT64_MIN;

    if (last_item != INT64_MIN && last_item+1 != value)
    {
        printf("Items not in sequence: %ld %ld\n", last_item, value);
    }
    last_item = value;

    // Uncomment the following if you want to see the data
    //printf("Item: %ld\n", value);
}
//*************************************************
int main(int argc, char** argv)
{
    linked_list_t *list;
    int opt;                    // command line arg
    int ii;                     // for index
    int64_t data;               // value to insert into list
    int64_t num_to_insert = 20;
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

    if (num_threads > 1)
    {
        printf("Multiple threads not implemented. Running a single thread.\n");
        num_threads = 1;
    }

    printf("Creating sorted list with %ld elements\n", num_to_insert);
    list = Init_List();

    for (ii=0; ii<num_to_insert; ii++)
    {
        data = Next_In_Sequence();
        Insert_In_Order(list, data);
    }

    printf("Checking sorted list\n");
    Traverse(list, Check_Item);

    if (Count(list) != num_to_insert) printf("List is not tracking size\n");

    Delete_List(list);

    return 0;
}

