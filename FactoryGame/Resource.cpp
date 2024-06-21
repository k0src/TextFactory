#include "Resource.h"

Resource::Resource(int quantity, int x, int y, char icon, std::string name)
    : quantity(quantity), x(x), y(y), isDepleted(false), icon(icon), name(name) {}

Resource::~Resource() {}

IronVein::IronVein(int quantity, int x, int y)
    : Resource(quantity, x, y, 'I', "Iron") {}

CopperVein::CopperVein(int quantity, int x, int y)
    : Resource(quantity, x, y, 'C', "Copper") {}

CoalVein::CoalVein(int quantity, int x, int y)
    : Resource(quantity, x, y, 'O', "Coal") {}

StoneVein::StoneVein(int quantity, int x, int y)
    : Resource(quantity, x, y, 'S', "Stone") {}