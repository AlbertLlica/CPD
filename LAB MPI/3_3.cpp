#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int comm_sz, my_rank;
    int local_value, received_value;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Cada proceso inicia con un valor local igual a su rango + 1
    local_value = my_rank + 1;

    // Estructura en árbol binario
    for (int step = 1; step < comm_sz; step *= 2) {
        if (my_rank % (2 * step) == 0) {
            // Recibir valor del proceso par
            MPI_Recv(&received_value, 1, MPI_INT, my_rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_value += received_value;  // Sumar el valor recibido
        } else {
            // Enviar el valor al proceso correspondiente
            MPI_Send(&local_value, 1, MPI_INT, my_rank - step, 0, MPI_COMM_WORLD);
            break;  // El proceso termina después de enviar
        }
    }

    // El proceso 0 imprime el resultado final
    if (my_rank == 0) {
        printf("Suma global = %d\n", local_value);
    }

    MPI_Finalize();
    return 0;
}
