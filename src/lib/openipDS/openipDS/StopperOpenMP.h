#ifndef _STOPPER_OPENMP_H_
#define _STOPPER_OPENMP_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <openipDS/Stopper.h>

/**
  * StopperOpenMP is a simple tool to measure time intervals
  */
namespace openip
{
class StopperOpenMP: public Stopper
{
public:
    /**
      * starts the stopper
      */
    virtual void start();

    /**
      * stops the stopper
      */
    virtual void stop();

    /**
      * converts to time
      */
    virtual void convertToTime();

    double begin;
    
    double end;
};
}
#endif
