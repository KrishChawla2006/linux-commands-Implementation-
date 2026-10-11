# CC = gcc
# CFLAGS = -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE
# BIN = bin

# COMMANDS = my_echo my_cat my_wc my_cp my_ls my_chmod

# all: $(COMMANDS)

# $(BIN):
# 	mkdir -p $(BIN)

# my_echo: src/my_echo.c | $(BIN)
# 	$(CC) $(CFLAGS) $< -o $(BIN)/$@

# my_cat: src/my_cat.c | $(BIN)
# 	$(CC) $(CFLAGS) $< -o $(BIN)/$@

# my_wc: src/my_wc.c | $(BIN)
# 	$(CC) $(CFLAGS) $< -o $(BIN)/$@

# my_cp: src/my_cp.c | $(BIN)
# 	$(CC) $(CFLAGS) $< -o $(BIN)/$@

# my_ls: src/my_ls.c | $(BIN)
# 	$(CC) $(CFLAGS) $< -o $(BIN)/$@

# my_chmod: src/my_chmod.c | $(BIN)
# 	$(CC) $(CFLAGS) $< -o $(BIN)/$@

# clean:
# 	rm -rf $(BIN)

# .PHONY: all clean


CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

BIN = bin

COMMANDS = my_echo my_cat my_wc my_cp my_ls my_chmod my_pwd my_date my_grep

all: $(addprefix $(BIN)/,$(COMMANDS)) $(BIN)/minishell

$(BIN):
	mkdir -p $(BIN)

$(BIN)/my_echo: src/my_echo.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

$(BIN)/my_cat: src/my_cat.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

$(BIN)/my_wc: src/my_wc.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

$(BIN)/my_cp: src/my_cp.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

$(BIN)/my_ls: src/my_ls.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

$(BIN)/my_chmod: src/my_chmod.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

$(BIN)/my_pwd: src/my_pwd.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

$(BIN)/my_date: src/my_date.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

$(BIN)/my_grep: src/my_grep.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

$(BIN)/minishell: minishell/main.c | $(BIN)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(addprefix $(BIN)/,$(COMMANDS)) $(BIN)/minishell

.PHONY: all clean
