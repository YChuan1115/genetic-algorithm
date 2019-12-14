/**
*   @author : koseng (Lintang)
*   @brief : Example use of GA
*/

#include <iostream>
#include <armadillo>

#include "genetic_algorithm.h"

constexpr auto POPULATION_SIZE(100);
constexpr auto NUM_DESIGN_VARIABLES(6);
constexpr auto DESIGN_VARIABLE_SIZE(1);

using GA = GeneticAlgorithm<double, POPULATION_SIZE, NUM_DESIGN_VARIABLES, DESIGN_VARIABLE_SIZE >;

int main(int argc, char** argv){

    arma::mat nom_A;
    nom_A << 0.9429 << -.0759 << arma::endr
          << .0759 << 0.9970     << arma::endr;

    arma::mat nom_B;
    nom_B << .0459 << arma::endr
          << .0718 << arma::endr;

    arma::mat nom_C(2, 2, arma::fill::eye);

    //-- Store it in file !!!
    arma::mat output_data;
//    output_data << 0.0183 << 0.0289 << arma::endr
//                << 0.0797 << 0.1313 << arma::endr
//                << 0.1297 << 0.2379 << arma::endr
//                << 0.1598 << 0.3339 << arma::endr
//                << 0.1406 << 0.3689 << arma::endr
//                << 0.0959 << 0.3649 << arma::endr
//                << 0.0819 << 0.4011 << arma::endr
//                << 0.0382 << 0.3927 << arma::endr
//                << 0.0279 << 0.4283 << arma::endr
//                << 0.0576 << 0.5290 << arma::endr
//                << 0.0672 << 0.6148 << arma::endr
//                << 0.0331 << 0.6437 << arma::endr
//                << 0.0236 << 0.7088 << arma::endr
//                << 0.0479 << 0.8328 << arma::endr
//                << 0.0290 << 0.9075 << arma::endr
//                << -0.0019 << 0.9690 << arma::endr
//                << -0.0348 << 1.0293 << arma::endr
//                << -0.1107 << 1.0240 << arma::endr
//                << -0.1824 << 1.0121 << arma::endr
//                << -0.2283 << 1.0274 << arma::endr
//                << -0.2884 << 1.0146 << arma::endr
//                << -0.3515 << 0.9857 << arma::endr
//                << -0.3987 << 0.9679 << arma::endr
//                << -0.4611 << 0.9166 << arma::endr
//                << -0.4820 << 0.9138 << arma::endr
//                << -0.4442 << 0.9990 << arma::endr
//                << -0.4374 << 1.0518 << arma::endr
//                << -0.4476 << 1.0854 << arma::endr
//                << -0.4774 << 1.0904 << arma::endr
//                << -0.4820 << 1.1306 << arma::endr;
    output_data << 0.00000 << 0.00000 << arma::endr
                << 0.00018 << 0.00028 << arma::endr
                << 0.00369 << 0.00584 << arma::endr
                << 0.01398 << 0.02227 << arma::endr
                << 0.01865 << 0.03021 << arma::endr
                << 0.02834 << 0.04635 << arma::endr
                << 0.02583 << 0.04366 << arma::endr
                << 0.03113 << 0.05337 << arma::endr
                << 0.03465 << 0.06063 << arma::endr
                << 0.03490 << 0.06305 << arma::endr
                << 0.04501 << 0.08132 << arma::endr
                << 0.05489 << 0.09989 << arma::endr
                << 0.06442 << 0.11864 << arma::endr
                << 0.06828 << 0.12921 << arma::endr
                << 0.08406 << 0.15932 << arma::endr
                << 0.09748 << 0.18691 << arma::endr
                << 0.10892 << 0.21265 << arma::endr
                << 0.12280 << 0.24359 << arma::endr
                << 0.13645 << 0.27575 << arma::endr
                << 0.14414 << 0.30027 << arma::endr
                << 0.15931 << 0.33837 << arma::endr
                << 0.17308 << 0.37652 << arma::endr
                << 0.18719 << 0.41767 << arma::endr
                << 0.20151 << 0.46193 << arma::endr
                << 0.21983 << 0.51560 << arma::endr
                << 0.23256 << 0.56407 << arma::endr
                << 0.26018 << 0.63991 << arma::endr
                << 0.28779 << 0.72062 << arma::endr
                << 0.31490 << 0.80597 << arma::endr
                << 0.34390 << 0.90038 << arma::endr;
//                << 0.37138 << 0.99922 << arma::endr;

    arma::mat state_data;
//    state_data << 0.0017 << 0.0026 << arma::endr
//             << 0.0349 << 0.0552 << arma::endr
//             << 0.1245 << 0.2075 << arma::endr
//             << 0.1350 << 0.2684 << arma::endr
//             << 0.1847 << 0.3994 << arma::endr
//             << 0.0966 << 0.3384 << arma::endr
//             << 0.0952 << 0.3914 << arma::endr
//             << 0.0686 << 0.4108 << arma::endr
//             << 0.0078 << 0.3747 << arma::endr
//             << 0.0479 << 0.4820 << arma::endr
//             << 0.0673 << 0.5760 << arma::endr
//             << 0.0672 << 0.6536 << arma::endr
//             << -0.0009 << 0.6339 << arma::endr
//             << 0.0481 << 0.7838 << arma::endr
//             << 0.0477 << 0.8817 << arma::endr
//             << 0.0104 << 0.9333 << arma::endr
//             << -0.0141 << 1.0048 << arma::endr
//             << -0.0555 << 1.0539 << arma::endr
//             << -0.1659 << 0.9941 << arma::endr
//             << -0.1989 << 1.0301 << arma::endr
//             << -0.2576 << 1.0248 << arma::endr
//             << -0.3192 << 1.0045 << arma::endr
//             << -0.3838 << 0.9670 << arma::endr
//             << -0.4136 << 0.9689 << arma::endr
//             << -0.5085 << 0.8643 << arma::endr
//             << -0.4554 << 0.9634 << arma::endr
//             << -0.4330 << 1.0346 << arma::endr
//             << -0.4418 << 1.0691 << arma::endr
//             << -0.4534 << 1.1017 << arma::endr
//             << -0.5015 << 1.0791 << arma::endr;
   state_data       << 0.00000 << 0.00000 << arma::endr
                    << 0.00018 << 0.00028 << arma::endr
                    << 0.00369 << 0.00584 << arma::endr
                    << 0.01398 << 0.02227 << arma::endr
                    << 0.01865 << 0.03021 << arma::endr
                    << 0.02834 << 0.04635 << arma::endr
                    << 0.02583 << 0.04366 << arma::endr
                    << 0.03113 << 0.05337 << arma::endr
                    << 0.03465 << 0.06063 << arma::endr
                    << 0.03490 << 0.06305 << arma::endr
                    << 0.04501 << 0.08132 << arma::endr
                    << 0.05489 << 0.09989 << arma::endr
                    << 0.06442 << 0.11864 << arma::endr
                    << 0.06828 << 0.12921 << arma::endr
                    << 0.08406 << 0.15932 << arma::endr
                    << 0.09748 << 0.18691 << arma::endr
                    << 0.10892 << 0.21265 << arma::endr
                    << 0.12280 << 0.24359 << arma::endr
                    << 0.13645 << 0.27575 << arma::endr
                    << 0.14414 << 0.30027 << arma::endr
                    << 0.15931 << 0.33837 << arma::endr
                    << 0.17308 << 0.37652 << arma::endr
                    << 0.18719 << 0.41767 << arma::endr
                    << 0.20151 << 0.46193 << arma::endr
                    << 0.21983 << 0.51560 << arma::endr
                    << 0.23256 << 0.56407 << arma::endr
                    << 0.26018 << 0.63991 << arma::endr
                    << 0.28779 << 0.72062 << arma::endr
                    << 0.31490 << 0.80597 << arma::endr
                    << 0.34390 << 0.90038 << arma::endr;
//                    << 0.37138 << 0.99922 << arma::endr;
//    arma::mat state_data;
//    state_data << 0.0008 << 0.0013 << arma::endr
//                << 0.0183 << 0.0289 << arma::endr
//                << 0.0797 << 0.1313 << arma::endr
//                << 0.1297 << 0.2379 << arma::endr
//                << 0.1598 << 0.3339 << arma::endr
//                << 0.1406 << 0.3689 << arma::endr
//                << 0.0959 << 0.3649 << arma::endr
//                << 0.0819 << 0.4011 << arma::endr
//                << 0.0382 << 0.3927 << arma::endr
//                << 0.0279 << 0.4283 << arma::endr
//                << 0.0576 << 0.5290 << arma::endr
//                << 0.0672 << 0.6148 << arma::endr
//                << 0.0331 << 0.6437 << arma::endr
//                << 0.0236 << 0.7088 << arma::endr
//                << 0.0479 << 0.8328 << arma::endr
//                << 0.0290 << 0.9075 << arma::endr
//                << -0.0019 << 0.9690 << arma::endr
//                << -0.0348 << 1.0293 << arma::endr
//                << -0.1107 << 1.0240 << arma::endr
//                << -0.1824 << 1.0121 << arma::endr
//                << -0.2283 << 1.0274 << arma::endr
//                << -0.2884 << 1.0146 << arma::endr
//                << -0.3515 << 0.9857 << arma::endr
//                << -0.3987 << 0.9679 << arma::endr
//                << -0.4611 << 0.9166 << arma::endr
//                << -0.4820 << 0.9138 << arma::endr
//                << -0.4442 << 0.9990 << arma::endr
//                << -0.4374 << 1.0518 << arma::endr
//                << -0.4476 << 1.0854 << arma::endr
//                << -0.4774 << 1.0904 << arma::endr;

    arma::mat input_data;
    input_data << 0.0367 << 0.7306 << 2.0859 << 0.7253 << 1.6935 << -1.0283 << 0.6502 << 0.1858 << -0.5590 << 1.5022
               << 1.2788 << 1.0331 << -0.3184 << 2.1146 << 1.3462 << 0.7044 << 1.0226 << 0.7410 << -0.7302 << 0.7176
               << 0.1782 << 0.0320 << -0.1432 << 0.4726 << -0.9797 << 1.9532 << 1.5141 << 0.9802 << 0.9656 << 0.2110 << arma::endr;

    arma::mat psi = arma::join_horiz(state_data, input_data.t());

    GA genetic;

    genetic.setCrossoverProb() = .5;
    genetic.setMutationProb() = .01;
    genetic.setNumGenerations() = 1000;
    genetic.setStdDevTol() = .01;
    genetic.setObjective() = [=](GA::GAStr x){
        GA::DV* dv = x.designVariables();
        arma::mat est_A;
        est_A << (*dv)[0].value << (*dv)[1].value << arma::endr
              << (*dv)[2].value << (*dv)[3].value << arma::endr;

//        est_A.print("Est. A : ");

        arma::mat est_B;
        est_B << (*dv)[4].value << arma::endr
              << (*dv)[5].value << arma::endr;

//        est_B.print("Est. B : ");
        //-- test
//        est_A = nom_A;
//        est_B = nom_B;
        arma::mat theta = arma::join_vert( est_A.t() * nom_C.t(), est_B.t() * nom_C.t() );

        arma::mat error( output_data -  psi*theta );
        auto val = arma::norm(error, 2);
//        std::cout << "Error mag. : " << val << std::endl;
        return val;
    };

//    genetic.addInequalityConstraint(
//                GA::InequalityConstraint{[](GA::GAStr x){
//                                             GA::DV* dv = x.designVariables();
//                                             return (*dv)[0].value + (*dv)[3].value;
//                                         }, 1.0});

//    genetic.addInequalityConstraint(
//                GA::InequalityConstraint{[](GA::GAStr x){
//                                             GA::DV* dv = x.designVariables();
//                                             return std::pow((*dv)[0].value + (*dv)[3].value, 2.)
//                                                    - 4. * ( ((*dv)[0].value * (*dv)[3].value) - ((*dv)[1].value * (*dv)[2].value) );
//                                         }, 1.0});

    genetic.addInequalityConstraint(
                GA::InequalityConstraint{[](GA::GAStr x){
                                             GA::DV* dv = x.designVariables();
                                             auto b = (*dv)[0].value + (*dv)[3].value;
                                             auto discriminant = std::pow(b, 2.)
                                                    - 4. * ( ((*dv)[0].value * (*dv)[3].value) - ((*dv)[1].value * (*dv)[2].value) );
                                             auto ret(.0); //-- RVO
                                             if(discriminant < .0){ //-- if complex number
                                                 ret = std::sqrt( std::pow(b * .5, 2) + (discriminant * .25) );
                                             }else{
                                                 ret = std::fabs( (b + std::sqrt(discriminant)) * .5 ) + std::fabs( (b - std::sqrt(discriminant)) * .5 );
                                             }
                                             return ret;
                                         }, 0.01});

    std::cout << "Preparing GA..." << std::endl;
    genetic.initialization();
    std::cout << "Solving..." << std::endl;
    genetic.generations();

    //test print
    std::cout << "Possible system : " << std::endl;
    arma::mat best_est_A;
    arma::mat best_est_B;
    arma::mat worst_est_A;
    arma::mat worst_est_B;
    auto best_fitness(.0);
    auto worst_fitness(1.);
    for(auto p:genetic.population()){
        GA::DV* dv = p.designVariables();
        arma::mat est_A;
        est_A << (*dv)[0].value << (*dv)[1].value << arma::endr
              << (*dv)[2].value << (*dv)[3].value << arma::endr;
//        est_A.print("Est. A : ");

        arma::mat est_B;
        est_B << (*dv)[4].value << arma::endr
              << (*dv)[5].value << arma::endr;
//        est_B.print("Est. B : ");
//        std::cout << "==========================================" << std::endl;

        if(p.getFit() > best_fitness){
            best_fitness = p.getFit();
            best_est_A = est_A;
            best_est_B = est_B;
        }

        if(p.getFit() < worst_fitness){
            worst_est_A = est_A;
            worst_est_B = est_B;
            worst_fitness = p.getFit();
        }
    }

    best_est_A.print("A = ");
    best_est_B.print("B = ");
    std::cout << "With fitness : " << best_fitness << std::endl;

    worst_est_A.print("Worst A : ");
    worst_est_B.print("Worst B : ");
    std::cout << "With fitness : " << worst_fitness << std::endl;

    arma::cx_vec eigval;
    arma::cx_mat eigvec;
    arma::eig_gen(eigval, eigvec, best_est_A);

    eigval.print("Eigenvalue : ");

    return 0;
}
