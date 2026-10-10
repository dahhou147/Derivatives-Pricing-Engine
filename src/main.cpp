#include <iostream>
#include <concepts>
#include <memory>
#include <cmath>
#include <optional>
#include <concepts>
#include <vector>

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
    void print() const {
        for(int i = 0; i < size; ++i){
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
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


class Model {
    private:
    std::string name ;

    public : 

    Model(std::string name_): name(name_){}

    void display_name() const {
        std::cout << "Model name: " << name << std::endl;
    }
};

void call_model(Model* model) {
    model->display_name();
}

std::optional<double> safe_divide(double numerator, double denominator) {
    if (denominator == 0.0) {
        return std::nullopt; // Retourne un std::optional vide si le dénominateur est zéro
    }
    return numerator / denominator; // Retourne le résultat de la division encapsulé dans un std::optional
}
template< typename T> concept defferentiable = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
    { a * b } -> std::same_as<T>;
    { std::sin(a) } -> std::same_as<T>;
};

template<defferentiable T>
T compute_expression(const T& x, const T& y) {
    return std::sin(x) + x * y;
}

template <typename T>
void f(T && buf) {
    std::cout << "Function f called" << std::endl;
}
// la fonction f accepte les rvlue et les lvalue cela ce qu'on appelle le forwarding reference, 
//mais dans le bloc buf devient une lvalue, pour conserver le type d'origine de buf, on utilise std::forward<T>(buf) pour le transmettre à la fonction f.

template <typename F, typename... args>
/*
definir les templates comme ça permet de créer des fonction generique et de meme creer des decorateurs.
*/
void g(F&& f, args&&... arguments) {
    (std::forward<F>(f)(std::forward<args>(arguments)), ...);
}

std::vector<int> add(const std::vector<int>& a , const std::vector<int>& b){
    std::size_t size = a.size();
    std::vector<int> result(size);
    const int* pa = a.data();
    const int* pb = b.data();
    int * res = result.data();

    for(std::size_t i =0; i< size; i++){
           res[i] = pa[i]+pb[i]; 
    }
    return result;
}
void axpy(std::size_t n, double a ,  double * __restrict_arr x,  double* __restrict_arr y ){
    for(std::size_t i = 0; i<n; i++){
        y[i] = a*x[i]+y[i]; 
    }
}

class payoff {
    public : 
    virtual  double operator()() const = 0;
};
class Put : public payoff{
    private :
    double strike;
    double spot ;
    public :
    Put(double spot, double strike): spot(spot), strike(strike){};
    double operator()()const override{
        return std::max(strike-spot, 0.0);
    }
};

class Call: public payoff{
    private:
    double strike;
    double spot;
    public: 
    Call(double spot, double strike) : spot(spot), strike(strike){};

    double operator()() const override{
        return std::max(spot-strike, 0.0);
    }
};

// crtp 
enum class Type {Call, Put};

template<class derived>
class Payoff_instrument {
    protected:  
    Type type;
    Payoff_instrument(Type type): type(type){};
    public :
    double price() const {
        return static_cast<const derived*>(this)-> payoff();
    }

};

class Call_CRTP : public Payoff_instrument<Call_CRTP>{
    private : 
    double spot;
    double strike;
    public :
    Call_CRTP(double spot, double strike): spot(spot), strike(strike), Payoff_instrument<Call_CRTP>(Type::Call){};

    double payoff() const {
        return std::max(spot-strike, 0.0);
    }
};
#include <iostream>
#include <memory>

struct Widget {
    int id;
    Widget(int i) : id(i) { std::cout << "  [Widget " << id << " créé]\n"; }
    ~Widget() { std::cout << "  [Widget " << id << " détruit]\n"; }
};

// ❌ PLUS COÛTEUX : Passage par valeur
// Copier le shared_ptr incrémente/décrémente le compteur de manière atomique.
void configurationLourde(std::shared_ptr<Widget> w) {
    std::cout << "  Dans configurationLourde, compteur = " << w.use_count() << "\n";
}

//  MIEUX/OPTIMISÉ : Passage par référence constante
// Pas de copie, pas d'incrémentation atomique. Le coût est identique à un pointeur brut.
void configurationLegere(const std::shared_ptr<Widget>& w) {
    std::cout << "  Dans configurationLegere, compteur = " << w.use_count() << "\n";
}

int main() {
    std::cout << "=== 1. Démonstration des coûts de copie ===\n";
    // Allocation unique et optimisée grâce à std::make_shared
    auto ptrPartage = std::make_shared<Widget>(42); 
    std::cout << "Compteur initial = " << ptrPartage.use_count() << "\n";

    std::cout << "\nAppel par valeur (coûteux) :\n";
    configurationLourde(ptrPartage); // Provoque des opérations atomiques cachées

    std::cout << "\nAppel par référence (gratuit) :\n";
    configurationLegere(ptrPartage); // Aucune opération atomique

    std::cout << "\n=== 2. Le piège de la mémoire avec std::make_shared ===\n";
    std::weak_ptr<Widget> ptrFaible;

    {
        // Un deuxième shared_ptr créé dans un bloc isolé
        auto ptrTemporaire = std::make_shared<Widget>(99);
        ptrFaible = ptrTemporaire; // Le weak_ptr observe l'objet
        
        std::cout << "Destruction imminente de ptrTemporaire...\n";
    } // ptrTemporaire sort du champ et est détruit

    std::cout << "\nStatut après le bloc :\n";
    if (ptrFaible.expired()) {
        std::cout << "-> L'objet Widget(99) est bien DÉTRUIT.\n";
        std::cout << "-> ATTENTION : À cause de std::make_shared, les octets occupés par\n";
        std::cout << "   le Widget ne sont pas encore rendus au système, car le bloc mémoire\n";
        std::cout << "   héberge aussi le compteur du ptrFaible encore vivant !\n";
    }

    return 0;
}
