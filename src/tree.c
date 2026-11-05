#include "tree.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

/********************************************************************
 * tree_node_t
 ********************************************************************/

/**
 * Construit et initialise un nouveau noeud d'un arbre binaire.
 * Le paramètre \p data correspond à la donnée à affecter au nouveau nœud.
 *
 * @param[in] data
 * @return le nouveau nœud créé
 */
static struct tree_node_t *new_tree_node(void *data) {
	// TO DO
}

void *get_tree_node_data(const struct tree_node_t *node) {
	// TO DO
}

struct tree_node_t *get_left(const struct tree_node_t *node) {
	// TO DO
}

struct tree_node_t *get_right(const struct tree_node_t *node) {
	// TO DO
}

void set_tree_node_data(struct tree_node_t *node, void *newData) {
	// TO DO
}

void set_left(struct tree_node_t *node, struct tree_node_t *newLeft) {
	// TO DO
}

void set_right(struct tree_node_t *node, struct tree_node_t *newRight) {
	// TO DO
}

/********************************************************************
 * tree_t (Complete Binary Tree)
 ********************************************************************/

struct tree_t *new_tree() {
	// TO DO
}

int tree_is_empty(const struct tree_t *T) {
	// TO DO
}

unsigned int get_tree_size(const struct tree_t *T) {
	// TO DO
}

struct tree_node_t *get_tree_root(const struct tree_t *T) {
	// TO DO
}

void increase_tree_size(struct tree_t *T) {
	// TO DO
}

void decrease_tree_size(struct tree_t *T) {
	// TO DO
}

void set_tree_root(struct tree_t *T, struct tree_node_t *newRoot) {
	// TO DO
}

/**
 * Libère récursivement le sous-arbre raciné au nœud \p node.
 * Dans le cas où le pointeur de fonction \p freeData n'est pas NULL,
 * la mémoire de la donnée du nœud actuel est aussi libérée.
 * NB : procédure récursive.
 *
 * @param[in] node
 * @param[in] freeData
 */
static void free_subtree(struct tree_node_t *node, void (*freeData)(void *)) {
	// TO DO
}

/**
 * NB : Utilisez la procédure récursive free_subtree.
 */
void delete_tree(struct tree_t *T, void (*freeData)(void *)) {
	// TO DO
}

/**
 * Affiche les éléments du sous-arbre raciné au nœud \p node
 * en réalisant un parcours préfixé.
 * Les données de chaque nœud sont afficher en utilisant le
 * pointer de fonction \p viewData.
 *
 * @param[in] node
 * @param[in] viewData
 */
static void view_preorder(const struct tree_node_t *node,
                          void (*viewData)(const void *)) {
	// TO DO
}

/**
 * Affiche les éléments du sous-arbre raciné au nœud \p node
 * en réalisant un parcours infixé.
 * Les données de chaque nœud sont afficher en utilisant le
 * pointer de fonction \p viewData.
 *
 * @param[in] node
 * @param[in] viewData
 */
static void view_inorder(const struct tree_node_t *node,
                         void (*viewData)(const void *)) {
	// TO DO
}

/**
 * Affiche les éléments du sous-arbre raciné au nœud \p node
 * en réalisant un parcours post-fixé.
 * Les données de chaque nœud sont afficher en utilisant le
 * pointer de fonction \p viewData.
 *
 * @param[in] node
 * @param[in] viewData
 */
static void view_postorder(const struct tree_node_t *node,
                           void (*viewData)(const void *)) {
	// TO DO
}

static void view_level_order(const struct tree_node_t *node,
                             void (*viewData)(const void *), int level) {
	// TO DO
}

/**
 * NB : Utilisez les procédures récursives view_preorder, view_inorder et
 * view_postorder. Rappel : order = 0 (view_preorder), 1 (view_postorder), 2
 * (view_inorder)
 */
void view_tree(const struct tree_t *T, void (*viewData)(const void *),
               int order) {
	// TO DO
}

/**
 * Insère récursivement un nouveau nœud de donnée \p data
 * dans le sous-arbre raciné au nœud \p node.
 * La position (par rapport à la racine \p node) où le nouveau nœud
 * va être insérer est indiquée par le paramètre \p position
 * (voir la figure ci-dessous pour la définition de la position d'un
 * sous-arbre).
 *
 *          0
 *       /     \
 *      1       2
 *     / \     / \
 *    3   4   5   6
 *   / \
 *  7  ...
 *
 * @param[in] node
 * @param[in] position
 * @param[in] data
 * @return le nœud \p node mis à jour
 */
static struct tree_node_t *insert_into_subtree(struct tree_node_t *node,
                                               unsigned int position,
                                               void *data) {
	// TO DO
}

/**
 * NB : Utilisez la fonction récursive insert_into_subtree afin de lancer
 * l'insertion.
 */
void tree_insert(struct tree_t *T, void *data) {
	// TO DO
}

/**
 * Supprime récursivement le dernier nœud du sous-arbre raciné au nœud \p node.
 * La position (par rapport à la racine \p node) du nœud à supprimer
 * est indiquée par le paramètre \p position
 * (voir la figure ci-dessous pour la définition de la position d'un
 * sous-arbre). La mémoire du dernier nœud est libérée mais pas la mémoire de sa
 * donnée qui est restituée.
 *
 *          0
 *       /     \
 *      1       2
 *     / \     / \
 *    3   4   5   6
 *   / \
 *  7  ...
 *
 * @param[in] node
 * @param[in] position
 * @param[out] data
 * @return le nœud \p node mis à jour
 */
static struct tree_node_t *remove_from_subtree(struct tree_node_t *node,
                                               unsigned int position,
                                               void **data) {
	// TO DO
}

/**
 * NB : Utilisez la fonction récursive remove_from_subtree afin de lancer la
 * suppression.
 */
void *tree_remove(struct tree_t *T) {
	// TO DO
}

/**
 * Restitue récursivement le nœud du sous-arbre raciné au nœud \p node
 * qui se trouve à \p position.
 * Cette position est définie par rapport à la racine \p node
 * (voir la figure ci-dessous pour la définition de la position d'un
 * sous-arbre).
 *
 *          0
 *       /     \
 *      1       2
 *     / \     / \
 *    3   4   5   6
 *   / \
 *  7  ...
 *
 * @param node
 * @param position
 * @return le dernier nœud de l'arbre
 */
static struct tree_node_t *get_tree_node_at_position(struct tree_node_t *node,
                                                     unsigned int position) {
	// TO DO
}

/**
 * NB : Utilisez la fonction récursive get_tree_node_at_position afin de lancer
 * la recherche.
 */
struct tree_node_t *tree_find_node(struct tree_t *T, unsigned int position) {
	// TO DO
}

void tree_swap_nodes_data(struct tree_node_t *node1,
                          struct tree_node_t *node2) {
	// TO DO
}
