#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "WordCount.h"
#include "FastCharacterReader.h"
#include "SlowCharacterReader.h"
#include "ParallelCharacterReader.h"
#include "CharTrie.h"
#include "vector.h"

int task_1()
{

    struct FastCharacterReader reader;
    FastCharacterReader_init(&reader);

    trie_node_t* root = collect_words(&reader, simple_reader_decorator);

    unsigned int count;
    trie_node_t** sorted_nodes = word_count(root, &count);
    char word[255];

    for (unsigned int i = 0; i < count; ++i)
    {
        trie_node_get_word(sorted_nodes[i], word);
        printf("word: %s, reps: %d\n", word, sorted_nodes[i]->repetitions);
    }


    trie_node_destroy(root);
    free(root);
    free(sorted_nodes);
    
    closeFast(&reader);
    return 0;
}


int task_2()
{
    thread_sync_t sync;

    const int thread_counter = 10;

    word_queue_t queue;
    memset(&queue, 0x00, sizeof(queue));
    sync.queue = &queue;

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    sync.mutex = &mutex;

    pthread_cond_t condition_variable;
    pthread_cond_init(&condition_variable, NULL);
    sync.condition_variable = &condition_variable;
    
    sync.active_thread_counter = thread_counter;

    pthread_t threads[thread_counter];
    thread_args_t thread_args[thread_counter];
    struct FastCharacterReader reader[thread_counter];

    for (int i = 0; i < thread_counter; ++i)
    {
        FastCharacterReader_init(&reader[i]);
        thread_args[i].sync = &sync;
        thread_args[i].reader = &reader[i];
        thread_args[i].number = i;

        pthread_create(threads+i, NULL, single_thread_word_read, (void*) &thread_args[i]);
    }


    trie_node_t* root = collect_words(&sync, parallel_reader_decorator);


    for (int i = 0; i < thread_counter; ++i)
    {
        pthread_join(threads[i], NULL);
        closeFast(&reader[i]);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition_variable);


    unsigned int count;
    trie_node_t** sorted_nodes = word_count(root, &count);
    char word[255];

    for (unsigned int i = 0; i < count; ++i)
    {
        trie_node_get_word(sorted_nodes[i], word);
        printf("word: %s, reps: %d\n", word, sorted_nodes[i]->repetitions);
        //offer[len-1] = 0;
        // memset(word, 0x00, 100);
    }
    free(sorted_nodes);


    trie_node_destroy(root);
    free(root);


    return 0;
}

int main()
{
    // task_1();
    // printf("\n\n\n");
    task_2();
}

