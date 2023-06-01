#ifndef GET_RECIPE_PAGES_FROM_LIST_H
#define GET_RECIPE_PAGES_FROM_LIST_H

/*  
//  This method visits a selection of pages, each of which contains many links to cocktail recipes.
//  There are two types of pages: "spirit" (gin,vodka,etc) and "type" (holiday,winter,fruity,etc).
//  These two types of pages require different regex patterns, so the method consists of two similar parts.
//
//  We add scraped links to a map: the key is the link to the recipe page, and the value is a string of keywords.
//  Map is convenient for adding multiple keywords,
//  for example if a cocktail appears on both the "vodka" and "winter" lists.
//  
//  The elements of the map are then output to a file.
//  Although we could instead pass the map directly to scrapeRecipePagesFromList.cpp,
//  it is preferred to have the recipeLinks on file, since it is not uncommon for scrapes to halt prematurely
//  (for example, in case of rate limiting), and this way we can resume where we left off.
*/

void getRecipePagesFromList();

#endif