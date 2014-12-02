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

Table::Table(deque<Link *> *links)
{
    for (deque<Link *>::iterator i = links->begin(); i != links->end(); ++i)
    {
        int dest = (*i)->GetDest();
        int latency = (*i)->GetLatency();

        updateTable(dest, dest, latency);

        // WriteTable(dest, dest, latency); TODO
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
ostream & Table::Print(ostream &os) const
{
  os << "Generic Table()";
  return os;
}
#endif

#if defined(LINKSTATE)
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


void Table::updateTable(unsigned int dest, unsigned int next, int latency)
{

    if(cost.at(dest) < latency) cout<<"Hm..... This ain't looking good"<<endl;

    cost.at(dest) = latency;
    hop.at(dest) = next;
}



ostream & Table::Print(ostream &os) const
{
    os << "DistanceVector Table()";
    return os;
}

//#endif