/** 
 * HuffmanTree.cc
 *
 * Class methods and implementation
 */

// Include header file
#include "PriorityQueue.hh"
#include "HuffmanTree.hh"

/** 
 * getRoot()
 *
 * Returns root of Huffman Tree
 */
Node* HuffmanTree::getRoot() {
    return root;
}

/** 
 * countFrequencies()
 *
 * Populate frequency table from file
 */
void HuffmanTree::countFrequencies( std::ifstream &inputFile ) {
    // Function variables
    char c;

    // Run through file
    while( !inputFile.eof() ) {
        // Read each character in file
        c = inputFile.get();

        if( (int) c != EOF ) {
            // Iterate counters in map
            frequencies[(int) c]++;
        }
    }
}

/** 
 * buildPriorityQueue()
 *
 * Constructs a min-heap priority queue
 */
void HuffmanTree::buildPriorityQueue( PriorityQueue &PQ ) {
    // Create iterator
    std::tr1::unordered_map< int, int >::iterator it;

    // Loop through frequency table
    for( it = frequencies.begin(); it != frequencies.end(); it++ ) {
        // Store key and value
        int key   = it -> first;
        int value = it -> second;

        // Create a new node
        Node *n = new Node();
        n -> value     = key;
        n -> frequency = value;
        n -> left      = NULL;      // Indicates this is a leaf node
        n -> right     = NULL;      // Indicates this is a leaf node

        // Insert leaf node into priority queue
        PQ.insert( n );
    }
}

/** 
 * buildHuffmanTree()
 *
 * Constructs Huffman Tree from
 * min-heap priority queue
 */
void HuffmanTree::buildHuffmanTree() {
    // Check if we will have potential overflow
    if( frequencies.size() > 255 ) {
        std::cout << "  Warning: Potential overflow of characters" << std::endl;
    }

    // Create priority queue
    PriorityQueue PQ( frequencies.size() );

    // Build priority queue
    buildPriorityQueue( PQ );

    // Build Huffman Tree from priority queue
    // with linked list tree implementation
    while( PQ.getTail() > 1 ) {
        // Create new node
        Node *z = new Node();

        // Create temporary node pointers
        Node *x = PQ.removeMin();
        Node *y = PQ.removeMin();

        // Update pointers
        z -> left = x;
        z -> right = y;

        // Set freqeuncy of new node
        z -> frequency = x -> frequency + y -> frequency;

        // Other members
        z -> value = 0;

        // Insert new node into priority queue
        PQ.insert( z );
    }

    // Safety check (should always be true)
    // If PQ only has one node, set it as root
    if( PQ.getTail() == 1 ) {
        root = PQ.removeMin();
    } else {
        std::cout << "An error has occured when building the Huffman Tree" << std::endl;
        std::cout << "Exiting ..." << std::endl;
        exit( EXIT_FAILURE );
    }
}

/** 
 * encode()
 *
 * Creates encoded file with header information
 * for decoding
 */
