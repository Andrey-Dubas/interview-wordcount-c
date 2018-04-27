#ifndef __CHARTRIE_H__
#define __CHARTRIE_H__

struct trie_node 
{
    char letter;
    unsigned int repetitions;
    struct trie_node* prev;
    struct trie_node* next[255];
};

typedef struct trie_node trie_node_t;

unsigned int trie_node_index(char letter);
void trie_node_add_word(trie_node_t* current_node, const char* word);
void trie_node_get_word(trie_node_t* current_node, char* buffer);
void trie_node_destroy(trie_node_t* current_node);



struct trie_node_depth_iterator
{
    struct trie_node_depth_iterator* prev;
    struct trie_node_depth_iterator* next;
    trie_node_t* current_node;
};

typedef struct trie_node_depth_iterator trie_node_depth_iterator_t;
trie_node_depth_iterator_t* trie_node_depth_next(trie_node_depth_iterator_t* it);
trie_node_depth_iterator_t* trie_node_begin(trie_node_t* it);
void trie_node_word(trie_node_t* node, char* buffer);

int trie_node_max_repetition(trie_node_t* root);
void trie_node_counter(trie_node_t* root, int* counters, int max_repetitions);

void trie_node_print(trie_node_t* root, int depth);

#endif // __CHARTRIE_H__
