#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int num_of_iterations = 20;
    double realExp = 2.7182818284590452353602874713527;
    double euler_number = 0;
    double partial_e;

    MPI_Status status;
    int ierr = MPI_Init(&argc, &argv);
    int num_procs, my_id, avg_rows_per_process, sender, num_rows_to_receive;
    int root_process = 0;
    int send_data_tag = 2001;
    int return_data_tag = 2002;

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    avg_rows_per_process = num_of_iterations / num_procs;

    if(my_id == root_process)
    {
        unsigned long long fact = 1;
        for(int i = 1; i < avg_rows_per_process; i++)
        {
            euler_number += 1. / fact;
            fact *= i;
        }

        printf("sum %.15f calculated by root process\n", euler_number);

        for(int flow_id = 1; flow_id < num_procs; flow_id++)
        {
            ierr = MPI_Recv(&partial_e, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
                  return_data_tag, MPI_COMM_WORLD, &status);

            sender = status.MPI_SOURCE;

            printf("Partial sum %.17f returned from process %i\n", partial_e, sender);

            euler_number += partial_e;
        }

         printf("The euler number value is: %.17f\n", euler_number );
         printf("The standart value is:     %.17f\n", realExp );
    }
    else
    {
        partial_e = 0;
        unsigned long long fact = 1;
        int end_it = (my_id + 1) * avg_rows_per_process;
        if ((my_id + 1) * avg_rows_per_process > num_of_iterations)
        {
            end_it = num_of_iterations;
        }
        fact = 1;
        for(int i = 1; i < my_id * avg_rows_per_process; i++)
        {
            fact *= i;
        }
        for(int i = my_id * avg_rows_per_process; i < (my_id + 1) * avg_rows_per_process; i++)
        {
            partial_e += 1. / fact;
            fact *= i;
        }


        //printf("I am in process number %d \n", my_id);
        ierr = MPI_Send( &partial_e, 1, MPI_DOUBLE, root_process,
           return_data_tag, MPI_COMM_WORLD);
    }

    ierr = MPI_Finalize();
    return 0;
}
