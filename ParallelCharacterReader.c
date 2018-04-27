#include <pthread.h>

#include "vector.h"
#include "WordCount.h"
#include "ParallelCharacterReader.h"

#include <stdio.h>
#include <errno.h>

void* single_thread_word_read(void* args)
{
    thread_args_t* thread_args = (thread_args_t*) args;
    NextCharError error;
    
    char word[255];

    do
    {
        error = get_single_word(thread_args->reader, word);
        // if (!((word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z')))
        // {
        //     if (error != NEXT_CHAR_OK)
        //     {
        //         pthread_mutex_lock(thread_args->sync->mutex);
        //         thread_args->sync->active_thread_counter--;
        //         pthread_mutex_unlock(thread_args->sync->mutex);
        //         pthread_cond_signal(thread_args->sync->condition_variable);
        //         break;
        //     }
        //     continue;
        // }

        // printf("single_thread_word_read: \'%s\'\n", word);

        pthread_mutex_lock(thread_args->sync->mutex);

        word_queue_push_copy(thread_args->sync->queue, word);
        *word = 0;

        if (error != NEXT_CHAR_OK)
        {
            thread_args->sync->active_thread_counter--;
        }

        pthread_mutex_unlock(thread_args->sync->mutex);
        pthread_cond_signal(thread_args->sync->condition_variable);

    } while (error == NEXT_CHAR_OK);



    return 0;
}

NextCharError parallel_thread_get_single_word(thread_sync_t* thread_sync, char* word)
{
    NextCharError char_error = NEXT_CHAR_OK;
    word_queue_t* queue = thread_sync->queue;

    pthread_mutex_lock(thread_sync->mutex);
    while (word_queue_is_empty(thread_sync->queue))
    {
        /*int ret = */
        pthread_cond_wait(thread_sync->condition_variable, thread_sync->mutex);
    }

    word = word_queue_pop_copy(thread_sync->queue, word, 255);
    if (!word)
    {
        // TODO:

    }
    char_error = thread_sync->active_thread_counter || !word_queue_is_empty(queue) ? NEXT_CHAR_OK : NEXT_CHAR_EOF;


    pthread_mutex_unlock(thread_sync->mutex);

    return char_error;
}
