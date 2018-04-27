
#include "vector.h"
#include <stdlib.h>
#include <string.h>

vector_t vector_init()
{
    const int InitialCapacity = 10;

    vector_t result;
    memset((void*)&result, 0x00, sizeof(result));
    result.capacity = InitialCapacity;
    result.word_vector = (char**) malloc(sizeof(char*) * result.capacity);

    return result;
}

void vector_destroy(struct vector* vector)
{

    for (int i = 0; i < vector->size; ++i)
    {
        free(vector->word_vector[i]);
    }
    free(vector->word_vector);
}

int vector_increase(struct vector* vector)
{
    vector->capacity *= 2;
    char** newBuffer = (char**) malloc(sizeof(char*) * vector->capacity);
    if (newBuffer)
    {
        memcpy(newBuffer, vector->word_vector, sizeof(char*)*vector->capacity/2);
        free(vector->word_vector);
        vector->word_vector = newBuffer;
    }
    return newBuffer ? 0 : -1;
}

int vector_push(struct vector* vector, char* word)
{
    int error = 0;
    if (vector->size == vector->capacity)
    {
        error = vector_increase(vector);
    }
    if (0 == error)
    {
        vector->word_vector[vector->size++] = word;
    }
    return error;
}

int vector_push_copy(struct vector* vector, char* word)
{
    char* word_copy = calloc(sizeof(char), (strlen(word)+1));
    strcpy(word_copy, word);
    return vector_push(vector, word_copy);
}

char** vector_begin(struct vector* vector)
{
    return vector->word_vector;
}

char** vector_end(struct vector* vector)
{
    return vector->word_vector + vector->size;
}
