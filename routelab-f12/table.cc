#include "table.h"
#include "messages.h"

Table::Table() {
    topo.clear();
}

Table::Table(const Table & rhs) {
    *this = rhs;
}

Table & Table::operator=(const Table & rhs) {
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

#if defined(DISTANCEVECTOR)
Table::Table(Node n) 
{
	deque<Link*> *links = n.GetOutgoingLinks();
	for (deque<Link*>::iterator i = links->begin(); i != links->end(); ++i)
	{
		int dest = (*i)->GetDest();
		int latency = (*i)->GetLatency();

		WriteTable(dest, dest, latency);
	}
	delete links;
}

bool
Table::CheckLatency(int dest, int next, double dist) 
{
    if (ReadTable(dest, next) > dist || ReadTable(dest, next) == -1)
	{
		return 1;
    }
    else
	{
		return 0;
	}
}

bool
Table::WriteTable(int dest, int next, double dist) 
{
	while (table.size() <= dest) 
	{
		table.push_back(*new(vector<double>));
	}
	while (table[dest].size() <= next) 
	{
		table[dest].push_back((double) -1);
	}
	table[dest][next] = dist;

	return CheckLatency(dest, next, dist);
}

double
Table::ReadTable(int dest, int next)
{
    if (table.size() <= dest || table[dest].size() <= next)
    {
		return -1;
	}
	else 
	{
		return table[dest][next];
	}
}

int
Table::GetNext(int dest)
{
    if (table.size() <= dest)
	{
		return -1;
    }

    int ret = 0;
	int ctr = 0;
	for (vector<double>::const_iterator i = table[dest].begin(); i != table[dest].end(); ++i)
	{
		if ((*i) != -1 && ((*i) < table[dest][ret] || table[dest][ret] == -1))
		{
			ret = ctr;
		}        
		ctr++;
	}
	return ret;
}

ostream&
Table::Print(ostream &os) const 
{
  if (this == NULL) 
  {
	return os;
  }

  os << endl << "Table: " << endl;
  int ctr = 0;
  for (vector<vector<double> >::const_iterator i = table.begin(); i != table.end(); ++i)
  {
	os << "to " << ctr++ << ": ";
	for (vector<double>::const_iterator j = (*i).begin(); j != (*i).end(); ++j) 
	{
		os << "\t" << (*j);
	}
	os << endl;
  }
  return os;
}
#endif
