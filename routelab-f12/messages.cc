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

RoutingMessage::RoutingMessage(Table &t, unsigned int src) // Initialize with src, dest, and latency
{
    costTable = t;
    src_node = src;
}

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}

map<int, double> RoutingMessage::getDistanceVector()
{
    return costTable.cost;
}

unsigned int RoutingMessage::getSrc()
{
    return src_node;
}