void HuffmanTree::encode( std::string filename, std::ifstream &input ) {
    // Function variables
    int  inputByte, outputByte, enPad;
    char c;

    // Rewind input file
    input.clear();
    input.seekg( 0, std::ios::beg );

    // Prepare output filename
    int pos = filename.find( ".txt" );
    std::string outputFilename = filename.substr( 0, pos ) + ".huf";
    std::cout << "  Encoded file is called " << outputFilename << std::endl;

    // Open output file
    std::ofstream output;
    output.open( outputFilename.c_str(), std::ios::out | std::ios::trunc );

    // Check if output file is ready for writing
    if( !output.good() ) {
        std::cout << "  Error opening output file" << std::endl;
        std::cout << "  Exiting..." << std::endl;
    }

    // Define BitIO writer object
    BitIO writer( output );

    // Write number of symbols to output file
    output << (unsigned char) frequencies.size();

    // Initialise and add a char (1 byte) to the counter
    outputByte = 8;

    // Write Huffman Tree to output file
    printHuffmanTree( writer );

    // Update output counter with size of Huffman Tree output
    outputByte += 10 * frequencies.size() - 1;

    // Initiate input file byte counter
    inputByte = 0;

    // Write codes to file
    // Grab each character
    while( input.get( c ) ) {
        // Update byte counter of input file
        inputByte++;

        // Cache length of code
        int codeLength = codes[c].length();

        // Add length of code to output counter
        outputByte += codeLength;

        // Write code to file char by char
        for( int i = 0; i < codeLength; i++ ) {
            if( codes[c][i] == '0' ) {
                writer.writeBit( (char) 0 );
            } else if( codes[c][i] == '1' ) {
                writer.writeBit( (char) 1 );
            }
        }
    }

    // Check if buffer needs padding
    enPad = writer.pad();

    // Write padding information
    writer.writeSymbol( (char) enPad );

    // Update output counter with padding size and extra byte of data
    outputByte += enPad + 8;

    // Divide number of bits in output file by 8 to get bytes
    //   Always divisible by 8 because of padding
    outputByte /= 8;

    //std::cout << "padding = " << enPad << std::endl;

    // Print out compression data
    std::cout << std::endl;
    std::cout << "    Size of original file:" << std::setw(10) << inputByte  
                                              << std::setw(6)  << "bytes" << std::endl;
    std::cout << "  Size of compressed file:" << std::setw(10) << outputByte 
                                              << std::setw(6)  << "bytes" << std::endl;
    std::cout << "        Compression ratio:" << std::setw(10) << (double) outputByte / (double) inputByte << std::endl;

    // Close files
    input.close();
    output.close();
}

/** 
 * decode()
 *
 * Decodes encoded file
 * Read header information first to reconstruct Huffman Tree
 */
void HuffmanTree::decode( std::string filename, std::ifstream &input ) {
    // Function variables
    int           numChars, pos, last, enPad, readNumBits, flag = 0;
    char          c;
    unsigned char bit;

    // Move file pointer to last byte
    input.seekg( -1, std::ios::end );

    // Cache position of file pointer
    last = input.tellg();
    
    // Get size of padding
    enPad = input.get();
    //std::cout << "Padding = " << enPad << std::endl;

    // Clear file flags
    input.clear();

    // Reset file pointer to start of file
    input.seekg( 0, std::ios::beg );

    // First get number of symbols
    numChars = input.get();

    // Create BitIO reader object
    BitIO reader( input );

    // Read in numChars number of symbols and build Huffman Tree
    while( !input.eof() && numChars != 0 ) {
        // Read one bit
        bit = reader.readBit();

        // Create the root node
        root = new Node();

        // Initial decision
        if( bit == 0x01 ) {
            // There is only one node, the root node
            root -> value = (int) reader.readSymbol();

            // Ensure child nodes are properly defined
            root -> left  = NULL;
            root -> right = NULL;

            // Decrement number of characters counter
            --numChars;
        } else {
            // There is more than one node, recurse left then right
            root -> left  = decodeHuffmanTree( reader, numChars );
            root -> right = decodeHuffmanTree( reader, numChars );
        }
    }

    // Prepare output filename
    pos = filename.find( ".huf" );
    std::string outputFilename = filename.substr( 0, pos ) + ".decoded.txt";
    std::cout << "  Beginning decoding process ..." << std::endl;

    // Open output file
    std::ofstream output;
    output.open( outputFilename.c_str(), std::ios::out | std::ios::trunc );

    // Check if output file is ready for writing
    if( !output.good() ) {
        std::cout << "  Error opening output file" << std::endl;
        std::cout << "  Exiting..." << std::endl;
    }

    // Initiate at root node
    Node *current = root;

    // Write out bits
    while( input.tellg() < last ) {
        // Grab next bit in file
        bit = reader.readBit();

        // Traverse down Huffman Tree
        current = ( ( bit == 0x00 ) ? current -> left : current -> right );

        // Check if next node is a leaf node
        if( current -> left == NULL || current -> right == NULL ) {
            // Write to file the previous symbol
            // if not the first pass of while loop
            if(flag) output << c;

            // Update symbol
            c = (char) (current -> value);

            // Set write flag
            flag = 1;

            // Reset current node to root node
            current = root;
        }
    }

    // Write out last character in previous section
    output << c;

    // Calculate how many bits to read from last byte
    readNumBits = ( enPad == 0 ) ? 8 : ( 8 - enPad );

    // Read last byte
    for( int i = 0; i < readNumBits; i++ ) {
        // Grab next bit in file
        bit = reader.readBit();

        // Traverse down Huffman Tree
        current = ( ( bit == 0x00 ) ? current -> left : current -> right );

        // Check if next node is a leaf node
        if( current -> left == NULL || current -> right == NULL ) {
            // Update symbol
            c = (char) (current -> value);

            // Write to file the symbol
            output << c;

            // Reset current node to root node
            current = root;
        }
    }

    // Close files
    input.close();
    output.close();

    // Ending declaration
    std::cout << "  Finished decoding ..." << std::endl;
    std::cout << "  Decoded file is called " << outputFilename << std::endl;
}

