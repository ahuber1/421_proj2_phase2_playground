# List of files
SOURCE_CODE = main.c \
	PSJF_Linked_List.c \
	PSJF_AVL_Tree.c

EXECUTABLE = a.out

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE_CODE)
	gcc -Wall $(SOURCE_CODE) -o $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f ./$(EXECUTABLE)
	rm -f ./*~
