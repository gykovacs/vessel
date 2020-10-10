#include <openipDS/StopperOpenMP.h>

#include <sstream>
#include <iostream>
#include <omp.h>

#define _CRT_SECURE_NO_WARNINGS

namespace openip
{
void StopperOpenMP::start()
{
  begin= omp_get_wtime();
}
void StopperOpenMP::stop()
{
  end= omp_get_wtime();
}
void StopperOpenMP::convertToTime()
{
  double d= end - begin;

  d*=1000;

  ms= d;
  s= ms / 1000;
  m= s / 60;
  h= m / 60;

  ms= ms % 1000;
  s= s % 60;
  m= m % 60;
}
}