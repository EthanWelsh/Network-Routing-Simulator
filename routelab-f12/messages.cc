#define LINKSTATE

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

#if defined(DISTANCEVECTOR)
    RoutingMessage::RoutingMessage(map<int, double> ccc, unsigned int src) // Initialize with src, dest, and latency
    {
        cost = ccc;
        src_node = src;
    }
#endif

ostream &RoutingMessage::Print(ostream &os) const
{
    os << "RoutingMessage("<<src_node<<")";
    return os;
}

#if defined(DISTANCEVECTOR)
    map<int, double> RoutingMessage::getDistanceVector()
    {
        return cost;
    }

    unsigned int RoutingMessage::getSrc()
    {
        return src_node;
    }
#endif

#if defined(LINKSTATE)
RoutingMessage::RoutingMessage(map<int, double> ccc, unsigned int src, int s) // Initialize with src, dest, and latency
{
    neighbor_table = ccc;
    src_node = src;
    seq = s;
}

map<int, double> RoutingMessage::getNeighborTable()
{
    return neighbor_table;
}
#endif