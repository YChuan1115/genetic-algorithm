#pragma once

#include <array>
#include <vector>

template <class Allele, int NumAllele>
class Chromosome{
public:
    using DesignVariables = std::array<Allele, NumAllele >;

    DesignVariables* designVariables(){return design_variables_;}

    Chromosome(){
        design_variables_ = new DesignVariables;
    }

    ~Chromosome(){
        delete design_variables_;
    }

    double fit;
    double probability;
    double cumulative_prob;

private:
    DesignVariables* design_variables_;    

};
