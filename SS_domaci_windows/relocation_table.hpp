#ifndef RELOCATION_TABLE
#define RELOCATION_TABLE

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class RelocationTable {



public:

    friend ostream& operator<<(ostream& os, const RelocationTable& rt);

};

#endif
