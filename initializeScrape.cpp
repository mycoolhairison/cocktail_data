
#include "getRecipePagesFromList.h"
#include "initializeScrape.h"
#include "scrapeRecipePagesFromList.h"
#include "stringRegexCurlMethods.h"

int main() {

    getRecipePagesFromList();
    scrapeRecipePagesFromList();
    
    return 0;
}
