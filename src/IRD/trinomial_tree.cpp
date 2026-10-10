#include <cmath>
#include <iostream>
#include <vector>

struct Node {
    double value= 0.0; // la valeur du noeud
    std::vector<Node*> children;
    Node(double val) : value(val) {}
};


class TreeModel {
    public :
    int branching_number = 0; // le nombre de branches du modèle d'arbre soit 2 pour le modèle binomial, soit 3 pour le modèle trinomial
    int number_of_steps = 0; // le nombre d'étapes du modèle d'arbre
    int j_max = 0; // le nombre maximum de branches à chaque étape du modèle d'arbre
    int j_min = 0; // le nombre minimum de branches à chaque étape du modèle d'arbre
    double dt = 0.0; // le pas de temps du modèle d'arbre
    double delta_x = 0.0; // le pas de l'arbre du modèle d'arbre
};