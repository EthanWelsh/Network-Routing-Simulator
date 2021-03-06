#ifndef _messages
#define _messages

#include <iostream>
#include "node.h"
#include "link.h"
#include "table.h"

struct RoutingMessage
{
    RoutingMessage();
    RoutingMessage(const RoutingMessage &rhs);

public:

#if defined(LINKSTATE)
    RoutingMessage(map<int, double> ccc, unsigned int src, int s);
    map<int, double> neighbor_table;
    unsigned int src_node;
    map<int, double> getNeighborTable();
    int seq;
#endif


#if defined(DISTANCEVECTOR)
    RoutingMessage(map<int, double> ccc, unsigned int src);
    map<int, double> getDistanceVector();
    unsigned int getSrc();
    map<int, double> cost;
    unsigned int src_node;
#endif

    RoutingMessage &operator=(const RoutingMessage &rhs);
    ostream & Print(ostream &os) const;
};

inline ostream & operator<<(ostream &os, const RoutingMessage & m) { return m.Print(os);}

#endif