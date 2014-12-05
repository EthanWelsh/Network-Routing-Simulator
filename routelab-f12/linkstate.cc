#include "linkstate.h"
#include "context.h"
#include <set>

LinkState::LinkState(unsigned n, SimulationContext* c, double b, double l) :
    Node(n, c, b, l)
{}

LinkState::LinkState(const LinkState & rhs) :
    Node(rhs)
{
    *this = rhs;
}

LinkState & LinkState::operator=(const LinkState & rhs) {
    Node::operator=(rhs);
    return *this;
}

LinkState::~LinkState()
{}

/** Write the following functions.  They currently have dummy implementations **/
void LinkState::LinkHasBeenUpdated(Link* l)
{
    cerr << *this << ": Link Update: " << *l << endl;
    
	// Get the latency and update the link in the table
	int src= l-> GetSrc();
	int dest= l-> GetDest();
	int latency= l-> GetLatency();
	routing_table.topo[src][dest].cost= latency;
	
	// Update the age of the link since it is being updated
	int age= routing_table.topo[src][dest].age;
    routing_table.topo[src][dest].age = ++age;
	
	// Send the message to the rest of the connections
	SendToNeighbors(new RoutingMessage());
}

void LinkState::ProcessIncomingRoutingMessage(RoutingMessage *m)
{
    //cerr << *this << " got a routing message: " << *m << " (ignored)" << endl;
	
	int src = m->src;
    int dest = m->dest;
	int age = m->age;
    int latency = m->latency;
	
	// Update the node's table with new info
	// First check to see if it exists
	if(routing_table.topo[src][dest].cost == -1)
    {
        // If not add it to the table
        routing_table.topo[src][dest].age = age;
        routing_table.topo[src][dest].cost = latency;
        routing_table.hopMapNeedsChanging = true; // TODO in table --- change made to the map
        SendToNeighbors(m);
    }
	// In the list so just update the age
	else if(routing_table.topo[src][dest].age < age)
    {
        // We have seen this combination before we now just update the age
        routing_table.topo[src][dest].age = age;
        routing_table.topo[src][dest].cost = lat;
        routing_table.hopMapNeedsChanging = true; // TODO in table --- change made to the map
        SendToNeighbors(m);
    }
}

void LinkState::TimeOut()
{
    cerr << *this << " got a timeout: (ignored)" << endl;
}

Node* LinkState::GetNextHop(Node *destination)
{ 
    // Run Djikstra on the map to make its up to date
	if (routing_table.hopMapNeedsChanging)
	{
		map<int, int> distances;
        set<int> visited;
		
		map<int, map<int, TopoLink> > topo = routing_table.topo;
        int size_of_topo = topo.size();
        set<int> queue;
		
		map<int, TopoLink>::const_iterator itr;
        map<int, TopoLink> neighbors;
        map<int, int>::const_iterator prev_itr;
        map<int, int> previous;
		
		for(int i =0; i< size_of_topo; i++) 
        {
            distances[i] = INT_MAX;
            previous[i] = -1;
        }
		distances[number] = 0;
        queue.insert(number); //Inserts itself into the queue
		
		while(!queue.empty())
		{
			int minimum= INT_MAX;
			int smallest_node= -1;
			for(int i=0; i< size_of_topo; i++)
			{
				if(queue.count(i) > 0)
				{  
                    if(visited.count(i) < 1)
                    {
                        if(dist[i] < min)
                        {
                            minimum = distances[i];
                            smallest_node = i;
                        }
                    }
				}
			}
		}
		
		// Remove the smallest value node from the queue
		queue.erase(smallest_node);
		visited.insert(smallest_node);
		
		// Find the inserted nodes values
		neighbors= topo[smallest];
		for(itr= neighbors.begin(); itr!= neighbors.end(); itr++)
		{
			if( visited.count(itr->first)< 1 ) 
			{
				int v = itr->first;
				int current_distance;
				int alt_distance;

				current_distance= distances[v];
				alt_distance= distances[smallest_node] + topo[smallest_node][v].cost;
				
				if(alt_distance < current_distance)
				{
					distances[v] = alt_distance;
					previous[v] = smallest_node;

					if( visited.count(v) < 1)
					{
						queue.insert(v);
					}
				}
			}
		}
		
		map<int, int> temp_hop_map;
        for(prev_itr = previous.begin(); prev_itr != previous.end(); prev_itr++)
		{
			unsigned int before = prev_itr->second;
            int current = prev_itr->first;
            int current2 = prev_itr->first;
            while(before != number)
			{
                current = before;
                before = previous[before];
            }
            temp_hop_map[current2]= current;
        }
		
		Node *temp_node = new Node(temp_hop_map[destination->number], NULL, 0, 0);
        Node * real_node = context->FindMatchingNode(const_cast<Node *>(a));
        routing_table.hopMap = temp_hop_map;
        routing_table.hopMapNeedsChanging = false;	// TODO adjust this name
        return real_node;
	}
	else
	{   
		Node *temp_node = new Node(routing_table.hopMap[dest->number], NULL, 0, 0);
        Node * real_node = context->FindMatchingNode(const_cast<Node *>(temp_node));
        return real_node;        
    } 
    return NULL;
}

Table* LinkState::GetRoutingTable()
{
    // This returns a copy of the routing table
	Table *copy= new Table(routing_table);
	return temp;
}

ostream & LinkState::Print(ostream &os) const
{ 
    Node::Print(os);
    return os;
}
