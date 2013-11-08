/** 
 * bitIO.hh
 *
 * Class Definitions
 */

#ifndef BITIO_HH
#define BITIO_HH

// Include libraries
#include <iostream>
#include <fstream>

class BitIO {
    public:
        BitIO();                                                    // Default constructor
        BitIO( std::ifstream &input );                              // Reader object
        BitIO( std::ofstream &output );                             // Writer object

        int  getNumBits();                                          // Returns number of bits currently in buffer
        void reset();                                               // Resets buffer to zero

        void writeSymbol( char character );                         // Writes a char to file
        void writeBit( char bit );                                  // Writes a binary bit to file

        int  pad();                                                 // Checks if latest buffer needs padding

        char readSymbol();                                          // Returns next 8 bits as char
        char readBit();                                             // Returns next bit as char (0/1)
        char peek();                                                // Returns next 8 bits and decremnts file pointer

    private:
        char byte;                                                  // Buffer
        int  numBits;                                               // Number of bits in buffer
        std::ifstream *input;                                       // Input file stream
        std::ofstream *output;                                      // Output file stream
};

#endif