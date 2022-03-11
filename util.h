#ifndef _UTIL_H
#define _UTIL_H

#define RETURN_VALUE_IF_TRUE(condition, value)                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (condition)                                                                                                 \
        {                                                                                                              \
            return value;                                                                                              \
        }                                                                                                              \
    } while (0)

#endif //_UTIL_H
