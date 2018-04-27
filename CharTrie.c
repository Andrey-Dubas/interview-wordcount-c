#include <stdlib.h>
#include <string.h>

#include "CharTrie.h"

#include <stdio.h>
#include <unistd.h>

unsigned int trie_node_index(char letter)
{
    if (letter >= 'A' && letter <= 'Z') letter += 32;
    return (unsigned int) letter;
}


trie_node_t* trie_node_add_word_impl(trie_node_t* current_node, trie_node_t* prev_node, const char* word)
{
    if (!current_node)
    {
        current_node = (trie_node_t*) malloc(sizeof(trie_node_t));
        memset(current_node, 0x00, sizeof(trie_node_t));
        current_node->letter = trie_node_index(*word);
        current_node->prev = prev_node;
    }

    char next_char = *(word+1);
    if (next_char)
    {
        unsigned int next_index = trie_node_index(next_char);
        current_node->next[next_index] = trie_node_add_word_impl(current_node->next[next_index], current_node, word+1);
    }
    else
    {
        current_node->repetitions++;
    }
    return current_node;
}

void trie_node_add_word(trie_node_t* root, const char* word)
{
    unsigned int index = trie_node_index(*word);
    root->next[index] = trie_node_add_word_impl(root->next[index], root, word);
}

void trie_node_get_word(trie_node_t* current_node, char* buffer)
{
    int len = 0;
    trie_node_t* it = current_node;
    while (it)
    {
        len++;
        it = it->prev;
    }

    buffer = buffer+len-2;
    *(buffer+1) = 0;
    it = current_node;
    do
    {
        *(buffer--) = it->letter;
    }
    while ((it = it->prev)->prev); // it is root

}

void trie_node_destroy(trie_node_t* current_node)
{
    for (int i = 0; i < 255; ++i)
    {
        if (current_node->next[i])
        {
            trie_node_destroy(current_node->next[i]);
            free(current_node->next[i]);
        }
    }
}




trie_node_depth_iterator_t* trie_node_depth_next(trie_node_depth_iterator_t* it)
{
    unsigned int i = it->next
        ? trie_node_index(it->next->current_node->letter)+1 // in case it just returned from adjacent node
        : 97;

    for (; i < 255; ++i)
    {
        if (it->current_node->next[i])
        {
            trie_node_depth_iterator_t* next_it = (trie_node_depth_iterator_t*) malloc(sizeof(*it));
            memset(next_it, 0x00, sizeof(*next_it));
            next_it->current_node = it->current_node->next[i];

            next_it->prev = it;

            free(it->next);
            it->next = next_it;

            return next_it;
        }
    }
    free(it->next);

    if (it->prev)
    {
        return trie_node_depth_next(it->prev);
    }
    else {
        free(it); // TODO: need??
        return 0; // we've done
    }
}

trie_node_depth_iterator_t* trie_node_begin(trie_node_t* node)
{
        trie_node_depth_iterator_t* next_it = (trie_node_depth_iterator_t*) malloc(sizeof(trie_node_depth_iterator_t));
        memset(next_it, 0x00, sizeof(*next_it));
        next_it->current_node = node;
        return next_it;
}

void trie_node_word(trie_node_t* node, char* buffer)
{
    do
    {
        *(buffer++) = node->letter;
        node = node->prev;
    } while (node);
    *buffer = '\0';
}

int trie_node_max_repetition(trie_node_t* root)
{
    trie_node_depth_iterator_t* it = trie_node_begin(root);
    int value = 0;
    do
    {
        value = value > it->current_node->repetitions ? value : it->current_node->repetitions;

    } while ( (it = trie_node_depth_next(it)) );

    return value;
}

void trie_node_counter(trie_node_t* root, int* counters, int max_repetitions)
{
    memset(counters, 0x00, sizeof(int) * (max_repetitions+1));

    // trie_node_depth_iterator_t* it = trie_node_depth_next(trie_node_begin(root));
    trie_node_depth_iterator_t* it = trie_node_begin(root);
    do
    {
        counters[it->current_node->repetitions]++;
    } while ( (it = trie_node_depth_next(it)) );
    counters[0] = 0;

}

void trie_node_print(trie_node_t* root, int depth)
{
    if (depth == 0) return;

    for (int i = 0; i < 255; ++i)
    {
        if (root->next[i])
        {
            trie_node_print(root->next[i], 1);
        }
    }

    if (root->repetitions)
    {
        char written[255];
        trie_node_get_word(root, written);
        printf("trie_node_print: %s, reps: %d\n", written, root->repetitions);
    }

}
