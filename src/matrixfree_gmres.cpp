#include <iostream>
#include <eigen3/Eigen/Core>
#include "matrixfree_gmres.h"


void matrixfree_gmres::matrixfree_gmres(const int dimx, const int k_max)
{
    n = dimx;
    kmax = k_max;
    h.resize(kmax+1,kmax+1);
    v.resize(kmax+1,n);
    errvec.resize(kmax+1);
}

void matrixfree_gmres::fdgmres(const double t, const Eigen::VectorXd& x0, const Eigen::MatrixXd& x, Eigen::VectorXd& s)
{
    int i, j, k;
    double beta, rho, nu, w1, w2;
    Eigen::VectorXd r(n), cvec(kmax+1), svec(kmax+1), gvec(kmax+1);

    s = 0;
    Func(t, x0, x, r);
    v.col(0) = r / r.norm();
    rho = r.norm();
    beta = rho;

    for(k=0; k<kmax; k++){
        // Modified Gram-Schmidt
        DhFunc(t, x0, v.col(k+1), v.col(k+1));
        for(j=0; j<k; j++){
            h(j,k) = v.col(k+1) * v.col(j);
            v.col(k+1) = v.col(k+1) - h(j,k) * v.col(j);
        }
        h(k+1,k) = v.col(k+1).norm();
        if(h(k+1,k) != 0)
            v.col(k+1) = v.col(k+1) / h(k+1,k);
        else
            std::cout << "fgmres() : breakdown" << std::endl;

        // Givens Rotation for the Lieast Squares Problem ||beta * e_1  - H_k * y^k||
        for(j=0; j<k; j++){
            w1 = cvec[i] * gvec[i] - svec[i] * gvec[i+1];
            w2 = svec[i] * gvec[i] + cvec[i] * gvec[i+1];
            gvec[i] = w1;
            gvec[i+1] = w2;
        }
        nu = std::sqrt(h(k,k)*h(k,k) + h(k+1,k)*h(k+1,k));
        if(nu != 0) {
            cvec[k] = h(k,k) / nu;
            svec[k] = - h(k+1,k) / nu;
            h(k,k) = cvec[k] * h(k,k) - svec[k] * h(k+1,k);
            h(k+1,k) = 0;
            w1 = cvec[k] * gvec[k] - svec[i] * gvec[k+1];
            w2 = svec[k] * gvec[k] + cvec[i] * gvec[k+1];
        }
        else
            std::cout << "error : h(k,k) = h(k+1,k) = 0\n";
        rho = std::fabs(g[k+1]);
        errvec[k+1] = rho;
    }

    // solve H_k * y^k = gvec
    for(i=kmax; i>=0; i--) {
        for(nu = gvec[i], j=i+1; j<k; j++){
            nu -= h(i,j) * cvec[j];
        }
        cvec[i] = nu / h(i,i);
    }
    s = v * cvec;
}