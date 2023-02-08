#ifndef GAME_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define GAME_H

#include <string>
#include <iostream>

class Game {

    private:
        int homeScore;
        int awayScore;
        std::string homeTeam;
        std::string awayTeam;

    public:
        //game();
        Game(std::string ihomeTeam, std::string iawayTeam, int ihomeScore, int iawayScore);
        ~Game();

        int getHomeScore();
        int getAwayScore();
        std::string getHomeTeam();
        std::string getAwayTeam();

        void setHomeScore(int newScore);
        void setAwayScore(int newScore);
        void setHomeTeam(std::string newName);
        void setAwayTeam(std::string newName);
};
    

#endif
