#include "getRecipePages.h"
#include "initializeScrape.h"
#include <regex>
#include <string>

void getRecipePages(std::unordered_map<std::string,std::string>& linksAndKeywords)
{

    //List of URL suffixes to scrape, along with relevant keywords.
    //Here we are scraping pages organized by the liquor in the cocktail.
    std::vector<std::vector<std::string>> allSuffixes;
    allSuffixes.push_back({"brandy-cognac-recipes-5117857","brandy,cognac"});
    allSuffixes.push_back({"champagne-recipes-5117856","champagne"});
    allSuffixes.push_back({"gin-recipes-5117855","gin"});
    allSuffixes.push_back({"liqueur-fortified-wine-recipes-5117854","liqueur"});
    allSuffixes.push_back({"mezcal-recipes-5117853","mezcal"});
    allSuffixes.push_back({"rum-recipes-5117852","rum"});
    allSuffixes.push_back({"sangria-recipes-5117851","sangria"});
    allSuffixes.push_back({"tequila-recipes-5117848","tequila"});
    allSuffixes.push_back({"vodka-recipes-5117850","vodka"});
    allSuffixes.push_back({"whiskey-recipes-5117847","whiskey"});

    for(std::vector<std::string> suffix : allSuffixes) {
        std::string url = "https://www.seriouseats.com/" + suffix[0];
        std::string htmlString = getPayload(url);

        //Truncate both ends of huge html payload;
        std::string truncatedHTMLString = getSubHTMLString(htmlString,"ItemList","numberOfItems");

        //Grab and trim suffixes of recipe pages according to regex pattern
        std::vector<std::string> regMatches = getAllRegexMatches(truncatedHTMLString,(std::regex)",\"url\": \".*\"",37,38);

        //Add scraped link/keyword pairs to our map
        for(std::string s : regMatches) {
            if(linksAndKeywords.find(s)==linksAndKeywords.end()) linksAndKeywords[s]=suffix[1];
            else linksAndKeywords[s]+=","+suffix[1];
        }
    }
}