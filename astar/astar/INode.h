#pragma once

class INode {
public:
    INode* parent;

    INode() : parent(0) {}
    virtual ~INode() {}

    virtual int getG() const = 0;
    virtual int getH() const = 0;
    int getF() const { return getG() + getH(); }

    // Returns a newly-allocated array of neighbor nodes; caller owns each node and the array.
    // Sets count to the number of neighbors.
    virtual INode** getNeighbors(int& count) const = 0;

    // Returns a null-terminated key uniquely identifying this state.
    // Pointer is valid for the lifetime of this node.
    virtual const char* getKey() const = 0;
};
