/**
*   @author : koseng (Lintang)
*   @brief : Simple implementation of Modern Optimization with Genetic Algorithm
*/

#pragma once

#include <algorithm>
#include <functional>
#include <random>
#include <iostream>

#include "ga_string.h"

template <typename Type,
          int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
class GeneticAlgorithm{
public:
    GeneticAlgorithm();
    ~GeneticAlgorithm();    

    struct BinaryAllele{
        using byte = unsigned char;
        byte value;
        BinaryAllele():value(0){}
        BinaryAllele& operator~(){
            value = (value) ? 0 : 1;
            return *this;
        }
    };

    struct ContinuousAllele{
        double value;
        ContinuousAllele():value(.0){}
        ContinuousAllele& operator~(){
            value = randProb() * 100.;
            return *this;
        }
    };

    using GAStr = GAString<
                    std::conditional<
                        std::is_same<Type, double>::value,
                        ContinuousAllele,
                        BinaryAllele>,
                    DesignVariableSize * NumDesignVariables>;
    using DV = typename GAStr::Chr::DesignVariables;
    using Allele = typename GAStr::Allele;
    using SubGAString = std::vector<Allele >;

    struct InequalityConstraint{
    public:
        std::function<double(GAStr) > constraint;
        double gain;
    };

    struct EqualityConstraint{
    public:
        std::function<double(GAStr) > constraint;
        double gain;
    };

    void generations();

private:
    using Population = std::vector<GAStr >;

    Population population_;    

    // genetic operator
    void reproduction();
    void crossover();
    void mutation();    

    using Objective = std::function<double(GAStr) >;
    Objective objective_;

    using IneqCstrs = std::vector<InequalityConstraint >;
    using EqCstrs = std::vector<EqualityConstraint >;

    IneqCstrs ineq_cstrs_;
    EqCstrs eq_cstrs_;

    inline double penalty(const GAStr& _str){
        auto result(.0);
        result += objective_(_str);

        auto pen(.0);
        for(auto ineq:ineq_cstrs_){
            pen = bracketing(ineq.constraint(_str));
            result += ineq.gain * pen * pen;
        }

        for(auto eq:eq_cstrs_){
            pen = eq.constraint(_str);
            result += eq.gain * pen * pen;
        }

        return result;
    }

    inline double bracketing(double _value){
        return _value > .0 ? _value : .0;
    }

    inline double calcFitness(const GAStr& _str){
        return 1./(1. + penalty(_str));
    }

    void evaluateFitness(){
        for(auto& str:population_){
            str.setFit() = calcFitness(str);
        }
    }

    double totalFitness(){
        auto total_fitness(.0);
        for(auto& str:population_){
            str.setFit() = calcFitness(str);
            total_fitness += str.getFit();
        }
        return total_fitness;
    }

    double calcStdDev(){
        auto fitness_avg(totalFitness() / PopulationSize);

        auto var(.0);
        for(auto str:population_){
            var += std::pow((str.getFit() - fitness_avg), 2.);
        }

        return std::sqrt(var);
    }

public:
    void initialization();

    Objective& setObjective(){
        return objective_;
    }

    inline double& setCrossoverProb(){
        return crossover_prob_;
    }

    inline double& setMutationProb(){
        return mutation_prob_;
    }

    inline double& setStdDevTol(){
        return std_dev_tol_;
    }

    inline int& setNumGenerations(){
        return num_generations_;
    }

    inline double getCrossoverProb() const{
        return crossover_prob_;
    }

    inline double getMutationProb() const{
        return mutation_prob_;
    }

    inline double getStdDevTol() const{
        return std_dev_tol_;
    }

    inline int getNumGenerations() const{
        return num_generations_;
    }

    inline Population& population(){
        return population_;
    }

    void addInequalityConstraint(InequalityConstraint _ineq_cstr){
        ineq_cstrs_.push_back(_ineq_cstr);
    }

