#include "getRecipePagesFromList.h"
#include "initializeScrape.h"
#include <regex>
#include <string>

void getRecipePagesFromList(std::unordered_map<std::string,std::string>& linksAndKeywords)
{
    //List of URL suffixes to scrape, along with relevant keywords
    //Here we are scraping pages sorted by season,liquor,holiday, etc.
    std::vector<std::vector<std::string>> allSuffixes;
    allSuffixes.push_back({"memorial-day-batch-cocktails","memorial,batch,holiday,spring,summer"});
    /*allSuffixes.push_back({"cinco-de-mayo-cocktail-recipes","cinco de mayo,holiday,spring"});
    allSuffixes.push_back({"simple-3-ingredient-drinks-to-make-at-home","simple,easy"});
    allSuffixes.push_back({"best-rye-whiskey-cocktail-recipes","rye,whiskey"});
    allSuffixes.push_back({"negroni-variations-bitter-cocktail-drink-fall-winter","negroni,bitter,fall,winter"});
    allSuffixes.push_back({"rum-cocktail-recipes","rum"});
    allSuffixes.push_back({"scotch-cocktail-recipes","scotch"});
    allSuffixes.push_back({"aperol-cocktails-recipes-mixed-drinks","aperol"});
    allSuffixes.push_back({"cocktails-everyone-should-know","classic"});
    allSuffixes.push_back({"what-to-make-with-cynar-3-cocktail-recipes-easy-drinks","cynar"});
    allSuffixes.push_back({"hot-cocktail-recipes","winter"});
    allSuffixes.push_back({"thanksgiving-cocktails","thanksgiving,holiday,fall,autumn"});
    allSuffixes.push_back({"frozen-cocktails","frozen"});
    allSuffixes.push_back({"apple-cocktails","apple,fall,autumn"});
    allSuffixes.push_back({"what-to-make-with-allspice-dram-cocktails-fall-drink-ideas","fall,allspice"});
    allSuffixes.push_back({"mezcal-cocktail-recipes-to-enjoy-mexicos-smoky-spirit","mezcal,smoky"});
    allSuffixes.push_back({"cocktails-with-hard-cider-woodchuck-crispin-apple-drinks-fall-entertaining-party-cocktails","apple,crisp,fall,autumn"});
    allSuffixes.push_back({"what-to-make-with-grapefruits-cocktail-drink-idea-recipes-winter-drinks","grapefruit,winter"});
    allSuffixes.push_back({"best-cocktails-mothers-day","holiday,light,brunch"});
    allSuffixes.push_back({"easy-bourbon-cocktails-just-one-bottle-drink-recipes-fall-entertaining","bourbon,fall,simple,easy"});
    allSuffixes.push_back({"international-summer-drinks-agua-frsca-refajo-caipirinha-nam-manao-hot-weather-drink-ideas","summer"});
    allSuffixes.push_back({"sparkling-cocktails","sparkling,bubbly"});
    allSuffixes.push_back({"beer-cocktail-drink-recipes","beer,summer"});
    allSuffixes.push_back({"best-gin-cocktails-recipes-drinks","gin"});
    allSuffixes.push_back({"forgotten-classic-cocktails-you-should-know-drink-recipes","classic"});
    allSuffixes.push_back({"easy-vodka-cocktails-one-bottle-simple-drink-recipes-made-with-vodka","vodka,easy,simple"});
    allSuffixes.push_back({"easy-rum-cocktails-simple-drinks-just-rum-fruit-soda-summer-drink-ideas","rum,easy,simple,summer"});
    allSuffixes.push_back({"cocktails-for-lovers-and-cynics-on-valentines","valentine"});
    allSuffixes.push_back({"the-cider-press-summer-cider-cocktails","summer,cider"});
    allSuffixes.push_back({"essential-fernet-branca-cocktails-making-drinks-at-home","fernet branca"});
    allSuffixes.push_back({"mardi-gras-party-cocktails-new-orleans-classic-drinks","mardi gras,holiday"});
    allSuffixes.push_back({"new-orleans-cocktail-recipes-celebrate-mardi-gras-party-how-to-make-hurricane-sazerac-pimms-gin-fizz-milk-punch","mardi gras,holiday"});
    allSuffixes.push_back({"easy-gin-cocktails","gin,easy,simple"});
    allSuffixes.push_back({"best-irish-whiskey-cocktail-recipes-st-patricks-day","whiskey,irish,patty's"});
    allSuffixes.push_back({"campari-cocktails","campari"});*/
    
    
    for(std::vector<std::string> suffix : allSuffixes) {
        std::string url = "https://www.seriouseats.com/" + suffix[0];
        std::string htmlString = getPayload(url);
        
        //Truncate both ends of huge html payload;
        std::string truncatedHTMLString = getSubHTMLString(htmlString,"base order","outro struct");

        //Grab and trim suffixes of recipe pages according to the regex pattern
        std::vector<std::string> regMatches = getAllRegexMatches(truncatedHTMLString,
            (std::regex)"featuredlink mntl-block\">[\n\r\\s]*<a[\n\r\\s]*href=\".*\"",63,64);
        
        //Add scraped link/keyword pairs to our map.
        for(std::string s : regMatches) {
            if(linksAndKeywords.find(s)==linksAndKeywords.end()) linksAndKeywords[s]=suffix[1];
            else linksAndKeywords[s]+=","+suffix[1];
        }
    }
}