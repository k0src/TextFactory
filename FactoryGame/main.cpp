#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <map>
#include <memory>
#include <conio.h>
#include "Building.h"
#include "Resource.h"

using namespace std;

const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 30;

map<string, int> USER_RESOURCES = {
    {"Iron", 100},
    {"Copper", 100},
    {"Coal", 100},
    {"Stone", 0},
    {"Power", 0}
};

pair<int, int> getResourceCoords(int width, int height) {
    int x = rand() % width;
    int y = rand() % height;
    return make_pair(x, y);
}

template <typename T>
unique_ptr<T> generateResource(int width, int height) {
    int quantity = rand() % 500001 + 500004;
    pair<int, int> coords = getResourceCoords(width, height);
    return make_unique<T>(quantity, coords.first, coords.second);
}

vector<vector<char>> createGrid(int width, int height) {
    vector<vector<char>> grid(height, vector<char>(width, '.'));
    return grid;
}

void updateGrid(vector<vector<char>>& grid, const vector<unique_ptr<Resource>>& resources, const vector<unique_ptr<Building>>& buildings) {
    for (const auto& resource : resources) {
        if (!resource->isDepleted) {
            grid[resource->y][resource->x] = resource->icon;
        }
    }
}

void displayGameState(int width, int height, const vector<vector<char>>& grid, const vector<unique_ptr<Resource>>& resources, const vector<unique_ptr<Building>>& buildings) {
    // Display the grid
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    // Display the resources
    cout << "Resources:" << endl;

    for (const auto& resource : resources) {
        cout << resource->name << " (" << resource->icon << ") " << " {" << resource->x << ", "
            << resource->y << "} " << ": Remaining: " << resource->quantity << endl;
    }

    if (!buildings.empty()) {
        cout << endl;
        cout << "Buildings:" << endl;
        for (const auto& building : buildings) {
            cout << building->name << " at (" << building->x << ", " << building->y << ")" << endl;
        }
    }
}

void mineResource(vector<vector<char>>& grid, vector<unique_ptr<Resource>>& resources, int x, int y) {
    for (auto& resource : resources) {
        if (resource->x == x && resource->y == y) {
            cout << resource->name << " mined!" << endl;
            if (!resource->isDepleted) {
                resource->quantity -= 1;
                USER_RESOURCES[resource->name] += 1;
                if (resource->quantity <= 0) {
                    resource->isDepleted = true;
                    grid[y][x] = '.';
                    char c;
                    cout << "Resource is depleted!" << endl;
                    cout << "Press any key to continue..." << endl;
                    cin >> c;
                }
            }
            else {
                char c;
                cout << "Resource is depleted!" << endl;
                cout << "Press any key to continue..." << endl;
                cin >> c;
            }
        }
    }
}

void operateBuildings(vector<unique_ptr<Building>>& buildings, vector<unique_ptr<Resource>>& resources, vector<vector<char>>& grid, map<string, int>& userResources) {
    for (auto& building : buildings) {
        if (auto* powerPlant = dynamic_cast<PowerPlant*>(building.get())) {
            powerPlant->operate(userResources, resources, grid);
        }
        else if (auto* miner = dynamic_cast<Miner*>(building.get())) {
            miner->operate(userResources, resources, grid);
        }
    }
}

