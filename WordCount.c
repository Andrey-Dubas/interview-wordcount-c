
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "vector.h"
#include "CharacterReader.h"
#include "FastCharacterReader.h"
#include "ParallelCharacterReader.h"
#include "WordCount.h"

#include "CharTrie.h"

NextCharError simple_reader_decorator(void* data, char* word)
{
    return get_single_word((struct FastCharacterReader*) data, word);
}

NextCharError parallel_reader_decorator(void* data, char* word)
{
    return parallel_thread_get_single_word((thread_sync_t*) data, word);
}

void trie_node_sort(trie_node_t* root, int* counters, int max_repetitions, trie_node_t** sorted)                          
{                                                                                                             
    memset(sorted, 0x00, sizeof(trie_node_t*) * counters[max_repetitions-1]);

    trie_node_depth_iterator_t* it = trie_node_begin(root);

    do
    {
        if (it->current_node->repetitions)
        {
            sorted[counters[it->current_node->repetitions-1]++] = it->current_node;
        }
    } while ((it = trie_node_depth_next(it)));

}


trie_node_t* collect_words(void* data, read_decorator_t read_decorator)
{
    char word[255];

    trie_node_t* root = (trie_node_t*) malloc(sizeof(trie_node_t));
    memset(root, 0x00, sizeof(*root));

    NextCharError ret;
    do
    {
        ret = read_decorator(data, &word[0]);
        if (word[0]) { 
            trie_node_add_word(root, &word[0]);
        }
        // free(word);
    } while (ret == NEXT_CHAR_OK);
    
    return root;
}

trie_node_t** word_count(trie_node_t* root, unsigned int* words_amount)
{
    int max_repetitions = trie_node_max_repetition(root);

    int* counters = (int*)malloc((max_repetitions+1) * sizeof(int));
    trie_node_counter(root, counters, max_repetitions);

    for (int i = 1; i < max_repetitions+1; ++i)
    {
        counters[i] += counters[i-1];
    }

    *words_amount = counters[max_repetitions];

    for (int i = 0; i < max_repetitions; ++i)
    {
        counters[i] = *words_amount-counters[i+1];
    }

    trie_node_t** sorted_nodes; 
    sorted_nodes = calloc(*words_amount, sizeof(trie_node_t*));

    trie_node_sort(root, counters, max_repetitions, sorted_nodes);

    free(counters);

    return sorted_nodes;
}

NextCharError get_single_word(struct FastCharacterReader* reader, char* word_buffer)
{
    char ch;
    NextCharError err;
    memset(word_buffer, 0x00, 255);
    int currentSymbol = 0;

    // find beginning of a word
    do {
        ch = 0;
        err = nextCharacterFast(reader, &ch);
    } while (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) && err == NEXT_CHAR_OK);
    word_buffer[currentSymbol++] = ch;

    do {
       err = nextCharacterFast(reader, &ch); 
       word_buffer[currentSymbol++] = ch;
    } while (err == NEXT_CHAR_OK &&
        ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '\''));
    word_buffer[--currentSymbol] = '\0';

    return err;
}
