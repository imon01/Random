/*
    Buffer interface

*/


#include  "threads.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

// file scope only



static int  malloc_check(void * malc_obj, const char* fname, const char* str_msg)
{
    
    if( malc_obj == NULL)
    {
        fprintf(stderr, "%s failed: %s\n", fname, str_msg);
        return 1;
    }
    fprintf(stderr, "%s passed: end msg\n", fname);
    return 0; 
}


// no checks on max memory allocation
buffer init_buffer(size_t size)
{
         
    buffer b =  malloc( sizeof(buffer) + sizeof(int)*size);
    if( malloc_check(b, "malloc", "size allocation failed"))
    {
        return 0;
    }

    b->limit = size;
    b->size = 0;
    //don't clear array bytes 
    return b;
}

int buffer_add(buffer b, int value)
{
    if(b->size > b->limit)
    {
        fprintf(stderr, "oh no! buffer is full, sorry won't add value\n");
        return 1;
    }

    b->array[b->size] = value;
    b->size++;

    fprintf(stdout, "new value added: %d\n", value);
    return 0;
}


int buffer_remove(buffer b)
{
    b->size--;
    if(b->size < 0)
    {
        fprintf(stderr, "oh no! buffer is empty, sorry won't delete  value");
        return -1; //should be other than -1
    }
    
    return b->array[b->size]; 
}


int buffer_free(buffer b)
{
    free(b);
    return 1;
    
}

void buffer_dump(buffer b)
{
    int size = 0 ;

    while( size < b->size)
    {
        printf("%d ", b->array[size++]);

    }
    printf("\n");
}
