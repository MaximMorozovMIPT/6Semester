#include "stdio.h"
#include "stdlib.h"

int main()
{
    double realExp = 2.7182818284590452353602874713527;
    double e = 1;
    unsigned long long fact = 1;
    for (int i = 1; i <= 20; i++)
    {
        fact *= i;
        e += 1./ fact;
    }
    printf("Euler number is %.17f \n", e);
    printf("Standart exp is %.17f ", realExp);
    return 0;
}
