#pragma once

/**
 * Liste générique doublement chaînée.
 */

/**
 * Un nœud d'une liste doublement chaînée contient :
 * (+) une donnée (\p data),
 * (+) une référence (\p successor) au nœud suivant, et
 * (+) une référence (\p predecessor) au nœud précédent.
 */
struct list_node_t {
	void * data;
	struct list_node_t * successor, * predecessor;
};

/**
 * Restitue la donnée du nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @return la donnée du nœud \p node
 */
void * get_list_node_data(const struct list_node_t * node);

/**
 * Restitue le successeur du nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @return le successeur du nœud \p node
 */
struct list_node_t * get_successor(const struct list_node_t * node);

/**
 * Restitue le prédécesseur du nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @return le prédécesseur du nœud \p node
 */
struct list_node_t * get_predecessor(const struct list_node_t * node);

/**
 * Remplace la donnée du nœud \p node par \p newData.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] newData 
 */
void set_list_node_data(struct list_node_t * node, void * newData);

/**
 * Remplace le successeur du nœud \p node par \p newSuccessor.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] newSuccessor
 */
void set_successor(struct list_node_t * node, struct list_node_t * newSuccessor);

/**
 * Remplace le prédécesseur du nœud \p node par \p newPredecessor.
 * Le nœud \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] newPredecessor
 */
void set_predecessor(struct list_node_t * node, struct list_node_t * newPredecessor);

/**
 * La liste est une structure contenant :
 * (+) une référence (\p head) sur son premier nœud,
 * (+) une référence (\p tail) sur son dernier nœud, et
 * (+) le nombre de ces nœuds (\p size).
 */
struct list_t {
	struct list_node_t * head, * tail;
	unsigned int size;
};

/**
 * Construit et initialise une liste doublement chaînée vide.
 * 
 * @return la nouvelle liste créée
 */
struct list_t * new_list();

/**
 * Renvoie 1 si la liste \p L est vide, sinon renvoie 0.
 * 
 * @param[in] L 
 * @return vrai (1) si la liste est vide
 */
int list_is_empty(const struct list_t * L);

/**
 * Restitue la taille (nombre d'éléments) de la liste \p L.
 * 
 * @param[in] L 
 * @return la taille de la liste \p L
 */
unsigned int get_list_size(const struct list_t * L);

/**
 * Restitue la tête (premier élément) de la liste \p L.
 * 
 * @param[in] L 
 * @return la tête de la liste \p L
 */
struct list_node_t * get_list_head(const struct list_t * L);

/**
 * Restitue la queue (dernier élément) de la liste \p L.
 * 
 * @param[in] L 
 * @return la queue de la liste \p L
 */
struct list_node_t * get_list_tail(const struct list_t * L);

/**
 * Incrémente la taille de la liste \p L par 1.
 * 
 * @param[in] L 
 */
void increase_list_size(struct list_t * L);

/**
 * Décrémente la taille de la liste \p L par 1.
 * 
 * @param[in] L 
 */
void decrease_list_size(struct list_t * L);

/**
 * Met à jour la taille de la liste \p L à \p newSize.
 * 
 * @param[in] L 
 * @param[in] newSize 
 */
void set_list_size(struct list_t * L, unsigned int newSize);

/**
 * Remplace la tête de la liste \p L par \p newHead.
 * 
 * @param[in] L 
 * @param[in] newHead 
 */
void set_list_head(struct list_t * L, struct list_node_t * newHead);

/**
 * Remplace la queue de la liste \p L par \p newTail.
 * 
 * @param[in] L 
 * @param[in] newTail 
 */
void set_list_tail(struct list_t * L, struct list_node_t * newTail);

/**
 * Deux possibilités pour libérer la mémoire de la liste \p L :
 * (+) Si le paramètre \p freeData n'est pas NULL,
 *     alors le pointeur de fonction \p freeData
 *     va servir à supprimer les données (data) référencées par
 *     les nœuds de la liste \p L.
 * (+) Si le paramètre \p freeData est NULL,
 *     alors les données (data) référencées par les nœuds
 *     de la liste \p L ne sont pas supprimées.
 * Dans tous les cas, les nœuds de la liste et la liste elle même
 * sont supprimés.
 *
 * @param[in] L 
 * @param[in] freeData
 */
void delete_list(struct list_t * L, void (*freeData)(void *));

/**
 * Affiche les éléments de la liste \p L.
 * Les données de chaque nœud sont affichées grâce au pointeur
 * de fonction \p viewData.
 * 
 * @param[in] L 
 * @param[in] viewData
 */
void view_list(const struct list_t * L, void (*viewData)(const void *));

/**
 * Insère en tête de la liste \p L un nouveau nœud de donnée \p data.
 * 
 * @param[in] L 
 * @param[in] data
 */
void list_insert_first(struct list_t * L, void * data);

/**
 * Insère à la fin de la liste \p L un nouveau nœud de donnée \p data.
 * 
 * @param[in] L 
 * @param[in] data
 */
void list_insert_last(struct list_t * L, void * data);

/**
 * Insère un nouveau nœud de donnée \p data dans la liste \p L
 * après le nœud indiqué par le pointeur \p node.
 * Le pointeur \p node désigne obligatoirement un nœud de la liste \p L.
 * 
 * @param[in] L
 * @param[in] data
 * @param[in] node
 */
void list_insert_after(struct list_t * L, void * data, struct list_node_t * node);

/**
 * Supprime le premier nœud de la liste \p L et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * La liste \p L ne peut pas être vide.
 * 
 * @param[in] L
 * @return la donnée du nœud supprimé
 */
void * list_remove_first(struct list_t * L);

/**
 * Supprime le dernier nœud de la liste \p L et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * La liste \p L ne peut pas être vide.
 * 
 * @param[in] L
 * @return la donnée du nœud supprimé
 */
void * list_remove_last(struct list_t * L);

/**
 * Supprime le nœud de la liste \p L indiqué par le pointeur \p node
 * et restitue sa donnée.
 * La mémoire du nœud supprimé est libérée mais pas la mémoire de la donnée.
 * La liste \p L ne peut pas être vide.
 * Le pointeur \p node désigne obligatoirement un nœud de la liste \p L.
 * 
 * @param[in] L
 * @param[in] node
 * @return la donnée du nœud supprimé
 */
void * list_remove_node(struct list_t * L, struct list_node_t * node);

/**
 * Permute les données des nœuds \p node1 et \p node2.
 * Les nœuds \p node1 et \p node2 ne peuvent pas être vides.
 *
 * @param[in] node1
 * @param[in] node2
 */
void list_swap_nodes_data(struct list_node_t * node1, struct list_node_t * node2);

/**
 * Renvoie 1 si la donnée \p data existe dans la liste \p L, sinon renvoie 0.
 *
 * @param[in] L
 * @param[in] data
 * @return vrai (1) si la donnée \p data existe dans la liste \p L
 */
int list_data_exist(struct list_t * L, void * data);