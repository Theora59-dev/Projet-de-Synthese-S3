#pragma once

/**
 * Graphe pondéré
 *
 * L'implémentation du graphe est basée sur une liste par sommet contenant
 * les arêtes incidentes à lui.
 */

struct edge_t;

/**
 * Un sommet (\p vertex_t) d'un graphe est une structure qui contient naturellement :
 * (+) un identifiant (\p id), et
 * (+) une liste avec les arêtes incidentes (\p incidence_list).
 *
 * Pour notre application de trouver le plus court chemin avec l'algorithme
 * de Dijkstra, nous utiliserons aussi les attributs suivants :
 * (+) \p total_distance : la total_distance (actuelle ou finale) à partir du nœud de depart
 * (+) \p predecessor : le prédécesseur de ce sommet dans le plus court chemin
 * (+) \p dict_position : l'indice vers la position au dictionnaire du tas qui contient ce sommet.
 *     (Attribut à utiliser comme deuxième argument (\p dict_position) dans le
 *     sous-programme \p heap_increase_priority.)
 */
struct vertex_t {
	char * id;
	struct list_t * incidence_list;

	unsigned long total_distance;
	struct vertex_t * predecessor;
	unsigned int dict_position;
};

/**
 * Restitue l'identifiant du sommet \p V.
 *
 * @param[in] V
 * @return l'identifiant du sommet \p V
 */
char * get_vertex_id(const struct vertex_t * V);

/**
 * Restitue la liste avec les arêtes incidentes au sommet \p V.
 *
 * @param[in] V
 * @return la liste avec les arêtes incidentes au sommet \p V
 */
struct list_t * get_vertex_incidence_list(const struct vertex_t * V);

/**
 * Restitue la distance du sommet \p V à partir du sommet de depart
 * dans l'algorithme de Dijkstra.
 *
 * @param[in] V
 * @return l'attribut total_distance
 */
unsigned long get_vertex_total_distance(const struct vertex_t * V);

/**
 * Restitue le sommet qui precède le sommet \p V dans le plus court chemin.
 *
 * @param[in] V
 * @return le prédécesseur de \p V
 */
struct vertex_t * get_vertex_predecessor(const struct vertex_t * V);

/**
 * Restitue la position au dictionnaire du tas pour le sommet \p V.
 *
 * @param[in] V
 * @return la position au dictionnaire du sommet \p V.
 */
unsigned int get_vertex_dict_position(const struct vertex_t * V);

/**
 * Met à jour la total_distance du sommet \p V à partir du sommet de depart par \p newDistance.
 *
 * @param[in] V
 * @param[in] newDistance
 */
void set_vertex_total_distance(struct vertex_t * V, unsigned long newDistance);

/**
 * Met à jour le prédécesseur du sommet \p V dans le plus court chemin par \p newPredecessor.
 *
 * @param[in] V
 * @param[in] newPredecessor
 */
void set_vertex_predecessor(struct vertex_t * V, struct vertex_t * newPredecessor);

/**
 * Remplace la position au dictionnaire du tas qui correspond au le sommet \p V par \p newPosition.
 * En fait, cette procedure est uniquement utilisée pour initialisée cette position.
 *
 * @param[in] V
 * @param[in] newPosition
 */
void set_vertex_dict_position(struct vertex_t * V, unsigned int newPosition);

/**
 * Ajoute à la liste d'arêtes incidentes au sommet \p V, l'arête \p E.
 *
 * @param[in] V
 * @param[in] E
 */
void vertex_add_incident_edge(struct vertex_t * V, struct edge_t * E);

/**
 * Libère la mémoire du sommet \p V.
 * La liste d'incidence sera libérée mais pas la mémoire des arêtes incluses.
 *
 * @param[in] V
 */
void delete_vertex(void * V);

/**
 * Affiche tous les attributs d'un sommet \p V.
 *
 * @param[in] V
 */
void view_vertex(const void * V);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/**
 * Une arête E=(U,V) d'un graphe est caractérisée par ces deux extrémités \p U
 * et \p V ainsi que par la distance entre ces deux sommets.
 */
struct edge_t {
	struct vertex_t * U, * V;
	unsigned int distance;
};

/**
 * Restitue l'extrémité U de l'arête \p E=(U,V).
 *
 * @param[in] E
 * @return l'extrémité U de l'arête \p E
 */
struct vertex_t * get_edge_endpoint_U(const struct edge_t * E);

/**
 * Restitue l'extrémité V de l'arête \p E=(U,V).
 *
 * @param[in] E
 * @return l'extrémité V de l'arête \p E
 */
struct vertex_t * get_edge_endpoint_V(const struct edge_t * E);

/**
 * Restitue la distance entre les sommets U et V de l'arête \p E=(U,V).
 *
 * @param[in] E
 * @return la distance entre les extrémités de l'arête
 */
unsigned int get_edge_distance(const struct edge_t * E);

/**
 * Libère la mémoire de l'arête \p E.
 *
 * @param[in] E
 */
void delete_edge(void * E);

/**
 * Affiche les attributs d'une arête \p edge.
 *
 * @param[in] edge
 */
void view_edge(const void * edge);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/**
 * Un graphe est un tableau (dynamique) contenant les sommets.
 */
typedef struct dyn_table_t * graph;

/**
 * Construit le graphe décrit dans le fichier \p filename.
 *
 * @param[in] filename
 * @return le graphe créé
 */
graph read_graph(const char * filename);

/**
 * Construit et restitue la liste d'arêtes du graphe \p G
 *
 * @param[in] G
 * @return la liste d'arêtes du graphe \p G
 */
struct list_t * get_graph_edges(graph G);

/**
 * Visualise le graphe \p G.
 *
 * @param[in] G
 */
void view_graph(graph G);

/**
 * Libère la mémoire du graphe \p G.
 *
 * @param[in] G
 */
void delete_graph(graph G);

/**
 * Re-initialise les trois caractéristiques des sommets du graphe
 * concernant l'algorithme de Dijkstra : total_distance, predecessor et dict_position .
 *
 * @param[in] G
 */
void reset_graph(graph G);