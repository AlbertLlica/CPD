#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int comm_sz, rank;
    int local_value, received_value;
    int global_sum;

    MPI_Init(&argc, &argv); // Inicializa MPI
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); // Tamaño total de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rango del proceso actual

    // Cada proceso genera un valor local basado en su rango (o puedes usar scanf)
    local_value = rank + 1;  // Ejemplo: Proceso 0 tiene 1, proceso 1 tiene 2, etc.

    // Verificar que el número de procesos sea potencia de dos
    int temp = comm_sz;
    while ((temp % 2 == 0) && (temp > 1)) {
        temp /= 2;
    }
    if (temp != 1) {
        if (rank == 0) {
            printf("Este programa requiere que el número de procesos sea potencia de dos.\n");
        }
        MPI_Finalize();
        return 0;
    }

    // Suma global usando la estructura de mariposa
    for (int step = 1; step < comm_sz; step *= 2) {
        int partner = rank ^ step; // Calcular proceso espejo usando XOR

        // Enviar y recibir valores
        MPI_Sendrecv(&local_value, 1, MPI_INT, partner, 0,
                     &received_value, 1, MPI_INT, partner, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Combinar los valores recibidos
        local_value += received_value;
    }

    // El proceso 0 tiene la suma global
    if (rank == 0) {
        printf("La suma global es: %d\n", local_value);
    }

    MPI_Finalize(); // Finaliza MPI
    return 0;
}
