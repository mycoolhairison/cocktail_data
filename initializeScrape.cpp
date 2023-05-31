#include "getRecipePages.h"
#include "getRecipePagesFromList.h"
#include "initializeScrape.h"
#include "scrapeRecipePagesFromList.h"
#include <chrono>
#include <curl/curl.h>
#include <fstream>
#include <regex>
#include <string>
#include <thread>

//Grabs HTML at url
std::string getPayload(std::string url) {

    //5 second pause to avoid hitting site too frequently
    std::chrono::seconds dura(5);
    std::this_thread::sleep_for(dura);

    CURL *curl;
    CURLcode res;
    std::string htmlString;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlString);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return htmlString;
}

//Called with curl to store html in a string
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//Reduce size of huge HTML payload, to allow for more robust regex patterns and faster searches
std::string getSubHTMLString(std::string htmlString, std::string startString, std::string endString) {
    int start = 0;
    int end = 0;
    if(htmlString.find(startString)!=std::string::npos)
        start = htmlString.find(startString)+startString.length()+2;
    if(htmlString.find(endString)!=std::string::npos)
        end = htmlString.find(endString);
    return (end==0 ? htmlString : htmlString.substr(start,end-start));
}

//Used for cleaning data; specifically to handle some unicode in the regex grabs
void replaceStringWithString(std::string& wholeString, std::string toDelete, int lengthOfDelete, std::string toInsert) {
    while(wholeString.find(toDelete) != std::string::npos) {
        wholeString.replace(wholeString.find(toDelete),lengthOfDelete,toInsert);
    }
}

//Replace some specific common unicode in the regex grabs
void replaceSomeUnicode(std::string& wholeString) {
    replaceStringWithString(wholeString,"&#39;",5,"'");
    replaceStringWithString(wholeString,"\xc2\xa0",2," ");
    replaceStringWithString(wholeString,"\xe2\x80\x94",3,"-");
    replaceStringWithString(wholeString,"\xe2\x80\x99",3,"'");
}

//Returns multiple matches for a regex pattern (with unicode replaced)
std::vector<std::string> getAllRegexMatches(std::string htmlString, std::regex matchMe, int trimStart, int trimEnd) {
    std::smatch sm;
    std::vector<std::string> ret;
    while(regex_search(htmlString, sm, matchMe)) {
        std::string temp = sm.str(0).substr(trimStart,sm.str(0).length()-trimEnd);
        replaceSomeUnicode(temp);
        ret.push_back(temp);
        htmlString = sm.suffix().str();
    }
    return ret;
}

//Returns single match for a regex pattern (with unicode replaced)
std::string getOneRegexMatch(std::string htmlString, std::regex matchMe, int trimStart, int trimEnd) {
    std::smatch sm;
    std::string ret = "";
    if(regex_search(htmlString, sm, matchMe)) {
        ret = sm.str(0).substr(trimStart,sm.str(0).length()-trimEnd);
        replaceSomeUnicode(ret);
    }
    return ret;
}

//For preferred formatting
std::vector<std::string> splitString(std::string toSplit, char delim) {
    std::vector<std::string> ret {};
    int i = 0;
    std::string temp {};
    while(i < toSplit.length()) {
        if(temp=="") {
            while(i<toSplit.length() && isspace(toSplit[i])) i++;
        }
        if(toSplit[i]!=delim) temp.push_back(toSplit[i]);
        else {
            if(!temp.empty()) ret.push_back(temp);
            temp="";
        }
        i++;
        if(i==toSplit.length()) ret.push_back(temp);
    }
    return ret;
}



int main() {

    /* The scrape consists of two distinct parts:
    ** First we visit a few dozen webpages, each of which contains many links to cocktail recipes.
    ** We add all these links to a file along with relevant keywords, chosen based on where we found the link.
    ** Since some links are found on multiple pages, we store the link/keyword pairs in a map. 
    ** Second we read links from the file, visit and scrape each page, and output the cocktail data.
    */

    std::unordered_map<std::string,std::string> linksAndKeywords;

    //Each method below finds cocktail links and adds link/keyword pairs to the map
    getRecipePages(linksAndKeywords);
    getRecipePagesFromList(linksAndKeywords);

    //Now output all link/keyword pairs from the map to links.txt.
    std::ofstream writeMe("links.txt", std::ios_base::app | std::ios_base::out);
    for(auto i : linksAndKeywords) {
        writeMe<<i.first<<","<<i.second<<'\n';
    }
    
    //The method below inputs links.txt, scrapes each page for cocktail data, and outputs the data.
    scrapeRecipePagesFromList();
    
    return 0;
}