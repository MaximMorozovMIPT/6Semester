#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <sys/time.h>

void initial_values(double **table, const int time_pts, const int x_pts, const double tau, const double h);
double angle(const double left, const double mid, const double tau, const double h);
double three_dots(const double left, const double mid, const double right, const double tau, const double h);
double cross(const double left, const double mid, const double right, const double bot, const double tau, const double h);
void write_csv(const char* name, double** table, const double tau, const double h, const int time_pts, const int x_pts);

int main(int argc, char** argv)
{
    MPI_Status status;
    int world_rank, world_size;
    int ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    
    const int time_pts = 800, x_pts = 800;
    const double tau = 0.01, h = 0.05;

    struct timeval begin, end;

    if (world_rank == 0)
    {
        MPI_Request get_requests[4];
        MPI_Request requests[x_pts - 2];
        MPI_Request requests_2[x_pts - 2];
        double ** table = (double **)calloc(time_pts, sizeof(double *));
        double * long_buf = (double *)calloc(x_pts * 5, sizeof(double));
        for (int i = 0; i < time_pts; i++)
        {
            table[i] = (double *)calloc(x_pts, sizeof(double));
        }

        gettimeofday(&begin, 0);
        initial_values(table, time_pts, x_pts, tau, h);
        for (int i = 2; i < time_pts; i++)
        {    
            for (int k = 1; k < x_pts - 1; k++)
            {
                int dest = k % (world_size - 1) + 1;
                long_buf[k * 4 + 0] = table[i-1][k-1];
                long_buf[k * 4 + 1] = table[i-1][k];
                long_buf[k * 4 + 2] = table[i-1][k+1];
                long_buf[k * 4 + 3] = table[i-2][k];
                MPI_Send_init(&long_buf[k * 4 + 0], 4, MPI_DOUBLE, dest, 3228, MPI_COMM_WORLD, requests + k - 1);
            }
            MPI_Startall(x_pts - 2, requests);
            for (int k = 1; k < x_pts - 1; k++)
            {
                int source = k % (world_size - 1) + 1;
                MPI_Recv_init(&table[i][k], 1, MPI_DOUBLE, source, 3229, MPI_COMM_WORLD, requests_2 + k - 1);
                // MPI_Recv(&get, 1, MPI_DOUBLE, source, 3229, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            }
            MPI_Startall(x_pts - 2, requests_2);
            MPI_Waitall(x_pts - 2, requests_2, MPI_STATUSES_IGNORE );
            table[i][x_pts - 1] = angle(table[i - 1][x_pts - 2], table[i - 1][x_pts - 1], tau, h);
        }
        gettimeofday(&end, 0);
        long seconds = end.tv_sec - begin.tv_sec;
        long microseconds = end.tv_usec - begin.tv_usec;
        double elapsed = seconds + microseconds*1e-6;
        printf("Time measured: %.3f seconds.\n", elapsed);
        write_csv("data.csv", table, tau, h, time_pts, x_pts);
        for (int i = 0; i < time_pts; i++)
        {
            free(table[i]);
        }
        free(table);
        free(long_buf);
    }

    if (world_rank != 0)
    {
        MPI_Request requests[2];
        double * long_buf = (double *)calloc(x_pts, sizeof(double));
        int j = 0;
        for(j;;j++)
        {
            double * buf_1 = (double *)calloc(4, sizeof(double));
            MPI_Recv(buf_1, 4, MPI_DOUBLE, MPI_ANY_SOURCE, 3228, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
            long_buf[j] = cross(buf_1[0], buf_1[1], buf_1[2], buf_1[3], tau, h);
            free(buf_1);
            MPI_Isend(&long_buf[j], 1, MPI_DOUBLE, 0, 3229, MPI_COMM_WORLD, requests + 1);
            if (j >= x_pts - 200)
            {
                j = 0;
            }
        }
        free(long_buf);
    }
}

// phi(x) = x^2, psi(t) = t
void initial_values(double **table, const int time_pts, const int x_pts, const double tau, const double h)
{
    for (int i = 0; i < time_pts; i++)
    {
        table[i][0] = i * tau;
    }
    for (int i = 0; i < x_pts; i++)
    {
        table[0][i] = i * i * h * h;
    }
    for (int i = 1; i < x_pts - 1; i++)
    {
        table[1][i] = three_dots(table[0][i-1], table[0][i], table[0][i+1], tau, h);
    }
    table[1][x_pts - 1] = angle(table[0][x_pts - 2], table[0][x_pts - 1], tau, h);
}

double angle(const double left, const double mid, const double tau, const double h)
{
    return tau * (mid - (mid - left) / h) + mid;
}

double three_dots(const double left, const double mid, const double right, const double tau, const double h)
{
    return tau * (mid - (left + right) / (2 * h)) + ((left + right) / 2);
}

double cross(const double left, const double mid, const double right, const double bot, const double tau, const double h)
{
    return bot + 2 * tau * (mid - (right - left) / (2 * h));
}

void write_csv(const char* name, double** table, const double tau, const double h, const int time_pts, const int x_pts)
{
    FILE* fd = fopen(name, "wb");

    for (unsigned int i = 0; i < time_pts; ++i) {
        for (unsigned int j = 0; j < x_pts; ++j) {
            fprintf(fd, "%.20f,", table[i][j]);
        }
    }
    fprintf(fd, "%d,%.20f,%d,%.20f", time_pts, tau, x_pts, h);
    fclose(fd);
}