#include "linkstate.h"

LinkState::LinkState(unsigned n, SimulationContext *c, double b, double l) :
        Node(n, c, b, l)
{
    seq = 0;
}

LinkState::LinkState(const LinkState &rhs) :
        Node(rhs)
{
    *this = rhs;
    seq = 0;
}

LinkState &LinkState::operator=(const LinkState &rhs)
{
    Node::operator=(rhs);
    return *this;
}

LinkState::~LinkState()
{
}

/** Write the following functions.  They currently have dummy implementations **/



void LinkState::LinkHasBeenUpdated(Link *l)
{

    cerr<<endl<<endl;
    cerr<<"***********************************************"<<endl;
    cerr<<"***********************************************"<<endl;
    cerr << endl << "LINK UPDATE: " << *l << endl << endl;

    int link_dest = l->GetDest();
    int link_cost = l->GetLatency();

    // If this is the first time we've seen this node...
    if(routing_table.neighbor_table.find(link_dest) == routing_table.neighbor_table.end())
    {
        routing_table.neighbor_table[link_dest] = link_cost;
        routing_table.hop[link_dest] = link_dest;
        routing_table.cost[link_dest] = link_cost;
    }
    else
    {

        double old_cost = routing_table.neighbor_table[link_dest];
        double change_in_cost = link_cost - old_cost;

        routing_table.neighbor_table[link_dest] = link_cost;

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
                cerr<<"The old length is " << routing_table.neighbor_table[table_dest] << " but we're adding " << change_in_cost << endl;

                routing_table.cost[table_dest] += change_in_cost;

                cerr<<"So the new value is " << routing_table.cost[table_dest] << endl;

                /* We have found a path to another node that uses the node which is the destination in the
                   changed connection. We now need to adjust our cost for this path accordingly.
                */

            }
        }
    }

    findImprove();

    cerr<<"SENDING MESSAGE TO NEIGHBORS (from " << GetNumber() << ")"<<endl;

    cerr<<"***********************************************"<<endl;
    cerr<<"***********************************************"<<endl;
    cerr<<endl<<endl;

    SendToNeighbors(new RoutingMessage(routing_table.neighbor_table, GetNumber(), seq));
}

void LinkState::ProcessIncomingRoutingMessage(RoutingMessage *m)
{
    cerr << endl << endl;
    cerr << "***********************************************" << endl;
    cerr << "***********************************************" << endl;
    cerr << "ROUTING MESSAGE:" << *m << endl << endl;

    if(message_seqs.find(m->src_node) == message_seqs.end())
    {
        routing_table.topology[m->src_node] = m->neighbor_table;
        Flood(m);
        message_seqs[m->src_node] = m->seq;
    }
    else
    {
        if (message_seqs[m->src_node] < m->seq )
        {
            routing_table.topology[m->src_node] = m->neighbor_table;
            Flood(m);
            message_seqs[m->src_node] = m->seq;
        }
    }

    cerr << *this << endl;
    cerr << "***********************************************" << endl;
    cerr << "***********************************************" << endl;

}


void LinkState::Flood(RoutingMessage *m)
{
    seq = m->seq;
    findImprove();
    SendToNeighbors(m);
}




class myNode
{
    int costToNode;
    int node;
    int src;

public:

    myNode(int n, int c, int s) : node(n), costToNode(c), src(s)
    {
    }

    friend bool operator<(const myNode &x, const myNode &y)
    {
        if (x.costToNode > y.costToNode) return true;
        else return false;
    }

    int getNode()const
    {
        return node;
    }

    int getCost()const
    {
        return costToNode;
    }

    int getSrc()const
    {
        return src;
    }
};



void LinkState::findImprove()
{
    int me = GetNumber();
    priority_queue<myNode> pq;

    routing_table.cost[me] = 0;

    typedef std::map<int, double>::iterator it_type;
    for(it_type it1 = routing_table.neighbor_table.begin(); it1 != routing_table.neighbor_table.end(); it1++)
    {
        int num_of_this_neighbor = it1->first;
        double cost_to_this_neighbor = it1->second;

        routing_table.cost[num_of_this_neighbor] = cost_to_this_neighbor;
        routing_table.hop[num_of_this_neighbor] = num_of_this_neighbor;

        pq.push(myNode(num_of_this_neighbor, cost_to_this_neighbor, num_of_this_neighbor));
    }

    myNode currentNode(0,0,0);
    while(pq.size() > 0)
    {
        currentNode = pq.top();
        pq.pop();

        int curr_num = currentNode.getNode();
        double curr_cost = currentNode.getCost();

        map<int, double> curr_node_table = routing_table.topology[curr_num];

        for(it_type it2 = routing_table.neighbor_table.begin(); it2 != routing_table.neighbor_table.end(); it2++)
        {
            int neighbor_of_curr = it2->first;
            double cost_to_neightbor_of_curr = it2->second + curr_cost;

            if(routing_table.cost.find(neighbor_of_curr) == routing_table.cost.end())
            { // could not find
                routing_table.cost[neighbor_of_curr] = cost_to_neightbor_of_curr;
                pq.push(myNode(neighbor_of_curr, cost_to_neightbor_of_curr, currentNode.getSrc()));
            }
            else
            {
                if(cost_to_neightbor_of_curr < routing_table.cost[neighbor_of_curr])
                {
                    routing_table.cost[neighbor_of_curr] = cost_to_neightbor_of_curr;
                }
            }




        }



    }






}


void LinkState::TimeOut()
{
    cerr << *this << " got a timeout: (ignored)" << endl;
}

Node *LinkState::GetNextHop(Node *destination)
{
    return NULL;
}

Table *LinkState::GetRoutingTable()
{
    return NULL;
}

ostream &LinkState::Print(ostream &os) const
{
    Node::Print(os);
    return os;
}