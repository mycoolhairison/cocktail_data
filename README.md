The purpose of this project is to scrape Cocktail recipe data from seriouseats.com.
initializeScrape.cpp contains main and calls the three other files.
get*.cpp each scrape preliminary pages to gather a list of cocktail recipe links, output to links.txt.
scrape*.cpp reads links.txt, visits and scrapes cocktail data from each page, and outputs to cocktailData.txt.