HEADERS :=	scsl.h				\
		Arena.h				\
		Buffer.h			\
		Commander.h			\
		Dictionary.h			\
		Exceptions.h			\
		Flag.h				\
		StringUtil.h			\
		Test.h				\
		TLV.h				\
		WinHelpers.h

SOURCES :=	Arena.cc			\
		Buffer.cc			\
		Commander.cc			\
		Dictionary.cc			\
		Exceptions.cc			\
		Flag.cc				\
		StringUtil.cc			\
		Test.cc				\
		TLV.cc				\
		WinHelpers.cc

BUILD :=	DEBUG
OBJS :=		$(patsubst %.cc,%.o,$(SOURCES))
LIBS :=		libscsl.a

TARGETS :=	$(LIBS) phonebook
TESTS :=	bufferTest dictionaryTest flagTest tlvTest
CXX :=		clang++
CXXFLAGS :=	-std=c++14 -Werror -Wall -Wextra -DSCSL_DESKTOP_BUILD	\
			-DSCSL_BUILD_TYPE=${BUILD}
ifeq ($(BUILD),DEBUG)
CXXFLAGS +=	-g -fsanitize=address
else
CXXFLAGS +=	-O2
endif

.PHONY: all 
all: $(TARGETS) $(TESTS) tags run-tests

tags: $(HEADERS) $(SOURCES)
	ctags $(HEADERS) $(SOURCES)

libscsl.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

.PHONY: print-%
print-%: ; @echo '$(subst ','\'',$*=$($*))'

klib.a: $(OBJS)

.PHONY: clean
clean:
	# build outputs
	rm -f $(TARGETS) $(TESTS) *.o

	# test miscellaneous
	rm -f core core.* tags arena_test.bin

.PHONY: run-tests
run-tests: $(TESTS)
	for testbin in $(TESTS); 		\
	do	 				\
		echo "./$${testbin}" ;		\
		./$${testbin}; 			\
	done

phonebook: phonebook.o $(LIBS)
	$(CXX) -o $@ $(CXXFLAGS) $@.o $(LIBS)

bufferTest: bufferTest.o $(LIBS)
	$(CXX) -o $@ $(CXXFLAGS) $@.o $(LIBS)

dictionaryTest: dictionaryTest.o $(LIBS)
	$(CXX) -o $@ $(CXXFLAGS) $@.o $(LIBS)

flagTest: flagTest.o $(LIBS)
	$(CXX) -o $@ $(CXXFLAGS) $@.o $(LIBS)

tlvTest: tlvTest.o $(LIBS)
	$(CXX) -o $@ $(CXXFLAGS) $@.o $(LIBS)

%.o: %.cc
	$(CXX) -o $@ -c $(CXXFLAGS) $<

