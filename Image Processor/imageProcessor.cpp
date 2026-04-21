#include "imageProcessor.h"

// Определение статического поля (обязательно в одной единице трансляции)
int ImageUtils::processedCount = 0;

int ImageUtils::getProcessedCount()
{
    return processedCount;
}