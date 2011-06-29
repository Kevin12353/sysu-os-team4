#ifndef FIXED_POINT
#define FIXED_POINT

#define PARA 16384

int64_t INT2FLOAT(int n);  //convert int to float
int FLOAT2INTZ(int64_t x);  //convert float to int, throwing
int FLOAT2INTN(int64_t x);  //convert float to int, rounding
int64_t FADDF(int64_t x,int64_t y);  //float plus float
int64_t FSUBF(int64_t x,int64_t y);  //float sub float
int64_t FADDI(int64_t x,int n);  //float plus int
int64_t FSUBI(int64_t x,int n);  //float sub int
int64_t FMULF(int64_t x,int64_t y);  //float mul float
int64_t FMULI(int64_t x,int n);  //float mul int
int64_t FDIVF(int64_t x,int64_t y);  //float div float
int64_t FDIVI(int64_t x,int n);  //float div int


#endif
