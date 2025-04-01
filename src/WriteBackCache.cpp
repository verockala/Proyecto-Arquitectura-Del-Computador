/* WriteBackCache.cpp */
#include <iostream>
#include <iomanip> // Para std::setw y otros manipuladores
#include "WriteBackCache.h"
#include "usos.h"

WriteBackCache::WriteBackCache(size_t size, MainMemory &mem)
    : maxSize(size), mainMemory(mem) {}

int WriteBackCache::read(int key)
{
    latenciaTotal += TIEMPO_ACCESO_CACHE; // Siempre accedemos a la caché primero

    if (cacheMap.find(key) == cacheMap.end())
    { // Fallo de caché
        leerFallos++;
        std::cout << "Fallo de cache en clave: " << key << "\n";
        int value = mainMemory.read(key);
        latenciaTotal += TIEMPO_ACCESO_MEMORIA; // Latencia por write-back
        if (value != -1)
        {
            // Si la caché está llena, escribir línea sucia en memoria
            if (accessOrder.size() >= maxSize)
            {
                flushIfDirty(std::prev(accessOrder.end())); // Revisa la LRU
                cacheMap.erase(accessOrder.back().first);
                accessOrder.pop_back();
            }
            // Agregar a la caché
            accessOrder.emplace_front(key, CacheLine{value, false});
            cacheMap[key] = accessOrder.begin();
        }
        return value;
    }
    else
    { // Acierto de caché
        leerAciertos++;
        accessOrder.splice(accessOrder.begin(), accessOrder, cacheMap[key]);
        std::cout << "Lectura en cache: " << key << " -> " << cacheMap[key]->second.value << "\n";
        return cacheMap[key]->second.value;
    }
}

void WriteBackCache::flushIfDirty(std::list<std::pair<int, CacheLine>>::iterator it) {
    if (it->second.dirty) { // Solo escribir en memoria si está sucio
        mainMemory.write(it->first, it->second.value);
        it->second.dirty = false; // Después de escribir en memoria, la línea ya no está sucia
        writeback++; // Contador de write-back
        latenciaTotal += TIEMPO_ACCESO_MEMORIA; // Penalización de latencia
    }
}

void WriteBackCache::write(int key, int value)
{
    latenciaTotal += TIEMPO_ACCESO_CACHE; // Siempre accedemos a la caché primero

    if (cacheMap.find(key) != cacheMap.end())
    { // Acierto de caché
        escrituras++;
        accessOrder.splice(accessOrder.begin(), accessOrder, cacheMap[key]);
        cacheMap[key]->second.value = value;
        cacheMap[key]->second.dirty = true; // Marcar como sucia
    }
    else
    { // Fallo de caché
        escrituras++;
        if (accessOrder.size() >= maxSize)
        {
            flushIfDirty(std::prev(accessOrder.end())); // Escribir LRU sucia en memoria
            cacheMap.erase(accessOrder.back().first);
            accessOrder.pop_back();
        }
        accessOrder.emplace_front(key, CacheLine{value, true}); // Nueva línea sucia
        cacheMap[key] = accessOrder.begin();
    }
}

void WriteBackCache::flush()
{
    for (auto &item : accessOrder)
    {
        if (item.second.dirty)
        {
            mainMemory.write(item.first, item.second.value);
            item.second.dirty = false;
            writeback++;
            latenciaTotal += TIEMPO_ACCESO_MEMORIA; // Latencia por write-back
        }
    }
}

void WriteBackCache::printStats() const
{
    std::cout << "\n============== Estadisticas de Write-Back Cache ===============\n";
    std::cout << std::setw(40) << std::left << "Metrica"
              << " | "
              << std::setw(20) << std::right << "Valores" << std::endl;

    std::cout << std::setw(40) << std::left << "========================================"
              << " | "
              << std::setw(20) << std::right << "====================" << std::endl;

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

    std::cout << std::setw(40) << std::left << "========================================"
              << " | "
              << std::setw(20) << std::right << "====================" << std::endl;

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

void WriteBackCache::printCacheContents() const
{
    std::cout << "\nContenido de la Cache (LRU primero):\n";
    for (const auto &item : accessOrder)
    {
        std::cout << item.first << ":" << item.second.value
                  << (item.second.dirty ? "* " : "  "); // '*' indica línea sucia
    }
    std::cout << "\n";
}
