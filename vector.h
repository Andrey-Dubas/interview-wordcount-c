#ifndef __VECTOR_C__
#define __VECTOR_C__

struct vector
{
    char** word_vector;
    unsigned int reps;
    unsigned int capacity;
    unsigned int size;
};

typedef struct vector vector_t;

struct vector vector_init(void);
void vector_destroy(struct vector* vector);
int vector_increase(struct vector* vector);
int vector_push(struct vector* vector, char* word);
int vector_push_copy(struct vector* vector, char* word);
char** vector_begin(struct vector* vector);
char** vector_end(struct vector* vector);

#endif // __VECTOR_C__
