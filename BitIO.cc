/** 
 * bitIO.cc
 *
 * Provides methods to write data to files
 */

// Include header file
#include "BitIO.hh"

/** 
 * BitIO()
 *
 * Default constructor
 */
BitIO::BitIO() {
    // Set default members
    byte    = 0;
    numBits = 0;
    input   = NULL;
    output  = NULL;
}

/** 
 * BitIO()
 *
 * Reader constructor
 */
BitIO::BitIO( std::ifstream &input ) {
    // Set default members
    byte    = 0;
    numBits = 0;
    output  = NULL;

    // Set input file stream
    this -> input   = &input;
}

/** 
 * BitIO()
 *
 * Writer constructor
 */
BitIO::BitIO( std::ofstream &output ) {
    // Set default members
    byte    = 0;
    numBits = 0;
    input   = NULL;

    // Set output file stream
    this -> output  = &output;
}

/** 
 * getNumBits()
 *
 * Returns number of bits currently in buffer
 * 0 <= n <= 7
 */
int BitIO::getNumBits() {
    return numBits;
}

/** 
 * reset()
 *
 * Resets buffer to zero
 */
void BitIO::reset() {
    byte    = 0;
    numBits = 0;
}

/** 
 * writeSymbol()
 *
 * Main function to write a char to a file
 * Passes a binary bit to writeBit()
 */
void BitIO::writeSymbol( char character ) {
    // Function variables
    unsigned char bit  = 0, 
                  mask = 1 << 7;

    // Need to pass character bitwise

    // Length of char is 8 bits
    for( int i = 0; i < 8; i++ ) {
        bit = 0;

        if( character & mask ) {
            bit = 1;
        }

        // Send to writeBit()
        writeBit( bit );

        // Bit shift!
        character <<= 1;
    }
}

/** 
 * writeBit()
 *
 * Subroutine to actually write bytes to file
 */
void BitIO::writeBit( char bit ) {
    // Shift byte to left by one
    byte <<= 1;

    // Swap the LSB for 1 if needed
    if( bit ) {
        byte |= 1;
    }

    // Incrememt counter
    numBits++;

    // Write byte to file if buffer is full
    //   number of bits in char == 8
    if( numBits == 8 ) {
        output -> put( byte );

        // Reset buffer
        reset();
    }
}

/** 
 * pad()
 *
 * Checks if buffer needs padding and pads
 * appropriately. Returns amount of padding
 * added
 */
int BitIO::pad() {
    int temp = 0;

    // If previous call to writeBit() wrote a byte
    // then numBits is reset to zero
    if( numBits != 0 ) {
        // Update temp
        temp = 8 - numBits;

        // Shift byte buffer
        byte <<= temp;

        // Write buffer to file
        output -> put( byte );

        // Reset buffer
        reset();
    }

    return temp;
}

/** 
 * readByte()
 *
 * Reads the next eight bits in input file
 * and returns a character
 */
char BitIO::readSymbol() {
    // Function variables
    char bit, bits = 0;

    // Loop through gathering 8 bits
    for( int i = 0; i < 8; i++ ) {
        // Read one bit
        bit = readBit();

        // Bit shift!
        bits <<= 1;

        // Append new bit
        bits |= bit;
    }

    // Return 8 bits
    return bits;
}

/** 
 * readBit()
 *
 * Reads the next bit in the input file
 * and returns a character (0x00 or 0x01)
 */
char BitIO::readBit() {
    // Function variables
             char bit = 0;
    unsigned char mask = 1 << 7;

    // If there are no bits in the buffer
    // grab the next 8 bits (char)
    if( !( input -> eof() ) && numBits == 0 ) {
        input -> get( byte );
        numBits = 8;
    }

    // If the most significant bit is a 1
    // set it to the return variable
    if( byte & mask ) {
        bit |= 1;
    }

    // Bit shift!
    byte <<= 1;

    // Decrement number of bits in buffer
    --numBits;

    // Return 1 bit
    return bit;
}

/** 
 * peek()
 *
 * Reads next 8 bits in input file
 * and returns a character
 * Decrements file position pointer
 */
char BitIO::peek() {
    // Store current file pointer position
    int pos = input -> tellg();

    // Cache contents of buffer
    char bufferCache = byte;

    // Cache next 8 bits
    char bits = readSymbol();

    // If we needed to grab next 8 bits, rewind
    if( pos < input -> tellg() ) {
        // Rewind pointer
        input -> unget();
    }

    // Replace buffer with old contents
    byte = bufferCache;

    // Return character
    return bits;
}