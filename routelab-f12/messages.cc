#include "messages.h"

RoutingMessage::RoutingMessage()
{}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs)
{
    *this = rhs;
}

RoutingMessage & RoutingMessage::operator=(const RoutingMessage &rhs) 
{
    return *this;
}

RoutingMessage::RoutingMessage(map<int, double> ccc, unsigned int src) // Initialize with src, dest, and latency
{
    cost = ccc;
    src_node = src;
}

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage("<<src_node<<")";
  return os;
}

map<int, double> RoutingMessage::getDistanceVector()
{
    return cost;
}

unsigned int RoutingMessage::getSrc()
{
    return src_node;
}

#if defined(LINKSTATE)
RoutingMessage::RoutingMessage(int a, int src, int dest, int lat)
{
    link_age = a;
    src= src;
    dest = dest;
    latency = lat;
}

ostream &RoutingMessage::Print(ostream &os) const
{
    os << "LinkState RoutingMessage()";
    return os;
}
#endif