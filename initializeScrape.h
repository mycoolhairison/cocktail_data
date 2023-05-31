#pragma once

#include <string>
#include <regex>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
std::string getPayload(std::string url);
std::string getSubHTMLString(std::string htmlString, std::string startString, std::string endString);
void replaceStringWithString(std::string& wholeString, std::string toDelete, int lengthOfDelete, std::string toInsert);
void replaceSomeUnicode(std::string& wholeString);
std::vector<std::string> getAllRegexMatches(std::string htmlString, std::regex matchMe, int trimStart, int trimEnd);   
std::string getOneRegexMatch(std::string htmlString, std::regex matchMe, int trimStart, int trimEnd);
std::vector<std::string> splitString(std::string toSplit, char delim);