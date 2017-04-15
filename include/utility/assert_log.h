#pragma once
#include "string_utility.h"
#ifdef assert_disable
#define assert_log(cond, msg, return_value) ((void)0)
#else
#define assert_log(cond, msg, return_value) \
do \
{ \
    if ( !( cond ) ) \
    { \
        utility::log( "Assert failed: %s", msg ); \
        assert( cond ); \
        return_value; \
    } \
} while ( 0 )
#endif