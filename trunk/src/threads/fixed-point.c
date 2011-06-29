#include <stdio.h>
#include "fixed-point.h"

int64_t 
INT2FLOAT(int n)
{
  return n * PARA ;
}

int 
FLOAT2INTZ(int64_t x)
{
  return x / PARA;
}

int 
FLOAT2INTN(int64_t x)
{
  if (x > 0)  
    return (x + PARA / 2) / PARA;
  else 
    return (x - PARA / 2) / PARA;
}

int64_t 
FADDF(int64_t x,int64_t y)
{ 
  return x + y;
}

int64_t 
FSUBF(int64_t x,int64_t y)
{
  return x - y;
}

int64_t 
FADDI(int64_t x,int n)
{
  return x + n * PARA;
}

int64_t 
FSUBI(int64_t x,int n)
{
  return x - n * PARA;
}
  
int64_t 
FMULF(int64_t x,int64_t y)
{
  return ((int64_t) x) * y / PARA;
}

int64_t 
FMULI(int64_t x,int n)
{
  return x * n;
}

int64_t 
FDIVF(int64_t x,int64_t y)
{
  return ((int64_t) x) * PARA / y ;
}
  
int64_t 
FDIVI(int64_t x,int n)
{
  return x / n;
}
