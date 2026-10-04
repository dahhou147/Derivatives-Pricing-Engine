#include <vector>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <functional>

struct Fwd {
    double val;
    std::vector<double> grad;   // taille d
};

Fwd operator+(const Fwd& a, const Fwd& b) {
    Fwd r{a.val + b.val, std::vector<double>(a.grad.size())};
    for (size_t i = 0; i < a.grad.size(); ++i)
        r.grad[i] = a.grad[i] + b.grad[i];
    return r;
}

Fwd operator*(const Fwd& a, const Fwd& b) {
    Fwd r{a.val * b.val, std::vector<double>(a.grad.size())};
    for (size_t i = 0; i < a.grad.size(); ++i)
        r.grad[i] = b.val * a.grad[i] + a.val * b.grad[i];
    return r;
}

Fwd sin(const Fwd& a) {
    Fwd r{std::sin(a.val), std::vector<double>(a.grad.size())};
    for (size_t i = 0; i < a.grad.size(); ++i)
        r.grad[i] = std::cos(a.val) * a.grad[i];
    return r;
}

struct Node;

struct Edge {
    Node* parent;
    double derivative;
};

struct Node {
    double val = 0.0;
    double adjoint = 0.0;
    std::vector<Edge> parents;
};

struct Tape {
    std::vector<std::unique_ptr<Node>> nodes;

    Node* create_node(double val) {
        auto node = std::make_unique<Node>();
        node->val = val;
        Node* ptr = node.get();
        nodes.push_back(std::move(node));
        return ptr;
    }

    // --- Opérations arithmétiques ---
    size_t mark() const {
        return nodes.size();
    }
    Node* add(Node* a, Node* b) {
        Node* r = create_node(a->val + b->val);
        r->parents.push_back({a, 1.0});
        r->parents.push_back({b, 1.0});
        return r;
    }

    Node* sub(Node* a, Node* b) {
        Node* r = create_node(a->val - b->val);
        r->parents.push_back({a, 1.0});
        r->parents.push_back({b, -1.0});
        return r;
    }

    Node* mul(Node* a, Node* b) {
        Node* r = create_node(a->val * b->val);
        r->parents.push_back({a, b->val});
        r->parents.push_back({b, a->val});
        return r;
    }

    Node* div(Node* a, Node* b) {
        Node* r = create_node(a->val / b->val);
        r->parents.push_back({a, 1.0 / b->val});
        r->parents.push_back({b, -a->val / (b->val * b->val)});
        return r;
    }

    Node* sin(Node* a) {
        Node* r = create_node(std::sin(a->val));
        r->parents.push_back({a, std::cos(a->val)});
        return r;
    }

    Node* cos(Node* a) {
        Node* r = create_node(std::cos(a->val));
        r->parents.push_back({a, -std::sin(a->val)});
        return r;
    }

    Node* exp(Node* a) {
        double e = std::exp(a->val);
        Node* r = create_node(e);
        r->parents.push_back({a, e});
        return r;
    }

    Node* log(Node* a) {
        Node* r = create_node(std::log(a->val));
        r->parents.push_back({a, 1.0 / a->val});
        return r;
    }
    Node * max(Node * a, Node * b) {
        Node * r = create_node(std::max(a->val, b->val));
        if (a->val > b->val) {
            r->parents.push_back({a, 1.0});
            r->parents.push_back({b, 0.0});
        } else {
            r->parents.push_back({a, 0.0});
            r->parents.push_back({b, 1.0});
        }
        return r;
    }

    // --- Reverse ---

    void backward(Node* output) {
        output->adjoint = 1.0;
        for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
            Node* n = it->get();
            for (auto& e : n->parents) {
                e.parent->adjoint += n->adjoint * e.derivative;
            }
        }
    }

    void backward_from(Node* output, size_t mark) {
        output->adjoint = 1.0;
        for (size_t i = nodes.size() - 1; i >= mark; --i) {
            Node* n = nodes[i].get();
            for (auto& e : n->parents) {
                e.parent->adjoint += n->adjoint * e.derivative;
            }
        }
    }
    void backward_prefix(size_t mark) {
        for (size_t i =mark; i-- > 0; ) {
            Node* n = nodes[i].get();
            for (auto& e : n->parents) {
                e.parent->adjoint += n->adjoint * e.derivative;
            }
        }
    }

    void rewind(size_t mark) {
        nodes.erase(nodes.begin() + mark, nodes.end());
    }

    Node * multi_mc(Node * a, double b) {
        Node * r = create_node(a->val * b);
        r->parents.push_back({a, b});
        return r;
    }
    Node * square(Node * a) {
        Node * r = create_node(a->val * a->val);
        r->parents.push_back({a, 2.0 * a->val});
        return r;
    }
    Node * sqrt(Node * a) {
        Node * r = create_node(std::sqrt(a->val));
        r->parents.push_back({a, 0.5 / std::sqrt(a->val)});
        return r;
    }

    void reset() {
        for (auto& n : nodes) n->adjoint = 0.0;
    }

    void clear() {
        nodes.clear();
    }
};

int main() {
    // on va calculer les derivées de par simulation de Monte Carlo d'une option européenne
    Tape tape;
    Node * spot_0 = tape.create_node(100.0);  // prix du sous-jacent
    Node * strike = tape.create_node(100.0); // prix d'exercice
    Node * maturity = tape.create_node(1.0); // maturité en années
    Node * rate = tape.create_node(0.05);     // taux d'intérêt sans risque
    Node * sigma = tape.create_node(0.2); // volatilité
    int nums_steps = 1000;
    int nums_paths = 10000;
    Node* sigma_sq = tape.square(sigma);
    Node * dt = tape.multi_mc(maturity, 1.0 / nums_steps);
    Node *  drift= tape.sub(rate, tape.multi_mc(sigma_sq, 0.5));
    Node * drift_dt = tape.mul(drift, dt);
    Node * sigma_sqrt_dt = tape.mul(sigma, tape.sqrt(dt));
    std::mt19937 gen(12345);  // seed fixe pour la reproducibilité
    std::normal_distribution<double> normal(0.0, 1.0);

    size_t mark = tape.mark();
    for (int i = 0; i<nums_paths; ++i) {
        Node * spot = spot_0;
        for (int j = 0; j < nums_steps; ++j) {
            double z = normal(gen);
            Node * diffusion = tape.multi_mc(sigma_sqrt_dt, z);
            Node * increment = tape.add(drift_dt, diffusion);
            spot = tape.mul(spot, tape.exp(increment));
        }
        Node * discount_factor = tape.exp(tape.mul(rate, tape.multi_mc(maturity, -1.0)));
        Node * payoff = tape.max(tape.sub(spot, strike), tape.create_node(0.0));
        Node * discounted_payoff = tape.mul(discount_factor, payoff);
        tape.backward_from(discounted_payoff, mark);
        tape.rewind(mark);
    }
    tape.backward_prefix(mark);
    std::cout << "Delta: " << spot_0->adjoint/nums_paths << std::endl;
    std::cout << "Vega: " << sigma->adjoint/nums_paths << std::endl;
    std::cout << "Rho: " << rate->adjoint/nums_paths << std::endl;
    std::cout << "Theta: " << maturity->adjoint/nums_paths << std::endl;
}