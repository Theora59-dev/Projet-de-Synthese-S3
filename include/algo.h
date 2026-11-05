#pragma once

#include "graph.h"

/**
 * Implementation de l'algorithme de Dijkstra pour trouver le plus court chemin
 * à partir d'un sommet de départ vers tous les autres sommets d'un graph.
 * Cette implementation utilise une file de priorité représentée par un tas-min.
 */

/**
 * Algorithme de Dijkstra
 * G : un graphe
 * source_name : nom du sommet de départ
 * heap_type : type du tas {0--tableaux dynamiques, 1--arbres binaires complets, 2--listes ordonnées}
 *
 * @param G
 * @param source_name
 * @param heap_type
 */
void Dijkstra(graph G, const char * source_name, int heap_type);

/**
 * Affichage de la solution de l'algorithme de Dijkstra en utilisant
 * comme sommet de depart le sommet \p source_name.
 * Pour chaque sommet du graphe \p G on doit déjà avoir défini
 * les valeurs total_distance et predecessor en exécutant l'algorithme de Dijkstra.
 *
 * @param G
 * @param source_name
 */
void view_solution(graph G, const char * source_name);

/**
 * Enregistrer dans le fichier \p out_filename la solution de l'algorithme de Dijkstra
 * en utilisant comme sommet de depart le sommet \p source_name.
 * Pour chaque sommet du graphe \p G on doit déjà avoir défini
 * les valeurs total_distance et predecessor en exécutant l'algorithme de Dijkstra.
 *
 * @param G
 * @param source_name
 */
void save_solution(const char * out_filename, graph G, const char * source_name);