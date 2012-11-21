/*! \page reference Reference Manual
\tableofcontents

After installing the library, several test program will also be
generated and can be found in the \em bin folder. For the external
interfaces, there are test programs respectively under the \em python
and \em matlab folders.  They provide examples of the different
interfaces that Bayesian-optimization provide.

First of all, make sure your system finds the correct libraries. In
Windows, you can copy the dlls to your working folder or include the
lib folder in the path.

In some *nix systems, including Ubuntu, Debian and Mac OS, the library
is installed by default in /usr/local/lib/. However, this folder is
not included in the linker, Python or Matlab paths by default. The
script \em exportlocalpaths.sh makes sure that the folder is included
in all the necessary paths.

After that, there are 3 steps that should be follow:
\li Define the function to optimize.
\li Set or modify the parameters of the optimization process. The set of
parameters and the default set can be found in parameters.h.  
\li Run the optimizer.

\section params Understanding the parameters

BayesOpt relies on a complex and highly configurable mathematical
model. Also, the key to nonlinear optimization is to include as much
knowledge as possible about the target function or about the
problem. Or, if the knowledge is not available, keep the model as
general as possible (to avoid bias). 

For that reason, the parameters are bundled in a structure or dictionary, depending on the API that we use. This is a brief explanation of every parameter:
\li \b n_iterations: Maximum number of iterations of BayesOpt. Each iteration corresponds with a target function evaluation. This is related with the budget of the application 
\li \b n_init_samples: BayesOpt requires an initial set of samples to learn a preliminar model of the target function. Each sample is also a function evaluation.
\li \b verbose_level: (integer value) Verbose level 1,2,3 -> stderr, 4,5,6 -> log file
\li \b log_filename: Filename of the log file (if applicable) 
\li \b theta: Kernel hyperparameters. Depends on the kernel selected.
\li \b n_theta: Number of kernel hyperparameters.
\li \b mu: Mean function hyperparameters. Depends on the mean function selected.
\li \b n_mu: Number of mean function hyperparameters 
\li \b alpha, \b beta, \b delta: Inverse-Gamma-Normal prior hyperparameters (if applicable) 
\li \b noise: Observation noise. For computer simulations or deterministic functions, it should be close to 0. However, to avoid numerical instability due to model inaccuracies, do not make it 0.
\li \b s_name: Name of the hierarchical surrogate function (gaussian process + hyperpriors)
\li \b k_name: Name of the kernel function. 
\li \b c_name: Name of the sample selection criterion. It is used to select which points to evaluate for each iteration of the optimization process.
\li \b m_name: Name of the mean function (parametric part of the surrogate function).

\section usage Using the library

Here we show a brief summary of the different ways to use the library:

\subsection cusage C/C++ callback usage

This interface is the most standard approach. Due to the large
compatibility with C code with other languages it could also be used
for other languages such as Fortran, Ada, etc.

The function to optimize must agree with the template provided in
bayesoptwpr.h

\code{.c}
double my_function (unsigned int n, const double *x, double *gradient, void *func_data);
\endcode

Note that the gradient has been included for future compatibility,
although in the current implementation, it is not used. You can just
ignore it or send a NULL pointer.

The parameters are defined in the bopt_params struct. The easiest way
to set the parameters is to use
\code{.c}
bopt_params initialize_parameters_to_default(void);
\endcode
and then, modify the necesary fields.

Once we have set the parameters and the function, we can called the optimizer
\code{.c}
  int bayes_optimization(int nDim, /* number of dimensions */
                         eval_func f, /* function to optimize */
                         void* f_data, /* extra data that is transfered directly to f */
			 const double *lb, const double *ub, /* bounds */
			 double *x, /* out: minimizer */
			 double *minf, /* out: minimum */
			 bopt_params parameters);
\endcode

\subsection cppusage C++ inheritance usage 

This is the most straighforward and complete method to use the
library. The object that must be optimized must inherit from the
BayesOptContinuous or BayesOptDiscrete classes. The objects can be
found in bayesoptcont.hpp and bayesoptdisc.hpp respectively.

Then, we just need to override one of the virtual functions called
evaluateSample, which can be called with C arrays and uBlas
vectors. Since there is no pure virtual functions, you can just
redefine your preferred interface.

\b Experimental: You can also override the checkReachability function
to include nonlinear restrictions.

