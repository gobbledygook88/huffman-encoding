/** 
 * Node.cc
 *
 * Class definitions and functions
 */

#ifndef NODE_HH
#define NODE_HH

// Include libraries
#include <iostream>
#include <iomanip>

/** 
 * Node
 *
 * Class definitions and methods
 */
class Node {
    public:
        Node() {}                   // Default constructor
        Node( Node *n );            // Copy Constructor
        ~Node();                    // Destructor

        void print();

        int value;
        int frequency;
        
        Node *left;
        Node *right;
};

#endif