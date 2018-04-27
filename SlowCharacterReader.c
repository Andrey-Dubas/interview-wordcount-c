#include <unistd.h>
#include "SlowCharacterReader.h"

NextCharError nextCharacterSlow(struct FastCharacterReader* reader, char* ch)
{
    usleep(50000);
    return nextCharacterFast(reader, ch);
}
