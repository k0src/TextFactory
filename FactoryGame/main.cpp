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

int GLOBAL_CLOCK = 1000;

map<string, int> USER_RESOURCES = {
    {"Iron", 100},
    {"Copper", 100},
    {"Coal", 100},
    {"Stone", 100},
    {"Power", 0},
	{"Steel", 0},
	{"Circuit", 0},
    {"Concrete", 0}
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

	for (const auto& building : buildings) {
		if (building->icon == '+')
		    grid[building->y][building->x] = building->icon;
	}
}

void displayGameState(int width, int height, const vector<vector<char>>& grid, const vector<unique_ptr<Resource>>& resources, const vector<unique_ptr<Building>>& buildings) {
    // Display the grid
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << "\033[1m" << grid[i][j] << " ";
        }
        cout << "\033[0m" << endl;
    }

    cout << endl;

    // Display the resources
    cout << "\033[1mResources:\033[0m" << endl << endl;

    for (const auto& resource : resources) {
        cout << "\033[1;32m" << resource->name << " (" << resource->icon << ") " << " {" << resource->x << ", "
            << resource->y << "} " << ": Remaining: " << resource->quantity << "\033[0m" << endl;
    }

    if (!buildings.empty()) {
        cout << endl;
        cout << "Buildings:" << endl << endl;
        for (const auto& building : buildings) {
            if (building->icon != '+')
                cout << "\033[1;32m" << building->name << " at (" << building->x << ", " << building->y << ")" << "\033[0m" << endl;
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
	cout << "4. Constructor" << endl;
	cout << "5. Belt" << endl;
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
    else if (buildingChoice == 4) {
        cout << "Not implemented" << endl;
    }
	else if (buildingChoice == 5) {
		if (grid[y][x] != '.' || any_of(buildings.begin(), buildings.end(), [x, y](const unique_ptr<Building>& building) {
			return building->x == x && building->y == y;
			})) {
			cout << "Invalid location: Belt can only be built on empty spaces!" << endl;
			cout << "Press any key to continue..." << endl;
			cin >> c;
			return;
		}

		newBuilding = make_unique<Belt>(x, y);
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
    cout << endl << "\033[1mEnter a [c]ommand: \033[0m";
    cin >> input;

    if (input == 'c') {
        char c;
        cout << endl;
        cout << "Commands:" << endl;
        cout << "[m]ine a resource" << endl;
        cout << "[b]uild a building" << endl;
		cout << "craf[t] an item" << endl;
        cout << "view my [r]esources" << endl;
        cout << "toggle [a]uto mode" << endl;
        cout << "[c]ommands" << endl;
        cout << "[q]uit the game" << endl;
        cout << endl << "Press any key to continue..." << endl;
        cin >> c;
    }
    else if (input == 'q') {
        exit(0);
    }
    else if (input == 'r') {
        char c;
        cout << endl << "My Resources:" << endl;
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
    else if (input == 't') {
        int craftingChoice;
        cout << endl;
        cout << "Crafting menu" << endl;
		cout << "1. Circuit" << endl;
		cout << "Enter the number of the item you want to craft: ";
		cin >> craftingChoice;

        char c;
		if (craftingChoice == 1) {
			if (USER_RESOURCES["Iron"] >= 5 && USER_RESOURCES["Copper"] >= 5) {
				USER_RESOURCES["Iron"] -= 5;
				USER_RESOURCES["Copper"] -= 5;
				USER_RESOURCES["Circuit"] += 1;
				cout << "Circuit crafted!" << endl;
			}
			else {
				cout << "Not enough resources to craft Circuit!" << endl;
			}
		}
		else {
			cout << "Invalid crafting choice!" << endl;
		}

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

void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten)) {
        return;
    }

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        return;
    }

    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten)) {
        return;
    }

    SetConsoleCursorPosition(hConsole, coordScreen);
}

void moveCursorTo(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
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
        clearScreen();
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
                    Sleep(GLOBAL_CLOCK);
                    continue;
                }
            }
            Sleep(GLOBAL_CLOCK);
            operateBuildings(buildings, resources, grid, USER_RESOURCES);
        }
    }
    return 0;
}