#ifndef _messages
#define _messages

#include <iostream>
#include "node.h"
#include "link.h"
#include "table.h"

struct RoutingMessage {
    RoutingMessage();
    RoutingMessage(const RoutingMessage &rhs);
    RoutingMessage(Table &t, unsigned int src);


public:
    map<int, double> getDistanceVector();
    unsigned int getSrc();

    Table costTable;
    unsigned int src_node;

    RoutingMessage &operator=(const RoutingMessage &rhs);

    ostream & Print(ostream &os) const;



};

inline ostream & operator<<(ostream &os, const RoutingMessage & m) { return m.Print(os);}


#endif