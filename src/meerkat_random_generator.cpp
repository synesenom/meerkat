#include "meerkat_random_generator.hpp"


meerkat::mk_random_generator::mk_random_generator()
{
    _a._isTableSet = false;
    init( rand()%1000 );
}

meerkat::mk_random_generator::mk_random_generator( int seed_ )
{
    _a._isTableSet = false;
    init( seed_ );
}

void meerkat::mk_random_generator::init( int seed_ )
{
    uint32_t s = (uint32_t)seed_;
    // make random numbers and put them into the buffer
    for(int i=0; i<5; i++)
    {
        s = 29943829*s - 1;
        _x[i] = s;
    }
    // randomize some more
    for(int i=0; i<19; i++)
        _random_bits();
}

uint32_t meerkat::mk_random_generator::_random_bits()
{
    uint64_t sum;
    sum = (uint64_t)2111111111UL * (uint64_t)_x[3] +
            (uint64_t)1492 * (uint64_t)(_x[2]) +
            (uint64_t)1776 * (uint64_t)(_x[1]) +
            (uint64_t)5115 * (uint64_t)(_x[0]) +
            (uint64_t)(_x[4]);
    _x[3] = _x[2];
    _x[2] = _x[1];
    _x[1] = _x[0];
    _x[4] = (uint32_t)(sum >> 32);
    _x[0] = (uint32_t)sum;
    return _x[0];
}

double meerkat::mk_random_generator::double_uniform( double min_, double max_ )
{
    double dsmall = (double)_random_bits() * (1./(65536.*65536.));
    return dsmall * (max_-min_) + min_;
}

double meerkat::mk_random_generator::double_bounded_pareto( double min_, double max_, double exponent_ )
{
    double l = pow( min_, 1.0-exponent_ );
    double h = pow( max_, 1.0-exponent_ );
    double r = double_uniform( 0.0, 1.0 );

    return pow( (h-l)*r + l, 1.0/(1.0-exponent_) );
}

double meerkat::mk_random_generator::double_normal( double mu_, double sigma_ )
{
    double U = double_uniform( 0.0, 1.0 );
    double V = double_uniform( 0.0, 1.0 );
    return mu_ + sigma_ * sqrt(-2.0*log(U)) * cos(6.28318*V) ;
}

void meerkat::mk_random_generator::double_shuffle( std::vector<double> &vec_ )
{
    int length = (int)vec_.size();
    int swaps = 10*length;
    int v1, v2;
    double elem;
    while( swaps > 0 )
    {
        v1 = integer_uniform( 0, length-1 );
        v2 = integer_uniform( 0, length-1 );
        if( v1 != v2 )
        {
            elem = vec_[v1];
            vec_[v1] = vec_[v2];
            vec_[v2] = elem;
            swaps--;
        }
    }
}

int meerkat::mk_random_generator::integer_uniform( int min_, int max_ )
{
    if( max_ == min_ )
        return min_;

    uint32_t interval = (uint32_t)(max_ - min_ + 1);
    uint64_t longran = (uint64_t)_random_bits() * interval;
    uint32_t iran = (uint32_t)(longran >> 32);

    return (int32_t)iran + min_;
}

int meerkat::mk_random_generator::integer_poisson( double lambda_ )
{
    double L = exp(-lambda_);
    int k = 0;
    double p = 1.0;

    do
    {
        k++;
        p *= double_uniform( 0.0, 1.0 );
    } while( p > L );

    return k-1;
}

int meerkat::mk_random_generator::integer_bounded_pareto( int min_, int max_, double exponent_ )
{
    return (int)double_bounded_pareto( (double)min_, (double)max_, exponent_ );
}

int meerkat::mk_random_generator::integer_normal( double mu_, double sigma_ )
{
    return (int)double_normal( mu_, sigma_ );
}

void meerkat::mk_random_generator::integer_shuffle( std::vector<int> &vec_ )
{
    int length = (int)vec_.size();
    int swaps = 10*length;
    int v1, v2;
    int elem;
    while( swaps > 0 )
    {
        v1 = integer_uniform( 0, length-1 );
        v2 = integer_uniform( 0, length-1 );
        if( v1 != v2 )
        {
            elem = vec_[v1];
            vec_[v1] = vec_[v2];
            vec_[v2] = elem;
            swaps--;
        }
    }
}

void meerkat::mk_random_generator::setup_alias_table( std::vector<double> &weights_ )
{
    _a._n = (int)weights_.size();
    double sum = 0.0;
    std::vector<double> p( _a._n, 0.0 );
    std::vector<int> small_worklist;
    std::vector<int> large_worklist;

    // fill up worklists
    for(int i=0; i<_a._n; i++)
        sum += weights_[i];
    for(int i=0; i<_a._n; i++)
    {
        p[i] = (double)_a._n * weights_[i] / sum;
        if( p[i] <  1.0 )
            small_worklist.push_back( i );
        else
            large_worklist.push_back( i );
    }

    // fill up alias table
    _a._prob.assign( _a._n, 0.0 );
    _a._alias.assign( _a._n, 0 );
    int small = 0, large = 0;
    while( !small_worklist.empty() && !large_worklist.empty() )
    {
        small = small_worklist.back();
        small_worklist.pop_back();
        large = large_worklist.back();
        large_worklist.pop_back();

        _a._prob[small] = p[small];
        _a._alias[small] = large;

        p[large] += p[small] - 1.0;
        if( p[large] < 1.0 )
            small_worklist.push_back( large );
        else
            large_worklist.push_back( large );
    }

    while( !large_worklist.empty() )
    {
        large = large_worklist.back();
        large_worklist.pop_back();
        _a._prob[large] = 1.0;
        _a._alias[large] = small;
    }

    while( !small_worklist.empty() )
    {
        small = small_worklist.back();
        small_worklist.pop_back();
        _a._prob[small] = 1.0;
        _a._alias[small] = small;
    }

    _a._isTableSet = true;
}

void meerkat::mk_random_generator::clear_alias_table()
{
    _a._n = 0;
    _a._prob.clear();
    _a._alias.clear();
    _a._isTableSet = false;
}

int meerkat::mk_random_generator::sample_alias_table()
{
    if( !_a._isTableSet )
    {
        printf( "meerkat_random_generator error: alias table is unitialized.\n" );
        exit( 0 );
    }
    int ret = integer_uniform( 0, _a._n-1 );
    if( double_uniform(0.0, 1.0) < _a._prob[ret] )
        return ret;
    else
        return _a._alias[ret];
}
