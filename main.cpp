/**
*   @author : koseng (Lintang)
*   @brief : Example use of GA
*/

#include <iostream>

#include "genetic_algorithm.h"

constexpr auto POPULATION_SIZE(20);
constexpr auto NUM_DESIGN_VARIABLES(10);
constexpr auto DESIGN_VARIABLE_SIZE(10);

using GA = GeneticAlgorithm<POPULATION_SIZE, NUM_DESIGN_VARIABLES, DESIGN_VARIABLE_SIZE >;

int main(int argc, char** argv){

    std::vector<double > objective_const{2.0,1.0,0.5,5.0,3.0,
                                         1.5,2.5,5.5,4.0,2.2};
    GA genetic;

    genetic.setCrossoverProb() = .7;
    genetic.setMutationProb() = .1;
    genetic.setNumGenerations() = 10;
    genetic.setStdDevTol() = 1.0;
    genetic.setObjective() = [objective_const](GA::GAStr x){
        auto result(.0);

        GA::DV* dv = x.designVariables();
        for(size_t i(0); i < DESIGN_VARIABLE_SIZE; i++){
            result += objective_const[i] * (*dv)[i].value;
        }
        return result;
    };

    genetic.addEqualityConstraint(
                GA::EqualityConstraint{[](GA::GAStr x){
                                           auto result(.0);

                                           GA::DV* dv = x.designVariables();
                                           for(size_t i(0); i < DESIGN_VARIABLE_SIZE; i++){
                                               result += (*dv)[i].value;
                                           }
                                           return result;
                                       }, 1.0});

    genetic.addInequalityConstraint(
                GA::InequalityConstraint{[](GA::GAStr x){
                                             auto result(.0);

                                             GA::DV* dv = x.designVariables();
                                             for(size_t i(0); i < DESIGN_VARIABLE_SIZE; i++){
                                                 result += (i%2) * (*dv)[i].value;
                                             }
                                             return result;
                                         }, .5});

    genetic.initialization();

    genetic.generations();

    //test print
    for(auto p:genetic.population()){
        GA::DV* dv = p.designVariables();
        for(auto v:(*dv)){
            std::cout << (int)v.value << std::endl;
        }
    }

    return 0;
}
