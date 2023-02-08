#include "gameObject.h"

Game::Game(std::string ihomeTeam, std::string iawayTeam, int ihomeScore, int iawayScore)
{
    homeScore = ihomeScore;
    awayScore = iawayScore;
    homeTeam = ihomeTeam;
    awayTeam = iawayTeam;
}

Game::~Game()
{
}

int Game::getHomeScore()
{
    return homeScore;
}

int Game::getAwayScore()
{
    return awayScore;
}

std::string Game::getHomeTeam()
{
    return homeTeam;
}

std::string Game::getAwayTeam()
{
    return awayTeam;
}

void Game::setHomeScore(int newScore)
{
    homeScore = newScore;
}

void Game::setAwayScore(int newScore)
{
    awayScore = newScore;
}

void Game::setAwayTeam(std::string newTeam)
{
    awayTeam = newTeam;
}

void Game::setHomeTeam(std::string newTeam)
{
    homeTeam = newTeam;
}

