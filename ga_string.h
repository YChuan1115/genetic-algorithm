/**
*   @author : koseng (Lintang)
*   @brief : For wrapper the chromosome template
*/
#pragma once

#include <chromosome.h>

template <int NumAlleles>
class GAString{
private:
    struct BinaryAllele{
        using byte = unsigned char;
        byte value;
        BinaryAllele(){
            value = 0;
        }
        BinaryAllele& operator~(){
            value = (value) ? 0 : 1;
            return *this;
        }
    };

public:
    using Chr = Chromosome<BinaryAllele, NumAlleles >;

private:
    Chr chromosome_;

    typename Chr::DesignVariables* dv_;

public:
    GAString(){
        dv_ = chromosome_.designVariables();
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

};
