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

        map<int, map< int, double > > distance_vectors;
        map<int, double> cost;
        map<int, int> hop;

        Table();

        Table(const Table &);
        Table(deque<Link *> *links);

        Table &operator=(const Table &);
        int getNextHop(unsigned);

        ostream &Print(ostream &os) const;

        // Anything else you need

        #if defined(LINKSTATE)

        #endif

        #if defined(DISTANCEVECTOR)

        //void Table::updateTable(unsigned int, unsigned int, int);



        #endif

    void updateTable(unsigned int dest, unsigned int next, double latency);

};

inline ostream &operator<<(ostream &os, const Table &t)
{
    return t.Print(os);
}


#endif
