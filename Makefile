TARGET :=	klib.a
TESTS :=	tlv_test
HEADERS :=	$(wildcard *.h)
SOURCES :=	$(wildcard *.cc)
OBJS :=		Arena.o Phonebook.o TLV.o

CXX :=		clang++
CXXFLAGS :=	-g -std=c++14 -Werror -Wall

.PHONY: all 
all: $(TARGET) $(TESTS) tags run-tests

tags: $(HEADERS) $(SOURCES)
	ctags $(HEADERS) $(SOURCES)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $(OBJS)

tlv_test: tlv_test.o $(TARGET)
	$(CXX) -o $@ $(CXXFLAGS) $@.o $(TARGET)

.PHONY: print-%
print-%: ; @echo '$(subst ','\'',$*=$($*))'

klib.a: $(OBJS)

%.o: %.cc
	$(CXX) -o $@ -c $(CXXFLAGS) $<

.PHONY: clean
clean:
	# build outputs
	rm -f $(TARGET) $(TESTS) *.o 

	# test miscellaneous
	rm -f core core.* tags arena_test.bin

.PHONY: run-tests
run-tests: $(TESTS)
	for testbin in $(TESTS); 		\
	do	 				\
		echo "./$${testbin}" ;		\
		./$${testbin}; 			\
	done
