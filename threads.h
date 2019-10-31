

#ifndef _THREADS_H_
#define _THREADS_H_


#include <stddef.h>



typedef struct buffer
{
    int limit;
    int size;
    int array[];


}*buffer;


buffer init_buffer(size_t size);

int buffer_add(buffer b, int value);
int buffer_remove(buffer b);
int buffer_free(buffer b);
void buffer_dump(buffer b);

#endif 
