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

#if defined(GENERIC) // TODO
ostream &RoutingMessage::Print(ostream &os) const
{
  os << "RoutingMessage()";
  return os;
}
#endif

#if defined(DISTANCEVECTOR)

RoutingMessage::RoutingMessage()	// Empty constructor
{
}

RoutingMessage::RoutingMessage(int source, int destination, int len)	// Initialize with src, dest, and latency
{
	src = source;
	dest = destination;
	latency = len;
}

RoutingMessage::RoutingMessage(const RoutingMessage &rhs)	// Copy from one to another
{
	src = rhs.src;
	dest = rhs.dest;
	latency = rhs.latency;
}

ostream &RoutingMessage::Print(ostream &os) const	// Print the information
{ 
	os << "path from " << src << " to " << dest << " of " << latency << endl; 
	return os; 
}

int 
RoutingMessage::GetSrc() const
{ 
	return src; 	// Return the source of this message
}

int
RoutingMessage::GetDest() const	
{ 
	return dest; // Return the destination of this message
}

int 
RoutingMessage::GetLatency() const 
{ 
	return latency; // Return the latency of this link
}
#endif
