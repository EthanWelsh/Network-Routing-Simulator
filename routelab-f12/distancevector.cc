#include "distancevector.h"

DistanceVector::DistanceVector(unsigned n, SimulationContext *c, double b, double l) :
        Node(n, c, b, l)
{
}

DistanceVector::DistanceVector(const DistanceVector &rhs) :
        Node(rhs)
{
    *this = rhs;
}

DistanceVector &DistanceVector::operator=(const DistanceVector &rhs)
{
    Node::operator=(rhs);
    return *this;
}

DistanceVector::~DistanceVector()
{
}


/** Write the following functions.  They currently have dummy implementations **/



/*
 Is called to inform you that an outgoing link connected to your node has just
 changed its properties. A pointer to a copy of the new Link is sent to you so
 that you’ll know the new latency to that particular neighbor. Don’t bother
 deleting the link. The framework will do this for you. You may want to update
 your tables and send further messages to your neighbors.
 */
void DistanceVector::LinkHasBeenUpdated(Link *l)
{
    cerr << *this << ": Link Update: " << *l << endl;
    // Q: We only need to send the RoutingMessage when we ourselves update one
    //    of our links, correct?

    // Q: Are we only responsible for updating our own forwarding table? Do we
    //    need to worry about changing any of the connections in our node or
    //    will the framework do this for us?

    // Q: Will only the latency change, or can other things such as the source
    //    and destination change too?

    // Q: When we built the table that we have at the time that
    //    LinkHasBeenUpdated is called, we very likely passed over certain
    //    paths because they were higher cost than what we had already.
    //    However, now that this change has been made, some of the paths that
    //    we initially rejected may now be preferable to the ones that we have.
    //    Do we need to atone for this immediately, and if so, how?

    // We should look through our hop table. When we find an entry where the
    // hop uses the node which is the DESTINATION of the passed in link, we
    // should change our table accordingly.


    // Get the information from the link that has changed.
    int link_src = l->GetSrc();
    int link_dest = l->GetDest();
    int link_cost = l->GetLatency();

    int table_dest;
    int table_neighbor;

    // Record the old cost that we've recorded about this link.
    int old_cost = routing_table.cost[table_neighbor];
    int change_in_cost = link_cost - old_cost;


    // Iterate through our map in order to find where we need to make adjustments to our paths costs
    // because of the changed link.
    typedef std::map<int, int>::iterator it_type;
    for(it_type iterator = routing_table.hop.begin(); iterator != routing_table.hop.end(); iterator++)
    {
        table_dest = iterator->first;
        table_neighbor = iterator->second;

        if(table_neighbor == link_dest)
        {   // We have found a path to another node that uses the node which is the destination in the
            // changed connection. We now need to adjust our cost for this path accordingly.

            routing_table.cost[table_dest] += change_in_cost;
        }

        
        
        
        
    }


    SendToNeighbors(new RoutingMessage());
}


/*
 called when a routing message arrives at a node. In response, you may send
 further routing messages using SendToNeighbors or SendToNeighbor. You may also
 update your tables. Don’t bother deleting the routing message. The framework
 will do this for you.
 */
void DistanceVector::ProcessIncomingRoutingMessage(RoutingMessage *m)
{
    cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;
}

void DistanceVector::TimeOut()
{
    cerr << *this << " got a timeout: (ignored)" << endl;
}


/*
 is called when the simulation wants to know what your node currently thinks is
 the next hop on the path to the destination node. You should consult your
 routing table and then return the correct next node for reaching the
 destination. The pointer returned should be to a copy of the next node. The
 framework will eventually delete this.
 */
Node *DistanceVector::GetNextHop(Node *destination)
{
    return NULL;
}


/*
 is called when the simulation wants to get a copy of your current routing
 table. The framework will eventually delete the table. We expect your routing
 table to be able to print itself.
 */
Table *DistanceVector::GetRoutingTable()
{
    return NULL;
}

ostream &DistanceVector::Print(ostream &os) const
{
    Node::Print(os);
    return os;
}
