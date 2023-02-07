#ifndef _FUNCTION_H
#define _FUNCTION_H

// keeps parameters to a function for later use (helps remove the unused
// parameter compiler warning)
#define UNUSED(x) (void)(x)

void panic(char* exception_message);

#endif
