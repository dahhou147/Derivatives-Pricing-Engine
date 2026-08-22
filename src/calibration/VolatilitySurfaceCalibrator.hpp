#ifndef VOLATILITYSURFACECALIBRATOR_HPP
#define VOLATILITYSURFACECALIBRATOR_HPP
#include <vector>
// ce que je veux calibrere la surface de vol sur les prix de marchée

struct ImpliedVol {
    double spot;
    double strike;
    double market_price;
    double maturity;
    double implied_vol;
};

struct MarketData {
    double strike;
    double maturity;
    double price;
};

// faire std::vector<Impliedvol> transformer cela en fichier JSON qui sera apres utiliser pour
// construire la surface ou interpoler les points les points de la surface de vol on calibre pas le
// modele de BS sur les prix de marché mais sur les volatilités implicites description :
//    * on fait un recuperer de marché juste le strike est la maturité on connait deja le spot et
//    recuperer les taux d'interet
// qui corresponds a cette maturité
// verifier

class BSCalibrator {
private:
    std::vector<MarketData> market_data; // les
    std::vector<ImpliedVol> results;

public:
    BSCalibrator(std::vector<MarketData> market_data_) : market_data(market_data_) {};

    std::vector<ImpliedVol> calibrator() const {};
};

#endif