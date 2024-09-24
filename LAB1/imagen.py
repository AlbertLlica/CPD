import matplotlib.pyplot as plt

tamanos = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000]
 tiempos_clasico = [13.4795, 80.0779, 269.907, 666.776, 1249.58, 2154.49, 3461.41, 5124.97, 7320.43, 10028.5, 13484.3, 17375.6, 22020.5, 27526, 34085.2, 41409, 49540.9, 58826, 68856.4, 80561.7]
 tiempos_bloques = [8.0584, 61.3593, 261.724, 625.485, 1041.81, 2171.41, 3249.38, 3969.18, 6394.04, 8168.78, 10808.5, 13744.3, 19592.9, 21582.9, 29217.1, 33647.6, 39570.6, 50595.7, 56624.4, 67528.8]

plt.figure(figsize=(12, 6))
plt.plot(tamanos, tiempos_clasico, marker='o', label='Multiplicación Clásica (3 bucles)')
plt.plot(tamanos, tiempos_bloques, marker='o', label='Multiplicación por Bloques (6 bucles)')
plt.xlabel('Tamaño de la Matriz')
plt.ylabel('Tiempo (milisegundos)')
plt.title('Comparación de Rendimiento entre Multiplicación Clásica y por Bloques')
plt.legend()
plt.grid(True)

plt.show()