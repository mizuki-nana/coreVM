#ifndef _ASSERT_THROW
  #ifdef __APPLE__
    #define _ASSERT_THROW(stmt, exc) ASSERT_THROW((stmt), (exc))
  #else
    #define _ASSERT_THROW(stmt, exc) ASSERT_ANY_THROW((stmt))
  #endif
#endif
