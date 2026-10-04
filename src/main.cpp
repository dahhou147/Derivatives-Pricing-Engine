#include <iostream>


class Buffer{
    
    private: 
    int size; 
    double * data; 

    public : 

    Buffer(int size_): size(size_){
        data = new double[size];
    }

    // Constructeur de copie pour copier les données d'un autre Buffer
    Buffer(const Buffer& other): size(other.size){
        data = new double[size];
        for(int i = 0; i < size; ++i){
            data[i] = other.data[i];
        }
    }
    // Constructeur de déplacement pour transférer la propriété des données d'un autre Buffer
    Buffer(Buffer&& other) noexcept : size(other.size), data(other.data) {
        other.data = nullptr; // Éviter la double libération de mémoire
    }
    // c'est un operateur de mouvement pour transférer la propriété des données d'un autre Buffer
    Buffer operator=(Buffer&& other) noexcept {
        if (this !=& other){
            delete[] data; // Libérer la mémoire existante
            size = other.size;
            data = other.data;
            other.data = nullptr; // Éviter la double libération de mémoire
        }
        return *this;
    }
    // Destructeur pour libérer la mémoire allouée
    ~Buffer(){
        delete[] data;
    }

    double & operator[](int index){
        return data[index];
    }
};

template <typename F, typename arg>
auto make_call(F&& f, arg&& a) {
    return std::forward<F>(f)(std::forward<arg>(a));
}
/*
&& le forward est utilisé pour transmettre les arguments à la fonction f de manière efficace, en préservant leur type et leur valeur (lvalue ou rvalue). 
Cela permet d'éviter des copies inutiles et d'optimiser les performances, surtout lorsque les arguments sont des objets volumineux ou complexes.

*/


int main() {
    auto f = [](int x) { return x * x; };
    int value = 5;
    int result = make_call(f, value);
    std::cout << "Result: " << result << std::endl; // Affiche "Result: 25"
    return 0;
}