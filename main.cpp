#include "CommandHandler.h"

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        int bulkSize = std::atoi(argv[1]);

        if (bulkSize > 0)
        {
            CommandHandler c(bulkSize);
            c.start();
        }
    }

    return 0;
}