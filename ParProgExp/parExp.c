#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int num_of_iterations = 20;
    double realExp = 2.7182818284590452353602874713527;
    double euler_number = 0;
    double partial_e;
    unsigned long long *factarr;
    unsigned long long *factarr2;
    factarr = (unsigned long long *)calloc(num_of_iterations, sizeof(unsigned long long));
    factarr2 = (unsigned long long *)calloc(num_of_iterations, sizeof(unsigned long long));

    MPI_Status status;
    int ierr = MPI_Init(&argc, &argv);
    int num_procs, my_id, avg_rows_per_process, sender, num_rows_to_receive;
    int root_process = 0;
    int send_data_tag = 2001;
    int return_data_tag = 2002;

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if(my_id == root_process)
    {
        avg_rows_per_process = num_of_iterations / num_procs;


        factarr[0] = 1;
        for (int i = 1; i < num_of_iterations; i++)
        {
            factarr[i] = factarr[i - 1] * i;
            //printf("%llu \n", factarr[i]);
        }

        for(int flow_id = 1; flow_id < num_procs; flow_id++)
        {
            int start_row = flow_id * avg_rows_per_process;
            int end_row = (flow_id + 1) * avg_rows_per_process - 1;

            if((num_of_iterations - end_row) < avg_rows_per_process)
               end_row = num_of_iterations - 1;

            int num_rows_to_send = end_row - start_row + 1;

            ierr = MPI_Send(&num_rows_to_send, 1 , MPI_INT,
                  flow_id, send_data_tag, MPI_COMM_WORLD);
            //printf("num_rows_to_receive %d %d \n", start_row, end_row);
            ierr = MPI_Send(&factarr[start_row], num_rows_to_send, MPI_UNSIGNED_LONG_LONG,
                  flow_id, send_data_tag, MPI_COMM_WORLD);
         }

         for(int i = 0; i < avg_rows_per_process; i++) {
            euler_number += 1. / factarr[i];
         }

         printf("sum %.15f calculated by root process\n", euler_number);

         for(int flow_id = 1; flow_id < num_procs; flow_id++) {

            ierr = MPI_Recv(&partial_e, 1, MPI_DOUBLE, MPI_ANY_SOURCE,
                  return_data_tag, MPI_COMM_WORLD, &status);

            sender = status.MPI_SOURCE;

            printf("Partial sum %.15f returned from process %i\n", partial_e, sender);

            euler_number += partial_e;
         }

         printf("The euler number value is: %.15f\n", euler_number );
         printf("The standart value is:     %.15f\n", realExp );
    }
    else
    {
         ierr = MPI_Recv( &num_rows_to_receive, 1, MPI_INT,
               root_process, send_data_tag, MPI_COMM_WORLD, &status);
         //printf("num_rows_to_receive %d \n", num_rows_to_receive);
         ierr = MPI_Recv( factarr2, num_rows_to_receive, MPI_UNSIGNED_LONG_LONG,
               root_process, send_data_tag, MPI_COMM_WORLD, &status);

         partial_e = 0;
         for(int i = 0; i < num_rows_to_receive; i++) {
            partial_e += 1. / factarr2[i];
         }

         //printf("I am in process number %d \n", my_id);
         ierr = MPI_Send( &partial_e, 1, MPI_DOUBLE, root_process,
               return_data_tag, MPI_COMM_WORLD);
    }

    ierr = MPI_Finalize();
    return 0;
}
