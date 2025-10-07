CC = gcc
ASAN_FLAGS =
ifeq ($(ASAN),1)
    ASAN_FLAGS = -fsanitize=address -fsanitize=undefined -fsanitize-trap=all
else ifeq ($(filter asan,$(MAKECMDGOALS)),asan)
    ASAN_FLAGS = -fsanitize=address -fsanitize=undefined -fsanitize-trap=all
endif

SRCDIR = src
INCDIR = include
ASSETDIR = assets/components
OBJDIR = obj
BIN = nukleon_server
DEFINES ?=
USE_UI ?=
C_FILES = server_main.c $(wildcard $(SRCDIR)/*.c)
OBJ_FILES = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(C_FILES)))

DEF_FILES = $(wildcard $(ASSETDIR)/*.def)

CFLAGS = -std=c17 -D_GNU_SOURCE -Wall -Wextra -Wpedantic -Wunused -Wcast-align -Wno-unused-function -Wshadow -Ofast \
         $(ASAN_FLAGS) $(DEFINES) -I$(INCDIR) -I$(ASSETDIR)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    PLATFORM_LDFLAGS = -lrt
    ifneq ($(strip $(USE_UI)),)
        PLATFORM_LDFLAGS += -lGLU -lGL -lX11
		CFLAGS += -DNK_ENABLE_UI_SAMPLER
		C_FILES += tigr.c
    endif
else
    PLATFORM_LDFLAGS =
    ifneq ($(strip $(USE_UI)),)
        PLATFORM_LDFLAGS += -lopengl32 -lgdi32
		CFLAGS += -DNK_ENABLE_UI_SAMPLER
		C_FILES += tigr.c
	endif
endif

LDFLAGS = $(ASAN_FLAGS) $(PLATFORM_LDFLAGS)

all: $(BIN)
	@echo Linker Flags: $(PLATFORM_LDFLAGS)

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