\code{.cpp}
class MyOptimization: public BayesOptContinous
{
 public:
  MyOptimization(bopt_params param):
    BayesOptContinous(param) {}

  double evaluateSample( const boost::numeric::ublas::vector<double> &query ) 
  {
     // My function here
  };

  bool checkReachability( const boost::numeric::ublas::vector<double> &query )
  { 
     // My restrictions here 
  };
};
\endcode

As with the callback interface, the parameters are defined in the
bopt_params struct. The easiest way to set the parameters is to use

\code{.c}
bopt_params initialize_parameters_to_default(void);
\endcode

and then, modify the necesary fields.

\subsection pyusage Python callback/inheritance usage

The file python/test.py provides examples of the two Python
interfaces.

\b Parameters: For both interfaces, the parameters are defined as a
Python dictionary with the same structure as the bopt_params struct in
the C/C++ interface. The enumerate values are replaced by strings
without the prefix. For example, the C_EI criteria is replaced by the
string "EI" and the M_ZERO mean function is replaced by the string
"ZERO".

The parameter dictionary can be initialized using 
\code{.py}
parameters = bayesopt.initialize_params()
\endcode

however, this is not necesary in general. If any of the parameter is
not included in the dictionary, the default value is included instead.

\b Callback: The callback interface is just a wrapper of the C
interface. In this case, the callback function should have the form
\code{.py}
def my_function (query):
\endcode
where \em query is a numpy array and the function returns a double
scalar.
	
The optimization process can be called as
\code{.py}
y_out, x_out, error = bayesopt.optimize(my_function, n_dimensions, lower_bound, upper_bound, parameters)
\endcode
where the result is a tuple with the minimum as a numpy array (x_out),
the value of the function at the minimum (y_out) and the error code.

\b Inheritance: The object oriented construction is similar to the C++ interface.

\code{.py}
class MyModule(bayesoptmodule.BayesOptModule):
    def evalfunc(self,query):
        """ My function """
\endcode

The BayesOptModule include atributes for the parameters (\em params),
number of dimensions (\em n) and bounds (\em lb and \em up).

Then, the optimization process can be called as
\code{.py}
my_instance = MyModule()
# set parameters, bounds and number of dimensions.
y_out, x_out, error = my_instance.optimize()
\endcode
wher the result is a tuple with the minimum as a numpy array (x_out),
the value of the function at the minimum (y_out) and the error code.

\subsection matusage Matlab/Octave callback usage

The file matlab/runtest.m provides an example of the Matlab/Octave
interface.

\b Parameters: The parameters are defined as a Matlab struct
equivalent to bopt_params struct in the C/C++ interface, except for
the \em theta and \em mu arrays which are replaced by Matlab
vectors. Thus, the number of elements (\em n_theta and \em n_mu) are not
needed. The enumerate values are replaced by strings without the
prefix. For example, the C_EI criteria is replaced by the string "EI"
and the M_ZERO mean function is replaced by the string "ZERO".

If any of the parameter is not included in the Matlab struct, the
default value is automatically included instead.

\b Callback: The callback interface is just a wrapper of the C
interface. In this case, the callback function should have the form
\code{.m}
function y = my_function (query):
\endcode
where \em query is a Matlab vector and the function returns a scalar.

The optimization process can be called (both in Matlab and Octave) as
\code{.m}
[x_out, y_out] = bayesopt('my_function', n_dimensions, parameters, lower_bound, upper_bound)
\endcode
where the result is the minimum as a vector (x_out) and the value of
the function at the minimum (y_out).

In Matlab, but not in Octave, the optimization can also be called with
function handlers
\code{.m}
[x_out, y_out] = bayesopt(@my_function, n_dimensions, parameters, lower_bound, upper_bound)
\endcode

\subsubsection notematlab Note on gcc versions:

Matlab for *nix OS may ship outdated libraries for gcc (e.g.: 4.3 in
2011b). You might get errors like this one:
\verbatim
/usr/lib/x86_64-linux-gnu/gcc/x86_64-linux-gnu/4.5.2/cc1: /usr/local/MATLAB/R2010b/sys/os/glnxa64/libstdc++.so.6: version `GLIBCXX_3.4.14' not found (required by /usr/lib/libppl_c.so.2)
\endverbatim

The solution is to change the symbolic links
in /matlabroot/sys/os/glnx86 for \em libgcc_s.so.1
and \em libstdc++.so.6 to point to the system libraries, which
typically can be found in /lib or /usr/lib.

*/
