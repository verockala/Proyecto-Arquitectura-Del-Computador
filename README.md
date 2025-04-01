# Estrategias de Escritura en Caché (Write-Through vs Write-Back)

![C++ 17](https://img.shields.io/badge/C++-17-blue)
![Licencia BSD-3-Clause](https://img.shields.io/badge/Licencia-BSD%203--Clause-blue)

Este proyecto implementa y compara las estrategias de escritura en caché Write-Through y Write-Back, analizando su impacto en el rendimiento y la coherencia de los datos.

- Write-Through: La escritura se realiza simultáneamente en la caché y la memoria principal, garantizando coherencia pero aumentando la latencia.

- Write-Back: La escritura se realiza primero en la caché y solo se actualiza la memoria principal cuando es necesario, mejorando el rendimiento pero con mayor riesgo de pérdida de datos en caso de fallo.

## Características
- Implementado en **C++17**.
- Simula la **política de reemplazo LRU.**
- Generacion de **dataset** de manera aleatoria.
- **Cálculo de estadísticas (latencia en ciclos, promedio de latencia por operacion, tasa de aciertos, lecturas y escrituras forzadas)**
- Menu Interactivo con multiples opciones **(Leer de la cache, Escribir en la cache, Mostrar contenido de cache y memoria, Ejecutar pruebas automaticas, Salir)**.

## Algoritmos Implementados
**Política de Reemplazo LRU:**

Permite mantener un seguimiento de los accesos más recientes y descarta el elemento menos utilizado cuando se requiere espacio. Es decir, cada vez que un dato es accedido, se actualiza su posición en la lista, moviéndolo al frente.

**Simulación de Escritura en Caché:** 

Se desarrollan algoritmos que permiten modelar el comportamiento de las estrategias Write-Through y Write-Back, actualizando la memoria caché y la memoria principal de acuerdo con las reglas de cada enfoque.
	
- Mecanismo de Write-Back: En esta estrategia, las escrituras se realizan solo en la caché y se marca el dato como "sucio". Antes de reemplazar un dato sucio, se escribe en memoria principal, reduciendo el número de accesos a la misma.
-	Mecanismo de Write-Through: Cada escritura se realiza inmediatamente tanto en la caché como en la memoria principal, asegurando coherencia, pero aumentando la latencia.

**Generación Aleatoria de Accesos a Memoria:** 

Se emplea la biblioteca <random> para simular patrones de acceso a memoria de manera aleatoria, permitiendo evaluar el rendimiento de las estrategias en diferentes escenarios.

**Ordenamiento y Búsqueda:** 

Se utiliza <algorithm> para optimizar ciertas operaciones relacionadas con la organización y recuperación de datos dentro de la simulación.

