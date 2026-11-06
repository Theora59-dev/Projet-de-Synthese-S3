#include "tree.h"

#include <assert.h>
#include <iso646.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aliases.h"

#include "list.h"
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
	tree_node_t *node = calloc(1, sizeof(tree_node_t));
	assert(node != NULL);

	node->data = data;
	node->left = NULL;
	node->right = NULL;

	return node;
}

void *get_tree_node_data(const struct tree_node_t *node) {
	assert(node);
	return node->data;
}

struct tree_node_t *get_left(const struct tree_node_t *node) {
	assert(node);
	return node->left;
}

struct tree_node_t *get_right(const struct tree_node_t *node) {
	assert(node);
	return node->right;
}

void set_tree_node_data(struct tree_node_t *node, void *newData) {
	assert(node);
	assert(newData);
	node->data = newData;
}

void set_left(struct tree_node_t *node, struct tree_node_t *newLeft) {
	assert(node);
	assert(newLeft);
	node->left = newLeft;
}

void set_right(struct tree_node_t *node, struct tree_node_t *newRight) {
	assert(node);
	assert(newRight);
	node->right = newRight;
}

/********************************************************************
 * tree_t (Complete Binary Tree)
 ********************************************************************/

struct tree_t *new_tree() {
	tree_t *tree = calloc(1, sizeof(tree_t));
	assert(tree != NULL);

	tree->root = NULL;
	tree->size = 0;

	return tree;
}

int tree_is_empty(const struct tree_t *T) {
	assert(T);
	return T->size == 0;
}

unsigned int get_tree_size(const struct tree_t *T) {
	assert(T);
	return T->size;
}

struct tree_node_t *get_tree_root(const struct tree_t *T) {
	assert(T);
	return T->root;
}

void increase_tree_size(struct tree_t *T) { T->size++; }

void decrease_tree_size(struct tree_t *T) { T->size--; }

void set_tree_root(struct tree_t *T, struct tree_node_t *newRoot) {
	assert(T);
	// Pas mettre assert(newRoot); car si suppression (genre
	// remove_from_subtree) ça retourne null
	T->root = newRoot;
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
	if (node == NULL) {
		return;
	}
	// On free la data uniquement si freeData n'est pas null
	free_subtree(get_left(node), freeData);
	free_subtree(get_right(node), freeData);
	if (freeData != NULL) {
		freeData(node->data);
	}
	// Free du noeud
	free(node);
	node = NULL;
}

/**
 * NB : Utilisez la procédure récursive free_subtree.
 */
void delete_tree(struct tree_t *T, void (*freeData)(void *)) {
	if (T == NULL) {
		return;
	}
	free_subtree(get_tree_root(T), freeData);
	free(T);
	T = NULL;
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
	assert(viewData);
	if (node != NULL) {
		printf("NODE[\n");
		viewData(get_tree_node_data(node));
		printf("]\n");

		view_preorder(get_left(node), viewData);
		view_preorder(get_right(node), viewData);
	}
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
	assert(viewData);
	if (node != NULL) {
		view_inorder(get_left(node), viewData);

		printf("NODE[\n");
		viewData(get_tree_node_data(node));
		printf("]\n");

		view_inorder(get_right(node), viewData);
	}
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
	assert(viewData);
	if (node != NULL) {
		view_postorder(get_left(node), viewData);
		view_postorder(get_right(node), viewData);

		printf("NODE[\n");
		viewData(get_tree_node_data(node));
		printf("]\n");
	}
}

static void view_level_order(const struct tree_node_t *node,
                             void (*viewData)(const void *), int level) {
	assert(viewData);
	if (node == NULL)
		return;

	list_t *queue = new_list();
	list_node_t *current_list_node = new_list_node((void *)node);
	set_list_head(queue, current_list_node);

	while (!list_is_empty(queue)) {
		current_list_node = get_list_head(queue);
		struct tree_node_t *current =
		    (struct tree_node_t *)get_list_node_data(current_list_node);

		list_remove_node(queue, current_list_node);

		printf("NODE[\n");
		viewData(get_tree_node_data(current));
		printf("]\n");

		if (current->right != NULL) {
			list_node_t *left_node = new_list_node(current->right);
			set_list_head(queue, left_node);
		}
		if (current->left != NULL) {
			list_node_t *right_node = new_list_node(current->left);
			set_list_head(queue, right_node);
		}
	}
	delete_list(queue, NULL);
}

