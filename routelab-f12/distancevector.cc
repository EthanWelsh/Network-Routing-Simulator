#include "distancevector.h"

#include <stdlib.h>

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

bool DistanceVector::findImprove()
{
    deque<Node*>neighborNodes = *Node::GetNeighbors();

    bool aChangeWasMade = false;

    deque<Node *>::iterator it;

    cerr<<"----------IMPROVE---------"<<endl;
    cerr<<"We are "<<GetNumber()<<" and our current table looks like this: "<<routing_table<<endl;

    for (it = neighborNodes.begin(); it != neighborNodes.end(); ++it)
    {
        int neighbor_node = (*it)->GetNumber(); // Look through every node in the graph

        cerr<<"\tLooking in "<< neighbor_node << endl;

        // Look at the distance vectors from this node to every other node in the graph
        map<int, double> node_vector = routing_table.distance_vectors[neighbor_node];

        // Look at every entry in their distance vector table and see if any of their paths are improvements
        std::map<int, double>::iterator iter;
        for(iter = node_vector.begin(); iter != node_vector.end(); iter++)
        {
            int dest_node = iter->first;

            double cost_to_neighbor = routing_table.cost[neighbor_node];
            double cost_from_neighbor_to_dest = iter->second;
            double new_total_cost = cost_to_neighbor + cost_from_neighbor_to_dest;

            double cost_in_table_at_current = routing_table.cost[dest_node];

            bool isNewLink = false;

            if(routing_table.cost.find(dest_node) == routing_table.cost.end())
            {
                isNewLink = true;
            }

            cerr<<"\t\t(" <<dest_node << ")The total cost is "<< new_total_cost<<endl;
            cerr<<"\t\t(" <<dest_node << ")The cost in table is "<< cost_in_table_at_current <<endl;

            // Compare the cost that we have at current to this node with the distance our neighbor is advertising
            if(new_total_cost < cost_in_table_at_current || isNewLink)
            {
                cerr<<"HEYHEYHEYHEYHEYHEYHEY"<<endl;
                routing_table.updateTable(dest_node, neighbor_node, new_total_cost);
                aChangeWasMade = true;
            }
        }
    }
    cerr<<"--------IMPROVE END-------"<<endl;
    cerr<<"NOW here's the table "<<routing_table<<endl;
    return aChangeWasMade;
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

    routing_table.cost[l->GetSrc()] = 0;

    cerr<<endl<<endl;
    cerr<<"***********************************************"<<endl;
    cerr<<"***********************************************"<<endl;
    cerr << endl << "LINK UPDATE: " << *l << endl << endl;

    // Get the information from the link that has changed.
    int link_dest = l->GetDest();
    double link_cost = l->GetLatency();

    // If you got an update on an edge that you haven't seen before.
    if(routing_table.cost.find(link_dest) == routing_table.cost.end())
    {
        cerr<<"Can't find " << link_dest << " in cost table. Adding it now!"<<endl;
        routing_table.updateTable(link_dest, link_dest, link_cost);
        cerr<<"I added it to the table: "<<routing_table<<endl;

        cerr<<"Finding an improved way to get to " << link_dest << endl;
        findImprove();
        cerr<<"Here is the improved table"<<routing_table<<endl;

        cerr<<endl<<*this<<endl;

        cerr<<"SENDING MESSAGE TO NEIGHBORS (from " << GetNumber() << ")"<<endl;

        cerr<<"***********************************************"<<endl;
        cerr<<"***********************************************"<<endl;
        cerr<<endl<<endl;

        SendToNeighbors(new RoutingMessage(routing_table.cost, GetNumber()));
        return;
    }

    // Record the old cost that we've recorded about this link.
    double old_cost = routing_table.cost[link_dest];
    double change_in_cost = link_cost - old_cost;

    cerr<<"You've already got "<<link_dest<<" down in your table with a cost of "<<old_cost<<endl;
    cerr<<"We're changing the link to " << link_cost<<" which is a change of "<<change_in_cost<<" from the original."<<endl;

    /* We should look through our hop table. When we find an entry where the
     hop uses the node which is the DESTINATION of the passed in link, we
     should change our table accordingly.*/

    int table_dest;
    int table_neighbor;

    /* Iterate through our map in order to find where we need to make adjustments to our paths costs
       because of the changed link.*/

    typedef std::map<int, int>::iterator it_type;
    for(it_type iterator = routing_table.hop.begin(); iterator != routing_table.hop.end(); iterator++)
    {
        table_dest = iterator->first;
        table_neighbor = iterator->second;

        if(table_neighbor == link_dest)
        {
            cerr<<"Your path to " << table_dest << " was also using " << table_neighbor << endl;
            cerr<<"The old length is " << routing_table.cost[table_dest] << " but we're adding " << change_in_cost << endl;

            routing_table.cost[table_dest] += change_in_cost;

            cerr<<"So the new value is " << routing_table.cost[table_dest] << endl;

            /* We have found a path to another node that uses the node which is the destination in the
               changed connection. We now need to adjust our cost for this path accordingly.
            */

        }
    }

    /* But we aren't done yet. We have updated our table, but it no longer reflects the shortest paths, as
       the link change could have drastically increased the cost of our link. We'll now look through all our
       neighbors and see who has the smallest path to offer us.
    */

    cerr<<"Phew. We've changed all the links that we need to change. Now time to make sure our paths are still the shortest."<<endl;
    cerr<<"Before"<<routing_table<<endl;

    findImprove();

    cerr<<"After"<<routing_table<<endl;


    /* Our tables are now updated. We have taken account for the change and reselected all our shortest paths just
     in case a better one existed. We now need to send a routing message to our neighbors with our new topo so
     that they can adjust their costs accordingly.
     */

    cerr<<endl<<*this<<endl;

    cerr<<"***********************************************"<<endl;
    cerr<<"***********************************************"<<endl;
    cerr<<endl<<endl;

    SendToNeighbors(new RoutingMessage(routing_table.cost, GetNumber()));
}

