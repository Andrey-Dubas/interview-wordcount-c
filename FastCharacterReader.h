#ifndef __FAST_CHARACTER_READER_H__
#define __FAST_CHARACTER_READER_H__

#include "CharacterReader.h"

struct FastCharacterReader
{
    char* buffer;
    int offset;
};

void FastCharacterReader_init(struct FastCharacterReader* reader);
NextCharError nextCharacterFast(struct FastCharacterReader* reader, char* ch);
void closeFast(struct FastCharacterReader* reader);


#endif // __FAST_CHARACTER_READER_H__
