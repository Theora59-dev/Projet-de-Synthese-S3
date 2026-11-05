# Makefile du projet de synthèse S3
#
# Cibles principales :
#   make            compile les exécutables main et expe
#   make run ...    exécute main avec les paramètres in, out, source et heap
#   make clean      supprime les objets et les exécutables
#   make help       affiche l'aide

CC       ?= gcc
CFLAGS   ?= -std=c11 -Wall -Wextra -Wpedantic -O2 -g
CPPFLAGS += -Iinclude
LDFLAGS  ?=
LDLIBS   += -lm

SRC_DIR   := src
BUILD_DIR := build

SRCS       := $(wildcard $(SRC_DIR)/*.c)
OBJS       := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
COMMON_OBJS := $(filter-out $(BUILD_DIR)/main.o $(BUILD_DIR)/expe.o,$(OBJS))

MAIN := $(BUILD_DIR)/main
EXPE := $(BUILD_DIR)/expe

.PHONY: all main expe run clean mrproper help

all: $(MAIN) $(EXPE)

main: $(MAIN)
expe: $(EXPE)

$(MAIN): $(BUILD_DIR)/main.o $(COMMON_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(EXPE): $(BUILD_DIR)/expe.o $(COMMON_OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

# Exemple :
#   make run in=graphe.txt out=solution.txt source=01 heap=0
run: $(MAIN)
	@test -n "$(in)" || { echo "Erreur : paramètre 'in' manquant (ex. make run in=graphe.txt out=solution.txt source=01 heap=0)"; exit 1; }
	@test -n "$(out)" || { echo "Erreur : paramètre 'out' manquant"; exit 1; }
	@test -n "$(source)" || { echo "Erreur : paramètre 'source' manquant"; exit 1; }
	@test -n "$(heap)" || { echo "Erreur : paramètre 'heap' manquant"; exit 1; }
	@case "$(heap)" in 0|1|2) ;; *) echo "Erreur : 'heap' doit valoir 0, 1 ou 2"; exit 1;; esac
	./$(MAIN) "$(in)" "$(out)" "$(source)" "$(heap)"

clean:
	rm -rf $(BUILD_DIR)

mrproper: clean

help:
	@echo "Cibles disponibles :"
	@echo "  all       compile build/main et build/expe (cible par défaut)"
	@echo "  main      compile uniquement build/main"
	@echo "  expe      compile uniquement build/expe"
	@echo "  run       exécute main : make run in=<fichier> out=<fichier> source=<noeud> heap=<0|1|2>"
	@echo "  clean     supprime le dossier build"
	@echo "  help      affiche cette aide"

-include $(OBJS:.o=.d)
