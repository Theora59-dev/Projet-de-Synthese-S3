#pragma once

/**
 * Tas-min générique
 *
 * Possibilité d'utiliser trois structures de données différentes pour la répresentation du tas :
 * (+) Tableau dynamique
 * (+) Arbre binaire complet
 * (+) Liste ordonnée
 */

/**
 * Un nœud du tas contient :
 * (+) une donnée (\p data),
 * (+) sa clé (\p key), et
 * (+) sa position au dictionnaire (\p dict_position).
 */
struct heap_node_t {
	unsigned long key;
	void * data;
	unsigned int dict_position;
};

/**
 * Restitue la clé du nœud d'un tas \p node.
 * Le nœud \p node ne peut pas être vide.
 *
 * @param[in] node
 * @return la clé du nœud d'un tas
 */
unsigned long get_heap_node_key(const struct heap_node_t * node);

/**
 * Restitue la donnée du nœud d'un tas \p node.
 * Le nœud \p node ne peut pas être vide.
 *
 * @param[in] node
 * @return la donnée du nœud d'un tas
 */
void * get_heap_node_data(const struct heap_node_t * node);

/**
 * Restitue la position au dictionnaire du tas pour le nœud \p node.
 * Le nœud \p node ne peut pas être vide.
 *
 * @param[in] node
 * @return la position au dictionnaire du nœud d'un tas
 */
unsigned int get_heap_node_dict_position(const struct heap_node_t * node);

/**
 * Remplace la clé du nœud d'un tas \p node par \p newKey.
 * Le nœud \p node ne peut pas être vide.
 *
 * @param[in] node
 * @param[in] newKey
 */
void set_heap_node_key(struct heap_node_t * node, unsigned long newKey);

/**
 * Remplace la donnée du nœud d'un tas \p node par \p newData.
 * Le nœud \p node ne peut pas être vide.
 *
 * @param[in] node
 * @param[in] newData
 */
void set_heap_node_data(struct heap_node_t * node, void * newData);

/**
 * Remplace la position au dictionnaire du tas pour le nœud \p node par \p newPosition.
 * Le nœud \p node ne peut pas être vide.
 *
 * @param[in] node
 * @param[in] newPosition
 */
void set_heap_node_dict_position(struct heap_node_t * node, unsigned int newPosition);

/**
 * Le tas est une structure qui contient les attributs suivants.
 * (+) La structure de données \p heap qui implémente le tas.
 * (+) Le dictionnaire \p dict : chaque élément du tas correspond à une position
 *     du dictionnaire qui ne change pas, même si la position de cet élément au tas
 *     peut changer. Il s'agit d'une référence fixe à l'extérieur du tas qui
 *     ne change pas quand le tas est réorganisé et qui contient la vraie position
 *     vers le tas de l'élément.
 *     Chaque case du dictionnaire contient un entier (\p int*) dans le cas où le tas
 *     est représenté par un tableau dynamique ou un arbre complet, et un nœud de
 *     liste (\p list_node_t*) dans le cas où le tas est représenté par une liste.
 * (+) Les pointeurs des 3 fonctions génériques implémentant les primitives du tas :
 *     (-) \p heap_insert
 *     (-) \p heap_extract_min
 *     (-) \p heap_increase_priority
 * (+) Les trois pointeurs de fonction suivants :
 *     (-) \p heap_is_empty : test de vacuité
 *     (-) \p view_heap : pour l'affichage
 *     (-) \p delete_heap : pour la suppression
 */
struct heap_t {
	void * heap;
	struct dyn_table_t * dict;
	unsigned int (*heap_insert)(struct heap_t * H, unsigned long key, void * data);
	struct heap_node_t * (*heap_extract_min)(struct heap_t * H);
	void (*heap_increase_priority)(struct heap_t * H, unsigned int dict_position, unsigned long newKey);
	int (*heap_is_empty)(const void * H);
	void (*view_heap)(const struct heap_t * H, void (*viewHeapNode)(const void *));
	void (*delete_heap)(struct heap_t * H, void (*freeHeapNode)(void *));
};

/**
 * Construire un tas vide en choisissant son implémentation (type) :
 * (0) dyn_table_heap
 * (1) tree_heap
 * (2) list_heap
 *
 * @param[in] type
 * @return le nouveau tas créé
 */
struct heap_t * new_heap(int type);

/**
 * Restitue la structure de données qui implémente le tas \p H.
 *
 * @param[in] H
 * @return le tas
 */
void * get_heap(const struct heap_t * H);

/**
 * Restitue le dictionnaire du tas \p H.
 *
 * @param[in] H
 * @return le dictionnaire du tas
 */
struct dyn_table_t * get_heap_dictionary(const struct heap_t * H);

/***************************************************
 * DYNAMIC TABLE HEAP
 ***************************************************/

/**
 * Insérer dans le tas \p H un élément de donnée \p data et priorité \p key.
 *
 * @param[in] H
 * @param[in] key
 * @param[in] data
 * @return la position au dictionnaire du nouveau élément
 */
