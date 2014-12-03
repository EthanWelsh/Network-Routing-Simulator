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

    // Get the information from the link that has changed.
    int link_src = l->GetSrc();
    int link_dest = l->GetDest();
    int link_cost = l->GetLatency();



    // Record the old cost that we've recorded about this link.
    int old_cost = routing_table.cost[link_dest];
    int change_in_cost = link_cost - old_cost;


    // We should look through our hop table. When we find an entry where the
    // hop uses the node which is the DESTINATION of the passed in link, we
    // should change our table accordingly.

    int table_dest;
    int table_neighbor;

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

            // At this point our map reflects the changes in cost that have been caused by the connection
            // change. We now need to update topo to reflect the new change there as well
            map<int, TopoLink> &col = routing_table.distance_vectors[link_src];
            TopoLink &toChange = col[table_dest];
            toChange.cost += link_cost;
        }
    }

    // But we aren't done yet. We have updated our table, but it no longer reflects the shortest paths, as
    // the link change could have drastically increased the cost of our link. We'll now look through all our
    // neighbors and see who has the smallest path to offer us.
    deque<Node*>neighborNodes = *Node::GetNeighbors();

    deque<Node *>::iterator it;

    for (it = neighborNodes.begin(); it != neighborNodes.end(); ++it)
    {
        int neighbor_node = (*it)->GetNumber(); // Look through every node in the graph


        // Look at the distance vectors from this node to every other node in the graph
        map<int, TopoLink> &node_vector = routing_table.distance_vectors[neighbor_node];

        // Find the distance specifically associated with the destination node of the changed link.
        // The cost is the cost to your neighbor plus the distance from your neighbor to the destination
        int cost_to_dest_through_node = routing_table.cost[neighbor_node] + node_vector[link_dest].cost;
        int cost_in_table_at_current = routing_table.cost[link_dest];

        // Compare the cost that we have at current to this node with the distance our neighbor is advertising
        if(cost_to_dest_through_node < cost_in_table_at_current)
        {
            // Change your cost and hop table.
            routing_table.cost[link_dest] = cost_to_dest_through_node;
            routing_table.hop[link_dest] = neighbor_node;

            // Change topo to reflect the changed in cost and hop.
            map<int, TopoLink> &col1 = routing_table.distance_vectors[link_src];
            TopoLink &toChange1 = col1[link_dest];
            toChange1.cost = cost_to_dest_through_node;
        }
    }

    // All our tables are now updated. We have taken account for the change and reselected all our shortest paths
    // just in case a better one existed. We now need to send a routing message to our neighbors with our new topo
    // so that they can adjust their costs accordingly.
    SendToNeighbors(new RoutingMessage()); // TODO
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
