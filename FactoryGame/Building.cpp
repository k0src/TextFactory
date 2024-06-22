#include "Building.h"
#include <vector>
#include <map>


extern std::map<std::string, int> USER_RESOURCES;

Building::Building(int x, int y, char icon, std::string name, int productionRate, int powerConsumption, int powerProduction, std::vector<std::pair<std::string, int>> constructionResources)
    : x(x), y(y), icon(icon), name(name), productionRate(productionRate), powerConsumption(powerConsumption), powerProduction(powerProduction), constructionResources(constructionResources) {}

Building::~Building() {}

Smelter::Smelter(int x, int y)
    : Building(x, y, 'M', "Smelter", 5, 10, 0, { {"Iron", 20}, {"Copper", 10} }) {}

void Smelter::operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) {
    // Implement Smelter operation logic here
}

Miner::Miner(int x, int y)
    : Building(x, y, 'N', "Miner", 5, 10, 0, { {"Iron", 10}, {"Coal", 5} }) {}

void Miner::operate(std::map<std::string, int>& userResources, std::vector<std::unique_ptr<Resource>>& resources, std::vector<std::vector<char>>& grid) {
    if (userResources["Power"] >= powerConsumption) {
        for (auto& resource : resources) {
            if (resource->x == x && resource->y == y && !resource->isDepleted) {
                if (grid[y][x] == 'I') {
                    userResources["Iron"] += productionRate;
                }
                else if (grid[y][x] == 'C') {
                    userResources["Copper"] += productionRate;
                }
                else if (grid[y][x] == 'O') {
                    userResources["Coal"] += productionRate;
                }
                else if (grid[y][x] == 'S') {
                    userResources["Stone"] += productionRate;
                }

                resource->quantity -= productionRate;
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

}