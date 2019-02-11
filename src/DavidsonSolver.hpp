#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Core>

#ifndef _DAVIDSON_SOLVER_
#define _DAVIDSON_SOLVER_

class DavidsonSolver
{

	public:

		DavidsonSolver();

		void set_iter_max(int N) { this->iter_max = N; }
		void set_tolerance(double eps) { this->tol = eps; }
		void set_max_search_space(int N) { this->max_search_space = N;}
		void set_initial_guess_size(int N) {this->size_initial_guess=N;}

		void set_correction(std::string method); 
		void set_jacobi_linsolve(std::string method);

		Eigen::VectorXd eigenvalues() {return this->_eigenvalues;}
		Eigen::MatrixXd eigenvectors() {return this->_eigenvectors;}

		template <typename MatrixReplacement>
		void solve(MatrixReplacement &A, int neigen, int size_initial_guess = 0);

	private :

		int iter_max = 1000;
		double tol = 1E-6;
		int max_search_space = 100;
		int size_initial_guess = 0;

		enum CORR {DPR,JACOBI,OLSEN};
		enum LSOLVE {CG,GMRES,LLT};

		CORR correction = CORR::DPR;
		LSOLVE jacobi_linsolve = LSOLVE::CG;

		Eigen::VectorXd _eigenvalues;
		Eigen::MatrixXd _eigenvectors; 

		Eigen::ArrayXd _sort_index(Eigen::VectorXd &V);
		Eigen::MatrixXd _get_initial_eigenvectors(Eigen::VectorXd &D, int size);
		Eigen::MatrixXd _solve_linear_system(Eigen::MatrixXd &A, Eigen::VectorXd &b); 
		Eigen::MatrixXd _QR(Eigen::MatrixXd &A);

		template <typename MatrixReplacement>
		Eigen::MatrixXd _jacobi_correction(MatrixReplacement &A, Eigen::VectorXd &r, Eigen::VectorXd &u, double lambda);
		Eigen::VectorXd _dpr_correction(Eigen::VectorXd &w, Eigen::VectorXd &A0, double lambda);
		Eigen::VectorXd _olsen_correction(Eigen::VectorXd &w, Eigen::VectorXd &A0, double lambda);

		template<class MatrixReplacement>
		void _update_projected_matrix(Eigen::MatrixXd &T, MatrixReplacement &A, Eigen::MatrixXd &V);
};


#endif