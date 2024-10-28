#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PING_PONG_LIMIT 10000  // Número de iteraciones ping-pong

int main(int argc, char** argv) {
    int rank, comm_sz;
    MPI_Init(&argc, &argv);               // Inicializa MPI
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);  // Obtiene número de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtiene el rango del proceso

    if (comm_sz < 2) {
        fprintf(stderr, "Este programa necesita al menos 2 procesos.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int count = 0;  // Contador de ping-pong
    int message = 0;
    MPI_Status status;

    // Variables para medir tiempo con clock() y MPI_Wtime()
    clock_t start_clock, end_clock;
    double start_mpi, end_mpi;

    if (rank == 0) {
        // Proceso 0 mide tiempo de inicio
        start_clock = clock();  
        start_mpi = MPI_Wtime();

        for (int i = 0; i < PING_PONG_LIMIT; i++) {
            // Enviar mensaje al proceso 1 (PING)
            MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

            // Recibir mensaje del proceso 1 (PONG)
            MPI_Recv(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        }

        // Mide tiempo de finalización
        end_clock = clock();
        end_mpi = MPI_Wtime();

        // Calcula el tiempo total en segundos
        double time_clock = (double)(end_clock - start_clock) / CLOCKS_PER_SEC;
        double time_mpi = end_mpi - start_mpi;

        // Muestra los resultados
        printf("Tiempo medido con clock(): %.6f segundos\n", time_clock);
        printf("Tiempo medido con MPI_Wtime(): %.6f segundos\n", time_mpi);
    } else if (rank == 1) {
        for (int i = 0; i < PING_PONG_LIMIT; i++) {
            // Recibir mensaje del proceso 0 (PING)
            MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

            // Enviar mensaje al proceso 0 (PONG)
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();  // Finaliza MPI
    return 0;
}
