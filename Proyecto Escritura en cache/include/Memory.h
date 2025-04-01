/* Memory.h */
#ifndef MEMORY_H
#define MEMORY_H

class Memory
{
public:
    virtual void write(int address, int value) = 0;
    virtual int read(int address) = 0;
    virtual ~Memory() {}
};

#endif