    void addEqualityConstraint(EqualityConstraint _eq_cstr){
        eq_cstrs_.push_back(_eq_cstr);
    }

private:
    // generator
    std::mt19937 rand_gen_;
    // distribution
    inline int randBit(){
        std::bernoulli_distribution rand_bit;
        return rand_bit(rand_gen_);
    }
    inline double randProb(){
        std::uniform_real_distribution<double > rand_prob(.0, 1.);
        return rand_prob(rand_gen_);
    }

    inline double uniIntDist(int _lower, int _upper){
        std::uniform_int_distribution<int > rand_number(_lower, _upper);
        return rand_number(rand_gen_);
    }

    double crossover_prob_;
    double mutation_prob_;
    double std_dev_tol_;
    int num_generations_;

};

template <typename Type,
          int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
GeneticAlgorithm<Type,
                 PopulationSize,
                 NumDesignVariables,
                 DesignVariableSize>::GeneticAlgorithm()
    : rand_gen_(std::random_device{}())
    , crossover_prob_(.5)
    , mutation_prob_(.5)
    , std_dev_tol_(1.0)
    , num_generations_(10)
    , population_(PopulationSize){

//    population_.resize(PopulationSize);

}

template <typename Type,
          int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
GeneticAlgorithm<Type,
                 PopulationSize,
                 NumDesignVariables,
                 DesignVariableSize>::~GeneticAlgorithm(){

}

template <typename Type,
          int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<Type,
                      PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::initialization(){

    for(auto& str:population_){
        DV* dv(str.designVariables());
        for(auto& v:(*dv)){
            v.value = randBit();
        }
    }
}

template <typename Type,
          int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<Type,
                      PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::reproduction(){
    auto total_fitness( totalFitness() );
    population_.front().setProb() = population_.front().getFit() / total_fitness;
    population_.front().setCumulativeProb() = population_.front().getProb();
    for(int i(1); i < (DesignVariableSize * NumDesignVariables); i++){
        population_[i].setProb() = population_[i].getFit() / total_fitness;
        population_[i].setCumulativeProb() = population_[i].getProb() +
                                             population_[i-1].getCumulativeProb();
    }

    Population mating_pool(PopulationSize);
    while(mating_pool.size() < PopulationSize){
        auto prob( randProb() );
        if(prob > .0 && prob <= population_.front().getCumulativeProb()){
            mating_pool.push_back(population_.front());
            continue;
        }

        for(int i(0); i < (PopulationSize - 1); i++){
            if(prob > population_[i].getCumulativeProb() &&
                    prob <= population_[i+1].getCumulativeProb()){
                mating_pool.push_back(population_[i+1]);
                break;
            }
        }
    }
}

template <typename Type,
          int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<Type,
                      PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::crossover(){
    constexpr int MINIMUM_SITE(.25 * (float)NumDesignVariables);

    std::vector<std::pair<size_t, size_t > > selected_str;

    for(int i(0); i < PopulationSize; i++){
        if(randProb() > crossover_prob_)
            selected_str.push_back(std::make_pair(i, uniIntDist(MINIMUM_SITE, NumDesignVariables - 1)));
    }
    // to make the loop index, just donate a little bit of memory
    selected_str.push_back(selected_str.front());

    for(size_t i(0); i < (selected_str.size() - 1); i++){
        DV* dv1( population_[selected_str[i].first].designVariables() );
        DV* dv2 (population_[selected_str[i+1].first].designVariables() );

        std::swap_ranges(dv1->begin() + selected_str[i].second, dv1->end(),
                         dv2->begin() + selected_str[i].second);
    }

}

template <typename Type,
          int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<Type,
                      PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::mutation(){
    // bit-wise mutation
    for(auto& str:population_){
        if(randProb() > mutation_prob_){
            size_t site(uniIntDist(0, DesignVariableSize - 1));
            DV* dv(str.designVariables());
            (*dv)[site] = ~(*dv)[site];
        }
    }
}

template <typename Type,
          int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<Type,
                      PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::generations(){
    for(int gen(0); gen < num_generations_; gen++){
        reproduction();
        crossover();
        mutation();

        if(calcStdDev() < std_dev_tol_)
            break;;
    }

}
