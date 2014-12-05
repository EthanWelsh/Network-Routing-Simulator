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
 along with the corresponding weights to each node. In addition to this, we'll fill out our hop table appropriately. In
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
	Table::Table()
	{
		topo.clear();
		hopMap.clear();
		change_the_hop_map= true;
	}
	
	int Table::UpdateLink(const Link *link)
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
    }
#endif

//#if defined(DISTANCEVECTOR)

/*
 Will change an entry in our table such that we mark that a path exists from us to the destination. We'll also record
 what our nextHop is in order to get the shortest cost path.
 */
void Table::updateTable(unsigned int dest, unsigned int next, double latency)
{
    cost[dest] = latency;
    hop[dest] = next;
}

int Table::getNextHop(unsigned dest)
{
    if(hop.find(dest) == hop.end())
    {
        cerr<<"No entry found in the hop map. Could not determine next hop. Returning -1." << endl;
        return -2;
    }
    else
    {
        return hop[dest];
    }
}

ostream & Table::Print(ostream &os) const
{
    os << endl << "*****DistanceVector Table()*****" << endl;

    os << "|======================|" << endl;
    os << "|         COST         |" << endl;
    os << "|======================|" << endl;
    map<int, double>::const_iterator it;
    for (it = cost.begin(); it != cost.end(); ++it)
    {
        int dest = it->first;
        int cost_to = it->second;
        if(cost_to > 9) os << "|     "<<dest<<"     |     "<<cost_to<<"   |"<<endl;
        else os << "|     "<<dest<<"     |     "<<cost_to<<"    |"<<endl;
    }
    os << "|======================|"<<endl;

    os<<endl;

    os << "|======================|"<<endl;
    os << "|         HOP          |"<<endl;
    os << "|======================|"<<endl;

    // map<int, int> hop;

    map<int, int>::const_iterator it1;
    for (it1 = hop.begin(); it1 != hop.end(); ++it1)
    {
        int dest = it1->first;
        int step = it1->second;
        os << "|     "<<dest<<"     |     "<<step<<"    |"<<endl;
    }
    os << "|======================|"<<endl << endl;

    return os;
}

//#endif