/** 
 * PriorityQueue.cc
 *
 * Class methods and implementation
 * for building and using a 
 * priority queue based on a min-heap
 * and arrays
 */

// Inlcude header file
#include "PriorityQueue.hh"

/** 
 * PriorityQueue()
 *
 * Default constructor initialises array to size n+1
 * since root node sits in heap[1]
 */
PriorityQueue::PriorityQueue( const int n ) {
    // Set size of heap
    size = n + 1;

    // Initialise heap array
    heap = new Node*[size];

    // Initialise tail pointer
    tail = 0;
}

/** 
 * getSize()
 *
 * Returns size of queue
 */
int PriorityQueue::getSize() {
    return size;
}

/** 
 * getTail()
 *
 * Returns the tail position of queue
 * i.e. number of nodes currently in queue
 */
int PriorityQueue::getTail() {
    return tail;
}

/** 
 * insert()
 *
 * Inserts a new node into the priority queue
 */
void PriorityQueue::insert( Node *node ) {
    // Check heap is not full
    if( tail == size ) {
        std::cout << "Heap is full";
        std::cout << "Exiting ..." << std::endl;

        exit( EXIT_FAILURE );
    }

    // Calculate parent using integer division
    int parent = ( ++tail ) / 2;

    // Insert node at end of heap
    heap[tail] = node;

    // Propogate up through the heap, swapping elements
    // until new node is in correct position in heap
    //   Note: we are building a min-heap
    while( parent > 0 && node -> frequency < heap[parent] -> frequency ) {
        // Swap nodes
        swap( parent, tail );

        // Update parent position
        parent /= 2;
    }
}

/** 
 * removeMin()
 *
 * Removes and returns Node of minimum
 * priority (frequency)
 * Note: this function does not delete
 * the node
 */
Node* PriorityQueue::removeMin() {
    // Store pointer to root element of heap
    Node *min = heap[1];

    // Replace root node with node at end of array
    heap[1] = heap[tail];

    // Initiate current position
    int current = 1;

    // Initiate child as left child
    int child  = 2 * current;

    // Propogate down heap, swapping elements of 
    // children with smaller frequency

    // While current node has a left child
    while( child < tail ) {
        // Check if right child exists 
        // and has frequency smaller than left child
        //   Note: we are building a min-heap
        if( child + 1 < tail && heap[child] -> frequency > heap[child+1] -> frequency ) {
            // The right child is better
            child++;
        }

        // Exit if current node is smaller than child node
        //   Note: we are building a min-heap
        if( heap[current] -> frequency < heap[child] -> frequency ) {
            break;
        }

        // Swap nodes
        swap( current, child );

        // Update child and left counters
        current = child;
        child   = 2 * current;
    }

    // Update tail pointer
    --tail;

    // Return Node
    return min;
}

/** 
 * swap()
 *
 * Swaps position (pointers) of two Nodes in heap
 */
void PriorityQueue::swap( const int a, const int b ) {
    // Create temporary node pointer
    Node *temp = heap[a];

    // Swap pointers
    heap[a] = heap[b];
    heap[b] = temp;
}

/** 
 * print()
 *
 * Prints out priority queue to terminal
 */
void PriorityQueue::print() {
    std::cout << std::setw(15) << "Node" 
              << std::setw(15) << "Value"
              << std::setw(15) << "Symbol"
              << std::setw(15) << "Frequency" << std::endl;

    for( int i = 1; i <= tail; i++ ) {
        std::cout << std::setw(15) << i 
                  << std::setw(15) << heap[i] -> value;

        if( heap[i] -> value == 10 ) {
            std::cout << std::setw(15) << "\\n";
        } else if( heap[i] -> value == 0 ) {
            std::cout << std::setw(15) << " ";
        } else {
            std::cout << std::setw(15) << (char) heap[i] -> value;
        }

        std::cout << std::setw(15) << heap[i] -> frequency << std::endl;
    }
}