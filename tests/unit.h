#ifndef _UNIT_H_INCLUDED_
#define _UNIT_H_INCLUDED_

#include <stdlib.h>

unsigned ERROR = 0;
unsigned ALL = 0;

#define CRED "\x1b[0;31m"
#define CGRN "\x1b[0;32m"
#define CRST "\x1b[0m"

#define DBLD "\x1b[1m"

#define SAYF(x...) printf(x);

#define ASSERT_INFO do {                                                              \
    ALL++;                                                                            \
  } while (0)

#define ASSERT_ERROR(x...) do {                                                       \
    ERROR++;                                                                          \
    SAYF(CRED DBLD "ASSERTION ERROR: " x);                                            \
    SAYF(CRST "Location: %s(), %s:%u\n\n",                                            \
      __FUNCTION__, __FILE__, __LINE__);                                              \
  } while (0)

#define ASSERT_EQUAL_INT(a, b) do {                                                   \
    ASSERT_INFO;                                                                      \
    if ((a) != (b)) {                                                                 \
      ASSERT_ERROR("%i != %i\n", (a), (b));                                           \
    }                                                                                 \
  } while (0)

#define ASSERT_EQUAL_UINT(a, b) do {                                                  \
    ASSERT_INFO;                                                                      \
    if ((a) != (b)) {                                                                 \
      ASSERT_ERROR("%u != %u\n", (a), (b));                                           \
    }                                                                                 \
  } while (0)

#define UNIT_STATUS_AND_EXIT do {                                                     \
    SAYF(DBLD "Status: %u PASSED, %u FAILED\n\n" CRST, ALL - ERROR, ERROR);           \
    ERROR > 0 ? exit(1) : exit(0);                                                    \
  } while (0)

#endif  /* _UNIT_H_INCLUDED_ */

