OBJS_DIR = .objs
SRC_DIR  = src
APPS_DIR = apps
TEST_DIR = tests
LIBS_DIR = libs

LIBS           := dictionary graph set vector queue
APPS_SRC_FILES := $(wildcard $(APPS_DIR)/*.c)
TEST_SRC_FILES := $(wildcard $(TEST_DIR)/*.c)

EXE_APPS  := $(patsubst $(APPS_DIR)/%.c,%,$(APPS_SRC_FILES))
EXE_TESTS := $(patsubst $(TEST_DIR)/%.c,%,$(TEST_SRC_FILES))

# set up compiler and linker
CC = clang
LD = clang

# define all the compilation flags
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter -Wmissing-declarations -Wmissing-variable-declarations
INC = -I./includes/
CFLAGS_COMMON = $(WARNINGS) $(INC) -std=c99 -c -MMD -MP -D_GNU_SOURCE
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2
CFLAGS_DEBUG = $(CFLAGS_COMMON) -O0 -g -DDEBUG 

# Find object files for libraries
OBJS_SRC := $(patsubst $(SRC_DIR)/%.c,%.o,$(wildcard $(SRC_DIR)/*.c))

# Find the libs files to generate

OBJS_APPS = $(OBJS_SRC)
OBJS_TEST = $(OBJS_SRC)

.PHONY: all
all: release

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(LIBS_DIR): 
	@mkdir -p $(LIBS_DIR)

.PHONY: print 
print:
	echo $(OBJS_SRC)

# build types
.PHONY: release
.PHONY: debug
.PHONY: test

release: $(LIBS_DIR) $(LIBS:%=$(LIBS_DIR)/lib%.a)
main:    $(EXE_MAIN)
debug:   clean $(EXE_MAIN)-debug
test: 	 $(EXE_TESTS)

# include dependencies
-include $(OBJS_DIR)/*.d

################################################################################
#                          Patterns to Create Objects                          #
# keep debug & release postfix for object files so we can always separate them #
################################################################################
$(OBJS_DIR)/%-debug.o: $(SRC_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

$(OBJS_DIR)/%-release.o: $(SRC_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# Define rules to compile object files for apps
$(OBJS_DIR)/%-debug.o: $(APPS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

$(OBJS_DIR)/%-release.o: $(APPS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# Define rules to compile object files for tests
$(OBJS_DIR)/%-debug.o: $(TEST_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

################################################################################
#                          Rules to Link Executables                           #
################################################################################
# Rules to link test executables
$(EXE_APPS): %: $(OBJS_DIR)/%-debug.o $(OBJS_APPS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ -o $(notdir $@)

# Rules to link test executables
$(EXE_TESTS): %: $(OBJS_DIR)/%-debug.o $(OBJS_TEST:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ -o $(notdir $@)

################################################################################
#                          Rules to Create Libraries                           #
################################################################################
LIB_DICT_OBJS = dictionary callbacks compare vector bitfield
$(LIBS_DIR)/libdictionary.a: $(LIB_DICT_OBJS:%=$(OBJS_DIR)/%-release.o)
	ar rcs $@ $^

LIB_GRAPH_OBJS = graph vector callbacks compare dictionary set
$(LIBS_DIR)/libgraph.a: $(LIB_GRAPH_OBJS:%=$(OBJS_DIR)/%-release.o)
	ar rcs $@ $^

LIB_SET_OBJS = set callbacks compare vector bitfield
$(LIBS_DIR)/libset.a: $(LIB_SET_OBJS:%=$(OBJS_DIR)/%-release.o)
	ar rcs $@ $^

LIB_VECTOR_OBJS = vector callbacks
$(LIBS_DIR)/libvector.a: $(LIB_VECTOR_OBJS:%=$(OBJS_DIR)/%-release.o)
	ar rcs $@ $^

LIB_QUEUE_OBJS = queue
$(LIBS_DIR)/libqueue.a: $(LIB_QUEUE_OBJS:%=$(OBJS_DIR)/%-release.o)
	ar rcs $@ $^

LIB_DISJOINT_SETS_OBJS = disjoint_sets
$(LIBS_DIR)/libdisjointsets.a: $(LIB_DISJOINT_SETS_OBJS:%=$(OBJS_DIR)/%-release.o)
	ar rcs $@ $^

LIB_HEAP_OBJS = heap vector
$(LIBS_DIR)/libheap.a: $(LIB_HEAP_OBJS:%=$(OBJS_DIR)/%-release.o)
	ar rcs $@ $^

################################################################################
#                              Bash Command Rules                              #
################################################################################
.PHONY: clean
clean:
	rm -rf .objs $(EXE_TESTS) $(EXE_APPS)
