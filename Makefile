CC = clang
CFLAGS = -std=c99 -m64 -g -DDEBUG -DENABLE_TRACE

INCLUDES = -Iruntime/src -Itestbed/src
LIBS = -lvulkan -lglfw -lshaderc_shared

BIN = build/bin
OBJ = build/obj

RUNTIME_SRC = $(shell find runtime/src -name '*.c')
TESTBED_SRC = $(shell find testbed/src -name '*.c')

RUNTIME_OBJ = $(patsubst %.c,$(OBJ)/%.o,$(RUNTIME_SRC))
TESTBED_OBJ = $(patsubst %.c,$(OBJ)/%.o,$(TESTBED_SRC))

all: $(BIN)/testbed
rebuild: clean all
clean: 
	@rm -rf build
run: all
	@cd $(BIN) && ./testbed

$(BIN)/libRuntime.a: $(RUNTIME_OBJ)
	@mkdir -p $(@D)
	ar rcs $@ $^

$(BIN)/testbed: $(TESTBED_OBJ) $(BIN)/libRuntime.a
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJ)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# These are commands, not files.
.PHONY: all clean rebuild run
