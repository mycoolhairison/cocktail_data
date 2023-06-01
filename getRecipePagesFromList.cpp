#include "getRecipePagesFromList.h"
#include "initializeScrape.h"
#include "stringRegexCurlMethods.h"
#include <regex>
#include <string>

void getRecipePagesFromList()
{
    //Define map to store link/keyword pairs
    std::unordered_map<std::string,std::string> recipeLinksAndKeywords;
    
    //Here we find recipe links on the "spirit" pages.
    std::vector<std::pair<std::string,std::string>> linksAndKeysBySpirit = inputLinksAndKeysFromFile(listBySpiritLinksAndKeysFilename);
    for(const std::pair<std::string,std::string>& linkAndKey : linksAndKeysBySpirit) {
        //Grab and truncate HTML
        std::string htmlString = getPayload(domainName+linkAndKey.first);
        std::string truncatedHTMLString = getSubHTMLString(htmlString,"ItemList","numberOfItems");

        //Grab and trim suffixes of recipe pages according to regex pattern
        std::vector<std::string> urlSuffixes = getAllRegexMatches(truncatedHTMLString,(std::regex)",\"url\": \".*\"",37,38);

        //Add scraped link/keyword pairs to our map
        for(const std::string& urlSuffix : urlSuffixes) {
            if(recipeLinksAndKeywords.find(urlSuffix)==recipeLinksAndKeywords.end()) {
                recipeLinksAndKeywords[urlSuffix]=linkAndKey.second;
            }
            else recipeLinksAndKeywords[urlSuffix]+=","+linkAndKey.second;
        }
    }

    //Here we find recipe links on the "type" pages.  Logic is same as above, just a different regex pattern.
    std::vector<std::pair<std::string,std::string>> linksAndKeysByType = inputLinksAndKeysFromFile(listByTypeLinksAndKeysFilename);
    for(const std::pair<std::string,std::string>& linkAndKey : linksAndKeysByType) {
        std::string htmlString = getPayload(domainName+linkAndKey.first);
        std::string truncatedHTMLString = getSubHTMLString(htmlString,"base order","outro struct");

        //Several different regex patterns needed here for garbage-tier human-entered HTML.
        std::vector<std::string> urlSuffixes = getAllRegexMatches(truncatedHTMLString,
            (std::regex)"featuredlink mntl-block\">[\n\r\\s]*<a[\n\r\\s]*href=\".*\"",63,64);
        if(urlSuffixes.empty()) {
            urlSuffixes = getAllRegexMatches(truncatedHTMLString,(std::regex)"<a href=\"[^\"]*\"(?=[^<>]*>Get)",37,38);
        }

        for(const std::string& urlSuffix : urlSuffixes) {
            if(recipeLinksAndKeywords.find(urlSuffix)==recipeLinksAndKeywords.end()) {
                recipeLinksAndKeywords[urlSuffix]=linkAndKey.second;
            }
            else recipeLinksAndKeywords[urlSuffix]+=","+linkAndKey.second;
        }
    }

    //Output link/key pairs to file
    outputLinksAndKeysToFile(recipeLinksAndKeywords,recipeLinksAndKeysFilename);
}