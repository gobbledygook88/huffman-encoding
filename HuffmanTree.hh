/** 
 * HuffmanTree.hh
 *
 * Class definitions
 */

#ifndef HUFFMANTREE_HH
#define HUFFMANTREE_HH

// Include libraries
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <string>
#include <tr1/unordered_map>
#include <fstream>
#include <sstream>

// Include definitions
#include "Node.hh"
#include "PriorityQueue.hh"
#include "BitIO.hh"

/** 
 * HuffmanTree.cc
 *
 * Class definitions and methods
 */

class HuffmanTree {
    public:
        HuffmanTree() {}                                                                    // Default constructor
        
        Node* getRoot();                                                                    // Returns root node
        void  countFrequencies( std::ifstream &inputFile );                                 // Build frequency table
        void  buildPriorityQueue( PriorityQueue &PQ);                                       // Build priority queue
        void  buildHuffmanTree();                                                           // Main Huffman Tree constructor

        void  encode( std::string filename, std::ifstream &input );                         // Create encoded file
        void  decode( std::string filename, std::ifstream &input );                         // Create decoded file

        void  printFrequencies();                                                           // Prints table of frequencies
        void  printPrefix();                                                                // Default prefix print function
        void  printPrefix( Node *node, std::string code );                                  // Main prefix print function

        void  printHuffmanTree( BitIO &writer );                                            // Writes Huffman Tree to file
        void  printHuffmanTree( Node *node, BitIO &writer );                                // Overload tree write function

        Node* decodeHuffmanTree( BitIO &reader, int &numChars );                            // Reads file bit by bit to construct Huffman Tree

    private:
        Node *root;
        std::tr1::unordered_map< int, int > frequencies;                                    // Unordered map to hold frequencies
        std::tr1::unordered_map< int, std::string > codes;                                  // Unordered map to hold prefix codes
};

#endif