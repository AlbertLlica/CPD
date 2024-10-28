#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int comm_sz, my_rank;
    int local_value, received_value;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    local_value = my_rank + 1;

    // Detectar procesos sobrantes
    int largest_power_of_two = 1;
    while (largest_power_of_two <= comm_sz / 2) {
        largest_power_of_two *= 2;
    }

    if (my_rank >= largest_power_of_two) {
        // Procesos sobrantes envían su valor lo antes posible
        MPI_Send(&local_value, 1, MPI_INT, my_rank - largest_power_of_two, 0, MPI_COMM_WORLD);
    } else {
        // Estructura de árbol para los procesos restantes
        for (int step = 1; step < largest_power_of_two; step *= 2) {
            if (my_rank % (2 * step) == 0) {
                MPI_Recv(&received_value, 1, MPI_INT, my_rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_value += received_value;
            } else {
                MPI_Send(&local_value, 1, MPI_INT, my_rank - step, 0, MPI_COMM_WORLD);
                break;
            }
        }

        // Procesos que recibieron de los sobrantes
        if (my_rank + largest_power_of_two < comm_sz) {
            MPI_Recv(&received_value, 1, MPI_INT, my_rank + largest_power_of_two, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_value += received_value;
        }
    }

    if (my_rank == 0) {
        printf("Suma global = %d\n", local_value);
    }

    MPI_Finalize();
    return 0;
}
