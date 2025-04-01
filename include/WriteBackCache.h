/* WriteBackCache.h */
#ifndef WRITEBACK_CACHE_H
#define WRITEBACK_CACHE_H
#include "Memory.h"
#include "usos.h"
#include "MainMemory.h"
#include <unordered_map>
#include <list>

class MainMemory; // Forward declaration

class WriteBackCache : public Memory
{
private:
    int latenciaTotal = 0, leerAciertos = 0, leerFallos = 0, escrituras = 0, writeback = 0;

    // tamano cache
    size_t maxSize;
    struct CacheLine
    {
        int value;
        bool dirty; // Dirty bit para write-back
    };
    std::list<std::pair<int, CacheLine>> accessOrder;
    std::unordered_map<int, std::list<std::pair<int, CacheLine>>::iterator> cacheMap;
    MainMemory &mainMemory;

    // Escribe una línea sucia en memoria principal antes de reemplazarla
    void flushIfDirty(const std::list<std::pair<int, CacheLine>>::iterator &it)
    {
        if (it->second.dirty)
        {
            mainMemory.write(it->first, it->second.value);
            it->second.dirty = false;               // Marcar como limpia
            writeback++;                            // Incrementar contador de write-backs
            latenciaTotal += TIEMPO_ACCESO_MEMORIA; // Latencia por write-back
        }
    }

public:
    WriteBackCache(size_t size, MainMemory &mem);
    int read(int key) override;
    void write(int key, int value) override;
    void flush();
    void printStats() const;
    void printCacheContents() const;
};

#endif