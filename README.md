# Project description
We scrape seriouseats.com for cocktail recipe data, which is added to an application with basic filtering capabilities.

# Organization
<ul>
  <li>The scrape is written in C++ and has two main components, contained in getRecipePagesFromList and scrapeRecipePagesFromList.</li>
  <li>The method getRecipePagesFromList reads from a file consisting of a list of (manually compiled) links to seriouseats subpages.  Each subpage contains a number of additional links to individual cocktail recipes, which are scraped using Regex and added to the file recipeLinks.</li>
  <li>The method scrapeRecipePagesFromList reads from recipeLinks, visits each page, scrapes recipe data using Regex, and adds the data to the file cocktailData</li>
  <li>The initializeScrape file contains main and calls the above two methods in order.</li>
  <li>The stringRegexCurlMethods file contains a number of methods for string manipulation, Regex interaction, and cURL methods for grabbing HTML returns from HTTP requests.</li>
  <li>The app is written in Swift and performs some basic filtering to respond to a user's selection criteria.</li>
</ul>

# Screenshots

Here are some screenshots with various filtering criteria selected.</br>

![Simulator Screenshot - iPhone 14 Pro - 2023-06-06 at 14 27 19](https://github.com/mycoolhairison/cocktail_data/assets/129091329/2e5fd11a-d669-4140-9d75-1f72f272ef3e)![Simulator Screenshot - iPhone 14 Pro - 2023-06-06 at 14 24 29](https://github.com/mycoolhairison/cocktail_data/assets/129091329/0e79d53a-2829-4897-86e0-e4dbe49ddd3f)![Simulator Screenshot - iPhone 14 Pro - 2023-06-06 at 14 26 51](https://github.com/mycoolhairison/cocktail_data/assets/129091329/e64ffed6-5fbc-4335-9865-9130348f0d7f)

Clicking a result will take you to the recipe page.</br>

![Simulator Screenshot - iPhone 14 Pro - 2023-06-06 at 14 28 53](https://github.com/mycoolhairison/cocktail_data/assets/129091329/5b940884-8c21-4068-b54a-3bbb745f32f7)

# Project development and the scraping process

The project idea arose from a common problem in my life: with a scattered selection of booze and ingredients, what cocktails could I make?  My original idea for this project was to scrape a collection of cocktail websites to get a wide range of recipes, and then do some sorting and filtering to find the best recipes.  But after scrolling through cocktail recipes on seriouseats.com, my go-to site for food and drinks, I realized that the collection there is sufficiently broad — containing both classic and experimental cocktails (buttermilk in a gin recipe, anyone?).  With about 500 cocktail recipes, I decided that collecting all of these would give me a pretty solid list of recipes.

When I actually investigated seriouseats.com to see how best to collect all of the cocktail data, I ran into some trouble.  Each cocktail/recipe has its own page, as can be seen in the screenshot above.  I wanted to visit all such recipe pages in a systematic way, and there wasn’t an obvious way to do this.  For comparison, here are a couple example which demonstrate how one might systematically traverse all pages.

## Examples

### Kroger

Suppose you want all the locations of the grocery store Kroger.  If you go to Kroger’s website and try to find stores, you end up here:
<img width="560" alt="Screenshot 2023-06-06 at 3 10 49 PM" src="https://github.com/mycoolhairison/cocktail_data/assets/129091329/2be5255c-772f-4cc8-bcd1-b73e2a7de3a0">

You are prompted to search for locations, by city or state or zip, but this is not ideal.  Though it's not the advertised search method, playing around with the URL a little bit will lead you to the following page:
<img width="560" alt="Screenshot 2023-06-06 at 3 44 44 PM" src="https://github.com/mycoolhairison/cocktail_data/assets/129091329/7d5b8e60-fc3f-4425-a650-c74099f8d2c6">
This page contains a link to every store location, so you could either scrape this single page (if you only care about city and state), or you could follow each link to grab a little more detail about each location.

### Costco 
Suppose you want all the locations of Costco.  With an initial search, we find that an example location appears at the following URL.</br>
https://www.costco.com/warehouse-locations/florence-ky-1197.html.</br>
Fiddling around with the link a bit, we find that the text after the final "/" and before "-1197" is irrelevant, and changing 1197 to 1198 will show you a new store location.  Explicitly, the URL</br>
https://www.costco.com/warehouse-locations/florence-ky-1198.html</br>
will return the location with ID 1198, in Pleasant Prairie, WI, even though it says Florence, KY in the address!  Similarly,</br>
https://www.costco.com/warehouse-locations/whatislove-1198.html</br>
and</br>
https://www.costco.com/warehouse-locations/babydonthurtme-1198.html</br>
and</br>
https://www.costco.com/warehouse-locations/nomore-1198.html</br>
all go to the same page.

This is great, because it allows us to visit pages simply by iterating through IDs, assuming we can reasonably bound the IDs (this wouldn’t be efficient if IDs have, for example, 9+ digits, and it would be pretty bad etiquette to hit a site so many times anyway).

## Seriouseats
Okay, back to seriouseats.com: I wasn’t able to find a page containing a long list of recipes, nor was I able to find any obvious identifiers to systematically iterate.  One could visit a few sites "randomly," by starting at a fixed page and continuing to follow links for "suggested" or "similar" items, grabbing any recipe URLs and adding them to a set for future scraping, but this seems far too unpredictable: if we somehow end up in a cycle of whiskey recipes, will we ever find a margarita?  I considered trying to first find lists of cocktails, like this one:</br>
https://www.seriouseats.com/cocktails-everyone-should-know</br>
and collecting the 25 links there to later grab the recipe details, but I also didn’t see any easy way to get a list of these lists either.  I considered searching "cocktails," but the results are less recipes and more articles about cocktails; and moreover, it seems the number of results is limited to 120.

I finally landed on a semi-manual approach.  Starting from here:</br>
https://www.seriouseats.com/cocktail-recipes-5117858
one can click on various spirits (brandy, vodka, etc) to get a list of links to all recipes containing that spirit.  So for example, collecting all the links at</br>
https://www.seriouseats.com/vodka-recipes-5117850</br>
and</br>
https://www.seriouseats.com/gin-recipes-5117855</br>
presumably gives me a complete list of recipes containing vodka and gin.  Some drinks, like the Fog Cutter, appear on multiple lists.

I panicked for a moment when I saw that the whiskey, tequila, and rum pages all contain exactly 64 entries — are the lists capped and incomplete?  Due to this uncertainty, I landed on a hybrid approach: scrape all these "spirit" pages (gin, whiskey, tequila) to get cocktail links, and also scrape a few dozen "big list" pages: easy cocktails, Memorial Day cocktails.  A bonus of this approach, which I did not realize at the time, but was able to incorporate later, is that I can add appropriate keywords depending on which list the link was on, and these keywords can be used for later cocktail filtering by season, difficulty, spirit, etc., as can be seen in the app screenshots.  Although this approach meant that I had to feed in about 30-40 initial links, and manually add the keywords, I couldn’t find a more reasonable systematic way to go about it.

Content with the 
Checking out the HTML for each of these "spirit pages", the links I needed were nicely formatted in JSON and collected together in the middle of 4000 lines of HTML:

{
"@type": "ListItem"
,"position": 9
,"url": "https://www.seriouseats.com/classic-sangrita-orange-pomegranate-recipe"
}
,
{
"@type": "ListItem"
,"position": 10
,"url": "https://www.seriouseats.com/the-best-frozen-lime-margaritas-recipe-tequila-cocktail-for-parties"
}
,
{
"@type": "ListItem"
,"position": 11
,"url": "https://www.seriouseats.com/cucumber-celery-tequila-cooler-simple-cocktail-summer-recipe"
}

The logic of the link-gathering method is pretty straightforward:

//Create vector of URL suffixes to scrape, along with relevant keywords.						
std::vector<std::vector<std::string>> allSuffixes;										
																				
//A sample suffix/keywords pair, there are a few dozen of these.								
allSuffixes.push_back({"brandy-cognac-recipes-5117857","brandy,cognac"});						
																				
for(std::vector<std::string> suffix : allSuffixes) {										
    std::string url = "https://www.seriouseats.com/" + suffix[0];							
																				
    //Grab the HTML located at the given URL;                    							
    std::string htmlString = getPayload(url); 											
                   																	
    //Truncate both ends of huge html payload;									  		
    std::string truncatedHTMLString = getSubHTMLString(htmlString,"ItemList","numberOfItems");		
																				
    //Grab and trim suffixes of recipe pages according to regex pattern						
    std::vector<std::string> regMatches = getAllRegexMatches(truncatedHTMLString,				
	   (std::regex)",\"url\": \".*\"",37,38);											
																						
    //Add scraped link, along with the keywords, to an unordered_map							
    for(std::string s : regMatches) {													
        if(linksAndKeywords.find(s)==linksAndKeywords.end()) linksAndKeywords[s]=suffix[1];		
        else linksAndKeywords[s]+=","+suffix[1];											
    } 																			
}																				

The logic of the method which actually scrapes the cocktail data is similar: visit each recipe page, collect relevant recipe data according to carefully constructed regex patterns, and output this data.

This gives us about 500 cocktails in a text file, formatted like this:

["Clamato Sangrita With Jalapeño and Coriander","5","1","clamato-sangrita-jalapeno-recipe","cocktail, tequila","Recipes,Recipes By Course,Drinks,Cocktails,Tequila","tequila"]

We have the name, some rating information, and a bunch of keywords which will help us with future filtering.

Although I was able to scrape the ingredients for each cocktail, the formatting of this ingredient list was so bad so as to be basically unusable.  For example, here’s the ingredient list for a “Michelada”

Tajín or other chile-salt blend (optional; see note)
Kosher or sea salt
2 ounces (60ml) fresh juice from 2 limes (see note), half a juiced lime reserved for the rim
2 teaspoons (10ml) hot sauce, preferably a Mexican-style brand like Tapatío (our favorite); see note
1 teaspoon (5ml) Worcestershire sauce
1 (12-ounce; 355ml) can or bottle light Mexican beer, such as Modelo, Pacífico, Tecate, Victoria, or Corona, well chilled

Ideally we’d want a searchable list to look like “Tajín, salt, limes, hot sauce, Worcestershire, Mexican beer”, but it would take quite a bit more work to map long descriptive strings to important keywords.  Unfortunately these ingredients were rarely listed with other keywords and tags in the HTML, and so I had to scrap the idea of searching by ingredient.

In the end, the collected keywords and ratings gave enough data to build a simple filtering app.  Here are some screenshots of the finished product.


