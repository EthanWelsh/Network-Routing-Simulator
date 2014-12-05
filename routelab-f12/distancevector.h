#ifndef _distancevector
#define _distancevector

#include "node.h"

class DistanceVector: public Node {
    private:
        // Anything you need in addition to Node members

    public:
        DistanceVector(unsigned, SimulationContext* , double, double);
        DistanceVector(const DistanceVector &);
        DistanceVector & operator=(const DistanceVector &);
        ~DistanceVector();

        // Inherited from Node
        void LinkHasBeenUpdated(Link *l);
        void ProcessIncomingRoutingMessage(RoutingMessage *m);
        void TimeOut();
        Node* GetNextHop(Node* destination);
        Table* getRoutingTable();
        int costToNeighbor(int neighborNum);
        void findImprove();

    ostream & Print(ostream & os) const;

        // Anything else
    void findImprove();
};

inline ostream & operator<<(ostream & os, const DistanceVector & n) {
    return n.Print(os);
}

#endif
