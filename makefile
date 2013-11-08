#
# file: Makefile
# 
# Compiles Huffman Tree program files

# Define compiler
CXX=g++

# Program names
en=encode
de=decode

# Program files
clSRC=HuffmanTree.cc PriorityQueue.cc Node.cc BitIO.cc
enSRC=encode.cc
deSRC=decode.cc

# Object files
clOBJ=$(clSRC:.cc=.o)
enOBJ=$(enSRC:.cc=.o)
deOBJ=$(deSRC:.cc=.o)

# Compile all files
all: $(clOBJ) $(enOBJ) $(deOBJ)
	$(CXX) $(clOBJ) $(enOBJ) -o $(en)
	$(CXX) $(clOBJ) $(deOBJ) -o $(de)

# Encode section
encode: $(clOBJ) $(enOBJ)
	$(CXX) $(clOBJ) $(enOBJ) -o $@

# Decode section
decode: $(clOBJ) $(deOBJ)
	$(CXX) $(clOBJ) $(deOBJ) -o $@

# Compile object files
%.o: %.cc
	$(CXX) -c $< -o $@

# Clean all files
clean:
	rm -f $(clOBJ) $(enOBJ) $(deOBJ) encode decode *.huf *.decoded.txt

# Clean object files
clean-objects:
	rm -f $(clOBJ) $(enOBJ) $(deOBJ)

# Clean encoded and decoded files
clean-files:
	rm -f *.huf *.decoded.txt
