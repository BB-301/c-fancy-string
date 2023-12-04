CC = gcc
ARCHIVER = ar

BUILD_DIR = build
EXAMPLES_BUILD_DIR = build-examples
DOCS_BUILD_DIR = build-doxygen
SOURCE_DIR = src
INCLUDE_DIR = include
TEST_DIR = test
EXAMPLES_DIR = examples

DOCKER_DOXYGEN_IMAGE_NAME=my_local_images/doxygen

LIB_VERSION = $(shell cat VERSION)
CURRENT_DIRECTORY = $(shell pwd)

INSTALL_PATH_MAN = /usr/local/man/man3
INSTALL_PATH_LIB = /usr/local/lib
INSTALL_PATH_INCLUDE = /usr/local/include

LIB_NAME = fancy_string
LIB_FULL_NAME = fancy_string.$(LIB_VERSION)

C_VERSION = c17
OPTIMIZATION_LEVEL = -O0

# Other flags to consider: -g (for debugging); -Wextra (I should use that as well)
# review whether `-fPIC` is properly used here

CFLAGS_LIB = $(OPTIMIZATION_LEVEL) \
	-std=$(C_VERSION) \
	-fpic \
	-Wall -Werror -Wextra -pedantic \
	-I./$(INCLUDE_DIR) -I./c-fancy-memory/include \
	-DFANCY_STRING_MEMORY_USAGE_FEATURE_ENABLED=1
CFLAGS_TEST = $(OPTIMIZATION_LEVEL) \
	-std=$(C_VERSION) \
	-Wall -Werror -Wextra -pedantic \
	-I./$(INCLUDE_DIR) -I./c-fancy-memory/include
CFLAGS_EXAMPLES = $(CFLAGS_TEST)
ARCHIVER_FLAGS = rcs

EXAMPLES_SOURCES = $(shell find $(EXAMPLES_DIR) -name "*.c" -type f)
EXAMPLES_SOURCES_NAMES = $(notdir $(EXAMPLES_SOURCES))

EXAMPLES_OBJECTS_NAMES = $(EXAMPLES_SOURCES_NAMES:.c=.o)
EXAMPLES_OBJECTS = $(addprefix $(EXAMPLES_BUILD_DIR)/, $(EXAMPLES_OBJECTS_NAMES))

EXAMPLES_BINARIES_NAMES = $(EXAMPLES_SOURCES_NAMES:.c=.bin)
EXAMPLES_BINARIES = $(addprefix $(EXAMPLES_BUILD_DIR)/, $(EXAMPLES_BINARIES_NAMES))

EXAMPLES_NAMES = $(EXAMPLES_SOURCES_NAMES:.c=)

.DEFAULT_GOAL := help

# =======================================
#               EXAMPLES
# =======================================

# [How to](https://stackoverflow.com/questions/15167766/makefile-dynamic-rules-w-no-gnu-make-pattern)
# Potential improvement: Could prefix each rule by `example_`. This would avoid conflicts
# if creating an example file that has a name that matches any of the recipes declare here. Ok
# for now, but to be kept in mind.

define ruletemp
$(patsubst %.c, $(EXAMPLES_BUILD_DIR)/%.o, $(notdir $(1))): $(1) $(EXAMPLES_BUILD_DIR)
	$$(CC) $$(CFLAGS_EXAMPLES) -c $$< -o $$@
endef

$(foreach src,$(EXAMPLES_SOURCES),$(eval $(call ruletemp, $(src))))

define ruletemp
$(patsubst %.o, $(EXAMPLES_BUILD_DIR)/%.bin, $(notdir $(1))): $(1) c_fancy_memory_object library_object
	$$(CC) $$(BUILD_DIR)/$$(LIB_FULL_NAME).o $$(BUILD_DIR)/fancy_memory.o $$< -o $$@
endef

$(foreach src,$(EXAMPLES_OBJECTS),$(eval $(call ruletemp, $(src))))

define ruletemp
$(patsubst %.bin, %, $(notdir $(1))): $(1)
	@echo "\nRunning '$(strip $1)' ...\n"
	@./$(strip $1)
	@echo ""
endef

$(foreach src,$(EXAMPLES_BINARIES),$(eval $(call ruletemp, $(src))))

# =======================================
#                LIBRARY
# =======================================

c_fancy_memory_object: \
	$(BUILD_DIR) \
	./c-fancy-memory/src/fancy_memory.c \
	./c-fancy-memory/include/fancy_memory.h
	$(CC) $(CFLAGS_LIB) -c ./c-fancy-memory/src/fancy_memory.c -o $(BUILD_DIR)/fancy_memory.o

