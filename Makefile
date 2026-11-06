
# Makefile du projet de synthèse S3

# Configuration
.DEFAULT_GOAL := all

CC       = gcc

# FLAGS STANDARDS:
CFLAGS   ?= -std=c11 -Wall -Wextra -Wpedantic -O2 -g

# FLAGS À TESTER ABSOLUMENT POUR ÊTRE SÛR QUE TOUT COMPILE CHEZ LE PROF !
# CFLAGS   ?= -std=c11 -Wall -Wextra -Wpedantic -Werror


CPPFLAGS += -Iinclude
LDFLAGS  ?=
LDLIBS   += -lm

SRC_DIR   := src
BUILD_DIR := build

TEST_DIR       := tests
TEST_BUILD_DIR := $(BUILD_DIR)/tests

# Sources et objets
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

COMMON_OBJS := $(filter-out \
    $(BUILD_DIR)/main.o \
    $(BUILD_DIR)/expe.o, \
    $(OBJS))

# Tests
TEST_SRCS    := $(wildcard $(TEST_DIR)/*.c)
TEST_HEADERS := $(wildcard $(TEST_DIR)/*.h)
TEST_BINS    := $(patsubst $(TEST_DIR)/%.c,$(TEST_BUILD_DIR)/%,$(TEST_SRCS))

# Exécutables
MAIN := $(BUILD_DIR)/main
EXPE := $(BUILD_DIR)/expe

.PHONY: all main expe test run clean mrproper help

# Compilation
all: $(MAIN) $(EXPE)

main: $(MAIN)
expe: $(EXPE)

$(MAIN): $(BUILD_DIR)/main.o $(COMMON_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(EXPE): $(BUILD_DIR)/expe.o $(COMMON_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Compilation individuelle des fichiers C
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

# Création du dossier de compilation
$(BUILD_DIR):
	mkdir -p $@

$(TEST_BUILD_DIR):
	mkdir -p $@

# Compilation et exécution de tous les tests
test: $(TEST_BINS)
	@set -e; \
	for test_bin in $(TEST_BINS); do \
		echo "== $$test_bin =="; \
		./$$test_bin; \
	done
	@echo "Tous les tests sont passés."

$(TEST_BUILD_DIR)/%: $(TEST_DIR)/%.c $(TEST_HEADERS) $(COMMON_OBJS) | $(TEST_BUILD_DIR)
	$(CC) $(CPPFLAGS) -I$(TEST_DIR) $(CFLAGS) $< $(COMMON_OBJS) $(LDFLAGS) $(LDLIBS) -o $@

# Exécution
run: $(MAIN)
	@test -n "$(in)" || { echo "Erreur : paramètre 'in' manquant"; exit 1; }
	@test -n "$(out)" || { echo "Erreur : paramètre 'out' manquant"; exit 1; }
	@test -n "$(source)" || { echo "Erreur : paramètre 'source' manquant"; exit 1; }
	@test -n "$(heap)" || { echo "Erreur : paramètre 'heap' manquant"; exit 1; }
	@case "$(heap)" in 0|1|2) ;; *) echo "Erreur : 'heap' doit valoir 0, 1 ou 2"; exit 1;; esac
	./$(MAIN) "$(in)" "$(out)" "$(source)" "$(heap)"

# Nettoyage
clean:
	rm -rf $(BUILD_DIR)

mrproper: clean

# Aide
help:
	@echo "Cibles disponibles :"
	@echo "  all       Compile main et expe"
	@echo "  main      Compile uniquement main"
	@echo "  expe      Compile uniquement expe"
	@echo "  test      Compile et exécute tous les tests"
	@echo "  run       Exécute main avec ses paramètres"
	@echo "  clean     Supprime le dossier build"
	@echo "  help      Affiche cette aide"

# Dépendances automatiques des headers
-include $(OBJS:.o=.d)
