#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <sys/time.h>

void merge(int *, int *, int, int, int);
void mergeSort(int *toSort, int *buff, int left, int right);
void RandomArray(int *sortArray, int numToSort);
void BestArray(int *sortArray, int numToSort);
void WorstArray(int *sortArray, int numToSort);

enum value{
    MAX_VALUE = 50000000
};

int main(int argc, char** argv)
{
    int world_rank, world_size;

    int ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int numToSort = 30000000;
    int *sortArray;
    struct timeval begin, end;
    if (world_rank == 0)
    {
        sortArray = malloc(numToSort * sizeof(int));
        //Use one of 3 functions
        //RandomArray(sortArray, numToSort);
        //BestArray(sortArray, numToSort);
        WorstArray(sortArray, numToSort);
        gettimeofday(&begin, 0);
    }

    int size = numToSort / world_size;
    int *sub_array = malloc(size * sizeof(int));
    ierr = MPI_Scatter(sortArray, size, MPI_INT, sub_array, size, MPI_INT, 0, MPI_COMM_WORLD);

    int *tmp_array = malloc(size * sizeof(int));
    mergeSort(sub_array, tmp_array, 0, (size - 1));
    
    int *sorted = NULL;
    if (world_rank == 0)
    {
        sorted = malloc(numToSort * sizeof(int));
    }

    ierr = MPI_Gather(sub_array, size, MPI_INT, sorted, size, MPI_INT, 0, MPI_COMM_WORLD);

    if(world_rank == 0)
    {
        int *other_array = malloc(numToSort * sizeof(int));

        for(int i = 0; i < world_size; i++)
        {
            merge(sorted, other_array, 0, i * size - 1, (i + 1) * size - 1);
        }
        gettimeofday(&end, 0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds*1e-6;
        printf("Time measured: %.3f seconds.\n", elapsed);

        free(sorted);
        free(other_array);
        free(sortArray);
    }
    free(sub_array);
    free(tmp_array);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}

void merge(int *toSort, int *buff, int left, int middle, int right)
{
    int lIter = left;
    int mIter = left;
    int rIter = middle + 1;

    while((lIter <= middle) && (rIter <= right))
    {
        if(toSort[lIter] <= toSort[rIter])
        {
            buff[mIter] = toSort[lIter];
            lIter++;
        }
        else
        {
            buff[mIter] = toSort[rIter];
            rIter++;
        }
        mIter++;
    }

    if(middle < lIter)
    {
        for(int i = rIter; i <= right; i++)
        {
                buff[mIter] = toSort[i];
                mIter++;
        }
    }
    else
    {
        for(int i = lIter; i <= middle; i++)
        {
            buff[mIter] = toSort[i];
            mIter++;
        }
    }

    for(int i = left; i <= right; i++)
    {
        toSort[i] = buff[i];
    }
}

void mergeSort(int *toSort, int *buff, int left, int right)
{
    if (left < right)
    {
        int middle = (left + right) / 2;
        mergeSort(toSort, buff, left, middle);
        mergeSort(toSort, buff, (middle + 1), right);
        merge(toSort, buff, left, middle, right);
    }
}

void RandomArray(int *sortArray, int numToSort)
{
    srand(time(NULL));
    for(int i = 0; i < numToSort; ++i)
    {
        sortArray[i] = rand() % MAX_VALUE;
    }
}

void BestArray(int *sortArray, int numToSort)
{
    for(int i = 0; i < numToSort; ++i)
    {
        sortArray[i] = i;
    }
}

void WorstArray(int *sortArray, int numToSort)
{
    FILE *fp = fopen("numbers.txt", "r");
    for(int i = 0; i < numToSort; ++i)
    {
        fscanf(fp, "%d", &sortArray[i]);
    }
}
