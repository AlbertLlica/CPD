#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>

// Función para encontrar el bin correspondiente a un valor
int Find_bin(float value, const std::vector<float>& bin_maxes, int bin_count, float min_meas) {
    int bin;
    for (bin = 0; bin < bin_count; bin++) {
        if (value < bin_maxes[bin]) break;
    }
    return bin;
}

int main(int argc, char* argv[]) {
    int comm_sz, my_rank, data_count, bin_count;
    float min_meas, max_meas;
    std::vector<float> data;
    std::vector<int> bin_counts;
    std::vector<float> bin_maxes;
    std::vector<int> local_bin_counts;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Proceso 0: Definir los datos y distribuirlos
    if (my_rank == 0) {
        
        data_count = 10; 
        bin_count = 5; 
        min_meas = 0.0; 
        max_meas = 100.0; 


        data = {5.0, 10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0};

        bin_counts.resize(bin_count, 0);
        bin_maxes.resize(bin_count);

        // Inicializar bin_maxes
        float bin_width = (max_meas - min_meas) / bin_count;
        for (int i = 0; i < bin_count; i++) {
            bin_maxes[i] = min_meas + bin_width * (i + 1);
        }
    }

    // Broadcast 
    MPI_Bcast(&data_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&bin_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&min_meas, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&max_meas, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);


    if (my_rank != 0) {
        bin_maxes.resize(bin_count);
    }

    // Broadcast para distribuir los valores de bin_maxes a todos los procesos
    MPI_Bcast(bin_maxes.data(), bin_count, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Calcular el número de datos por proceso
    int local_data_count = data_count / comm_sz;
    int remaining_data = data_count % comm_sz;

    // Crear el array de conteo y desplazamientos para Scatterv
    std::vector<int> sendcounts(comm_sz, local_data_count);
    std::vector<int> displs(comm_sz, 0);

    // Ajustar los counts para los procesos que reciben un dato extra
    for (int i = 0; i < remaining_data; i++) {
        sendcounts[i]++;
    }

    // Calcular los desplazamientos
    for (int i = 1; i < comm_sz; i++) {
        displs[i] = displs[i - 1] + sendcounts[i - 1];
    }

    // Crear buffer de recepción local_data en función de sendcounts
    std::vector<float> local_data(sendcounts[my_rank]);

    // Utilizar MPI_Scatterv para enviar los datos
    MPI_Scatterv(data.data(), sendcounts.data(), displs.data(), MPI_FLOAT,
                  local_data.data(), local_data.size(), MPI_FLOAT,
                  0, MPI_COMM_WORLD);

    // Asignar memoria para el conteo local de bins en cada proceso
    local_bin_counts.resize(bin_count, 0);

    // Contar cuántos datos caen en cada bin (para los datos locales)
    for (int i = 0; i < local_data.size(); i++) {
        int bin = Find_bin(local_data[i], bin_maxes, bin_count, min_meas);
        if (bin < bin_count) { // Asegurarse de que el bin esté dentro del rango
            local_bin_counts[bin]++;
        }
    }

    // Reducir los conteos locales de todos los procesos en el proceso 0
    MPI_Reduce(local_bin_counts.data(), bin_counts.data(), bin_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Proceso 0: Imprimir el histograma final
    if (my_rank == 0) {
        std::cout << "Histograma:\n";
        for (int i = 0; i < bin_count; i++) {
            float bin_min = (i == 0) ? min_meas : bin_maxes[i - 1];
            std::cout << "Bin " << i << " [" << bin_min << " - " << bin_maxes[i] << "): " << bin_counts[i] << "\n";
        }
    }

    MPI_Finalize();
    return 0;
}
