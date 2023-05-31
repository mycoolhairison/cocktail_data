#include "initializeScrape.h"
#include "scrapeRecipePagesFromList.h"
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

class Cocktail {
public:
    std::string name;
    std::string spirit;
    double rating;
    int numberOfRatings;
    std::vector<std::string> ingredients;
    std::vector<std::string> keywords;
    std::vector<std::string> tags;
    std::string keywordString;
    std::string moreKeys;
    std::string tagsString;
    std::string url;
    std::string id;
    std::string description;

    //Output cocktail data in Swift-compatible format (for array of array of strings).
    void ship(std::string fileName) {
        std::ofstream writeMe(fileName, std::ios_base::app | std::ios_base::out);
        writeMe<<"[\""<<name<<"\",\""<<rating<<"\",\""<<numberOfRatings<<"\",\""<<url<<"\",\""
            <<keywordString<<"\",\""<<tagsString<<"\",\""<<moreKeys<<"\"],";
    }

};

void scrapeRecipePagesFromList() {

    //input link/keyword pair from file
    std::vector<std::vector<std::string>> linksAndKeys;
	std::string line;
	std::fstream file ("links.txt", std::ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			std::stringstream str(line);
            std::vector<std::string> splits = splitString(line,',');
            linksAndKeys.push_back(splits);
        }
	}



    for(int j = 0; j<linksAndKeys.size(); j++) {

        //Parse the link and the extra keys
        std::string urlSuffix = linksAndKeys[j][0];
        std::string url = "https://www.seriouseats.com/" + urlSuffix;

        std::string extraKeys="";
        for(int i = 1; i<linksAndKeys[j].size()-1; i++) {
            extraKeys+=(linksAndKeys[j][i]+",");
        }
        if(linksAndKeys[j].size()>1) {
            extraKeys+=linksAndKeys[j][linksAndKeys[j].size()-1];
        }
        
        //Initialize an element of the Cocktail class and add the URL and keys
        Cocktail drinkMe;
        drinkMe.url = urlSuffix;
        drinkMe.moreKeys=extraKeys;

        //Initiate scrape to gather additional data.
        std::string htmlString = getPayload(url);

        //Some links don't lead to cocktail recipes; this check ensures we're at a recipe page
        if(htmlString.find("recipeIngredient")!=std::string::npos) {

            //Isolate header text
            std::string headerString = getSubHTMLString(htmlString,"head class","Twitter Card");

            //Grab data from header
            drinkMe.id = getOneRegexMatch(headerString,(std::regex)"id=\"[^\"]*\"",4,5);
            drinkMe.description = getOneRegexMatch(headerString,(std::regex)"description\" content=\"[^\"]*\" item",22,28);
            drinkMe.tagsString = getOneRegexMatch(headerString,(std::regex)"parsely-tags\" content=\"[^\"]*\"",23,24);
            drinkMe.tags = splitString(drinkMe.tagsString,',');
            drinkMe.name = getOneRegexMatch(headerString,(std::regex)"<title>[^<]*</title>",7,15);

            //Remove redundant words from name
            if(drinkMe.name.find(" Cocktail Recipe")!=std::string::npos)
                drinkMe.name.replace(drinkMe.name.find(" Cocktail Recipe"),16,"");
            if(drinkMe.name.find(" Recipe")!=std::string::npos)
                drinkMe.name.replace(drinkMe.name.find(" Recipe"),7,"");
            
            //Isolate keywords/rating chunk
            std::string keywordsAndRating = getSubHTMLString(htmlString,"AggregateRating","recipeIngredient");
            
            //Grab keywords
            drinkMe.keywordString = getOneRegexMatch(keywordsAndRating,(std::regex)"keywords\": \"[^\"]*\"",12,13);
            drinkMe.keywords = splitString(drinkMe.keywordString,',');

            //Grab ratings
            if(keywordsAndRating.find("ratingValue") != std::string::npos) {
                drinkMe.rating = std::stod(getOneRegexMatch(keywordsAndRating,(std::regex)"ratingValue\": \"[^\"]*\"",15,16));
                drinkMe.numberOfRatings = std::stoi(getOneRegexMatch(keywordsAndRating,(std::regex)"ratingCount\": \"[^\"]*\"",15,16));
            }
            else {
                drinkMe.rating = -1;
                drinkMe.numberOfRatings = 0;
            }
            drinkMe.spirit = getOneRegexMatch(keywordsAndRating,(std::regex)"recipeCategory\": \\[\"[^\"]*\"",19,20);

            //Grab ingredients
            if(htmlString.find("structured-ingredients__list-item") != std::string::npos) {
                drinkMe.ingredients = getAllRegexMatches(getSubHTMLString(htmlString,"ingredients section","instructions section"),
                    (std::regex)"name=\"true\">[^<]*</sp",12,16);
            }
            else {
                drinkMe.ingredients = getAllRegexMatches(getSubHTMLString(htmlString,"recipeIngredient","recipeInstructions"),
                    (std::regex)"\"[^\"]*\"",1,2);
            }

            //Output collected recipe data
            drinkMe.ship("cocktailData.txt");
        }
    }

    

}