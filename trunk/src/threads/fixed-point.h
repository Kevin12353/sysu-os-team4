#ifndef FIXED_POINT
#define FIXED_POINT

#define PARA 16384

typedef int64_t fp;

fp INT2FLOAT(int n);
int FLOAT2INTZ(fp x);
int FLOAT2INTN(fp x);
fp FADDF(fp x,fp y);
fp FSUBF(fp x,fp y);
fp FADDI(fp x,int n);
fp FSUBI(fp x,int n);
fp FMULF(fp x,fp y);
fp FMULI(fp x,int n);
fp FDIVF(fp x,fp y);
fp FDIVI(fp x,int n);

#endif
