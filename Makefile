CC ?= cc
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c99 -Wall -Wextra -Werror -pedantic

BUILD_DIR := build
TEST_BIN := $(BUILD_DIR)/test_api_compile
SRC := src/ppg_ibi.c
TEST_SRC := tests/test_api_compile.c
PUBLIC_HEADERS := include/ppg_ibi.h include/ppg_ibi_config.h
INTERNAL_HEADERS := src/ppg_ibi_internal.h

.PHONY: all test check-no-dynamic clean

all: $(TEST_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TEST_BIN): $(SRC) $(TEST_SRC) $(PUBLIC_HEADERS) $(INTERNAL_HEADERS) | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) $(TEST_SRC) -o $(TEST_BIN)

test: $(TEST_BIN) check-no-dynamic
	./$(TEST_BIN)

check-no-dynamic:
	! grep -R -n -E '\b(malloc|calloc|realloc)[[:space:]]*\(' include src tests/test_api_compile.c

clean:
	rm -rf $(BUILD_DIR)
