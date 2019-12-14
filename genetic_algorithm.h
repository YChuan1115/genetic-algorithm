/**
*   @author : koseng (Lintang)
*   @brief : Simple implementation of Modern Optimization with Genetic Algorithm
*/

#pragma once

#include <algorithm>
#include <functional>
#include <random>
#include <iostream>
#include <cassert>
#include <type_traits>

#include "ga_string.h"

#define GA_ASSERT(rule, msg) assert(rule && msg)

//#define CROSSOVER_DEBUG

template <typename Type,
          int population_size,
          int num_design_variables,
          int design_variable_size>
class GeneticAlgorithm{
public:
    GeneticAlgorithm();
    ~GeneticAlgorithm();    

//    template<typename _Type = Type, std::enable_if<std::is_integral<_Type>::value> >
//    struct is_admissible{};

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
    private:
        static std::mt19937 rand_gen_;
        static std::uniform_real_distribution<double> dist_;        
    public:
        double value;
        ContinuousAllele():value(dist_(rand_gen_)){}
        ContinuousAllele& operator~(){
            value = dist_(rand_gen_);
            return *this;
        }
    };

    using Allele = typename std::conditional_t<
                                std::is_same<Type, double>::value,
                                ContinuousAllele,
                                BinaryAllele>;
    using GAStr = GAString<Allele, design_variable_size * num_design_variables>;
    using DV = typename GAStr::Chr::DesignVariables;    
    using SubGAString = std::vector<Allele>;

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
    using Population = std::vector<GAStr>;

    Population population_;    

    // genetic operator
    void reproduction();
    void crossover();
    void mutation();    

    using Objective = std::function<double(GAStr)>;
    Objective objective_;

    using IneqCstrs = std::vector<InequalityConstraint>;
    using EqCstrs = std::vector<EqualityConstraint>;

    IneqCstrs ineq_cstrs_;
    EqCstrs eq_cstrs_;

    inline double penalty(const GAStr& _str){
        auto result(.0);
        result += objective_(_str);

        auto pen(.0);
        for(auto ineq:ineq_cstrs_){
            pen = bracketing( ineq.constraint(_str) );            
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
        return 1./( 1. + penalty(_str) );
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
//        std::cout << total_fitness << std::endl;
        return total_fitness;
    }

    double calcStdDev(){
        auto fitness_avg(totalFitness() / population_size);

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
        GA_ASSERT(_ineq_cstr.gain >= .0 && _ineq_cstr.gain <= 1.0, "Gain must be [0,1]");
        ineq_cstrs_.push_back(_ineq_cstr);
    }

    void addEqualityConstraint(EqualityConstraint _eq_cstr){
        GA_ASSERT(_eq_cstr.gain >= .0 && _eq_cstr.gain <= 1.0, "Gain must be [0,1]");
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
          int population_size,
          int num_design_variables,
          int design_variable_size>
std::mt19937 GeneticAlgorithm<Type,
                 population_size,
                 num_design_variables,
                 design_variable_size>::ContinuousAllele::rand_gen_( std::random_device{}() );

template <typename Type,
          int population_size,
          int num_design_variables,
          int design_variable_size>
std::uniform_real_distribution<double> GeneticAlgorithm<Type,
                 population_size,
                 num_design_variables,
                 design_variable_size>::ContinuousAllele::dist_(-1., 1.);

template <typename Type,
          int population_size,
          int num_design_variables,
          int design_variable_size>
GeneticAlgorithm<Type,
                 population_size,
                 num_design_variables,
                 design_variable_size>::GeneticAlgorithm()
    : rand_gen_(std::random_device{}())
    , crossover_prob_(.5)
    , mutation_prob_(.5)
    , std_dev_tol_(1.0)
    , num_generations_(10)
    , population_(population_size){

//    population_.resize(population_size);

}

template <typename Type,
          int population_size,
          int num_design_variables,
          int design_variable_size>
GeneticAlgorithm<Type,
                 population_size,
                 num_design_variables,
                 design_variable_size>::~GeneticAlgorithm(){

}

template <typename Type,
          int population_size,
          int num_design_variables,
          int design_variable_size>
void GeneticAlgorithm<Type,
                      population_size,
                      num_design_variables,
                      design_variable_size>::initialization(){

    for(auto& str:population_){
        DV* dv(str.designVariables());
        for(auto& v:(*dv)){
            if(std::is_same<Allele, ContinuousAllele>::value)
                v.value = randProb();
            else if(std::is_same<Allele, BinaryAllele>::value)
                v.value = randBit();
            else
                GA_ASSERT(false, "Unknown Type of Allele.");
        }
    }
}

template <typename Type,
          int population_size,
          int num_design_variables,
          int design_variable_size>
void GeneticAlgorithm<Type,
                      population_size,
                      num_design_variables,
                      design_variable_size>::reproduction(){
    auto total_fitness( totalFitness() );
    population_.front().setProb() = population_.front().getFit() / total_fitness;
    population_.front().setCumulativeProb() = population_.front().getProb();
    for(int i(1); i < population_size; i++){
//        std::cout << population_[i].getFit() << std::endl;
        population_[i].setProb() = population_[i].getFit() / total_fitness;
        population_[i].setCumulativeProb() = population_[i].getProb() +
                                             population_[i-1].getCumulativeProb();
    }

    Population mating_pool;
    while(mating_pool.size() < population_size){
        auto prob( randProb() );
        if(prob > .0 && prob <= population_.front().getCumulativeProb()){
            mating_pool.push_back(population_.front());
            continue;
        }

        for(int i(0); i < (population_size - 1); i++){
            if(prob > population_[i].getCumulativeProb() &&
                    prob <= population_[i+1].getCumulativeProb()){
                mating_pool.push_back(population_[i+1]);
                break;
            }
        }
    }

    population_ = mating_pool;
}

template <typename Type,
          int population_size,
          int num_design_variables,
          int design_variable_size>
void GeneticAlgorithm<Type,
                      population_size,
                      num_design_variables,
                      design_variable_size>::crossover(){

    constexpr int MINIMUM_SITE(.25 * (float)(num_design_variables * design_variable_size)); //-- 25% from num. of allele
//    constexpr int HALF_POPULATION(population_size * .5);
    constexpr int ONE_QUARTER_POPULATION(population_size * .25);

    std::sort(population_.begin(), population_.end(), [](GAStr str1, GAStr str2){
        return str1.getFit() > str2.getFit();
    });

    std::vector<std::pair<size_t, size_t > > selected_str;

    for(int i(0); i < ONE_QUARTER_POPULATION; i++){ //-- take the best string only
//        std::cout << i << ". Fit: " << population_[i].getFit() << std::endl;
        if(randProb() > (1. - crossover_prob_))
            selected_str.push_back(std::make_pair(i, uniIntDist(MINIMUM_SITE, (design_variable_size * num_design_variables) - 1)));
    }
    if(selected_str.empty())return;
    // to make the loop index, just donate a little bit of memory
    selected_str.push_back(selected_str.front());
    int target_start_idx( population_size - ( selected_str.size() * 2) );
    for(size_t i(0); i < (selected_str.size() - 1); i++){
        DV* dv1( population_[selected_str[i].first].designVariables() );
        DV* dv2( population_[selected_str[i+1].first].designVariables() );
        DV new_dv1(*dv1);
        DV new_dv2(*dv2);
#ifdef CROSSOVER_DEBUG
        std::cout << "Before : " << std::endl;
        std::cout << (*dv1)[0].value << ";" << (*dv1)[1].value << ";" << (*dv1)[2].value << ";" << (*dv1)[3].value << ";" << (*dv1)[4].value << ";" << (*dv1)[5].value << std::endl;
        std::cout << (*dv2)[0].value << ";" << (*dv2)[1].value << ";" << (*dv2)[2].value << ";" << (*dv2)[3].value << ";" << (*dv2)[4].value << ";" << (*dv2)[5].value << std::endl;
#endif
//        std::cout << "1.F : " << selected_str[i].first << " ; " <<  selected_str[i].second << std::endl;
//        std::cout << "2.F : " << selected_str[i+1].first << " ; " <<  selected_str[i+1].second << std::endl;
//        std::swap_ranges(dv1->begin() + selected_str[i].second, dv1->end(),
//                         dv2->begin() + selected_str[i].second);

        std::swap_ranges(new_dv1.begin() + selected_str[i].second, new_dv1.end(),
                         new_dv2.begin() + selected_str[i].second);

        DV* target_new_dv1 = population_[target_start_idx].designVariables();
        *target_new_dv1 = new_dv1;
        DV* target_new_dv2 = population_[target_start_idx].designVariables();
        *target_new_dv2 = new_dv2;
        target_start_idx += 2;
//        std::cout << "3. " << idx2 << std::endl;
#ifdef CROSSOVER_DEBUG
        std::cout << "After : " << "at site : " << selected_str[i].second << std::endl;
        std::cout << (*dv1)[0].value << ";" << (*dv1)[1].value << ";" << (*dv1)[2].value << ";" << (*dv1)[3].value << ";" << (*dv1)[4].value << ";" << (*dv1)[5].value << std::endl;
        std::cout << (*dv2)[0].value << ";" << (*dv2)[1].value << ";" << (*dv2)[2].value << ";" << (*dv2)[3].value << ";" << (*dv2)[4].value << ";" << (*dv2)[5].value << std::endl;
#endif
    }

}

template <typename Type,
          int population_size,
          int num_design_variables,
          int design_variable_size>
void GeneticAlgorithm<Type,
                      population_size,
                      num_design_variables,
                      design_variable_size>::mutation(){
    std::size_t site(0);
    constexpr int ONE_QUARTER_POPULATION(population_size * .25);

    for(std::size_t i(ONE_QUARTER_POPULATION); i < population_.size(); i++){
        if(randProb() > (1. - mutation_prob_)){
            site = uniIntDist(0, (design_variable_size * num_design_variables) - 1);
            DV* dv( population_[i].designVariables() );
            (*dv)[site] = ~(*dv)[site];
        }
    }
}

template <typename Type,
          int population_size,
          int num_design_variables,
          int design_variable_size>
void GeneticAlgorithm<Type,
                      population_size,
                      num_design_variables,
                      design_variable_size>::generations(){
    int gen(0);
    auto fit_std_dev(.0);
    for(; gen < num_generations_; gen++){
        reproduction();
        crossover();
        mutation();
        fit_std_dev = calcStdDev();
        std::cout << "Generation : " << gen << " with std. dev fitness : " << fit_std_dev << std::endl;
        if(fit_std_dev < std_dev_tol_){
            break;
        }
    }
    std::cout << "Finished at " << gen << " generations." << std::endl;
    std::cout << "Fitness std. dev : " << fit_std_dev << std::endl;
}
