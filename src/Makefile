MAKEFLAGS += -s # silent mode
FLAGS = -Wall -Wextra -Werror -std=c11
TEST_FLAGS = -lcheck -lm -pthread

SRC = $(shell find . -mindepth 2 -name "*.c")
OBJ = $(SRC:.c=.o)
LIB_NAME = s21_decimal.a
TEST_DIR = ./test
REPORT_DIR = ./report

OS := $(shell uname -s)
OS_RELEASE := $(shell cat /etc/os-release 2>/dev/null | grep '^ID=' | cut -d= -f2)

ifeq ($(OS), Linux)
	TEST_FLAGS += -lgcov -lrt
	ifneq ($(OS_RELEASE), arch)
		TEST_FLAGS += -lsubunit
	endif
endif

# List of targets from the task: all, clean, test, s21_decimal.a, gcov_report

all: $(LIB_NAME) test

$(LIB_NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	gcc $(FLAGS) -c $< -o $@

test: $(LIB_NAME) # generate, compile and run tests
	checkmk $(TEST_DIR)/*.check > $(TEST_DIR)/test.c
	gcc $(FLAGS) -o $(TEST_DIR)/test.o -c $(TEST_DIR)/test.c
	gcc $(FLAGS) -o $(TEST_DIR)/test $(TEST_DIR)/test.o $(LIB_NAME) $(TEST_FLAGS)
	$(TEST_DIR)/$@

gcov_report: checkmk $(TEST_DIR)/test.c # generate coverage report
	gcc $(FLAGS) -fprofile-arcs -ftest-coverage $(SRC) -o $(TEST_DIR)/test.out $(TEST_FLAGS)
	$(TEST_DIR)/test.out
	lcov -t "Report" -c -d $(TEST_DIR) -o $(TEST_DIR)/coverage.info $(addprefix --include=, $(SRC))
	genhtml $(TEST_DIR)/coverage.info --output-directory $(REPORT_DIR)

c: clean # shortcut
clean:
	rm -rf $(LIB_NAME)
	find . -mindepth 2 -name "*.o" -delete
	find $(TEST_DIR) -type f -not -name "*.check" -not -name "*.h" -delete
	rm -rf *.a *.out *.info
	rm -rf report/