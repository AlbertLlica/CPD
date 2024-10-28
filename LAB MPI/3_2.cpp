#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, comm_size;
    long long int tosses = 0; // Inicializar en 0 para evitar problemas si no se recibe input
    long long int local_tosses, in_circle = 0, global_in_circle;
    double x, y, pi_estimate;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    if (rank == 0) {
        printf("Ingrese el número total de lanzamientos: ");
        fflush(stdout); // Asegúrate de que el mensaje se muestre antes de la entrada
        scanf("%lld", &tosses);
    }

    // Broadcast del número total de lanzamientos a todos los procesos
    MPI_Bcast(&tosses, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Cada proceso calcula cuántos lanzamientos le corresponden
    local_tosses = tosses / comm_size;
    unsigned int seed = time(NULL) + rank; // Genera un seed diferente para cada proceso

    // Realiza el lanzamiento de dardos
    for (long long int toss = 0; toss < local_tosses; toss++) {
        x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0; // Genera x en el rango [-1, 1]
        y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0; // Genera y en el rango [-1, 1]
        if (x * x + y * y <= 1.0) { // Comprueba si está dentro del círculo
            in_circle++;
        }
    }

    // Reduce el conteo de los dardos que están en el círculo
    MPI_Reduce(&in_circle, &global_in_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // El proceso 0 calcula e imprime la estimación de pi
    if (rank == 0) {
        if (tosses > 0) { // Verifica que el número de lanzamientos sea positivo
            pi_estimate = 4.0 * global_in_circle / ((double)tosses);
            printf("Estimación de π: %f\n", pi_estimate);
        } else {
            printf("El número total de lanzamientos debe ser mayor que cero.\n");
        }
    }

    MPI_Finalize();
    return 0;
}