library_object: \
	$(BUILD_DIR) \
	$(INCLUDE_DIR)/$(LIB_NAME).h \
	./c-fancy-memory/include/fancy_memory.h \
	$(SOURCE_DIR)/$(LIB_NAME).c
	$(CC) $(CFLAGS_LIB) -c $(SOURCE_DIR)/$(LIB_NAME).c -o $(BUILD_DIR)/$(LIB_FULL_NAME).o

library: c_fancy_memory_object library_object
	$(CC) -shared $(BUILD_DIR)/$(LIB_FULL_NAME).o $(BUILD_DIR)/fancy_memory.o -o $(BUILD_DIR)/lib$(LIB_FULL_NAME).so
	ln -sf $(BUILD_DIR)/lib$(LIB_FULL_NAME).so $(BUILD_DIR)/lib$(LIB_NAME).so
	$(ARCHIVER) $(ARCHIVER_FLAGS) $(BUILD_DIR)/lib$(LIB_FULL_NAME).a $(BUILD_DIR)/$(LIB_FULL_NAME).o $(BUILD_DIR)/fancy_memory.o
	ln -sf $(BUILD_DIR)/lib$(LIB_FULL_NAME).a $(BUILD_DIR)/lib$(LIB_NAME).a

# NOTE ABOUT USING SUDO
# Putting sudo here avoids that the library files in the build directory
# be owned by the `root` user.
install: library
	sudo cp $(BUILD_DIR)/lib$(LIB_FULL_NAME).a $(INSTALL_PATH_LIB)/lib$(LIB_FULL_NAME).a
	sudo ln -sf $(INSTALL_PATH_LIB)/lib$(LIB_FULL_NAME).a $(INSTALL_PATH_LIB)/lib$(LIB_NAME).a
	sudo cp $(BUILD_DIR)/lib$(LIB_FULL_NAME).so $(INSTALL_PATH_LIB)/lib$(LIB_FULL_NAME).so
	sudo ln -sf $(INSTALL_PATH_LIB)/lib$(LIB_FULL_NAME).so $(INSTALL_PATH_LIB)/lib$(LIB_NAME).so
	sudo cp $(INCLUDE_DIR)/$(LIB_NAME).h $(INSTALL_PATH_INCLUDE)/$(LIB_NAME).h

install_with_docs: docs install
	sudo cp $(DOCS_BUILD_DIR)/man/man3/$(LIB_NAME).h.3 $(INSTALL_PATH_MAN)/$(LIB_NAME).h.3

# NOTE ABOUT USING SUDO
# Unlike "install", sudo could be passed externally here (i.e. sudo make uninstall),
# but using it here makes it more consistent.
# And the first line with 'echo' is there because we use `-f` to rm, so
# if the password is wrong on the first file, make will ask again, and if the
# password is subsequently correct, the first file will not be deleted. So here,
# 'echo' will fail with a bad password and make will exit.
# TO DO: Test this again to make sure it's true
uninstall:
	@sudo echo "THANK YOU"
	sudo rm -f $(INSTALL_PATH_LIB)/lib$(LIB_NAME).a
	sudo rm -f $(INSTALL_PATH_LIB)/lib$(LIB_FULL_NAME).a
	sudo rm -f $(INSTALL_PATH_LIB)/lib$(LIB_NAME).so
	sudo rm -f $(INSTALL_PATH_LIB)/lib$(LIB_FULL_NAME).so
	sudo rm -f $(INSTALL_PATH_INCLUDE)/$(LIB_NAME).h
	sudo rm -f $(INSTALL_PATH_MAN)/$(LIB_NAME).h.3


# =======================================
#             UNIT TESTING 
# =======================================

test_object: \
	library_object \
	$(TEST_DIR)/main.c
	$(CC) $(CFLAGS_TEST) \
		-DFANCY_STRING_TEST_LOOP_ENABLED=0 \
		-c $(TEST_DIR)/main.c -o $(BUILD_DIR)/test.o

build_test: c_fancy_memory_object test_object
	$(CC) $(BUILD_DIR)/test.o $(BUILD_DIR)/fancy_memory.o $(BUILD_DIR)/$(LIB_FULL_NAME).o -o $(BUILD_DIR)/test
	
test: build_test
	./$(BUILD_DIR)/test

test_object_with_library: library_object test_object

build_test_shared: test_object_with_library
	$(CC) $(BUILD_DIR)/test.o -l$(LIB_FULL_NAME) -L./$(BUILD_DIR) -I./$(INCLUDE) -o $(BUILD_DIR)/test_shared
	
