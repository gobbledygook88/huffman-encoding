/** 
 * Node.cc
 *
 * Class methods
 */

// Include header file
#include "Node.hh"

/** 
 * Node()
 *
 * Copy constructor
 */
Node::Node( Node *n ) {
    // Set members
    value     = n -> value;
    frequency = n -> frequency;

    if( n -> left != NULL ) {
        left = new Node();
        *left = *( n -> left );
    }

    if( n -> right != NULL ) {
        right = new Node();
        *right = *( n -> right );
    }
}

/** 
 * ~Node()
 *
 * Destructor deletes child pointers
 * if declared on construction
 */
Node::~Node() {
    // Delete left node
    if( left != NULL ) {
        delete left;
    }

    // Delete right node
    if( right != NULL ) {
        delete right;
    }
}

/** 
 * print()
 *
 * Prints node information to terminal
 * for debugging purposes
 */
void Node::print() {
    std::cout << std::setw(10) << "Value: "     << std::setw(5)  << value     << " "
              << std::setw(10) << "Frequency: " << std::setw(20) << frequency << " ";

    if( left != NULL ) {
        std::cout << "LEFT ";
    }

    if( right != NULL ) {
        std::cout << "RIGHT ";
    }

    std::cout << std::endl;
}