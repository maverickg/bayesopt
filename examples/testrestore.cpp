#include "testfunctions.hpp"
#include "bopt_state.hpp"

int main()
{
    // First optimization
    bopt_params par1 = initialize_parameters_to_default();
    par1.n_iterations = 190;
    par1.random_seed = 0;
    par1.verbose_level = 1;
    par1.noise = 1e-10;

    BraninNormalized branin1(par1);
    vectord result(2);
    
    // Initialize first optimization that will stop at half run
    size_t stopAt = par1.n_iterations/2;
    branin1.initializeOptimization();
    for(size_t i = branin1.getCurrentIter(); i < stopAt; i++){
        branin1.stepOptimization();
    }
    // Save state
    bayesopt::BOptState state;
    branin1.saveOptimization(state);
    std::cout << "STATE ITERS: " << state.mCurrentIter << std::endl;
    
    result = branin1.getFinalResult();
    std::cout << "Branin1 Result: " << result << "->" 
        << branin1.evaluateSample(result) << std::endl;
    
    std::cout << "RESTORING OPTIMIZATION" << std::endl;
    std::cout << "======================" << std::endl;    
    
    // Second optimization (restored from first optimization state)
    bopt_params par2 = initialize_parameters_to_default();
    par2.n_iterations = 190;
    par2.random_seed = 0;
    par2.verbose_level = 1;
    par2.noise = 1e-10;

    BraninNormalized branin2(par2);

    // Restore operation and optimization
    branin2.restoreOptimization(state);
    for(size_t i = branin2.getCurrentIter(); i < par2.n_iterations; i++){
        branin2.stepOptimization();
    }
    
    result = branin2.getFinalResult();
    std::cout << "Branin2 Result: " << result << "->" 
        << branin2.evaluateSample(result) << std::endl;
  

}
