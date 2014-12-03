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

RoutingMessage::RoutingMessage(Table &t) // Initialize with src, dest, and latency
{
    costTable = t;
}

ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}

map<int, double> getDistanceVector()
{
    return costTable.cost;
}