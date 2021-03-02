#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

long B=10000; /* Working base */
long LB=4;    /* Log10(base)  */

/*
** Set the big real x to the small integer Integer
*/
void SetToInteger (long n, long *x, long Integer) {
  long i;
  for (i=1; i<n; i++) x[i] = 0;
  x[0] = Integer;
}
/*
** Is the big real x equal to zero \?
*/
long IsZero (long n, long *x) {
  long i;
  for (i=0; i<n; i++)
    if (x[i])	return 0;
	return 1;
}
/*
** Addition of big reals : x += y
**  Like school addition with carry management
*/
void Add (long n, long *x, long *y) {
  long carry=0, i;
  for (i=n-1; i>=0; i--) {
    x[i] += y[i]+carry;
    if (x[i]<B) carry = 0;
    else {
      carry = 1;
      x[i] -= B;
    }
  }
}
/*
** Division of the big real x by the integer d
**  Like school division with carry management
*/
void Div (long n, long *x, long d) {
  long carry=0, xi, q, i;
  for (i=0; i<n; i++) {
    xi    = x[i]+carry*B;
    q     = xi/d;
    carry = xi-q*d;
    x[i]  = q;
  }
}
/*
** Print the big real x
*/
void Print (long n, long *x) {
  long i;
  printf ("%ld.", x[0]);
  for (i=1; i<n; i++) {
    printf ("%.4ld", x[i]);
    if (i%25==0) printf ("%8ld\n", i*4);
  }
  printf ("\n");
}

int main(int argc, char **argv)
{

    long NbDigits = 1000;
    long size = 1 + NbDigits / LB;
    int num_of_iterations = 600;

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
         long *e = (long *)calloc(size, sizeof(long));
         long *partial_e = (long *)calloc(size, sizeof(long));
         long *uk = (long *)calloc(size, sizeof(long));
         long k = 1;

         // Formula used : e = 1+1/1!+1/2!+...+1/k!+...
         SetToInteger (size, e,  1); /* e  = 1 */
         SetToInteger (size, uk, 1);	/* uk = 1 */
         for(int i = 0; i < avg_rows_per_process; i++) {
             Div (size, uk, k);  /* uk = u(k-1)/k */
             Add (size, e,  uk); /* e  = e + uk */
             k++;
         }
         for(int flow_id = 1; flow_id < num_procs; flow_id++) {

            ierr = MPI_Recv(partial_e, size, MPI_LONG, MPI_ANY_SOURCE,
                  return_data_tag, MPI_COMM_WORLD, &status);

            sender = status.MPI_SOURCE;
            Add(size, e, partial_e);
         }
         Print(size, e);
    }
    else
    {
        long *e = (long *)calloc(size, sizeof(long));
        long *uk = (long *)calloc(size, sizeof(long));
        long k = 1;

        // Formula used : e = 1+1/1!+1/2!+...+1/k!+...
        SetToInteger (size, e,  1); /* e  = 1 */
        SetToInteger (size, uk, 1);	/* uk = 1 */
        for(int i = 0; i < my_id * avg_rows_per_process; i++) {
            Div (size, uk, k);  /* uk = u(k-1)/k */
            k++;
            //printf("%ld \n", k);
        }

        for(int i = my_id * avg_rows_per_process; i < (my_id + 1) * avg_rows_per_process; i++) {
            Div (size, uk, k);  /* uk = u(k-1)/k */
            Add (size, e,  uk); /* e  = e + uk */
            k++;

        }
        e[0] = 0;

        ierr = MPI_Send(e, size, MPI_LONG, root_process,
               return_data_tag, MPI_COMM_WORLD);
    }

    ierr = MPI_Finalize();
    return 0;
}
