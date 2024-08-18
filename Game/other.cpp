#include "other.h"

int Get_2_N_POW(int n)
{
    int res=-1;
    while(n){
        n>>=1;
        ++res;
    }
    return res;
}
