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

//#if defined(GENERIC) // TODO
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
//#endif
