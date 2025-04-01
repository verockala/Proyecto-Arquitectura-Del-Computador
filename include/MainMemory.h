/* Memory.h */
#ifndef MAINMEMORY_H
#define MAINMEMORY_H
#include <vector>
#include "Memory.h"

class MainMemory : public Memory
{
private:
    std::vector<int> memory;

public:
    MainMemory(size_t size);
    void write(int address, int value) override;
    int read(int address) override;
    void printMemoryContents() const;
};

#endif