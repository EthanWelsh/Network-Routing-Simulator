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

Table &Table::operator=(const Table &rhs)
{
    /* For now,  Change if you add more data members to the class */
    topo = rhs.topo;
    return *this;
}

#if defined(GENERIC)

#endif


#if defined(LINKSTATE)

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
        int cost_to = (int) it->second;
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


    os << endl;
    os << endl;

    os << "|======================|"<<endl;
    os << "|         NEH          |"<<endl;
    os << "|======================|"<<endl;


    map<int, double>::const_iterator it2;
    for (it2 = neighbor_table.begin(); it2 != neighbor_table.end(); ++it2)
    {
        int dest = it2->first;
        int cost_to = (int) it2->second;
        if(cost_to > 9) os << "|     "<<dest<<"     |     "<<cost_to<<"   |"<<endl;
        else os << "|     "<<dest<<"     |     "<<cost_to<<"    |"<<endl;
    }
    os << "|======================|"<<endl << endl;

    os << endl;
    os << endl;

    os << "\t\t|======================|"<<endl;
    os << "\t\t|======================|"<<endl;
    os << "\t\t||         TOPO        ||"<<endl;
    os << "\t\t|======================|"<<endl;
    os << "\t\t|======================|"<<endl;


    map<int, map <int, double> >::const_iterator it3;
    for(it3 = topology.begin(); it3 != topology.end(); ++it3)
    {
        int myNeh = it3->first;

        os << endl;
        os << "\t\t|======================|"<<endl;
        os << "\t\t|           "<<myNeh<<"          |"<<endl;
        os << "\t\t|======================|"<<endl;


        map<int, double> costNeh = it3->second;

        map<int, double>::const_iterator it4;
        for(it4 = costNeh.begin(); it4 != costNeh.end(); ++it4)
        {
            int dest = it4->first;
            int cost_to = (int) it4->second;
            if(cost_to > 9) os << "\t\t|     "<<dest<<"     |     "<<cost_to<<"   |"<<endl;
            else os << "\t\t|     "<<dest<<"     |     "<<cost_to<<"    |"<<endl;
        }
        os << "\t\t|======================|"<<endl << endl;
    }
    return os;
}

#endif

#if defined(DISTANCEVECTOR)

/*
 Will change an entry in our table such that we mark that a path exists from us to the destination. We'll also record
 what our nextHop is in order to get the shortest cost path.
 */
void Table::updateTable(unsigned int dest, unsigned int next, double latency)
{
    cost[dest] = latency;
    hop[dest] = next;
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
        int cost_to = (int) it->second;
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

    os << endl;
    os << endl;

    os << "|======================|"<<endl;
    os << "|======================|"<<endl;
    os << "||          DV        ||"<<endl;
    os << "|======================|"<<endl;
    os << "|======================|"<<endl;


    map<int, map <int, double> >::const_iterator it2;
    for(it2 = distance_vectors.begin(); it2 != distance_vectors.end(); ++it2)
    {
        int myNeh = it2->first;

        os << endl;
        os << "|======================|"<<endl;
        os << "|           "<<myNeh<<"          |"<<endl;
        os << "|======================|"<<endl;

        map<int, double> costNeh = it2->second;

        map<int, double>::const_iterator it3;
        for(it3 = costNeh.begin(); it3 != costNeh.end(); ++it3)
        {
            int dest = it3->first;
            int cost_to = (int) it3->second;
            if(cost_to > 9) os << "|     "<<dest<<"     |     "<<cost_to<<"   |"<<endl;
            else os << "|     "<<dest<<"     |     "<<cost_to<<"    |"<<endl;
        }
        os << "|======================|"<<endl << endl;
    }
    return os;
}

#endif