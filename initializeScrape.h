
#ifndef INITIALIZE_SCRAPE_H
#define INITIALIZE_SCRAPE_H

#include <string>
#include <regex>

//  Apparently constexpr does not work with std::string and it would be good to know the proper solution.
//  This still lets me avoid hard-coding these names, but these char arrays will be cast to strings later.
constexpr char domainName[] = "https://www.seriouseats.com/";
constexpr char recipeLinksAndKeysFilename[] = "recipeLinks.txt";
constexpr char listBySpiritLinksAndKeysFilename[] = "listBySpiritLinks.txt";
constexpr char listByTypeLinksAndKeysFilename[] = "listByTypeLinks.txt";
constexpr char cocktailDataFilename[] = "cocktailData.txt";

#endif