test_shared: build_test_shared
	./$(BUILD_DIR)/test_shared

# NOTE: Some compilers (like clang) will look for the dynamic version
# of the library first and link dynamically if such a library exists.
# So when both `.a` and `.so` are found in the same directory, clang
# will use the dynamic version and I don't think there is an option
# to tell it otherwise. In such instance, one is therefore left with
# the following two options if static linking is what is needed: (1)
# make sure there is only a static library at the pointed path, or (2)
# specify the library file directly (as done here) instead of using the `-l`
# flag syntax.
build_test_static: test_object_with_library
	$(CC) $(BUILD_DIR)/test.o $(BUILD_DIR)/lib$(LIB_FULL_NAME).a -I./$(INCLUDE) -o $(BUILD_DIR)/test_static

test_static: build_test_static	
	./$(BUILD_DIR)/test_static

test_object_integration: \
	library_object \
	$(TEST_DIR)/main.c
	$(CC) $(CFLAGS_TEST) \
		-DFANCY_STRING_TEST_LOOP_ENABLED=1 \
		-c $(TEST_DIR)/main.c -o $(BUILD_DIR)/test_integration.o

build_test_integration: c_fancy_memory_object test_object_integration
	$(CC) $(BUILD_DIR)/test_integration.o $(BUILD_DIR)/fancy_memory.o $(BUILD_DIR)/$(LIB_FULL_NAME).o -o $(BUILD_DIR)/test_integration
	
test_integration: build_test_integration
	./$(BUILD_DIR)/test_integration

# =======================================
#                  MISC
# =======================================

$(BUILD_DIR):
	@if ! [ -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi;

$(EXAMPLES_BUILD_DIR):
	@if ! [ -d $(EXAMPLES_BUILD_DIR) ]; then mkdir $(EXAMPLES_BUILD_DIR); fi;

.PHONY: clean help examples docs docs_for_website

docs:
	docker build -f doxygen/Dockerfile -t $(DOCKER_DOXYGEN_IMAGE_NAME) .
	docker run \
    	--rm \
    	-v $(CURRENT_DIRECTORY):/my-dir \
    	-w /my-dir \
    	$(DOCKER_DOXYGEN_IMAGE_NAME) doxygen doxygen/Doxyfile

docs_for_website:
	docker build -f doxygen/Dockerfile -t $(DOCKER_DOXYGEN_IMAGE_NAME) .
	docker run \
    	--rm \
    	-v $(CURRENT_DIRECTORY):/my-dir \
		-v $(CURRENT_DIRECTORY)/../c-fancy-string-docs/docs/v$(LIB_VERSION):/my-dir/build-doxygen \
    	-w /my-dir \
    	$(DOCKER_DOXYGEN_IMAGE_NAME) doxygen doxygen/Doxyfile

examples:
	@echo "\nTo run the individual examples:"
	@for NAME in $(EXAMPLES_NAMES); do echo "\n- make $$NAME"; done;
	@echo ""

clean:
	rm -rf ./$(BUILD_DIR);
	rm -rf ./$(DOCS_BUILD_DIR);
	rm -rf ./$(EXAMPLES_BUILD_DIR);

help:
	@echo "\n- make library\n\tBuilds the library (both the shared and static versions)"
	@echo "\n- make install\n\tInstalls the library (note: this requires 'sudo' internally)"
	@echo "\n- make install_with_docs\n\tInstalls the library, including the man3 page (notes: this requires 'sudo' internally; will call docker to build the docs)"
	@echo "\n- make uninstall\n\tUninstalls the library (note: this requires 'sudo' internally)"
	@echo "\n- make test\n\tRuns the unit tests"
	@echo "\n- make test_integration\n\tRuns the unit tests inside a loop (can be used for memory leak detection)"
	@echo "\n- make docs\n\tBuilds a Docker container containing Doxygen and runs it to generate the Doxygen documentation website"
	@echo "\n- make docs_for_website\n\tBuilds the Doxygen website using Docker and outputs the result into '../c-fancy-string-docs/docs/v$(LIB_VERSION)'."
	@echo "\n- make examples\n\tPrints the list of examples that can be run using 'make <example_name>'"
	@echo "\n- make test_shared\n\tRuns the unit tests (using the shared library)"
	@echo "\n- make test_static\n\tRuns the unit tests (using the static library)"
	@echo "\n- make clean\n\tCleans up (i.e. deletes the '${BUILD_DIR}' directory)"
	@echo "\n- make help\n\tPrints this summary of the available recipes"
	@echo ""