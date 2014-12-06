#include "linkstate.h"

LinkState::LinkState(unsigned n, SimulationContext *c, double b, double l) : Node(n, c, b, l)
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


void LinkState::LinkHasBeenUpdated(Link *l)
{
    cerr<<endl<<endl;
    cerr<<"***********************************************"<<endl;
    cerr<<"***********************************************"<<endl;
    cerr << endl << "LINK UPDATE: " << *l << endl << endl;

    int link_dest = l->GetDest();
    int link_cost = l->GetLatency();

    routing_table.neighbor_table[link_dest] = link_cost;

    cerr<<routing_table<<endl;

    seq++;

    cerr<< "("<<GetNumber()<<") SENDING MESSAGE TO NEIGHBORS (" << seq << ")"<<endl;


    SendToNeighbors(new RoutingMessage(routing_table.neighbor_table, GetNumber(), seq));

    cerr<<"***********************************************"<<endl;
    cerr<<"***********************************************"<<endl;
    cerr<<endl<<endl;
}



void LinkState::ProcessIncomingRoutingMessage(RoutingMessage *m)
{
    if(m->src_node == GetNumber()) return;

    if(message_seqs.find(m->src_node) == message_seqs.end())
    {
        routing_table.topology[m->src_node] = m->neighbor_table;
        message_seqs[m->src_node] = m->seq;
        Flood(m);
    }
    else
    {
        if (message_seqs[m->src_node] < m->seq )
        {
            routing_table.topology[m->src_node] = m->neighbor_table;
            message_seqs[m->src_node] = m->seq;
            Flood(m);
        }
        else
        {
            //cerr<< "(" <<GetNumber() << ") DISCARDING message. I already have a SEQ num of "<< message_seqs[m->src_node] << " from " << m->seq << endl;
        }
    }

    cerr << "(" <<GetNumber() << ") ROUTING MESSAGE: (" << m->src_node << " -> " << GetNumber() << ") : " << m->seq << endl;
}


void LinkState::Flood(RoutingMessage *m)
{
    cerr<<"(" <<GetNumber()<< ") FLOODING: "<<m->src_node << " (" << m->seq << ")" <<endl;
    SendToNeighbors(m);
}

// Class created for purposes of the PQ.
class myNode
{
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

    int costToNode;
};


void LinkState::findImprove()
{
    routing_table.cost.clear();
    routing_table.hop.clear();

    int me = GetNumber();
    priority_queue<myNode> pq;

    routing_table.cost[me] = 0;

    // Add all your neighbors to the cost and hop tables and put them in the pq
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

    // Take a node at a time off the list and add that nodes' neighbors if it results in a better shorter path than before
    while(pq.size() > 0)
    {
        currentNode = pq.top();
        pq.pop();

        int curr_num = currentNode.getNode();
        double curr_cost = currentNode.getCost();

        map<int, double> curr_node_table = routing_table.topology[curr_num];

        for(it_type it2 = curr_node_table.begin(); it2 != curr_node_table.end(); it2++)
        {
            int neighbor_of_curr = it2->first;
            double cost_to_neighbor_of_curr = it2->second + curr_cost;

            if(routing_table.cost.find(neighbor_of_curr) == routing_table.cost.end())
            { // could not find
                routing_table.cost[neighbor_of_curr] = cost_to_neighbor_of_curr;
                routing_table.hop[neighbor_of_curr] = currentNode.getSrc();

                pq.push(myNode(neighbor_of_curr, cost_to_neighbor_of_curr, currentNode.getSrc()));
            }
            else
            {
                if(cost_to_neighbor_of_curr < routing_table.cost[neighbor_of_curr])
                {
                    routing_table.cost[neighbor_of_curr] = cost_to_neighbor_of_curr;
                    routing_table.hop[neighbor_of_curr] = currentNode.getSrc();
                    pq.push(myNode(neighbor_of_curr, cost_to_neighbor_of_curr, currentNode.getSrc()));
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
    findImprove();
    int dest = destination->GetNumber();
    int next = routing_table.hop[dest];

    Node *n = new Node(0, NULL, 0, 0);
    n->SetNumber(next);

    return n;
}


Table *LinkState::GetRoutingTable()
{
    return &routing_table;
}


ostream &LinkState::Print(ostream &os) const
{
    //Node::Print(os);
    return os;
}