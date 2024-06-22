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
    int amountOfResource;
    int maxAmountOfResource = 100;
    int powerConsumption;
    int powerProduction;
    std::string resourceType;
    Building* prev = nullptr;
    Building* next = nullptr;
    std::vector<std::pair<std::string, int>> constructionResources;

    Building(int x, int y, char icon, std::string name, int amountOfResource, int powerConsumption, int powerProduction, std::vector<std::pair<std::string, int>> constructionResources);
    virtual ~Building();

    virtual void operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) = 0;
	int getAmount() const { return amountOfResource; }
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
    // next & prev pointer
    // amount of item
    // pass if belt has resource
        // get from prev
    // pass to next
};

#endif