/** 
 * printFrequencies()
 *
 * Prints frequency table to terminal
 */
void HuffmanTree::printFrequencies() {
    std::cout << std::endl;
    std::cout << "  Finished counting frequencies. Here are the results:" << std::endl;

    std::tr1::unordered_map< int, int >::iterator it;

    for( it = frequencies.begin(); it != frequencies.end(); it++ ) {
        // Print out key
        if( it -> first == 10 ) {
            std::cout << std::setw(5) << "\\n";
        } else {
            std::cout << std::setw(5) << (char) ( it -> first );
        }

        // Print out value
        std::cout << std::setw(5) << it -> second << std::endl;
    }
}

/** 
 * printPrefix()
 *
 * Overload prefix print function
 */
void HuffmanTree::printPrefix() {
    // String to store code of symbol
    std::string code = "";

    std::cout << "  Prefix codes:" << std::endl;

    // Start printing at root node
    printPrefix( root, code );
}

/** 
 * printPrefix()
 *
 * Recursively prints prefix codes to terminal
 */
void HuffmanTree::printPrefix( Node *node, std::string code) {
    if( node -> left == NULL || node -> right == NULL ) {
        // Print symbol and code to terminal
        if( node -> value == 10 ) {
            std::cout << std::setw(5)  << "\\n";
        } else if( node -> value == 0 ) {
            std::cout << std::setw(5)  << " ";
        } else {
            std::cout << std::setw(5)  << (char) node -> value;
        }        

        std::cout << std::setw(20) << code << std::endl;

        // Add symbol and code to code table
        codes[node -> value] = code;
    } else {
        printPrefix( node -> left, code + "0" );
        printPrefix( node -> right, code + "1" );
    }
}

/** 
 * printHuffmanTree()
 *
 * Overload print function for Huffman Tree
 */
void HuffmanTree::printHuffmanTree( BitIO &writer ) {
    // Start printing at root node
    printHuffmanTree( root, writer );
}

/** 
 * printHuffmanTree()
 *
 * Recursilvely prints Huffman Tree
 */
void HuffmanTree::printHuffmanTree( Node *node, BitIO &writer ) {
    if( node -> left == NULL || node -> right == NULL ) {
        // Write 1 and symbol to output filename
        writer.writeBit( (char) 1 );
        writer.writeSymbol( (char) ( node -> value ) );
    } else {
        // Write 0 to output file
        writer.writeBit( (char) 0 );

        // Recurse down Huffman Tree
        printHuffmanTree( node -> left, writer );
        printHuffmanTree( node -> right, writer );
    }
}

/** 
 * decodeHuffmanTree()
 *
 * Converts Huffman Tree string into actual Huffman Tree
 */
Node* HuffmanTree::decodeHuffmanTree( BitIO &reader, int &numChars ) {
    // Function variables
    char bit;

    // Read next bit in file
    bit = reader.readBit();

    // Create new node
    Node *n = new Node();

    // If bit is 1, we have a leaf node
    if( bit == 0x01 ) {
        // Set symbol
        n -> value = (int) reader.readSymbol();

        // Set child nodes
        n -> left  = NULL;
        n -> right = NULL;

        // Other node attributes
        n -> frequency = 0;

        // Decrement number of characters left
        --numChars;
    } else if( bit == 0x00 ) {
        // Set node attributes
        n -> value     = 0;
        n -> frequency = 0;

        // If bit is 0, decode left and right nodes
        n -> left  = decodeHuffmanTree( reader, numChars );
        n -> right = decodeHuffmanTree( reader, numChars );
    } else {
        std::cout << "Something bad is happening ..." << std::endl;
    }

    // Set root node of Huffman Tree
    return n;
}