/*
 called when a routing message arrives at a node. In response, you may send
 further routing messages using SendToNeighbors or SendToNeighbor. You may also
 update your tables. Don’t bother deleting the routing message. The framework
 will do this for you.
 */
void DistanceVector::ProcessIncomingRoutingMessage(RoutingMessage *m)
{
    cerr << endl << endl;
    cerr << "***********************************************"<<endl;
    cerr << "***********************************************"<<endl;
    cerr << "ROUTING MESSAGE:" << *m << endl << endl;

    cerr << endl << endl;

    // Your neighbor changed their table and has something to tell you. We'll
    // look over their cost map (called distanceVector).
    map<int, double> dv = m->getDistanceVector();

    routing_table.distance_vectors[m->getSrc()] = dv;

    /* Look over every entry in their distance vector. If we find any cost path
       that is better than the entry that we already have (or if we have yet to
       establish any connection with the node at all), then we'll update our
       tables accordingly
    */

    bool weMadeAChange = findImprove();

    cerr << *this << endl;
    cerr << "***********************************************"<<endl;
    cerr << "***********************************************"<<endl;

    if(weMadeAChange)
    {
        SendToNeighbors(new RoutingMessage(routing_table.cost, GetNumber()));
    }

}

int DistanceVector::costToNeighbor(int neighborNum)
{

    deque<Link *> *myNeighbors = GetOutgoingLinks();

    for(unsigned int i = 0; i < myNeighbors->size(); i++)
    {
        int thisLink = myNeighbors->at(i)->GetDest();
        if(neighborNum == thisLink)
        {
            return myNeighbors->at(i)->GetLatency();
        }
    }
    cout<<"Error! You asked for the cost to "<<neighborNum<<" but I can't find it."<<endl;
    return -1;
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


ostream &DistanceVector::Print(ostream &os) const
{
    Node::Print(os);
    return os;
}


