#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int nearest_power_of_two(int n) {
    int p = 1;
    while (p <= n) p *= 2;
    return p / 2;
}

int main(int argc, char** argv) {
    int comm_sz, rank;
    int local_value, received_value, global_sum;

    MPI_Init(&argc, &argv);  // Inicializa MPI
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);  // Tamaño total de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Rango del proceso actual

    // Cada proceso genera un valor local basado en su rango
    local_value = rank + 1;  // Ejemplo: Proceso 0 tiene 1, proceso 1 tiene 2, etc.

    // Encontrar la potencia de dos más cercana menor o igual al número de procesos
    int active_procs = nearest_power_of_two(comm_sz);

    // Los procesos sobrantes envían sus valores a los primeros procesos
    if (rank >= active_procs) {
        int target = rank - active_procs;
        MPI_Send(&local_value, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
    } else if (rank < comm_sz - active_procs) {
        MPI_Recv(&received_value, 1, MPI_INT, rank + active_procs, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        local_value += received_value;
    }

    // Suma global usando estructura mariposa solo con los procesos activos
    for (int step = 1; step < active_procs; step *= 2) {
        int partner = rank ^ step;

        if (partner < active_procs) {
            MPI_Sendrecv(&local_value, 1, MPI_INT, partner, 0,
                         &received_value, 1, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            local_value += received_value;
        }
    }

    // El proceso 0 tiene la suma global
    if (rank == 0) {
        printf("La suma global es: %d\n", local_value);
    }

    MPI_Finalize();  // Finaliza MPI
    return 0;
}
