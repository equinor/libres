#define HAVE_GMTIME_R
#define HAVE_TIMEGM
#define HAVE_LOCALTIME_R
#define HAVE_REALPATH
#define HAVE__USLEEP
#define HAVE_FNMATCH
#define HAVE_FTRUNCATE
#define HAVE_POSIX_CHDIR
/* #undef HAVE_WINDOWS_CHDIR */
#define HAVE_POSIX_GETCWD
/* #undef HAVE_WINDOWS_GETCWD */
#define HAVE_TZNAME
/* #undef HAVE_ROUND */
#define HAVE_VA_COPY
#define HAVE_SIGBUS
#define HAVE_PTHREAD
#define HAVE_PID_T
#define HAVE_EXECINFO
#define HAVE_FORK
#define HAVE_POSIX_MKDIR
/* #undef HAVE_WINDOWS_MKDIR */
#define HAVE_GETPWUID
#define HAVE_FSYNC
#define HAVE_CHMOD
#define HAVE_MODE_T
#define HAVE_CXX_SHARED_PTR
#define HAVE_POSIX_UNLINK
/* #undef HAVE_WINDOWS_UNLINK */
/* #undef HAVE_SIGHANDLER_T */

#define HAVE_POSIX_ACCESS
/* #undef HAVE_WINDOWS__ACCESS */
/* #undef HAVE_WINDOWS_GET_TEMP_PATH */
/* #undef HAVE_WINDOWS_TZNAME */

#ifdef HAVE_VA_COPY
#define UTIL_VA_COPY(target,src) va_copy(target,src)
#else
#define UTIL_VA_COPY(target,src) target = src
#endif


/*
  The HAVE_UTIL_ABORT_INTERCEPT is required to allow testing of the
  util_abort() functionality; in prinicple this could be implemented
  also in the simple util_abort() function without backtrace
  capabilities, but currently it is only implemented in the full
  util_abort_gnu() function.
*/

#ifdef HAVE_FORK
#ifdef HAVE_PTHREAD
#ifdef HAVE_EXECINFO
#ifdef HAVE_GETPWUID

#define HAVE_BACKTRACE
#define HAVE_UTIL_ABORT_INTERCEPT

#endif
#endif
#endif
#endif
