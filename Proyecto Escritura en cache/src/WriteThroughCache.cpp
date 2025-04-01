#include <iostream>
#include <iomanip> // Para std::setw y otros manipuladores
#include "WriteThroughCache.h"
#include "usos.h"

WriteThroughCache::WriteThroughCache(size_t size, MainMemory &mem) : maxSize(size), mainMemory(mem) {}

int WriteThroughCache::read(int key)
{
    latenciaTotal += TIEMPO_ACCESO_CACHE;

    if (cacheMap.find(key) == cacheMap.end())
    {
        leerFallos++;
        std::cout << "Fallo de cache en clave: " << key << "\n";

        int value = mainMemory.read(key);
        latenciaTotal += TIEMPO_ACCESO_MEMORIA;
        if (value != -1)
            write(key, value); // Escribir en la caché y en memoria principal
        return value;
    }
    else
    {
        leerAciertos++;
        accessOrder.splice(accessOrder.begin(), accessOrder, cacheMap[key]);
        std::cout << "Lectura en cache: " << key << " -> " << cacheMap[key]->second << "\n";
        return cacheMap[key]->second;
    }
}

void WriteThroughCache::write(int key, int value)
{
    // Escritura inmediata en memoria principal (propia del Write-Through)
    mainMemory.write(key, value); 

    // Sumar la latencia de acceso a memoria y a caché
    latenciaTotal += TIEMPO_ACCESO_MEMORIA + TIEMPO_ACCESO_CACHE;
    escrituras++; // Incrementar contador de escrituras

    // Verificar si la clave ya está en caché
    if (cacheMap.find(key) != cacheMap.end())
    {
        // Si la clave ya existe, moverla al frente en la lista de acceso (LRU)
        accessOrder.splice(accessOrder.begin(), accessOrder, cacheMap[key]);

        // Actualizar el valor de la clave en caché
        cacheMap[key]->second = value;
    }
    else
    {
        // Si la caché está llena, eliminar el elemento menos recientemente usado (LRU)
        if (accessOrder.size() >= maxSize)
        {
            int lruKey = accessOrder.back().first; // Obtener la clave LRU
            accessOrder.pop_back(); // Eliminar el último elemento de la lista
            cacheMap.erase(lruKey); // Eliminar la clave del mapa
        }

        // Insertar la nueva clave y valor al inicio (más recientemente usado)
        accessOrder.emplace_front(key, value);
        cacheMap[key] = accessOrder.begin(); // Actualizar el mapa de caché
    }
}


void WriteThroughCache::printStats() const
{
    // Tasa de aciertos (usamos un valor calculado previamente)

    std::cout << "\n============== Estadisticas de Write-Through Cache ===============\n";

    // Imprimir encabezados de la tabla
    std::cout << std::setw(40) << std::left << "Metrica"
              << " | "
              << std::setw(20) << std::right << "Valores" << std::endl;

    // Imprimir la línea divisoria
    std::cout << std::setw(40) << std::left << "========================================"
              << " | "
              << std::setw(20) << std::right << "====================" << std::endl;

    // Imprimir los datos con una línea divisoria entre las columnas
    std::cout << std::setw(40) << std::left << "Tiempo total de latencia"
              << " | "
              << std::setw(10) << std::right << latenciaTotal << " ciclos" << std::endl;
    std::cout << std::setw(40) << std::left << "Lecturas (aciertos)"
              << " | "
              << std::setw(10) << std::right << leerAciertos << std::endl;
    std::cout << std::setw(40) << std::left << "Lecturas (fallos)"
              << " | "
              << std::setw(10) << std::right << leerFallos << std::endl;
    std::cout << std::setw(40) << std::left << "Escrituras"
              << " | "
              << std::setw(10) << std::right << escrituras << std::endl;

    // Línea de separación entre las secciones
    std::cout << std::setw(40) << std::left << "========================================"
              << " | "
              << std::setw(20) << std::right << "====================" << std::endl;

    // Imprimir el resto de los datos
    std::cout << std::setw(40) << std::left << "Latencia promedio por operacion"
              << " | "
              << std::setw(10) << std::right << (latenciaTotal / float(leerAciertos + leerFallos + escrituras)) << " ciclos" << std::endl;

    if (leerAciertos + leerFallos > 0)
    {
        float tasaAciertos = (leerAciertos * 100.0) / (leerAciertos + leerFallos);
        std::cout << std::setw(40) << std::left << "Tasa de aciertos"
                  << " | "
                  << std::setw(10) << std::right << tasaAciertos << "%" << std::endl;
    }
    else
    {
        std::cout << std::setw(40) << std::left << "Tasa de aciertos"
                  << " | "
                  << std::setw(10) << std::right << "N/A (no hay lecturas)" << std::endl;
    }
}

void WriteThroughCache::printCacheContents() const
{
    std::cout << "\nContenido de la Cache Write-Through (LRU primero):\n";
    for (const auto &item : accessOrder)
    {
        std::cout << item.first << ":" << item.second << "  ";
    }
    std::cout << "\n";
}
