CC		:= g++
CFLAGS	:= -std=c++17 -Wall -Wextra -g -lncurses

BIN		:= bin
SRC		:= src
TEST    := test
INCLUDE	:= include

DEV_IMAGE=brink
DEV_CONTAINER=$(DEV_IMAGE)-dev

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= brink.exe
SOURCEDIRS	:= $(SRC)
else
EXECUTABLE	:= brink
SOURCEDIRS	:= $(shell find $(SRC) -type d)
# TESTDIRS	:= $(shell find $(TEST) -type d)
endif

SOURCES		:= $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS)))
# TESTS       := ./test
OBJECTS		:= $(SOURCES:.cpp=.o)

all: $(BIN)/$(EXECUTABLE)

.PHONY: clean
clean:
	-$(RM) $(BIN)/$(EXECUTABLE)
	-$(RM) $(OBJECTS)

.PHONY: clean-dev
clean-dev:
	-docker kill $(DEV_CONTAINER)
	-docker rm $(DEV_CONTAINER)
	-docker rmi ($DEV_IMAGE)

.PHONY: dev
dev:
ifeq (, $(shell which docker-compose 2> /dev/null))
	$(error docker-compose not in $(PATH), make sure both docker and docker-compose are installed)
else
	@docker-compose run brink bash
endif

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ 