/**
*   @author : koseng (Lintang)
*   @brief : Simple implementation of Modern Optimization with Genetic Algorithm
*/

#pragma once

#include <functional>
#include <random>

#include "ga_string.h"

template <int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
class GeneticAlgorithm{
public:
    GeneticAlgorithm();
    ~GeneticAlgorithm();

    using GAStr = GAString<DesignVariableSize * NumDesignVariables >;
    using DV = typename GAStr::Chr::DesignVariables;

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

private:
    using Population = std::vector<GAStr >;

    Population population_;    

    // genetic operator
    void reproduction();
    void crossover();
    void mutation();

    void generations();

    using Objective = std::function<double(GAStr) >;
    Objective objective_;

    using IneqCstrs = std::vector<InequalityConstraint >;
    using EqCstrs = std::vector<EqualityConstraint >;

    IneqCstrs ineq_cstrs_;
    EqCstrs eq_cstrs_;

    inline double penalty(const GAStr& _str){
        double result(.0);

        result += objective_(_str);

        auto pen(.0);

        for(auto ineq:ineq_cstrs_){
            pen = bracketing(ineq.constraint(_str));
            result += ineq.gain * pen * pen;
        }

        for(auto eq:eq_cstrs_){
            pen = eq.contraint(_str);
            result += eq.gain * pen * pen;
        }

        return result;
    }

    inline double bracketing(double _value){
        return _value > 0 ? _value : 0;
    }

    inline double calcFitness(const GAStr& _str){
        return 1.0/(1.0 + penalty(_str));
    }

    void evaluateFitness(){
        for(auto& str:population_){
            str.setFit() = calcFitness(str);
        }
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
    std::bernoulli_distribution rand_bit_;    
    inline double randProb() const{
        std::uniform_real_distribution<double > rand_prob(0,1);
        return rand_prob(rand_gen_);
    }

    inline double uniIntDist(int _lower, int _upper) const{
        std::uniform_int_distribution<int > rand_number(_lower, _upper);
        return rand_number(rand_gen_);
    }

    double crossover_prob_;
    double mutation_prob_;
    double std_dev_tol_;
    int num_generations_;

};

template <int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
GeneticAlgorithm<PopulationSize,
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

template <int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
GeneticAlgorithm<PopulationSize,
                 NumDesignVariables,
                 DesignVariableSize>::~GeneticAlgorithm(){

}

template <int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::initialization(){



    for(auto& str:population_){
        DV* dv =  str.designVariables();
        for(auto& v:(*dv)){
            v.value = rand_bit_(rand_gen_);
        }
    }
}

template <int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::reproduction(){

    auto total_fitness(.0);
    for(auto str:population_){
        total_fitness += str.getFit(str);
    }

    population_.front().setProb() = population_.front().getFit() / total_fitness;
    population_.front().setCumulativeProb() = population_.front().getProb();
    for(int i(1); i < (DesignVariableSize * NumDesignVariables); i++){
        population_[i].setProb() = population_[i].getFit() / total_fitness;
        population_[i].setCumulativeProb() = population_[i].getProb() +
                                             population_[i-1].getCumulativeProb();
    }

    Population mating_pool(PopulationSize);

    while(mating_pool.size() < PopulationSize){
        auto prob(randProb());
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

template <int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::crossover(){
    constexpr int MINIMUM_SITE(.25 * PopulationSize);
    int site(uniIntDist(MINIMUM_SITE, PopulationSize - 1));

    int selected_str1(uniIntDist(0, PopulationSize - 1));
    int selected_str2(uniIntDist(0, PopulationSize - 1));

    for(int i(site); i < (NumDesignVariables * DesignVariableSize); i++){

    }

}

template <int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::mutation(){

}

template <int PopulationSize,
          int NumDesignVariables,
          int DesignVariableSize>
void GeneticAlgorithm<PopulationSize,
                      NumDesignVariables,
                      DesignVariableSize>::generations(){

}
