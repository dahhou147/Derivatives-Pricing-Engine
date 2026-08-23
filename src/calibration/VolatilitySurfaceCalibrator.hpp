#ifndef VOLATILITYSURFACECALIBRATOR_HPP
#define VOLATILITYSURFACECALIBRATOR_HPP
#include <types.hpp>
#include <vector>
// ce que je veux calibrere la surface de vol sur les prix de marchée

// faire std::vector<Impliedvol> transformer cela en fichier JSON qui sera apres utiliser pour
// construire la surface ou interpoler les points les points de la surface de vol on calibre pas le
// modele de BS sur les prix de marché mais sur les volatilités implicites description :
//    * on fait un recuperer de marché juste le strike est la maturité on connait deja le spot et
//    recuperer les taux d'interet
// qui corresponds a cette maturité
// verifier

class BSCalibrator {
private:
    std::vector<MarketOptionData> market_data; // les
    std::vector<ImpliedVol> results;

public:
    BSCalibrator(std::vector<MarketOptionData> & market_data_) : market_data(market_data_) {};

    void fit();

    std::vector<ImpliedVol> get_ivs() const;
};


class HestonCalibrator{
    private: 
    std::vector<MarketOptionData> market_data;
    HestonParams params;

    public : 

    HestonCalibrator(std::vector<MarketOptionData> & market_data_): market_data(market_data_){}


    void fit();

    HestonParams GetParams() const; 
    void SetParams(const HestonParams & calibrated_params); 
};

#endif