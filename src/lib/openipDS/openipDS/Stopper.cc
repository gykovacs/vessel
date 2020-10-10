#include <openipDS/Stopper.h>

#include <time.h>
#include <sstream>
#include <iostream>
#include <omp.h>

#define _CRT_SECURE_NO_WARNINGS

namespace openip
{
    int tprintf(const char* fmt, ...)
    {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        va_list arg;
        va_start(arg, fmt);
        if ( omp_get_num_threads() <= 1 )
            printf("%02d:%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        else
            printf("%02d:%02d:%02d %d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, omp_get_thread_num());
        vprintf(fmt, arg);
        va_end(arg);

        fflush(stdout);

        return 0;
    }

    int setprintf(const char* fmt, ...)
    {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        va_list arg;
        va_start(arg, fmt);
        fprintf(stderr, "%02d:%02d:%02d ERROR: ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        vfprintf(stderr, fmt, arg);
        va_end(arg);

        fflush(stderr);

        return 0;
    }

    int ftprintf(FILE* f, const char* fmt, ...)
    {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        va_list arg;
        va_start(arg, fmt);
        fprintf(f, "%02d:%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        vfprintf(f, fmt, arg);
        va_end(arg);

        fflush(f);

        return 0;
    }

    int eprintf(const char* fmt, ...)
    {
        va_list arg;
        va_start(arg, fmt);
        fprintf(stderr, "ERROR: ");
        vprintf(fmt, arg);
        va_end(arg);

        fflush(stderr);

        return 0;
    }

    int feprintf(FILE* f, const char* fmt, ...)
    {
        va_list arg;
        va_start(arg, fmt);
        fprintf(f, "ERROR: ");
        vfprintf(f, fmt, arg);
        va_end(arg);

        fflush(f);

        return 0;
    }

    int wprintf(const char* fmt, ...)
    {
        va_list arg;
        va_start(arg, fmt);
        fprintf(stderr, "WARNING: ");
        vprintf(fmt, arg);
        va_end(arg);

        fflush(stderr);

        return 0;
    }

    int weprintf(FILE* f, const char* fmt, ...)
    {
        va_list arg;
        va_start(arg, fmt);
        fprintf(f, "WARNING: ");
        vfprintf(f, fmt, arg);
        va_end(arg);

        fflush(f);

        return 0;
    }

    int etprintf(const char* fmt, ...)
    {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        va_list arg;
        va_start(arg, fmt);
        fprintf(stderr, "%02d:%02d:%02d ERROR: ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        vprintf(fmt, arg);
        va_end(arg);

        fflush(stdout);

        return 0;
    }

    int fetprintf(FILE* f, const char* fmt, ...)
    {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        va_list arg;
        va_start(arg, fmt);
        fprintf(f, "%02d:%02d:%02d ERROR: ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        vfprintf(f, fmt, arg);
        va_end(arg);

        fflush(f);

        return 0;
    }

    int wtprintf(const char* fmt, ...)
    {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        va_list arg;
        va_start(arg, fmt);
        fprintf(stderr, "%02d:%02d:%02d WARNING: ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        vprintf(fmt, arg);
        va_end(arg);

        fflush(stderr);

        return 0;
    }

    int fwtprintf(FILE* f, const char* fmt, ...)
    {
        time_t rawtime;
        struct tm * timeinfo;
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        va_list arg;
        va_start(arg, fmt);
        fprintf(f, "%02d:%02d:%02d WARNING: ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        vfprintf(f, fmt, arg);
        va_end(arg);

        fflush(f);

        return 0;
    }

    std::string getTimeString(char separator)
    {
      time_t rawtime;
      struct tm * timeinfo;
      time ( &rawtime );
      timeinfo = localtime ( &rawtime );
      
      std::stringstream ss;
      ss << timeinfo->tm_hour << separator << timeinfo->tm_min << separator << timeinfo->tm_sec;
      
      return ss.str();
    }
    
    void Stopper::start()
    {
    }
    void Stopper::stop()
    {
    }
    void Stopper::convertToTime()
    {
    }

    std::string Stopper::getElapsedTime()
    {
      convertToTime();
      char buffer[256];

      sprintf(buffer,"%d:%d:%d.%d (h:m:s)", h, m, s, ms);

      return std::string(buffer);
    }
}
