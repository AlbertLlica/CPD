#include <iostream>
#include <vector>
#include <chrono>
#include <random> 

using namespace std;
using namespace chrono;

vector<vector<int>> productoMatriz(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int filas_A = A.size();
    int columnas_A = A[0].size();
    int columnas_B = B[0].size();

    vector<vector<int>> resultado(filas_A, vector<int>(columnas_B, 0));

    for (int i = 0; i < filas_A; i++) {
        for (int j = 0; j < columnas_B; j++) {
            for (int k = 0; k < columnas_A; k++) {
                resultado[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return resultado;
}

void llenarMatriz(vector<vector<int>>& matriz) {
    srand(time(nullptr)); 

    for (int i = 0; i < matriz.size(); i++) {
        for (int j = 0; j < matriz[0].size(); j++) {
            matriz[i][j] = rand() % 10 + 1; 
        }
    }
}

int main() {

    high_resolution_clock::time_point inicio, fin;
    duration<double, milli> tiempo_transcurrido; 

    for (int i = 100; i <= 2000; i += 100) {
        vector<vector<int>> A(i, vector<int>(i, 0)); 
        vector<vector<int>> B(i, vector<int>(i, 0)); 
        vector<vector<int>> resultado(i, vector<int>(i, 0)); 

        llenarMatriz(A);
        llenarMatriz(B);

        inicio = high_resolution_clock::now();
        resultado = productoMatriz(A, B);
        fin = high_resolution_clock::now();
        tiempo_transcurrido = duration_cast<duration<double, milli>>(fin - inicio); 

        cout << "Matriz: " << i << "x" << i << " - tiempo: " << tiempo_transcurrido.count() << " milisegundos" << endl; 
    }

    return 0;
}