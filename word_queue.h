#ifndef __WORD_QUEUE_H__
#define __WORD_QUEUE_H__

struct word_queue_node
{
    char* word;
    struct word_queue_node* prev;
    struct word_queue_node* next;
};
typedef struct word_queue_node word_queue_node_t; 

struct word_queue
{
    word_queue_node_t* front;
    word_queue_node_t* back;
};

typedef struct word_queue word_queue_t;

void  word_queue_push_copy(word_queue_t* queue, const char* word);
char* word_queue_pop(word_queue_t* queue);
char* word_queue_pop_copy(word_queue_t* queue, char* buffer, int size);
int word_queue_is_empty(word_queue_t* queue);
void word_queue_destroy(word_queue_t* queue);

#endif // __WORD_QUEUE_H__

