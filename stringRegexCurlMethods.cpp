
#include "stringRegexCurlMethods.h"
#include <chrono>
#include <curl/curl.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <thread>

std::string getPayload(const std::string& url) {
    //5 second pause to avoid hitting site too frequently
    std::chrono::seconds dura(5);
    std::this_thread::sleep_for(dura);

    CURL *curl;
    CURLcode res;
    std::string htmlString;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlString);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return htmlString;
}

size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string getSubHTMLString(const std::string& htmlString, const std::string& startString, const std::string& endString) {
    int start = 0;
    int end = 0;
    if(htmlString.find(startString)!=std::string::npos) {
        start = htmlString.find(startString)+startString.length()+2;
    }
    if(htmlString.find(endString)!=std::string::npos) {
        end = htmlString.find(endString);
    }
    return (end==0 ? htmlString : htmlString.substr(start,end-start));
}

void replaceStringWithString(std::string& wholeString, const std::string& toDelete, const int& lengthOfDelete, const std::string& toInsert) {
    while(wholeString.find(toDelete) != std::string::npos) {
        wholeString.replace(wholeString.find(toDelete),lengthOfDelete,toInsert);
    }
}

void replaceSomeUnicode(std::string& wholeString) {
    replaceStringWithString(wholeString,"&#39;",5,"'");
    replaceStringWithString(wholeString,"\xc2\xa0",2," ");
    replaceStringWithString(wholeString,"\xe2\x80\x94",3,"-");
    replaceStringWithString(wholeString,"\xe2\x80\x99",3,"'");
}

std::vector<std::string> getAllRegexMatches(std::string htmlString, const std::regex& matchMe, const int& trimStart, const int& trimEnd) {
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

std::string getOneRegexMatch(const std::string& htmlString, const std::regex& matchMe, const int& trimStart, const int& trimEnd) {
    std::smatch sm;
    std::string ret = "";
    if(regex_search(htmlString, sm, matchMe)) {
        ret = sm.str(0).substr(trimStart,sm.str(0).length()-trimEnd);
        replaceSomeUnicode(ret);
    }
    return ret;
}

std::pair<std::string,std::string> splitStringOnce(const std::string& toSplit, const char& delim) {
    std::pair<std::string,std::string> ret {};
    if(toSplit.find(delim)!=std::string::npos) {
        ret.first = toSplit.substr(0,toSplit.find(delim));
        ret.second = toSplit.substr(toSplit.find(delim)+1,toSplit.length()-toSplit.find(delim));
    }
    return ret;
}

std::vector<std::pair<std::string,std::string>> inputLinksAndKeysFromFile(std::string fileName) {
    std::vector<std::pair<std::string,std::string>> ret;
    std::string line;
	std::fstream file (fileName, std::ios::in);
	if(file.is_open()) {
		while(getline(file, line)) {
			std::stringstream str(line);
            ret.push_back(splitStringOnce(line,';'));
        }
	}
    return ret;
}

void outputLinksAndKeysToFile(std::unordered_map<std::string,std::string> toOutput, std::string fileName) {
    std::ofstream writeMe(fileName, std::ios_base::app | std::ios_base::out);
    for(auto i : toOutput) {
        writeMe<<i.first<<";"<<i.second<<'\n';
    }
}
