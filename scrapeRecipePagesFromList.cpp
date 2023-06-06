
#include "initializeScrape.h"
#include "scrapeRecipePagesFromList.h"
#include "stringRegexCurlMethods.h"
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

class Cocktail {
private:
    std::string name;
    double rating;
    int numberOfRatings;
    std::string keywordString;
    std::string moreKeys;
    std::string tagsString;
    std::string urlSuffix;
    bool full;

public:
    //Constructor which scrapes given page for cocktail data, sets variable "full" to true if successful
    Cocktail(const std::string& cocktailURL, const std::string& extraKeys) {
        urlSuffix = cocktailURL;
        moreKeys = extraKeys;
        std::string htmlString = getPayload(domainName+urlSuffix);

        //Some links don't lead to cocktail recipes; this check ensures we're at a recipe page
        if(htmlString.find("recipeIngredient")!=std::string::npos) {
            full=true;

            //Isolate header text, grab header data, and remove redundant words from name
            std::string headerString = getSubHTMLString(htmlString,"head class","Twitter Card");
            name = getOneRegexMatch(headerString,(std::regex)"<title>[^<]*</title>",7,15);
            tagsString = getOneRegexMatch(headerString,(std::regex)"parsely-tags\" content=\"[^\"]*\"",23,24);
            replaceStringWithString(name," Cocktail Recipe",16,"");
            replaceStringWithString(name," Recipe",7,"");
            
            //Isolate keywords/rating text, grab keywords and rating data
            std::string keywordsAndRating = getSubHTMLString(htmlString,"AggregateRating","recipeIngredient");
            keywordString = getOneRegexMatch(keywordsAndRating,(std::regex)"keywords\": \"[^\"]*\"",12,13);
            if(keywordsAndRating.find("ratingValue") != std::string::npos) {
                rating = std::stod(getOneRegexMatch(keywordsAndRating,(std::regex)"ratingValue\": \"[^\"]*\"",15,16));
                numberOfRatings = std::stoi(getOneRegexMatch(keywordsAndRating,(std::regex)"ratingCount\": \"[^\"]*\"",15,16));
            }
            else {
                rating = -1;
                numberOfRatings = 0;
            }
        }
        else {
            full = false;
        }

    }

    bool isFull() { return full; }

    //Output cocktail data in Swift-compatible format (for array of array of strings).
    void ship(std::string fileName) {
        std::ofstream writeMe(fileName, std::ios_base::app | std::ios_base::out);
        writeMe<<"[\""<<name<<"\",\""<<rating<<"\",\""<<numberOfRatings<<"\",\""<<urlSuffix<<"\",\""
            <<keywordString<<"\",\""<<tagsString<<"\",\""<<moreKeys<<"\"],";
    }
};

void scrapeRecipePagesFromList() {
    std::vector<std::pair<std::string,std::string>> recipeLinksAndKeys = inputLinksAndKeysFromFile(recipeLinksAndKeysFilename);
    for(const std::pair<std::string,std::string>& linkAndKey : recipeLinksAndKeys) {
        std::string urlSuffix = linkAndKey.first;
        std::string extraKeys = linkAndKey.second;
        
        //This constructor scrapes given page and sets member variables.
        Cocktail drink = Cocktail(urlSuffix,extraKeys);
        //Output data provided scrape/constructor was successful.
        if(drink.isFull()) drink.ship(cocktailDataFilename);
    }
}
