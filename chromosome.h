/**
* @author : koseng (Lintang)
* @brief : Chromosome
*/

#pragma once

#include <array>
#include <vector>

template <typename Allele, int num_allele>
class Chromosome{
public:
    using DesignVariables = std::array<Allele, num_allele >;

    DesignVariables* designVariables(){return design_variables_;}

    Chromosome()
        : design_variables_ (new DesignVariables)
        , fit(.0)
        , probability(.0)
        , cumulative_prob(.0){
    }

    ~Chromosome(){
//        delete design_variables_;
    }

    double fit;
    double probability;
    double cumulative_prob;

private:
    DesignVariables* design_variables_;    

};
