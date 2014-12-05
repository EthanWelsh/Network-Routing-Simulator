#ifndef _messages
#define _messages

#include <iostream>
#include "node.h"
#include "link.h"
#include "table.h"

struct RoutingMessage {
    RoutingMessage();
    RoutingMessage(const RoutingMessage &rhs);
    RoutingMessage(map<int, double> ccc, unsigned int src);
    RoutingMessage(int a, int src, int dest, int lat);


public:
    map<int, double> getDistanceVector();
    unsigned int getSrc();

    map<int, double> cost;
    unsigned int src_node;

    RoutingMessage &operator=(const RoutingMessage &rhs);

    ostream & Print(ostream &os) const;

    //#ifdef(LINKSTATE)


    int dest;
    int src;
	int age;
    int latency;
    int link_age;

    //#endif


};

inline ostream & operator<<(ostream &os, const RoutingMessage & m) { return m.Print(os);}


#endif