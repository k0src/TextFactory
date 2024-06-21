#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>

class Resource {
public:
    int quantity;
    int x, y;
    bool isDepleted;
    char icon;
    std::string name;

    Resource(int quantity, int x, int y, char icon, std::string name);
    virtual ~Resource();
};

class IronVein : public Resource {
public:
    IronVein(int quantity, int x, int y);
};

class CopperVein : public Resource {
public:
    CopperVein(int quantity, int x, int y);
};

class CoalVein : public Resource {
public:
    CoalVein(int quantity, int x, int y);
};

class StoneVein : public Resource {
public:
    StoneVein(int quantity, int x, int y);
};

#endif