
#ifndef __CHARACTER_READER_H__
#define __CHARACTER_READER_H__

enum NextCharError_e
{
    NEXT_CHAR_OK,
    NEXT_CHAR_EOF,
    NEXT_CHAR_INTERRUPTED_EXCEPTION
};

typedef enum NextCharError_e NextCharError;

#endif // __CHARACTER_READER_H__
