#
# Makefile
# build geometric_transform
#

CC=g++
SRC=histogram.cpp
OBJ=histogram
transform: $(SRC)
		$(CC) `pkg-config --cflags --libs opencv` $(SRC) -o $(OBJ)
        #-mmacosx-version-min=10.8
clean: histogram
	rm $(OBJ)
