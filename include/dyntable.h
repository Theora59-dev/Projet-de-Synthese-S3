#pragma once

/**
 * Tableau Dynamique générique
 */

/**
 * Le tableau dynamique est une structure contenant :
 * (+) un tableau (\p T) de pointeurs vers des données de type void,
 * (+) la taille (\p size) du tableau, et
 * (+) le nombre de cases actuellement utilisées (\p used).
 * Condition : \p size / 4 \< \p used \<= \p size
 */
struct dyn_table_t {
	void **T;
	unsigned int size;
	unsigned int used;
};

/**
 * Construit et initialise un tableau dynamique vide.
 * La taille initiale du tableau est égale à 1,
 * tandis que ne contient aucun élément.
 *
 * @return le nouveau tableau dynamique créé
 */
struct dyn_table_t * new_dyn_table();

/**
 * Restitue la donnée de l'élément de \p position du tableau dynamique \p table.
 * La \p position doit être parmi les cases utilisées du tableau.
 *
 * @param[in] table
 * @param[in] position
 * @return la donnée de \p position du tableau \p table
 */
void * get_dyn_table_data(const struct dyn_table_t * table, unsigned int position);

/**
 * Restitue la taille (nombre de cases disponibles mais pas nécessairement utilisées)
 * du tableau dynamique \p table.
 *
 * @param[in] table
 * @return la taille du tableau \p table
 */
unsigned int get_dyn_table_size(const struct dyn_table_t * table);

/**
 * Restitue le nombre de cases utilisées du tableau dynamique \p table.
 *
 * @param[in] table
 * @returnle le nombre de cases utilisées du tableau  \p table.
 */
unsigned int get_dyn_table_used(const struct dyn_table_t * table);

/**
 * Renvoie 1 si le tableau dynamique \p table est complètement rempli,
 * sinon renvoie 0.
 *
 * @param[in] table
 * @return vrai (1) si le tableau est rempli
 */
int dyn_table_is_full(const struct dyn_table_t * table);

/**
 * Renvoie 1 si le tableau dynamique \p table contient moins que size/4 éléments,
 * sinon renvoie 0.
 *
 * @param[in] table
 * @return vrai (1) si le tableau contient moins que size/4 éléments
 */
int dyn_table_is_quasi_empty(const struct dyn_table_t * table);

/**
 * Renvoie 1 si le tableau dynamique \p table ne contient pas d'éléments,
 * sinon renvoie 0.
 *
 * @param[in] table
 * @return vrai (1) si le tableau est vide
 */
int dyn_table_is_empty(const struct dyn_table_t * table);

/**
 * Remplace la donnée de \p position du tableau dynamique \p table par \p newData.
 *
 * @param[in] table
 * @param[in] position
 * @param[in] newData
 */
void set_dyn_table_data(struct dyn_table_t * table, unsigned int position, void * newData);

/**
 * Met à jour la taille du tableau dynamique \p table à \p newSize.
 *
 * @param[in] table
 * @param[in] newSize
 */
void set_dyn_table_size(struct dyn_table_t * table, unsigned int newSize);

/**
 * Incrémente le nombre des cases utilisées au tableau dynamique \p table par 1.
 *
 * @param[in] table
 */
void increase_dyn_table_used(struct dyn_table_t * table);

/**
 * Décrémente le nombre des cases utilisées au tableau dynamique \p table par 1.
 *
 * @param[in] table
 */
void decrease_dyn_table_used(struct dyn_table_t * table);

/**
 * Deux possibilités pour libérer la mémoire du tableau dynamique \p table :
 * (+) Si le paramètre \p freeData n'est pas NULL,
 *     alors le pointeur de fonction \p freeData
 *     va servir à supprimer les données (data) des différentes cases utilisées
 *     du tableau dynamique \p table.
 * (+) Si le paramètre \p freeData est NULL,
 *     alors les données (data) des différentes cases utilisées
 *     du tableau dynamique \p table ne sont pas supprimées.
 * Dans tous les cas, le tableau est supprimé.
 *
 * @param[in] table
 * @param[in] freeData
 */
void delete_dyn_table(struct dyn_table_t * table, void (*freeData)(void *));

/**
 * Affiche les éléments du tableau dynamique \p table.
 * La donnée de chaque case du tableau est affichée grâce au pointeur
 * de fonction \p viewData.
 *
 * @param[in] table
 * @param[in] viewData
 */
void view_dyn_table(const struct dyn_table_t * table, void (*viewData)(const void *));

/**
 * Insère à la premiere position disponible (position used) du tableau dynamique
 * \p table un nouveau élément de donnée \p data.
 * Si le tableau est déjà rempli, la taille du tableau est doublé avant
 * d'insérer le nouveau élément.
 *
 * @param[in] table
 * @param[in] data
 */
void dyn_table_insert(struct dyn_table_t * table, void * data);

/**
 * Supprime le dernier élément (position used-1)
 * du tableau dynamique \p table et restitue sa donnée.
 * Si le tableau après la suppression contient moins que size/4
 * éléments, alors sa taille est divisée par 2.
 *
 * @param[in] table
 * @return la donnée de l'élément supprimé
 */
void * dyn_table_remove(struct dyn_table_t * table);

/**
 * Permute les données des positions \p pos1 et \p pos2 du tableau dynamique \p table.
 * Les positions \p pos1 et \p pos2 doivent correspondre à des positions
 * utilisées du tableau.
 *
 * @param[in] table
 * @param[in] pos1
 * @param[in] pos2
 */
void dyn_table_swap_nodes_data(struct dyn_table_t * table, unsigned int pos1, unsigned int pos2);