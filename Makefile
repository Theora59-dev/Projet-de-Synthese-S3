
# Makefile du projet de synthèse S3

# Configuration
.DEFAULT_GOAL := all

CC       = gcc
CFLAGS   ?= -std=c11 -Wall -Wextra -Wpedantic -O2 -g
CPPFLAGS += -Iinclude
LDFLAGS  ?=
LDLIBS   += -lm

SRC_DIR   := src
BUILD_DIR := build

# Sources et objets
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

COMMON_OBJS := $(filter-out \
    $(BUILD_DIR)/main.o \
    $(BUILD_DIR)/expe.o, \
    $(OBJS))

# Exécutables
MAIN := $(BUILD_DIR)/main
EXPE := $(BUILD_DIR)/expe

.PHONY: all main expe run clean mrproper help

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
	@echo "  run       Exécute main avec ses paramètres"
	@echo "  clean     Supprime le dossier build"
	@echo "  help      Affiche cette aide"

# Dépendances automatiques des headers
-include $(OBJS:.o=.d)
