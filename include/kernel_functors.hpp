/** \file kernel_functors.hpp \brief Kernel (covariance) functions */
/*
-------------------------------------------------------------------------
   This file is part of BayesOpt, an efficient C++ library for 
   Bayesian optimization.

   Copyright (C) 2011-2012 Ruben Martinez-Cantin <rmcantin@unizar.es>
 
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

#ifndef  _KERNEL_FUNCTORS_HPP_
#define  _KERNEL_FUNCTORS_HPP_

#include "parameters.h"
#include "specialtypes.hpp"
#include "elementwise_ublas.hpp"

///\addtogroup KernelFunctions
//@{

/** \brief Interface for kernel functors */
class Kernel
{
public:
  static Kernel* create(kernel_name name, const vectord &theta);
  virtual void setScale( const vectord &theta ) = 0;
  virtual double getScale(int index) = 0;
  virtual vectord getScale() = 0;
  virtual double operator()( const vectord &x1, const vectord &x2 ) = 0;
  virtual double getGradient( const vectord &x1, const vectord &x2,
			      int grad_index ) = 0;
  virtual ~Kernel(){};
};


/////////////////////////////////////////////////////////////////////////////
/** \brief Abstract class for isotropic kernel functors */
class ISOkernel : public Kernel
{
public:
  void setScale( const vectord &theta ) {mTheta = theta(0);};
  double getScale(int index) {return mTheta;};
  vectord getScale() {return svectord(1,mTheta);};

  virtual ~ISOkernel(){};

protected:
  inline double computeScaledNorm2(const vectord &x1, const vectord &x2)
  {  
    assert(x1.size() == x2.size());
    return norm_2(x1-x2)/mTheta; 
  };

protected:
  double mTheta;
};


/////////////////////////////////////////////////////////////////////////////
/** \brief Abstract class for anisotropic kernel functors. 
 * Typically ARD (Automatic Relevance Determination)
 */
class ARDkernel : public Kernel
{
public:
  void setScale( const vectord &theta ){mTheta=theta;};
  double getScale(int index) {return mTheta(index);};
  vectord getScale() { return mTheta; };

  virtual ~ARDkernel(){};

protected:
  inline vectord computeScaledDiff(const vectord &x1, const vectord &x2)
  {
    assert(x1.size() == x2.size());
    assert(x1.size() == mTheta.size());

    vectord xd = x1-x2;
    return ublas_elementwise_div(xd, mTheta); 
  };

protected:
  vectord mTheta;
};



////////////////////////////////////////////////////////////////////////////////
/** \brief Matern kernel of 1st order */
class MaternIso1: public ISOkernel
{
public:
  double operator()( const vectord &x1, const vectord &x2)
  {
    double r = computeScaledNorm2(x1,x2);
    return exp(-r);
  };

  double getGradient( const vectord &x1, const vectord &x2,
		      int grad_index)
  {
    double r = computeScaledNorm2(x1,x2);
    return r*exp(-r);
  };
};


////////////////////////////////////////////////////////////////////////////////
/** \brief Matern kernel of 3rd order */
class MaternIso3: public ISOkernel
{
public:
  double operator()( const vectord &x1, const vectord &x2)
  {
    double r = sqrt(3.0) * computeScaledNorm2(x1,x2);
    double er = exp(-r);
    return (1+r)*er;
  };

  double getGradient( const vectord &x1, const vectord &x2,
		      int grad_index)
  {
    double r = sqrt(3.0) * computeScaledNorm2(x1,x2);
    double er = exp(-r);
    return r*r*er; 
  };
};


////////////////////////////////////////////////////////////////////////////////
/** \brief Matern kernel of 5th order */
class MaternIso5: public ISOkernel
{
public:
  double operator()( const vectord &x1, const vectord &x2)
  {
    double r = sqrt(5.0) * computeScaledNorm2(x1,x2);
    double er = exp(-r);
    return (1+r*(1+r/3))*er;
  };
  double getGradient( const vectord &x1, const vectord &x2,
		      int grad_index)
  {    
    double r = sqrt(5.0) * computeScaledNorm2(x1,x2);
    double er = exp(-r);
    return r*(1+r)/3*r*er; 
  };
};



////////////////////////////////////////////////////////////////////////////////
/** \brief Square exponential (Gaussian) kernel. Isotropic version. */
class SEIso: public ISOkernel
{
public:
  double operator()( const vectord &x1, const vectord &x2)
  {
    double rl = computeScaledNorm2(x1,x2);
    double k = rl*rl;
    return exp(-k/2);
  };
  double getGradient(const vectord &x1, const vectord &x2,
		     int grad_index)
  {
    double rl = computeScaledNorm2(x1,x2);
    double k = rl*rl;
    return exp(-k/2)*k;
  };
};


////////////////////////////////////////////////////////////////////////////////
/** \brief Square exponential (Gaussian) kernel. ARD version. */
class SEArd: public ARDkernel
{
public:
  double operator()( const vectord &x1, const vectord &x2 )
  {
    vectord ri = computeScaledDiff(x1,x2);
    double rl = norm_2(ri);
    double k = rl*rl;
    return exp(-k/2);
  };
  
  double getGradient(const vectord &x1, const vectord &x2,
		     int grad_index)
  {
  
    vectord ri = computeScaledDiff(x1,x2);
    double rl = norm_2(ri);
    double k = rl*rl;
    return exp(-k/2)*sqrt(ri(grad_index));
  };
};


//@}

#endif
