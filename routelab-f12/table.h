#ifndef _table
#define _table

#include <iostream>
#include <map>

using namespace std;

struct TopoLink {
    TopoLink(): cost(-1), age(0) {}

    TopoLink(const TopoLink & rhs) {
        *this = rhs;
    }

    TopoLink & operator=(const TopoLink & rhs) {
        this->cost = rhs.cost;
        this->age = rhs.age;

        return *this;
    }

    int cost;
    int age;
};

// Students should write this class
class Table {
    private:
        map < int, map < int, TopoLink > > topo;
    public:
        Table();
        Table(const Table &);
        Table & operator=(const Table &);

        ostream & Print(ostream &os) const;

        // Anything else you need

        #if defined(LINKSTATE)
        #endif

        #if defined(DISTANCEVECTOR)
        #endif
};

inline ostream & operator<<(ostream &os, const Table & t) { return t.Print(os);}


#endif
