#include <iostream>
#include <chrono>
using namespace std;

void run_test(int size) {
    double** A = new double* [size];
    double* x = new double[size];
    double* y = new double[size];

    for (int i = 0; i < size; i++) {
        A[i] = new double[size];
        for (int j = 0; j < size; j++) {
            A[i][j] = 0.0;
        }
        x[i] = 0.0;
        y[i] = 0.0;
    }

    // Tiempo primer bucle
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            y[i] += A[i][j] * x[j];
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Tamaño: " << size << " - Tiempo primer bucle: " << duration.count() << " microsegundos" << std::endl;

    for (int i = 0; i < size; i++) {
        y[i] = 0.0;
    }

    // Tiempo segundo bucle
    start_time = std::chrono::high_resolution_clock::now();

    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; i++) {
            y[i] += A[i][j] * x[j];
        }
    }

    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Tamaño: " << size << " - Tiempo segundo bucle: " << duration.count() << " microsegundos" << std::endl << "---------------------------------------------------------" << std::endl;

    // Liberar memoria
    for (int i = 0; i < size; i++) {
        delete[] A[i];
    }
    delete[] A;
    delete[] x;
    delete[] y;
}

int main() {
    // Tamaños de prueba
    int sizes[] = { 100, 500, 1000, 10000 };

    for (int size : sizes) {
        run_test(size);
    }

    return 0;
}
