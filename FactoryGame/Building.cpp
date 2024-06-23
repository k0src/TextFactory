#include "Building.h"
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>

extern std::map<std::string, int> USER_RESOURCES;

Building::Building(int x, int y, char icon, std::string name, int amountOfResource, int powerConsumption, int powerProduction, std::vector<std::pair<std::string, int>> constructionResources)
    : x(x), y(y), icon(icon), name(name), amountOfResource(amountOfResource), powerConsumption(powerConsumption), powerProduction(powerProduction), constructionResources(constructionResources) {}

Building::~Building() {}

Smelter::Smelter(int x, int y)
    : Building(x, y, 'M', "Smelter", 5, 10, 0, { {"Iron", 20}, {"Copper", 10} }) {}

void Smelter::operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) {
    if (prev && prev->amountOfResource > 0) {
        char c;
        int receivedAmount = std::min(prev->amountOfResource, maxAmountOfResource - amountOfResource);
        std::cout << "Smelter at {" << x << ", " << y << "} receiving " << receivedAmount << " of " << prev->resourceType << " from {" << prev->x << ", " << prev->y << "}" << std::endl;
        std::cin >> c;
        if (prev->resourceType == "Iron") {
            char c;
            std::cout << "iron" << std::endl;
			std::cin >> c;
        }
        else if (prev->resourceType == "Copper") {
            char c;
            std::cout << "copper" << std::endl;
			std::cin >> c;
        }
        else if (prev->resourceType == "Stone") {
			char c;
            std::cout << "stone" << std::endl;
			std::cin >> c;
        }

        prev->amountOfResource -= receivedAmount;
        amountOfResource += receivedAmount;
        resourceType = prev->resourceType;
    }
}

Miner::Miner(int x, int y)
    : Building(x, y, 'N', "Miner", 5, 10, 0, { {"Iron", 10}, {"Coal", 5} }) {}

void Miner::operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) {
    if (userResources["Power"] >= powerConsumption) {
        for (auto& resource : resources) {
            if (resource->x == x && resource->y == y && !resource->isDepleted) {
                int extractedAmount = std::min(amountOfResource, resource->quantity);

                if (resource->icon == 'I') {
                    resourceType = "Iron";
                }
                else if (resource->icon == 'C') {
                    resourceType = "Copper";
                }
                else if (resource->icon == 'O') {
                    resourceType = "Coal";
                }
                else if (resource->icon == 'S') {
                    resourceType = "Stone";
                }

                resource->quantity -= extractedAmount;
                if (next) {
                    next->amountOfResource += extractedAmount;
                    next->resourceType = resourceType;
                }
                else {
                    userResources[resourceType] += extractedAmount;
                }

                if (resource->quantity <= 0) {
                    resource->isDepleted = true;
                    grid[y][x] = '.';
                }

                userResources["Power"] -= powerConsumption;
                break;
            }
        }
    }
}

PowerPlant::PowerPlant(int x, int y)
    : Building(x, y, 'P', "Power Plant", 0, 0, 50, { {"Coal", 50} }) {}

void PowerPlant::operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) {
    if (grid[y][x] == 'O') {
        userResources["Power"] += powerProduction;

        for (auto& resource : resources) {
            if (resource->x == x && resource->y == y && !resource->isDepleted && resource->name == "Coal") {
                resource->quantity -= 1;
                if (resource->quantity <= 0) {
                    resource->isDepleted = true;
                    grid[y][x] = '.';
                }
                break;
            }
        }
    }
}

Belt::Belt(int x, int y)
	: Building(x, y, '+', "Belt", 0, 0, 0, { {"Iron", 5} }) {}

void Belt::operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) {
    if (prev && amountOfResource < maxAmountOfResource) {
        int transferAmount = std::min(prev->amountOfResource, maxAmountOfResource - amountOfResource);
        if (prev->resourceType == resourceType || resourceType.empty()) {
            prev->amountOfResource -= transferAmount;
            amountOfResource += transferAmount;
            resourceType = prev->resourceType;
            char c;
            std::cout << "Belt at {" << x << ", " << y << "} receiving " << transferAmount << " of " << resourceType << " from {" << prev->x << ", " << prev->y << "}" << std::endl;
			std::cin >> c;
        }
    }

    if (next && amountOfResource > 0) {
        int transferAmount = std::min(amountOfResource, next->maxAmountOfResource - next->amountOfResource);
        if (next->resourceType == resourceType || next->resourceType.empty()) {
            amountOfResource -= transferAmount;
            next->amountOfResource += transferAmount;
            next->resourceType = resourceType;
            if (amountOfResource == 0) {
                resourceType.clear();
            }
			char c;
            std::cout << "Belt at {" << x << ", " << y << "} transferring " << transferAmount << " of " << resourceType << " to {" << next->x << ", " << next->y << "}" << std::endl;
			std::cin >> c;
        }
    }
}