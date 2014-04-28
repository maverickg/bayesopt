/*
-------------------------------------------------------------------------
   This file is part of BayesOpt, an efficient C++ library for 
   Bayesian optimization.

   Copyright (C) 2011-2013 Ruben Martinez-Cantin <rmcantin@unizar.es>
 
   BayesOpt is free software: you can redistribute it and/or modify it 
   under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   BayesOpt is distributed in the hope that it will be useful, but 
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with BayesOpt.  If not, see <http://www.gnu.org/licenses/>.
------------------------------------------------------------------------
*/

#include "testfunctions.hpp"

int main(int nargs, char *args[])
{
  size_t dim = 1;

  bopt_params parameters = initialize_parameters_to_default();
  parameters.n_init_samples = 10;
  parameters.n_iterations = 300;
  set_surrogate(&parameters,"sGaussianProcess");
  parameters.kernel.hp_mean[0] = 1.0;
  parameters.kernel.hp_std[0] = 100.0;
  parameters.kernel.n_hp = 1;

  // parameters.crit_name = "cHedge(cEI,cLCB,cExpReturn,cOptimisticSampling)";
  // parameters.epsilon = 0.0;

  ExampleOneD opt(dim,parameters);
  vectord result(dim);
  opt.optimize(result);
  
  std::cout << "Result:" << result << std::endl;
  opt.printOptimal();

  return 0;
}
