#ifndef ___MSVC_GETTIMEOFDAY_H__
#define ___MSVC_GETTIMEOFDAY_H__


#include <time.h>
#include <windows.h>

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

//WinRT não possui essa struct na windows.h
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
struct timeval {
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
};
#endif 

struct timezone_s
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

typedef struct timezone_s timezone_t;

static int gettimeofday(struct timeval *tv, timezone_t *tz)
{
  FILETIME ft;
  unsigned __int64 tmpres = 0;

  if (NULL != tv)
  {
    GetSystemTimeAsFileTime(&ft);

    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;

    tmpres /= 10;  /*convert into microseconds*/
    /*converting file time to unix epoch*/
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }

  if (NULL != tz)
  {
    long tzSec;
	int daylight;
    
	if(!_get_timezone(&tzSec))
		tz->tz_minuteswest = tzSec / 60;

	if(!_get_daylight(&daylight))
		tz->tz_dsttime = daylight;
  }

  return 0;
}

#endif