OBJS_DIR = .objs
SRC_DIR  = src
APPS_DIR = apps
TEST_DIR = tests

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
CFLAGS_DEBUG = $(CFLAGS_COMMON) -O0 -g -DDEBUG -pg

# Find object files for libraries
OBJS_SRC := $(patsubst $(SRC_DIR)/%.c,%.o,$(wildcard $(SRC_DIR)/*.c))

OBJS_APPS   = $(OBJS_SRC) 
OBJS_TEST   = $(OBJS_SRC)

.PHONY: all
all: release

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

.PHONY: print 
print:
	echo $(OBJS_SRC)

# build types
.PHONY: release
.PHONY: debug
.PHONY: test

release: $(EXE_TESTS) $(EXE_MAIN)
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
$(EXE_MAIN): $(OBJS_MAIN:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ -o $@

# Rules to link test executables
$(EXE_APPS): %: $(OBJS_DIR)/%-debug.o $(OBJS_:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ -o $(notdir $@)

# Rules to link test executables
$(EXE_TESTS): %: $(OBJS_DIR)/%-debug.o $(OBJS_TEST:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ -o $(notdir $@)

################################################################################
#                          Rules to Create Libraries                           #
################################################################################


################################################################################
#                              Bash Command Rules                              #
################################################################################
.PHONY: clean
clean:
	rm -rf .objs $(EXE_TESTS) $(EXE_CLIENT) $(EXE_SERVER) $(EXE_CLIENT)-debug $(EXE_SERVER)-debug $(EXE_MAIN)

build:
	docker build -t neilk3/linux-dev-env .

build-release:
	docker build -t neilk3/web-server -f Dockerfile.release .

start: build
	docker run -it --rm -p 80:8000 -v `pwd`:/mount neilk3/linux-dev-env

start-release: build-release
	docker run -it --rm -p 80:8000 -v `pwd`/favicon.png:/service/favicon.png neilk3/web-server

trace: debug
	sudo dtrace -s timing.d -c "./server-debug 80"

valgrind: server-debug
	valgrind --leak-check=full --show-leak-kinds=all ./server-debug 8000

leaks: server-debug
	export MallocStackLogging=1 && ./server-debug 80