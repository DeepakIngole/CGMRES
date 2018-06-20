#ifndef SIMULATOR_H
#define SIMULATOR_H


#include <iostream>
#include <fstream>
#include <chrono>
#include <eigen3/Eigen/Core>
#include "nmpc_solver.hpp"
#include "numerical_integrator.hpp"

class simulator : public numerical_integrator{
private:
    double tsim, ht;
    nmpc_model model;
public:
    void savedata(std::ofstream& x_data, std::ofstream& u_data, const double t, const Eigen::VectorXd& x, const Eigen::VectorXd& u);
    void simulation(nmpc_solver solver, const Eigen::VectorXd& x0, const double sim_time, const double sample_ht, const std::string file_name);
};

#endif