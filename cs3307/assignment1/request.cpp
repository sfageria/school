
#include <cstddef>
#include <iostream>
#include <array>
#include <curl/curl.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string>


#include "request.h"


using namespace std;

struct MemoryStruct
{
    char* memory;
    size_t size;
};

struct team* teams;

static size_t
WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    //((string*)userp)->append((char*)contents, 0, size*nmemb);
    size_t realsize = size * nmemb;


    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

vector<Game> createVector(nlohmann::json & myjson) {
    vector<Game> vGame;

    auto& dates = myjson["dates"];
    //cout << dates.items() << endl;
    //cout << myjson["dates"][21] << endl;

    for (int i = 0; i < dates.items(); i++)
    {
        auto z = dates[i];
        if (z.is_null()) {
            //cout << "HERE";
            break;
        }
        //std::cout << myjson["dates"][0]["games"][0]["teams"]["away"]["score"]<< std::endl;
            auto item = dates[i]["games"];
            int games = sizeof(item);
            //cout << item.items() << endl;
            for (int x = 0; x < games; x++)
            {
                try {

                    if (myjson["dates"][i]["games"][x]["gameType"] != "R") {
                     //   cout << myjson["dates"][i]["games"][x]["gameType"] << endl;
			    continue;


                    }
                    //cout << myjson["dates"][i]["games"][x]["gameType"] << endl;
                    
                    int awayScore = myjson["dates"][i]["games"][x]["teams"]["away"]["score"];
                    int homeScore = myjson["dates"][i]["games"][x]["teams"]["home"]["score"];

                    string awayTeam = myjson["dates"][i]["games"][x]["teams"]["away"]["team"]["name"];
                    string homeTeam = myjson["dates"][i]["games"][x]["teams"]["home"]["team"]["name"];

                    Game tempGame(homeTeam, awayTeam, homeScore, awayScore);
                    vGame.push_back(tempGame);

                    if (awayScore == 0 && homeScore == 0) {
                //        cout << "I IS: ";
                //        cout << i;
                //        cout << "    X IS: ";
                //        cout << x << endl;
                    }
                
                     //cout << "I IS: ";
                     //cout << i;
                     //cout << "    X IS: ";
                     //cout << x << endl;
                     //cout << "AWAYSCORE  ";
                     //cout << awayScore << endl;

                } catch (...) {
                    x = games;
                    // cout << "catch  I IS: \n";
                    // cout << i;
                    // cout << "   X IS: ";
                    // cout << x << endl;
                }
                //cout << myjson["dates"][i] << endl;
                //cout << tempGame.getHomeTeam() << endl;
                //cout << " vs. " << endl;
                //cout << tempGame.getAwayTeam() << endl;
                
            }

            //auto &teams = game["teams"];
            // cout << games << endl;
    }
    
    return vGame;
    
}

void initRequest()
{
    CURL* curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;

    chunk.memory = (char*)malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0;                  /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* specify URL to get */
    curl_easy_setopt(curl_handle, CURLOPT_URL, "https://statsapi.web.nhl.com/api/v1/schedule?season=20222023");

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    //string response;
    //curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);

    /* we pass our 'chunk' struct to the callback function */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);

    /* some servers do not like requests that are made without a user-agent
     field, so we provide one */
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    /* get it! */
    res = curl_easy_perform(curl_handle);

    /* check for errors */
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    else
    {
        /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */
        //cout << "Variable response : " << response;
        //printf("%s", chunk.memory);
        nlohmann::json myjson = nlohmann::json::parse(chunk.memory);
        vector<Game> vGame = createVector(myjson);
        int size = vGame.size();


        for (int i = 0; i < size; i++) {
            Game tempGame = vGame[i];
            cout << tempGame.getHomeTeam();
            cout << " vs. ";
            cout << tempGame.getAwayTeam();
            cout << ":   ";
            cout << tempGame.getHomeScore();
            cout << " - ";
            cout << tempGame.getAwayScore() << endl;
        }
        //auto &teamName = myjson["dates"]["games"]["teams"]["away"]["team"]["name"];
        //std::cout << myjson["dates"][0]["games"][0]["teams"]["away"]["score"]<< std::endl;
        
        //auto &games = myjson["test-data"];
        //teams = new struct team [1];
        // printf(teamName);
        //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    free(chunk.memory);

    /* we are done with libcurl, so clean it up */
    curl_global_cleanup();

}


