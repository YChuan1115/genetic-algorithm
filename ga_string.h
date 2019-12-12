/**
*   @author : koseng (Lintang)
*   @brief : For wrapper the chromosome template
*/
#pragma once

#include <chromosome.h>

template <typename Allele, int NumAlleles>
class GAString{
public:
    using Chr = Chromosome<Allele, NumAlleles >;

public:
    GAString(){
        dv_ = chromosome_.designVariables();
    }

    ~GAString(){

    }

    inline typename Chr::DesignVariables* designVariables(){
        return dv_;
    }

    inline double& setFit(){
        return chromosome_.fit;
    }

    inline double getFit() const{
        return chromosome_.fit;
    }

    inline double& setProb(){
        return chromosome_.probability;
    }

    inline double getProb() const{
        return chromosome_.probability;
    }

    inline double getCumulativeProb() const{
        return chromosome_.cumulative_prob;
    }

    inline double& setCumulativeProb(){
        return chromosome_.cumulative_prob;
    }

private:
    Chr chromosome_;

    typename Chr::DesignVariables* dv_;

};