void buildBuilding(vector<vector<char>>& grid, vector<unique_ptr<Resource>>& resources, vector<unique_ptr<Building>>& buildings, int x, int y) {
    char c;
    cout << "Select a building to build:" << endl;
    cout << "1. Smelter" << endl;
    cout << "2. Miner" << endl;
    cout << "3. Power Plant" << endl;
    cout << "Enter the number of the building you want to build: ";
    int buildingChoice;
    cin >> buildingChoice;

    unique_ptr<Building> newBuilding;
    if (buildingChoice == 1) {
        newBuilding = make_unique<Smelter>(x, y);
    }
    else if (buildingChoice == 2) {
        auto it = find_if(resources.begin(), resources.end(), [x, y](const unique_ptr<Resource>& resource) {
            return resource->x == x && resource->y == y &&
                (resource->name == "Iron" || resource->name == "Copper" || resource->name == "Stone" || resource->name == "Coal") && !resource->isDepleted;
            });

        if (it != resources.end()) {
            newBuilding = make_unique<Miner>(x, y);
        }
        else {
            char c;
            cout << "Invalid location: Miner can only be built on a resource deposits and where no building exists!" << endl;
			cout << "Press any key to continue..." << endl;
			cin >> c;
            return;
        }
    }
    else if (buildingChoice == 3) {
        auto it = find_if(resources.begin(), resources.end(), [x, y](const unique_ptr<Resource>& resource) {
            return resource->x == x && resource->y == y &&
                resource->name == "Coal" && !resource->isDepleted;
            });

        if (it != resources.end()) {
            newBuilding = make_unique<PowerPlant>(x, y);
        }
        else {
            char c;
            cout << "Invalid location: Power Plant can only be built on coal deposits and where no building exists!" << endl;
            cout << "Press any key to continue..." << endl;
            cin >> c;
            return;
        }
    }
    else {
        cout << "Invalid building choice!" << endl;
        cout << "Press any key to continue..." << endl;
        cin >> c;
        return;
    }

    for (const auto& resource : newBuilding->constructionResources) {
        if (USER_RESOURCES[resource.first] < resource.second) {
            cout << "Not enough " << resource.first << " to build " << newBuilding->name << "!" << endl;
            cout << "Press any key to continue..." << endl;
            cin >> c;
            return;
        }
    }

    for (const auto& resource : newBuilding->constructionResources) {
        USER_RESOURCES[resource.first] -= resource.second;
    }

    buildings.push_back(move(newBuilding));
    updateGrid(grid, resources, buildings);
}

void handleInput(vector<vector<char>>& grid, vector<unique_ptr<Resource>>& resources, vector<unique_ptr<Building>>& buildings, bool& autoMode) {
    char input;
    cout << "Enter a command ('c' - view all commands): ";
    cin >> input;

    if (input == 'c') {
        char c;
        cout << "Commands:" << endl;
        cout << "m - mine a resource" << endl;
        cout << "b - build a building" << endl;
        cout << "q - quit the game" << endl;
        cout << "r - view my resources" << endl;
        cout << "c - view all commands" << endl;
        cout << "a - toggle auto mode" << endl;
        cout << endl << "Press any key to continue..." << endl;
        cin >> c;
    }
    else if (input == 'q') {
        exit(0);
    }
    else if (input == 'r') {
        char c;
        cout << "My Resources:" << endl;
        for (const auto& resource : USER_RESOURCES) {
            cout << resource.first << ": " << resource.second << endl;
        }
        cout << "Press any key to continue..." << endl;
        cin >> c;
    }
    else if (input == 'm') {
        int x, y;
        cout << "Enter the x and y coordinates of the resource you want to mine: ";
        cin >> x >> y;
        mineResource(grid, resources, x, y);
    }
    else if (input == 'b') {
        int x, y;
        cout << "Enter the x and y coordinates where you want to build: ";
        cin >> x >> y;
        buildBuilding(grid, resources, buildings, x, y);
    }
    else if (input == 'a') {
        autoMode = !autoMode;
        char c;
        cout << "Auto mode " << (autoMode ? "enabled" : "disabled") << endl;
        cout << "Press any key to continue..." << endl;
        cin >> c;
    }
    else {
        char c;
        cout << "Invalid command!" << endl;
        cout << "Press any key to continue..." << endl;
        cin >> c;
    }
}

int main() {
    srand(time(nullptr));
    int width = GRID_WIDTH;
    int height = GRID_HEIGHT;

    vector<vector<char>> grid = createGrid(width, height);
    vector<unique_ptr<Resource>> resources;
    vector<unique_ptr<Building>> buildings;

    for (int i = 0; i < 3; i++) {
        resources.push_back(generateResource<IronVein>(width, height));
        resources.push_back(generateResource<CopperVein>(width, height));
        resources.push_back(generateResource<CoalVein>(width, height));
        resources.push_back(generateResource<StoneVein>(width, height));
    }

    bool autoMode = false;

    while (true) {
        system("cls");
        updateGrid(grid, resources, buildings);
        displayGameState(width, height, grid, resources, buildings);

        if (!autoMode) {
            handleInput(grid, resources, buildings, autoMode);
        }
        else {
            if (_kbhit()) {
                char input = _getch();
                if (input == 'a') {
                    autoMode = false;
                    cout << "Auto mode disabled" << endl;
                    Sleep(1000);
                    continue;
                }
            }
            Sleep(1000);
            operateBuildings(buildings, resources, grid, USER_RESOURCES);
        }
    }
    return 0;
}