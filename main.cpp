#include <iostream>
#include <random>
#include <vector>
#include <limits> // Para std::numeric_limits
#include "MainMemory.h"
#include "Memory.h"
#include "WriteThroughCache.h"
#include "WriteBackCache.h"

std::vector<int> generateRandomDataset(size_t size, int minAddr, int maxAddr);
int getValidNumber();
void realizarPruebasAutomáticas(WriteThroughCache *cacheWriteThrough, WriteBackCache *cacheWriteBack, int min, int max);

int main()
{
    int option, addr, value, valueThrough, valueBack;
    const size_t MIN_ADDR = 0;
    const size_t CACHE_SIZE = 20;
    const size_t DATASET_SIZE = 25;
    const size_t MEM_SIZE = 1000;

    MainMemory mainMemThrough(MEM_SIZE + 1);
    MainMemory mainMemBack(MEM_SIZE + 1);

    WriteThroughCache *cacheWriteThrough = new WriteThroughCache(CACHE_SIZE, mainMemThrough);
    WriteBackCache *cacheWriteBack = new WriteBackCache(CACHE_SIZE, mainMemBack);

    std::vector<int> dataset = generateRandomDataset(DATASET_SIZE, MIN_ADDR, MEM_SIZE);

    std::cout << "=== Dataset generado ===\n";
    for (int addr : dataset)
    {
        std::cout << addr << " ";
    }
    std::cout << "\n";

    for (int i = 0; i < dataset.size(); ++i)
    {
        int addr = dataset[i];
        cacheWriteBack->write(i, addr);
        cacheWriteThrough->write(i, addr);
    }

    std::cout << "\n======= Contenido Write-Back =======\n";
    cacheWriteBack->printCacheContents();
    mainMemBack.printMemoryContents();

    std::cout << "\n======= Contenido Write-Through =======\n";
    cacheWriteThrough->printCacheContents();
    mainMemThrough.printMemoryContents();

    std::cout << "\n=== Estadisticas iniciales ===\n";
    cacheWriteThrough->printStats();
    std::cout << "\n";
    cacheWriteBack->flush();
    cacheWriteBack->printStats();

    do
    {
        std::cout << "\n=== Menu ===\n";
        std::cout << "1. Leer de la cache\n";
        std::cout << "2. Escribir en la cache\n";
        std::cout << "3. Mostrar contenido de cache y memoria\n";
        std::cout << "4. Ejecutar pruebas automaticas\n";
        std::cout << "5. Salir\n";
        std::cout << "Ingrese su opcion: ";
        option = getValidNumber();

        switch (option)
        {
        case 1:
            std::cout << "Ingrese direccion a leer: ";
            addr = getValidNumber();

            valueThrough = cacheWriteThrough->read(addr);
            valueBack = cacheWriteBack->read(addr);

            if (valueThrough != -1 || valueBack != -1)
            {
                std::cout << "\nValor leido (Write-Through): " << valueThrough << "\n";
                std::cout << "Valor leido (Write-Back): " << valueBack << "\n";
            }
            else
            {
                std::cout << "Direccion no contiene datos.\n";
            }

            cacheWriteThrough->printStats();
            std::cout << "\n";
            cacheWriteBack->printStats();
            break;

        case 2:
            std::cout << "Ingrese direccion a escribir: ";
            addr = getValidNumber();
            std::cout << "Ingrese valor: ";
            value = getValidNumber();

            cacheWriteThrough->write(addr, value);
            cacheWriteBack->write(addr, value);

            std::cout << "Dato escrito en cache (Write-Through y Write-Back).\n";
            cacheWriteThrough->printStats();
            cacheWriteBack->printStats();
            break;

        case 3:
            std::cout << "\n======= Contenido Write-Through =======\n";
            cacheWriteThrough->printCacheContents();
            mainMemThrough.printMemoryContents();

            std::cout << "\n======= Contenido Write-Back =======\n";
            cacheWriteBack->printCacheContents();
            mainMemBack.printMemoryContents();
            break;

        case 4:
            realizarPruebasAutomáticas(cacheWriteThrough, cacheWriteBack, MIN_ADDR, DATASET_SIZE);
            cacheWriteThrough->printStats();
            std::cout << "\n";
            cacheWriteBack->flush(); // Asegurarse de que se escriban los datos sucios en memoria
            cacheWriteBack->printStats();
            break;

        case 5:
            std::cout << "Saliendo...\n";
            break;

        default:
            std::cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (option != 5);

    cacheWriteBack->flush();
    return 0;
}

std::vector<int> generateRandomDataset(size_t size, int minAddr, int maxAddr)
{
    std::vector<int> dataset(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(minAddr, maxAddr);

    for (auto &addr : dataset)
    {
        addr = dist(gen);
    }
    return dataset;
}

void realizarPruebasAutomáticas(WriteThroughCache *cacheWriteThrough, WriteBackCache *cacheWriteBack, int min, int max)
{
    int clave;
    std::vector<int> claves;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distFrecuente(0, 10);  // Valores más frecuentes
    std::uniform_int_distribution<> distOcasional(11, 25); // Algunos menos frecuentes

    for (int i = 0; i < 50; ++i) // Más iteraciones para ver diferencias
    {
        if (i % 3 == 0) // Cada 3 iteraciones, usamos un valor menos frecuente
            clave = distOcasional(gen);
        else
            clave = distFrecuente(gen); // Usamos valores más probables

        cacheWriteThrough->read(clave);
        cacheWriteBack->read(clave);
    }
}

int getValidNumber()
{
    int number;
    while (true)
    {
        std::cin >> number;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. Ingrese un numero: ";
        }
        else
        {
            return number;
        }
    }
}
