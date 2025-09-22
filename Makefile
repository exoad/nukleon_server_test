CC = gcc
ASAN_FLAGS =
ifeq ($(ASAN),1)
    ASAN_FLAGS = -fsanitize=address -fsanitize=undefined -fsanitize-trap=all
else ifeq ($(filter asan,$(MAKECMDGOALS)),asan)
    ASAN_FLAGS = -fsanitize=address -fsanitize=undefined -fsanitize-trap=all
endif

SRCDIR   = src
INCDIR   = include
ASSETDIR = assets/components
OBJDIR   = obj
BIN      = nukleon_server

C_FILES = $(wildcard *.c) $(wildcard $(SRCDIR)/*.c)
OBJ_FILES = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(C_FILES)))

DEF_FILES = $(wildcard $(ASSETDIR)/*.def)

CFLAGS = -Wall -Wextra -Wpedantic -Wunused -Wcast-align -Wno-unused-function -Wshadow -O3 \
         $(ASAN_FLAGS) -I$(INCDIR) -I$(ASSETDIR)
LDFLAGS = $(ASAN_FLAGS) -lrt

all: $(BIN)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BIN): $(OBJ_FILES) | $(OBJDIR)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c $(DEF_FILES) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEF_FILES) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(BIN)

re: fclean all

asan:
	@$(MAKE) all ASAN=1

.PHONY: all clean fclean re asan