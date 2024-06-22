#ifndef BUILDING_H
#define BUILDING_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include "Resource.h"

extern std::map<std::string, int> USER_RESOURCES;

class Building {
public:
    char icon;
    std::string name;
    int x, y;
    int productionRate;
    int powerConsumption;
    int powerProduction;
    std::vector<std::pair<std::string, int>> constructionResources;

    Building(int x, int y, char icon, std::string name, int productionRate, int powerConsumption, int powerProduction, std::vector<std::pair<std::string, int>> constructionResources);
    virtual ~Building();

    virtual void operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) = 0;
};

class Smelter : public Building {
public:
    Smelter(int x, int y);
    void operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) override;
};

class Miner : public Building {
public:
    Miner(int x, int y);
    void operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) override;
};

class PowerPlant : public Building {
public:
    PowerPlant(int x, int y);
    void operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) override;
};

class Constructor : public Building {
public:
	Constructor(int x, int y);
	void operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) override;
};

class Belt : public Building {
public:
	Belt(int x, int y);
	void operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) override;
};

#endif