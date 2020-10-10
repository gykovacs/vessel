#include <openipDS/StopperCPU.h>

#include <sstream>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

namespace openip
{
    void StopperCPU::start()
    {
      begin= clock();
    }
    void StopperCPU::stop()
    {
      end= clock();
    }
    void StopperCPU::convertToTime()
    {
      float d= end - begin;

      d*=1000;
      d/=CLOCKS_PER_SEC;

      ms= d;
      s= ms / 1000;
      m= s / 60;
      h= m / 60;

      ms= ms % 1000;
      s= s % 60;
      m= m % 60;
    }
}