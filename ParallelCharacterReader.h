
#ifndef __PARALLELCHARACTERREADER_H__
#define __PARALLELCHARACTERREADER_H__

#include <pthread.h>
#include "word_queue.h"
#include "SlowCharacterReader.h"
#include "FastCharacterReader.h"

void* single_thread_word_read(void *arg);

struct thread_sync
{
    pthread_mutex_t* mutex;
    pthread_cond_t* condition_variable;
    word_queue_t* queue;
    int active_thread_counter;
};

typedef struct thread_sync thread_sync_t;

struct thread_args
{
    thread_sync_t* sync;
    int number; // for debug
    struct FastCharacterReader* reader;
};

typedef struct thread_args thread_args_t;

NextCharError parallel_thread_get_single_word(thread_sync_t* thread_sync, char* word);

#endif //__PARALLELCHARACTERREADER_H__
