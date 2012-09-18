/** -*- c++ -*- \file bayesoptdisc.hpp \brief Discrete Bayesian optimization */
/*
-----------------------------------------------------------------------------
   This file is part of BayesOptimization, an efficient C++ library for 
   Bayesian optimization.

   Copyright (C) 2011 Ruben Martinez-Cantin <rmcantin@unizar.es>
 
   BayesOptimization is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   BayesOptimization is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with BayesOptimization.  If not, see <http://www.gnu.org/licenses/>.
-----------------------------------------------------------------------------
*/


#ifndef  _BAYESOPTDISC_HPP_
#define  _BAYESOPTDISC_HPP_

#include "bayesoptbase.hpp"

/** \addtogroup BayesOptimization */
/*@{*/

/**
 * \brief Sequential Kriging Optimization using different non-parametric 
 * processes as surrogate (kriging) functions. 
 */
class BayesOptDiscrete : public BayesOptBase
{
 public:
  
  /** 
   * Constructor
   * 
   * @param validSet  Set of potential inputs
   * @param gp        Pointer to the surrogate model
   */
  BayesOptDiscrete( vecOfvec &validSet,
	    bopt_params params,
	    bool uselogfile = false,
	    const char* logfilename = "bayesopt.log");

  /** 
   * Default destructor
   * 
   * @return 
   */
  virtual ~BayesOptDiscrete();

  /** 
   * Execute the optimization process of the function defined in evaluateSample.
   * We assume that the function is defined in the [0,1] hypercube, as a 
   * normalized representation of the bound constrains.
   * 
   * @see scaleInput
   * @see evaluateSample
   *
   * @param bestPoint returns the optimum value in a ublas::vector defined in 
   * the hypercube [0,1], it might also be used as an initial point
   * 
   * @return 1 if terminate successfully, 0 otherwise
   */
  int optimize( vectord &bestPoint);
  

protected:


  int sampleRandomPoints( size_t nSamples );
  int findOptimal(vectord &xOpt);

protected:

  vecOfvec mInputSet;               ///< List of input points

};

/**@}*/
// end namespaces


#endif
