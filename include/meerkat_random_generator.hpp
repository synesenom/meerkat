/* meerkat random generator.
 *
 * Class containing various pseudo random generators.
 *
 * Copyright (c) 2016, Enys Mones.
 */

#ifndef MEERKAT_RANDOM_GENERATOR_HPP
#define MEERKAT_RANDOM_GENERATOR_HPP

#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "time.h"
#include <inttypes.h>
#include <vector>

namespace meerkat {

// random generator class
class mk_random_generator
{
public:
    /**
     * @brief The mk_alias_table struct  Encapsulates variables for the alias table method.
     */
    struct mk_alias_table
    {
        bool _isTableSet;               // Is the table properly set up?
        int _n;                         // Size of the table.
        std::vector<double> _prob;      // Probabilities to use.
        std::vector<int> _alias;        // Aliases.
    };

    /**
     * @brief mk_random_generator  Empty constructor, initializes seed.
     */
    mk_random_generator();

    /**
     * @brief mk_random_generator  Constructor with seed given.
     * @param seed_                Seed of the random generator.
     */
    mk_random_generator( int seed_ );

    /**
     * @brief init   Sets seed.
     * @param _seed  Seed to set.
     */
    void init( int _seed );

    /**
     * @brief double_uniform  Generates a uniformly distributed double.
     * @param min_            Minimum value.
     * @param max_            Maximum value.
     * @return                The generated double.
     */
    double double_uniform( double min_, double max_ );

    /**
     * @brief double_bounded_pareto  Generates a double following the bounded Pareto distribution.
     * @param min_                   Minimum value.
     * @param max_                   Maximum value.
     * @param exponent_              The exponent parameter.
     * @return                       The generated double.
     */
    double double_bounded_pareto( double min_, double max_, double exponent_ );

    /**
     * @brief double_normal  Generates a normally distributed double.
     * @param mu_            Distribution mean.
     * @param sigma_         Distribution standard deviation.
     * @return               The generated double.
     */
    double double_normal( double mu_, double sigma_ );

    /**
     * @brief double_shuffle  Shuffles a vetor of doubles.
     * @param vec_            Vector to shuffle.
     */
    void double_shuffle( std::vector<double> &vec_ );

    /**
     * @brief integer_uniform  Generates a uniformly distributed integer.
     * @param min_             Minimum value.
     * @param max_             Maximum value.
     * @return                 The generated integer.
     */
    int integer_uniform( int min_, int max_ );

    /**
     * @brief integer_poisson  Generates an integer with Poisson distribution.
     * @param lambda_          The parameter of the distribution.
     * @return                 The generated integer.
     */
    int integer_poisson( double lambda_ );

    /**
     * @brief integer_bounded_pareto  Generates an integer following the bounded Pareto distribution.
     * @param min_                    Minimum value.
     * @param max_                    Maximum value.
     * @param exponent_               The exponent parameter.
     * @return                        The generated integer.
     */
    int integer_bounded_pareto( int min_, int max_, double exponent_ );

    /**
     * @brief integer_normal  Generates an approximate normally distributed integer (rounds only).
     * @param mu_             Distribution mean.
     * @param sigma_          Distribution standard deviation.
     * @return                The generated integer.
     */
    int integer_normal( double mu_, double sigma_ );

    /**
     * @brief integer_shuffle  Shuffles a vector of integers.
     * @param vec_             Vector to shuffle.
     */
    void integer_shuffle( std::vector<int> &vec_ );

    /**
     * @brief setup_alias_table  Sets up alias table.
     * @param weights_           Weights to use for the table.
     */
    void setup_alias_table( std::vector<double> &weights_ );

    /**
     * @brief sample_alias_table  Generates a random integer from the table.
     * @return                    The generated integer.
     */
    int sample_alias_table();

    /**
     * @brief clear_alias_table  Clears alias table.
     */
    void clear_alias_table();

private:
    uint32_t _x[5];               // Array of integers used by the Mother-of-all method.
    mk_alias_table _a;            // Alias table.
    uint32_t _random_bits();      // Generates random bits.
};

}

#endif // MEERKAT_RANDOM_GENERATOR_HPP
