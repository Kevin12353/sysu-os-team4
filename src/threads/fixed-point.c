#include <stdio.h>
#include "fixed-point.h"

fp 
INT2FLOAT(int n)
{
  return n*PARA ;
}

int 
FLOAT2INTZ(fp x)
{
  return x / PARA;
}

int 
FLOAT2INTN(fp x)
{
  if (x > 0)  
    return (x + PARA / 2) / PARA;
  else 
    return (x - PARA / 2) / PARA;
}

fp 
FADDF(fp x,fp y)
{ 
  return x+y;
}

fp 
FSUBF(fp x,fp y)
{
  return x-y;
}

fp 
FADDI(fp x,int n)
{
  return x + n * PARA;
}

fp 
FSUBI(fp x,int n)
{
  return x - n * PARA;
}
  
fp 
FMULF(fp x,fp y)
{
  return ((int64_t) x) * y / PARA;
}

fp 
FMULI(fp x,int n)
{
  return x * n;
}

fp 
FDIVF(fp x,fp y)
{
  return ((int64_t) x) * PARA / y ;
}
  
fp 
FDIVI(fp x,int n)
{
  return x / n;
}