/**
 * NB : Utilisez les procédures récursives view_preorder, view_inorder et
 * view_postorder. Rappel : order = 0 (view_preorder), 1 (view_postorder), 2
 * (view_inorder)
 */
void view_tree(const struct tree_t *T, void (*viewData)(const void *),
               int order) {
	assert(T);
	assert(viewData);
	switch (order) {
	case 0:
		view_preorder(get_tree_root(T), viewData);
		break;
	case 1:
		view_postorder(get_tree_root(T), viewData);
		break;
	case 2:
		view_inorder(get_tree_root(T), viewData);
		break;
	default:
		assert(NULL);
	}
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
	if (node == NULL) {
		return new_tree_node(data);
	}
	if (position == 0) {
		return node;
	}

	/*
	 * Le chemin depuis la racine est donné par l'écriture binaire de
	 * position + 1, en ignorant le bit de poids fort :
	 * 0 = fils gauche, 1 = fils droit.
	 * Exemple : position 4 -> 5 = 101 -> gauche puis droite.
	 */
	unsigned int path = position + 1;
	unsigned int depth = 0;
	while ((path >> (depth + 1)) != 0) {
		depth++;
	}

	struct tree_node_t *current = node;
	for (unsigned int level = depth; level > 0; level--) {
		unsigned int go_right = (path >> (level - 1)) & 1u;

		if (level == 1) {
			if (go_right) {
				current->right = new_tree_node(data);
			} else {
				current->left = new_tree_node(data);
			}
		} else {
			current = go_right ? current->right : current->left;
			if (current == NULL) {
				/* Position invalide : rien à insérer. */
				return node;
			}
		}
	}

	return node;
}

/**
 * NB : Utilisez la fonction récursive insert_into_subtree afin de lancer
 * l'insertion.
 */
void tree_insert(struct tree_t *T, void *data) {
	assert(T);
	assert(data);
	tree_node_t *root =
	    insert_into_subtree(get_tree_root(T), get_tree_size(T), data);
	increase_tree_size(T);
	set_tree_root(T, root);
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
	if (node == NULL) {
		return node;
	}
	if (position == 0) {
		if (data != NULL) {
			*data = get_tree_node_data(node);
		}

		free(node);
		return NULL;
	}

	/* Même encodage du chemin que pour l'insertion. */
	unsigned int path = position + 1;
	unsigned int depth = 0;
	while ((path >> (depth + 1)) != 0) {
		depth++;
	}

	struct tree_node_t *parent = node;
	for (unsigned int level = depth; level > 1; level--) {
		unsigned int go_right = (path >> (level - 1)) & 1u;
		parent = go_right ? parent->right : parent->left;
		if (parent == NULL) {
			return node;
		}
	}

	unsigned int go_right = path & 1u;
	struct tree_node_t *target = go_right ? parent->right : parent->left;
	if (target == NULL) {
		return node;
	}

	if (data != NULL) {
		*data = get_tree_node_data(target);
	}
	free(target);

	if (go_right) {
		parent->right = NULL;
	} else {
		parent->left = NULL;
	}

	return node;
}

/**
 * NB : Utilisez la fonction récursive remove_from_subtree afin de lancer la
 * suppression.
 */
void *tree_remove(struct tree_t *T) {
	assert(T);
	assert(!tree_is_empty(T));

	void *data = NULL;
	uint last_position = get_tree_size(T) - 1;

	tree_node_t *root =
	    remove_from_subtree(get_tree_root(T), last_position, &data);

	set_tree_root(T, root);
	decrease_tree_size(T);

	return data;
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
	if (node == NULL || position == 0) {
		return node;
	}

	/* Même encodage du chemin que pour l'insertion. */
	unsigned int path = position + 1;
	unsigned int depth = 0;
	while ((path >> (depth + 1)) != 0) {
		depth++;
	}

	struct tree_node_t *current = node;
	for (unsigned int level = depth; level > 0 && current != NULL; level--) {
		unsigned int go_right = (path >> (level - 1)) & 1u;
		current = go_right ? current->right : current->left;
	}

	return current;
}

/**
 * NB : Utilisez la fonction récursive get_tree_node_at_position afin de lancer
 * la recherche.
 */
struct tree_node_t *tree_find_node(struct tree_t *T, unsigned int position) {
	return get_tree_node_at_position(get_tree_root(T), position);
}

void tree_swap_nodes_data(struct tree_node_t *node1,
                          struct tree_node_t *node2) {
	assert(node1);
	assert(node2);

	void *tmp = node1->data;
	node1->data = node2->data;
	node2->data = tmp;
}
