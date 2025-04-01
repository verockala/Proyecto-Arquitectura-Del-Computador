#ifndef WRITETHROUGH_CACHE_H
#define WRITETHROUGH_CACHE_H

#include "Memory.h"
#include "MainMemory.h"
#include "usos.h"
#include <unordered_map>
#include <list>

class MainMemory; // Forward declaration

class WriteThroughCache : public Memory
{
private:
    size_t maxSize;
    std::list<std::pair<int, int>> accessOrder;
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cacheMap;
    int latenciaTotal = 0, leerAciertos = 0, leerFallos = 0, escrituras = 0;
    MainMemory &mainMemory;

public:
    WriteThroughCache(size_t size, MainMemory &mem); // Corregido el constructor
    int read(int key) override;
    void write(int key, int value) override;
    void printStats() const;
    void printCacheContents() const;
};

#endif
