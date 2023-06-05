#ifndef STRING_REGEX_CURL_METHODS_H
#define STRING_REGEX_CURL_METHODS_H

#include <string>
#include <regex>

//Grabs HTML at url
std::string getPayload(const std::string& url);

//Called with curl to store HTML in a string
size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

//Reduce size of huge HTML payload, to allow for more robust regex patterns and faster searches
std::string getSubHTMLString(const std::string& htmlString, const std::string& startString, const std::string& endString);

//Used for cleaning data; specifically to handle some unicode in the regex grabs
void replaceStringWithString(std::string& wholeString, const std::string& toDelete, const int& lengthOfDelete, const std::string& toInsert);

//Replace some specific common unicode in the regex grabs
void replaceSomeUnicode(std::string& wholeString);

//Returns multiple matches for a regex pattern (with unicode replaced)
std::vector<std::string> getAllRegexMatches(std::string htmlString, const std::regex& matchMe, const int& trimStart, const int& trimEnd);   

//Returns single match for a regex pattern (with unicode replaced)
std::string getOneRegexMatch(const std::string& htmlString, const std::regex& matchMe, const int& trimStart, const int& trimEnd);

//Split a string at first instance of given delimiter
std::pair<std::string,std::string> splitStringOnce(const std::string& toSplit, const char& delim);

//Input link/key pairs which are delimited by semicolon
std::vector<std::pair<std::string,std::string>> inputLinksAndKeysFromFile(std::string fileName);

//Output link/key pairs delimited by semicolon
void outputLinksAndKeysToFile(std::unordered_map<std::string,std::string> toOutput, std::string fileName);

#endif