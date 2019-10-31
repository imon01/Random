/*

File interface to threaded application. Quick refresher to synchronization techniques.
    -Threads write to buffer in order
    -Threads write to buffer in random order
    -Threads and user write to buffer, with user using software-interrupt 
        to halt threads and write a value



TODO:
    -Finish handleing SIGTERM
    -


 gcc --std=c99 -lpthread thread-main.c -o thread-buffer-example 
*/





#include "threads.h"

#include <stddef.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct
{
    buffer b; 
    int value;
}args_t;


void driver_example();
void signal_callback(int signo);
void signal_callback_term(int signo);
void *thread_callback(void * arg);


static volatile buffer b;
static volatile int global_term = 0;
static  pthread_mutex_t write_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t write_cond = PTHREAD_COND_INITIALIZER;


int main()
{

    signal(SIGINT, signal_callback); 
    signal(SIGTERM, signal_callback_term); 
    driver_example();

    return 0;
}






void driver_example()
{
    //signal(SIGINT|SIGTERM, signal_callback); 
    buffer local_b = init_buffer(16);

    if( local_b == NULL)
    {
        printf("buffer error, not running example\n");
        return; 
    }
    b = local_b;  
   
    //initialized here because it looks nice :)
    // still placed in heap tho B) 


    pthread_t thread_1, thread_2;
  
    //bad practice, smh
    int val1 = 1;
    int val2 = 2;
    int retval [] = {0,0};
    args_t args[] = { {b, 1,}, {b,2}};   
    retval[0] = pthread_create( &thread_1, 0, &thread_callback, (void*)& val1);
    retval[1] = pthread_create( &thread_2, 0, &thread_callback, (void*) &val2);
    pthread_join( thread_1, 0);
    pthread_join( thread_2, 0);

    buffer_free(b);

}


void *thread_callback(void * arg)
{
    
    for(;;)
    {
        pthread_mutex_lock(&write_mutex);
        //TODO: print thread id
        //TODO debug, 
        //receive buffer_add error code, wait for user to empty buffer 
        if( global_term)
        {
            pthread_mutex_unlock(&write_mutex);
            break; 
        }


        if( buffer_add( b, *(int*)arg))
        {
            fprintf(stderr, "User: send SIGINT ( CTRL-C)\n");
            pthread_cond_wait(&write_cond, &write_mutex);
        }
         
        pthread_mutex_unlock(&write_mutex);

//        if( global_term ) 
//        {
//            //end program
//            break;
//        }
    }
}



void signal_callback(int signo)
{
    
    //switch(signo)
    //{
    //    case SIGINT:  //issue; 
            buffer_remove(b);
            pthread_cond_signal(&write_cond);
            signal(SIGINT, signal_callback); 
    //        break;
    //    case SIGTERM:
    //        fprintf(stdout, "**(Mr. Burns void) excellent...");
    //        global_term = buffer_free(b);
    //        break;
    //    default:
    //        fprintf(stderr, "undefine signo.... oops\n");        
    //}
    return;
}



void signal_callback_term(int signo)
{
    fprintf(stdout, "**(Mr. Burns void) excellent...");
    global_term = 1;
}
