/** 
 * encode.cc
 *
 * Application to encode a plain text file
 * using the Huffman encoding scheme
 */

// Include libraries
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

// Include class files
#include "Node.hh"
#include "PriorityQueue.hh"
#include "HuffmanTree.hh"




/** 
 * main()
 *
 * Implementation and testing
 */
int main( int argc, char *argv[] ) {
    // Program variables
    size_t      pos;
    std::string input;

    // Determine if file names are given via command line
    if( argc > 1 ) {
        // Read second argument
        input = argv[1];
    } else {
        // Ask for filenames from stdin
        std::cout << "Which file would you like to encode? ";
        std::cin >> input;
    }

    // Naive check that filename has .txt extension
    pos = input.find( ".txt" );
    if( pos == std::string::npos ) {
        std::cout << "  Unsupported file. Must have .txt extension" << std::endl;
        exit( EXIT_FAILURE );
    }

    // Construct Huffman Tree
    HuffmanTree HT;

    // Open file
    std::ifstream inputFile;
    inputFile.open( input.c_str(), std::ios::in );

    // Check if file opens successfully
    if( !inputFile.good() ) {
        std::cout << "  Cannot open file" << std::endl;
        std::cout << "  Exiting ..." << std::endl;
        exit( EXIT_FAILURE );
    }

    // Check if empty file
    if( inputFile.peek() == std::ifstream::traits_type::eof() ) {
        std::cout << "  This is an empty file. No compression required" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Build frequency table
    HT.countFrequencies( inputFile );

    // Print out results
    HT.printFrequencies();

    // Build Huffman tree
    HT.buildHuffmanTree();

    // Print out prefix codes
    HT.printPrefix();

    // Begin encoding
    HT.encode( input, inputFile );

    // Close file
    inputFile.close();

    return EXIT_SUCCESS;
}