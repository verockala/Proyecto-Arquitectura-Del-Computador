/* Memory.cpp */
#include <iostream>
#include <iomanip> // Para std::setw y std::left
#include "Memory.h"
#include "MainMemory.h"

MainMemory::MainMemory(size_t size) : memory(size, -1) {}

void MainMemory::write(int address, int value)
{
    if (address >= 0 && address < memory.size())
    {
        memory[address] = value;
    }
}

int MainMemory::read(int address)
{
    if (address >= 0 && address < memory.size())
    {
        return memory[address];
    }
    return -1;
}

void MainMemory::printMemoryContents() const
{
    std::cout << "\nContenido de Memoria Principal:\n";
    for (size_t i = 0; i < memory.size(); ++i)
    {
        if (memory[i] != -1)
            std::cout << i << ":" << memory[i] << "  ";
    }
    std::cout << "\n";
}
