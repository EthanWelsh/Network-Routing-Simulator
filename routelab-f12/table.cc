#include <vector>
#include "table.h"
#include "messages.h"

Table::Table()
{
    topo.clear();
}

Table::Table(const Table &rhs)
{
    *this = rhs;
}

/*
 This constructor will be used by our implementation of DV. It will start by adding ourselves to the forwarding table,
 aloung with the corresponding weights to each node. In addition to this, we'll fill out our hop table appropriately. In
 order to run this, you'll pass in this.getOutGoingConnections() from the node that you're trying to make this table for.
 */
Table::Table(deque<Link *> *links)
{
    for (deque<Link *>::iterator i = links->begin(); i != links->end(); ++i) // TODO why ++i instead of i++???
    {
        unsigned int dest = (*i)->GetDest();
        double latency = (*i)->GetLatency();

        updateTable(dest, dest, latency);
    }
    delete links;
}


Table &Table::operator=(const Table &rhs)
{
    /* For now,  Change if you add more data members to the class */
    topo = rhs.topo;

    return *this;
}

#if defined(GENERIC)

#endif

#if defined(LINKSTATE)
	/*int Table::UpdateLink(const Link *link)
	{
		int src = link->GetSrc();
		int dest = link->GetDest();
		int latency = link->GetLatency();
		int age= ++table[src][dest].age;

		table[src][dest].age = age;
		table[source][dest].cost = latency;
		return age;
	}

    ostream & Table::Print(ostream &os) const
    {
      os << "LinkState Table()";
      return os;
    }*/
#endif

//#if defined(DISTANCEVECTOR)

/*
 Will change an entry in our table such that we mark that a path exists from us to the destination. We'll also record
 what our nextHop is in order to get the shortest cost path.
 */
void Table::updateTable(unsigned int dest, unsigned int next, double latency)
{
    if(cost.at(dest) < latency) cout<<"Hm..... This ain't looking good"<<endl;
    cost[dest] = latency;
    hop[dest] = next;
}

int Table::getNextHop(unsigned dest)
{
    if(hop.find(dest) == hop.end())
    {
        cerr<<"No entry found in the hop map. Could not determine next hop. Returning -1." << endl;
        return -1;
    }
    else
    {
        return hop[dest];
    }
}

ostream & Table::Print(ostream &os) const
{
    os << "DistanceVector Table()";
    return os;
}

//#endif