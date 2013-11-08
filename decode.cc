/** 
 * decode.cc
 *
 * Application to decode an Huffman encoded text file
 */

// Include libraries
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

// Include class files
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
        std::cout << "Which file would you like to decode? ";
        std::cin >> input;
    }

    // Naive check that filename has .huf extension
    pos = input.find( ".huf" );
    if( pos == std::string::npos ) {
        std::cout << "  Unsupported file. Must have .huf extension" << std::endl;
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

    // Let the decoding commence!
    HT.decode( input, inputFile );

    return EXIT_SUCCESS;
}