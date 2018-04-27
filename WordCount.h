#include "vector.h"
#include "FastCharacterReader.h"
#include "CharTrie.h"

#ifndef __WORDCOUNT_C__
#define __WORDCOUNT_C__

typedef NextCharError (*read_decorator_t)(void* data, char* word);

typedef NextCharError *read_character_function_t (struct FastCharacterReader*, char*);

NextCharError simple_reader_decorator(void* data, char* word);
NextCharError parallel_reader_decorator(void* data, char* word);

trie_node_t* collect_words(void* data, read_decorator_t read_decorator);
trie_node_t** word_count(trie_node_t* root, unsigned int* words_amount);
NextCharError get_single_word(struct FastCharacterReader* reader, char* word);

#endif // __WORDCOUNT_C__
