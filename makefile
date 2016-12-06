# List of files
SOURCE_CODE = main.c \
	PSJF_Linked_List.c \
	PSJF_AVL_Tree.c \
	PSJF_Shared.c \
	PSJF_Dictionary.c

EXECUTABLE = a.out
OUTPUT = output.txt

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE_CODE)
	gcc -Wall $(SOURCE_CODE) -o $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE) > $(OUTPUT)

open: $(OUTPUT)
	atom $(OUTPUT)

clean:
	rm -f ./$(EXECUTABLE)
	rm -f ./*~
	rm -f $(OUTPUT)
