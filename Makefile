#
# Makefile
# build geometric_transform
#

CC=g++
SRC=geometric_transform.cpp
transform: $(SRC)
		$(CC) `pkg-config --cflags --libs opencv` $(SRC) -o transform
        #-mmacosx-version-min=10.8
clean: transform
	rm transform
