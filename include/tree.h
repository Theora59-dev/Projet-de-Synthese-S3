#pragma once

/**
 * Arbre Binaire Complet générique
 *
 * Dans un arbre binaire complet tous les niveau de l'arbre sont remplis
 * sauf éventuellement le dernier niveau sur lequel les nœuds se trouvent
 * le plus à gauche possible.
 */

/**
 * Un nœud d'un arbre binaire contient :
 * (+) une donnée (\p data),
 * (+) une référence (\p left) au fils gauche, et
 * (+) une référence (\p right) au fils droit.
 */
struct tree_node_t {
	void * data;
	struct tree_node_t * left, * right;
};

/**
 * Restitue la donnée du nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @return la donnée du nœud \p node
 */
void * get_tree_node_data(const struct tree_node_t * node);

/**
 * Restitue le fils gauche du nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @return le fils gauche du nœud \p node
 */
struct tree_node_t * get_left(const struct tree_node_t * node);

/**
 * Restitue le fils droit du nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @return le fils droit du nœud \p node
 */
struct tree_node_t * get_right(const struct tree_node_t * node);

/**
 * Remplace la donnée du nœud \p node par \p newData.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] newData 
 */
void set_tree_node_data(struct tree_node_t * node, void * newData);

/**
 * Remplace le fils gauche du nœud \p node par \p newLeft.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] newLeft 
 */
void set_left(struct tree_node_t * node, struct tree_node_t * newLeft);

/**
 * Remplace le fils gauche du nœud \p node par \p newRight.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] newRight 
 */
void set_right(struct tree_node_t * node, struct tree_node_t * newRight);

/**
 * L'arbre binaire complet est une structure contenant :
 * (+) une référence (\p root) sur sa racine, et
 * (+) le nombre de ces nœuds (\p size).
 */
struct tree_t {
	struct tree_node_t * root;
	unsigned int size;
};

/**
 * Construit et initialise un arbre binaire complet vide.
 * 
 * @return le nouveau arbre binaire complet créé
 */
struct tree_t * new_tree();

/**
 * Renvoie 1 si l'arbre \p T est vide, sinon renvoie 0.
 * 
 * @param[in] T 
 * @return vrai (1) si l'arbre est vide
 */
int tree_is_empty(const struct tree_t * T);

/**
 * Restitue la taille (nombre d'éléments) de l'arbre \p T.
 * 
 * @param[in] T 
 * @return la taille de l'arbre \p T
 */
unsigned int get_tree_size(const struct tree_t * T);

/**
 * Restitue la racine de l'arbre \p T.
 * 
 * @param[in] T 
 * @return la racine de l'arbre \p T
 */
struct tree_node_t * get_tree_root(const struct tree_t * T);

/**
 * Incrémente la taille de l'arbre \p T par 1.
 * 
 * @param[in] T 
 */
void increase_tree_size(struct tree_t * T);

/**
 * Décrémente la taille de l'arbre \p T par 1.
 * 
 * @param[in] T 
 */
void decrease_tree_size(struct tree_t * T);

/**
 * Remplace la racine de l'arbre \p T par \p newRoot.
 * 
 * @param[in] T 
 * @param[in] newRoot 
 */
void set_tree_root(struct tree_t * T, struct tree_node_t * newRoot);

/**
 * Deux possibilités pour libérer la mémoire de l'arbre \p T :
 * (+) Si le paramètre \p freeData n'est pas NULL,
 *     alors le pointeur de fonction \p freeData
 *     va servir à supprimer les données (data) référencées par
 *     les nœuds de l'arbre \p T.
 * (+) Si le paramètre \p freeData est NULL,
 *     alors les données (data) référencées par les nœuds
 *     de l'arbre \p T ne sont pas supprimées.
 * Dans tous les cas, les nœuds de l'arbre et l'arbre lui même
 * sont supprimés.
 *
 * @param[in] T
 * @param[in] freeData
 */
void delete_tree(struct tree_t * T, void (*freeData)(void *));

/**
 * Affiche les éléments de l'arbre \p T.
 * Les données de chaque nœud sont affichées grâce au pointeur
 * de fonction \p viewData.
 * Le paramètre \p order spécifie l'ordre d'affichage :
 * (+) 0 = ordre pré-fixe,
 * (+) 1 = ordre post-fixe, et
 * (+) 2 = ordre in-fixe.
 *
 * @param[in] T
 * @param[in] viewData
 * @param[in] order
 */
void view_tree(const struct tree_t * T, void (*viewData)(const void *), int order);

/**
 * Insère au dernier niveau et le plus à gauche possible
 * dans l'arbre \p T un nouveau nœud de donnée \p data.
 * 
 * @param[in] T 
 * @param[in] data 
 */
void tree_insert(struct tree_t * T, void * data);

/**
 * Supprime le dernier nœud de l'arbre \p T et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * L'arbre \p T ne peut pas être vide.
 * 
 * NB : le dernier nœud d'un arbre binaire complet se trouve au dernier
 * niveau et correspond au nœud de ce niveau le plus à droite possible.
 * 
 * @param[in] T
 * @return la donnée du nœud supprimé
 */
void * tree_remove(struct tree_t * T);

/**
 * Restitue le nœud qui se trouve à la \p position de l'arbre \p T.
 * L'arbre \p T ne peut pas être vide.
 * La position doit être valide dans l'arbre \p T, c'est-à-dire
 * ça doit être strictement inférieure à la taille de l'arbre.
 *
 * NB : La position d'un nœud dans un arbre binaire complet est définie
 * dans la figure suivante.
 *
 *          0
 *       /     \
 *      1       2
 *     / \     / \
 *    3   4   5   6
 *   / \
 *  7  ...
 *
 * @param[in] T
 * @param[in] position
 * @return le nœud de l'arbre \p T qui correspond à \p position
 */
struct tree_node_t * tree_find_node(struct tree_t * T, unsigned int position);

/**
 * Permute les données des nœuds \p node1 et \p node2.
 * Les nœuds \p node1 et \p node2 ne peuvent pas être vides.
 * 
 * @param[in] node1 
 * @param[in] node2 
 */
void tree_swap_nodes_data(struct tree_node_t * node1, struct tree_node_t * node2);