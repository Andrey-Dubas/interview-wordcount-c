#include <string.h>
#include <stdlib.h>

#include "word_queue.h"


void  word_queue_push_copy(word_queue_t* queue, const char* word)
{
    int len = strlen(word);
    char* dst = calloc(sizeof(char), len+1);

    word_queue_node_t* node = malloc(sizeof(word_queue_node_t));
    memset(node, 0x00, sizeof(word_queue_node_t));
    node->word = strcpy(dst, word);

    if (word_queue_is_empty(queue))
    {
        queue->back = queue->front = node;
    }
    else
    {
        queue->front->next = node;
        node->prev = queue->front;

        queue->front = node;
    }
}

char* word_queue_pop_copy(word_queue_t* queue, char* buffer, int size)
{
    char* cpy = word_queue_pop(queue);
    if (strlen(cpy) < size)
    {
        strcpy(buffer, cpy);
        free(cpy);
        return buffer;
    }
    else
    {
        // TODO: ???
        return 0;
    }

}

char* word_queue_pop(word_queue_t* queue)
{
    int is_last = queue->back == queue->front;
    word_queue_node_t* node = queue->back;
    queue->back = queue->back->next;
    char* word = node->word;
    memset(node, 0x00, sizeof(*node));
    free(node);

    if (is_last)
    {
        queue->back = queue->front = 0;
    }
    return word;
}

int  word_queue_is_empty(word_queue_t* queue)
{
    return !queue->back;
}

void word_queue_destroy(word_queue_t* queue)
{
    word_queue_node_t* node = queue->front;
    do
    {
        node = node->next;
        free(node->prev);
    } while (!node->next);
    free(node);
}
