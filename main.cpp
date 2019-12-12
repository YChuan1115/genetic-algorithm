/**
*   @author : koseng (Lintang)
*   @brief : Example use of GA
*/

#include <iostream>
#include <armadillo>

#include "genetic_algorithm.h"

constexpr auto POPULATION_SIZE(30);
constexpr auto NUM_DESIGN_VARIABLES(6);
constexpr auto DESIGN_VARIABLE_SIZE(1);

using GA = GeneticAlgorithm<double, POPULATION_SIZE, NUM_DESIGN_VARIABLES, DESIGN_VARIABLE_SIZE >;

int main(int argc, char** argv){

    arma::mat nom_A;
    nom_A << -.5572 << -.7814 << arma::endr
          << .7814  << 0.     << arma::endr;

    arma::mat nom_B;
    nom_B << .5 << arma::endr
          << .7 << arma::endr;

    arma::mat nom_C(2, 2, arma::fill::eye);

    //-- Store it in file !!!
    arma::mat output_data;
    output_data << 0.0183 << 0.0289 << arma::endr
                << 0.0797 << 0.1313 << arma::endr
                << 0.1297 << 0.2379 << arma::endr
                << 0.1598 << 0.3339 << arma::endr
                << 0.1406 << 0.3689 << arma::endr
                << 0.0959 << 0.3649 << arma::endr
                << 0.0819 << 0.4011 << arma::endr
                << 0.0382 << 0.3927 << arma::endr
                << 0.0279 << 0.4283 << arma::endr
                << 0.0576 << 0.5290 << arma::endr
                << 0.0672 << 0.6148 << arma::endr
                << 0.0331 << 0.6437 << arma::endr
                << 0.0236 << 0.7088 << arma::endr
                << 0.0479 << 0.8328 << arma::endr
                << 0.0290 << 0.9075 << arma::endr
                << 0.0019 << 0.9690 << arma::endr
                << 0.0348 << 1.0293 << arma::endr
                << 0.1107 << 1.0240 << arma::endr
                << 0.1824 << 1.0121 << arma::endr
                << 0.2283 << 1.0274 << arma::endr
                << 0.2884 << 1.0146 << arma::endr
                << 0.3515 << 0.9857 << arma::endr
                << 0.3987 << 0.9679 << arma::endr
                << 0.4611 << 0.9166 << arma::endr
                << 0.4820 << 0.9138 << arma::endr
                << 0.4442 << 0.9990 << arma::endr
                << 0.4374 << 1.0518 << arma::endr
                << 0.4476 << 1.0854 << arma::endr
                << 0.4774 << 1.0904 << arma::endr
                << 0.4820 << 1.1306 << arma::endr;

    arma::mat state_data;
    state_data << 0.0017 << 0.0026 << arma::endr
             << 0.0349 << 0.0552 << arma::endr
             << 0.1245 << 0.2075 << arma::endr
             << 0.1350 << 0.2684 << arma::endr
             << 0.1847 << 0.3994 << arma::endr
             << 0.0966 << 0.3384 << arma::endr
             << 0.0952 << 0.3914 << arma::endr
             << 0.0686 << 0.4108 << arma::endr
             << 0.0078 << 0.3747 << arma::endr
             << 0.0479 << 0.4820 << arma::endr
             << 0.0673 << 0.5760 << arma::endr
             << 0.0672 << 0.6536 << arma::endr
             << -0.0009 << 0.6339 << arma::endr
             << 0.0481 << 0.7838 << arma::endr
             << 0.0477 << 0.8817 << arma::endr
             << 0.0104 << 0.9333 << arma::endr
             << -0.0141 << 1.0048 << arma::endr
             << -0.0555 << 1.0539 << arma::endr
             << -0.1659 << 0.9941 << arma::endr
             << -0.1989 << 1.0301 << arma::endr
             << -0.2576 << 1.0248 << arma::endr
             << -0.3192 << 1.0045 << arma::endr
             << -0.3838 << 0.9670 << arma::endr
             << -0.4136 << 0.9689 << arma::endr
             << -0.5085 << 0.8643 << arma::endr
             << -0.4554 << 0.9634 << arma::endr
             << -0.4330 << 1.0346 << arma::endr
             << -0.4418 << 1.0691 << arma::endr
             << -0.4534 << 1.1017 << arma::endr
             << -0.5015 << 1.0791 << arma::endr;

    arma::mat input_data;
    input_data << 0.0367 << 0.7306 << 2.0859 << 0.7253 << 1.6935 << 1.0283 << 0.6502 << 0.1858 << 0.5590 << 1.5022
               << 1.2788 << 1.0331 << 0.3184 << 2.1146 << 1.3462 << 0.7044 << 1.0226 << 0.7410 << 0.7302 << 0.7176
               << 0.1782 << 0.0320 << 0.1432 << 0.4726 << 0.9797 << 1.9532 << 1.5141 << 0.9802 << 0.9656 << 0.2110 << arma::endr;

    arma::mat psi = arma::join_vert(state_data.t(), input_data).t();

    GA genetic;

    genetic.setCrossoverProb() = .5;
    genetic.setMutationProb() = .2;
    genetic.setNumGenerations() = 200;
    genetic.setStdDevTol() = .1;
    genetic.setObjective() = [=](GA::GAStr x){
        GA::DV* dv = x.designVariables();
        arma::mat est_A;
        est_A << (*dv)[0].value << (*dv)[1].value << arma::endr
              << (*dv)[2].value << (*dv)[3].value << arma::endr;

        arma::mat est_B;
        est_B << (*dv)[4].value << arma::endr
              << (*dv)[5].value << arma::endr;

        arma::mat theta = arma::join_vert( est_A.t() * nom_C.t(), est_B.t() * nom_C.t() );

        arma::mat error( output_data -  psi*theta );
        auto val = arma::norm(error, 2);
        return val;
    };

    genetic.addInequalityConstraint(
                GA::InequalityConstraint{[](GA::GAStr x){
                                             GA::DV* dv = x.designVariables();
                                             return (*dv)[0].value + (*dv)[3].value;
                                         }, .5});

    genetic.addInequalityConstraint(
                GA::InequalityConstraint{[](GA::GAStr x){
                                             GA::DV* dv = x.designVariables();
                                             return std::pow((*dv)[0].value + (*dv)[3].value, 2.)
                                                    - 4. * (*dv)[1].value * (*dv)[2].value;
                                         }, .5});

    genetic.initialization();
    genetic.generations();

    //test print
    std::cout << "Possible system : " << std::endl;
    arma::mat best_est_A;
    arma::mat best_est_B;
    auto best_fitness(.0);
    for(auto p:genetic.population()){
        GA::DV* dv = p.designVariables();
        arma::mat est_A;
        est_A << (*dv)[0].value << (*dv)[1].value << arma::endr
              << (*dv)[2].value << (*dv)[3].value << arma::endr;
        est_A.print("Est. A : ");

        arma::mat est_B;
        est_B << (*dv)[4].value << arma::endr
              << (*dv)[5].value << arma::endr;
        est_B.print("Est. B : ");
        std::cout << "==========================================" << std::endl;

        if(p.getFit() > best_fitness){
            best_fitness = p.getFit();
            best_est_A = est_A;
            best_est_B = est_B;
        }
    }

    best_est_A.print("Best A : ");
    best_est_B.print("Best B : ");
    std::cout << "With fitness : " << best_fitness << std::endl;

    return 0;
}
