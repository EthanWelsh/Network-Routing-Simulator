#ifndef _table
#define _table

#include <iostream>
#include <map>
#include <deque>
#include "link.h"

using namespace std;

struct TopoLink
{
    TopoLink() : cost(-1), age(0)
    {
    }

    TopoLink(const TopoLink &rhs)
    {
        *this = rhs;
    }

    TopoLink &operator=(const TopoLink &rhs)
    {
        this->cost = rhs.cost;
        this->age = rhs.age;

        return *this;
    }

    int cost;
    int age;
};

// Students should write this class
class Table
{
private:


public:

    map<int, map< int, TopoLink > > topo;

    #if defined(DISTANCEVECTOR)
    map<int, map< int, double > > distance_vectors;
    map<int, double> cost;
    map<int, int> hop;
    void updateTable(unsigned int dest, unsigned int next, double latency);
    #endif

    Table();
    Table(const Table &);
    Table(deque<Link *> *links);
    Table &operator=(const Table &);
    ostream &Print(ostream &os) const;

    #if defined(LINKSTATE)
    map<int, map< int, double > > topology;
    map<int, double> neighbor_table;
    map<int, int> hop;
    map<int, double> cost;
    #endif

};

inline ostream &operator<<(ostream &os, const Table &t)
{
    return t.Print(os);
}

#endif