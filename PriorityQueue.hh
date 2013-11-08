/** 
 * PriorityQueue.hh
 *
 * Class definitions
 */

#ifndef PRIORITYQUEUE_HH
#define PRIORITYQUEUE_HH

// Include libraries
#include <iostream>
#include <cstdlib>
#include <iomanip>

// Include classes
#include "Node.hh"

/** 
 * PriorityQueue
 *
 * Class definitions
 */
class PriorityQueue {
    public:
        PriorityQueue( const int n );                    // Main constructor: Empty queue

        int    getSize();
        int    getTail();

        void   insert( Node *node );
        Node*  removeMin();

        void   swap( const int a, const int b );         // Method for swap position of two nodes in heap

        void   print();

    private:
        int    size;
        Node **heap;                                     // Array of Node pointers
        int    tail;
};

#endif