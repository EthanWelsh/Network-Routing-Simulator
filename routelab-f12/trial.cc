//: C07:PriorityQueue3.cpp
// A more complex use of priority_queue.
#include <iostream>
#include <queue>
#include <string>

using namespace std;

class myNode
{
    int node;
    int costToNode;

public:
    myNode(int n, int c) : node(n), costToNode(c)
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
};

int main()
{
    priority_queue<myNode> toDoList;
    toDoList.push(myNode(2, 10));
    toDoList.push(myNode(1, 50));
    toDoList.push(myNode(4, 40));
    toDoList.push(myNode(3, 5));
    toDoList.push(myNode(5, 25));


    while (!toDoList.empty())
    {
        cout << toDoList.top().getNode() << " : " << toDoList.top().getCost() << endl;
        toDoList.pop();
    }
    return 0;
}