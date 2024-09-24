#include <iostream>
#include <vector>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

void multiplicarMatricesPorBloques(vector<vector<int>>& A, vector<vector<int>>& B, vector<vector<int>>& C, int tamanoBloque) {
    int n = A.size(); 
    for (int i0 = 0; i0 < n; i0 += tamanoBloque) {
        for (int j0 = 0; j0 < n; j0 += tamanoBloque) {
            for (int k0 = 0; k0 < n; k0 += tamanoBloque) {
                for (int i = i0; i < min(i0 + tamanoBloque, n); i++) {
                    for (int j = j0; j < min(j0 + tamanoBloque, n); j++) {
                        for (int k = k0; k < min(k0 + tamanoBloque, n); k++) {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

void llenarMatrizAleatoriamente(vector<vector<int>>& matriz) {
    srand(time(nullptr)); 

    for (int i = 0; i < matriz.size(); i++) {
        for (int j = 0; j < matriz[0].size(); j++) {
            matriz[i][j] = rand() % 10 + 1; 
        }
    }
}
int main() {

    int tamanoBloque = 4;
    for (int i = 100; i <= 2000; i += 100) {
        tamanoBloque += 8;
        vector<vector<int>> A(i, vector<int>(i, 0));
        vector<vector<int>> B(i, vector<int>(i, 0));
        vector<vector<int>> resultado(i, vector<int>(i, 0));

        llenarMatrizAleatoriamente(A);
        llenarMatrizAleatoriamente(B);

        high_resolution_clock::time_point inicio = high_resolution_clock::now();
        multiplicarMatricesPorBloques(A, B, resultado, tamanoBloque);
        high_resolution_clock::time_point fin = high_resolution_clock::now();
        duration<double, milli> tiempo_transcurrido = duration_cast<duration<double, milli>>(fin - inicio);

        cout << "Matriz: " << i << "x" << i << " - tiempo: " << tiempo_transcurrido.count() << " milisegundos" << endl; 
    }

    return 0;
}