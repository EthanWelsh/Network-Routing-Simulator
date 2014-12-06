#ifndef _linkstate
#define _linkstate

#include "node.h"
#include <queue>

class LinkState : public Node
{
private:
    // Anything you need in addition to Node members

public:
    LinkState(unsigned, SimulationContext *, double, double);

    LinkState(const LinkState &);

    LinkState &operator=(const LinkState &);

    ~LinkState();

    // Inherited from Node
    void LinkHasBeenUpdated(Link *l);

    void ProcessIncomingRoutingMessage(RoutingMessage *m);

    void TimeOut();

    Node *GetNextHop(Node *destination);

    Table *GetRoutingTable();

    ostream &Print(ostream &os) const;

    void findImprove();
    void Flood(RoutingMessage *m);

    map<int, int> message_seqs;
    int seq;
};

inline ostream &operator<<(ostream &os, const LinkState &n)
{
    return n.Print(os);
}

#endif