unsigned int dyn_table_heap_insert(struct heap_t * H, unsigned long key, void * data);

/**
 * Extraire du tas \p H le nœud avec la plus grande priorité.
 *
 * @param[in] H
 * @return la donnée du nœud qu'on a extrait
 */
struct heap_node_t * dyn_table_heap_extract_min(struct heap_t * H);

/**
 * Dans le tas \p H, modifier à \p newKey la priorité du nœud
 * qui se trouve à la position \p dict_position du dictionnaire.
 *
 * @param[in] H
 * @param[in] dict_position
 * @param[in] newKey
 */
void dyn_table_heap_increase_priority(struct heap_t * H, unsigned int dict_position, unsigned long newKey);

/**
 * Verifier si le tas est vide.
 *
 * @param[in] H
 * @return vrai (1) si le tas est vide, sinon faux (0)
 */
int dyn_table_heap_is_empty(const void * H);

/**
 * Afficher les éléments du tas \p H.
 * Chaque élément est affiché grace au pointeur de fonction \p viewHeapNode.
 *
 * @param[in] H
 * @param[in] viewHeapNode
 */
void view_dyn_table_heap(const struct heap_t * H, void (*viewHeapNode)(const void *));

/**
 * Libérer la mémoire du tas \p H.
 * Chaque élément est libéré en utilisant le pointeur de fonction \p freeHeapNode.
 * Le dictionnaire doit être aussi libéré.
 *
 * @param[in] H
 * @param[in] freeHeapNode
 */
void delete_dyn_table_heap(struct heap_t * H, void (*freeHeapNode)(void *));

/***************************************************
 * COMPLETE BINARY TREE HEAP
 ***************************************************/

/**
 * Insérer dans le tas \p H un élément de donnée \p data et priorité \p key.
 *
 * @param[in] H
 * @param[in] key
 * @param[in] data
 * @return
 */
unsigned int tree_heap_insert(struct heap_t * H, unsigned long key, void * data);

/**
 * Extraire du tas H le nœud avec la plus grande priorité.
 *
 * @param[in] H
 * @return
 */
struct heap_node_t * tree_heap_extract_min(struct heap_t * H);

/**
 * Dans le tas \p H, modifier à \p newKey la priorité du nœud
 * qui se trouve à la position \p dict_position du dictionnaire.
 *
 * @param[in] H
 * @param[in] dict_position
 * @param[in] newKey
 */
void tree_heap_increase_priority(struct heap_t * H, unsigned int dict_position, unsigned long newKey);

/**
 * Verifier si le tas est vide.
 *
 * @param[in] H
 * @return vrai (1) si le tas est vide, sinon faux (0)
 */
int tree_heap_is_empty(const void * H);

/**
 * Afficher les éléments du tas \p H.
 * Chaque élément est affiché grace au pointeur de fonction \p viewHeapNode.
 *
 * @param[in] H
 * @param[in] viewHeapNode
 */
void view_tree_heap(const struct heap_t * H, void (*viewHeapNode)(const void *));

/**
 * Libérer la mémoire du tas \p H.
 * Chaque élément est libéré en utilisant le pointeur de fonction \p freeHeapNode.
 * Le dictionnaire doit être aussi libéré.
 *
 * @param[in] H
 * @param[in] freeHeapNode
 */
void delete_tree_heap(struct heap_t * H, void (*freeHeapNode)(void *));

/***************************************************
 * ORDERED LIST HEAP
 ***************************************************/

/**
 * Insérer dans le tas \p H un élément de donnée \p data et priorité \p key.
 *
 * @param[in] H
 * @param[in] key
 * @param[in] data
 * @return
 */
unsigned int list_heap_insert(struct heap_t * H, unsigned long key, void * data);

/**
 * Extraire du tas H le nœud avec la plus grande priorité.
 *
 * @param[in] H
 * @return
 */
struct heap_node_t * list_heap_extract_min(struct heap_t * H);

/**
 * Dans le tas \p H, modifier à \p newKey la priorité du nœud
 * qui se trouve à la position \p dict_position du dictionnaire.
 *
 * @param[in] H
 * @param[in] dict_position
 * @param[in] newKey
 */
void list_heap_increase_priority(struct heap_t * H, unsigned int dict_position, unsigned long newKey);

/**
 * Verifier si le tas est vide.
 *
 * @param[in] H
 * @return vrai (1) si le tas est vide, sinon faux (0)
 */
int list_heap_is_empty(const void * H);

/**
 * Afficher les éléments du tas \p H.
 * Chaque élément est affiché grace au pointeur de fonction \p viewHeapNode.
 *
 * @param[in] H
 * @param[in] viewHeapNode
 */
void view_list_heap(const struct heap_t * H, void (*viewHeapNode)(const void *));

/**
 * Libérer la mémoire du tas \p H.
 * Chaque élément est libéré en utilisant le pointeur de fonction \p freeHeapNode.
 * Le dictionnaire doit être aussi libéré.
 *
 * @param[in] H
 * @param[in] freeHeapNode
 */
void delete_list_heap(struct heap_t * H, void (*freeHeapNode)(void *));