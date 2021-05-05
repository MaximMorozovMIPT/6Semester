#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>

struct integrate
{
    double begin, end, h, sum, num_of_steps;
};

double Function(double x)
{
    return sin(1.0 / x);
}

void* Integrate(void *args) 
{
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    struct integrate* data = (struct integrate*)args;
    double sum = 0;
    double local_x = data->begin;
    for(int i = 1; i < data->num_of_steps; ++i)
    {
        local_x += data->h;
        sum += 2 * Function(local_x);
    }
    sum += Function(data->begin);
    sum += Function(data->end);
    sum *= data->h;
    sum /= 2;
    data->sum = sum;
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds*1e-6;
    printf("Time measured in thread: %.3f seconds.\n", elapsed);
    return 0;
}
 
int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("No epsilon\n");
        return 1;
    }
    int num_of_thread = 4;
    double max_der = 1e12;//maybe unused
    int num_of_steps = 1e4;//for thread
    double eps = strtod(argv[1], NULL);
    pthread_t* thread = (pthread_t*)calloc(num_of_thread, sizeof(pthread_t));
    int status, status_addr;
    struct integrate* data = (struct integrate*)calloc(num_of_thread, sizeof(struct integrate));
  
    for(;;)
    {
        double coub_delta = eps * 3 * num_of_steps * num_of_steps / max_der;
        double delta = pow(coub_delta, 1.0 / 3.0);
        printf("Delta = %.15f", delta);
        if (delta >= 0.25)
        {
            data[0].begin = 0.001;
            data[0].end = 0.25;
            data[0].h = (data[0].end - data[0].begin) / num_of_steps;
            for(int i = 1; i < num_of_thread; ++i)
            {
                data[i].begin = 0.25 * i;
                data[i].end = 0.25 * (i + 1);
                data[i].h = (data[i].end - data[i].begin) / num_of_steps;
            }
            printf(" Good\n");
            break;
        }
        else if(delta <= 0.002)
        {
            num_of_steps *= 10;
            printf(" Too small\n");
            continue;
        }
        else 
        {
            data[0].begin = 0.001;
            data[0].end = delta + data[0].begin;
            data[0].h = (data[0].end - data[0].begin) / num_of_steps;
            delta = (1 - data[0].end) / (num_of_thread - 1);
            for(int i = 1; i < num_of_thread; ++i)
            {
                data[i].begin = data[i - 1].end;
                data[i].end = data[i].begin + delta;
                if(i == num_of_thread - 1)
                {
                    data[i].end = 1;
                }
                data[i].h = (data[i].end - data[i].begin) / num_of_steps;
            }
            printf(" Good\n");
            break;
        }
    }

    for (int i = 0; i < num_of_thread; ++i)
    {
        data[i].sum = 0;
        data[i].num_of_steps = num_of_steps;
    }

    for (int i = 0; i < num_of_thread; ++i)
    {
        status = pthread_create(&thread[i], NULL, Integrate, (void*)&data[i]);
    }

    for (int i = 0; i < num_of_thread; ++i)
    {
        status = pthread_join(thread[i], (void**)&status_addr);
    }
    double res = 0;
    for (int i = 0; i < num_of_thread; ++i)
    {
        res += data[i].sum;
    }
    printf("Result integral = %.15f\n", res);
    return 0;
}