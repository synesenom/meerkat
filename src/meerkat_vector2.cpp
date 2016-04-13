#include "meerkat_vector2.hpp"

meerkat::mk_vector2::mk_vector2()
{
    _x = 0.0;
    _y = 0.0;
}

meerkat::mk_vector2::mk_vector2( const double x_, const double y_ )
{
    _x = x_;
    _y = y_;
}

double meerkat::mk_vector2::x() const
{
    return _x;
}

double meerkat::mk_vector2::y() const
{
    return _y;
}

void meerkat::mk_vector2::set_x( const double x_ )
{
    _x = x_;
}

void meerkat::mk_vector2::set_y( const double y_ )
{
    _y = y_;
}

void meerkat::mk_vector2::set( const double x_, const double y_ )
{
    _x = x_;
    _y = y_;
}

meerkat::mk_vector2& meerkat::mk_vector2::operator=( const mk_vector2 &vector_ )
{
    _x = vector_._x;
    _y = vector_._y;

    return *this;
}

meerkat::mk_vector2& meerkat::mk_vector2::operator+=( const mk_vector2 &vector_ )
{
    _x += vector_._x;
    _y += vector_._y;

    return *this;
}

const meerkat::mk_vector2 meerkat::mk_vector2::operator+( const mk_vector2 &vector_ ) const
{
    mk_vector2 result = *this;
    result += vector_;

    return result;
}

meerkat::mk_vector2& meerkat::mk_vector2::operator-=( const mk_vector2 &vector_ )
{
    _x -= vector_._x;
    _y -= vector_._y;

    return *this;
}

const meerkat::mk_vector2 meerkat::mk_vector2::operator-( const mk_vector2 &vector_ ) const
{
    mk_vector2 result = *this;
    result -= vector_;

    return result;
}

double meerkat::mk_vector2::operator *( const mk_vector2 &vector_ ) const
{
    return _x*vector_._x + _y*vector_._y;
}

meerkat::mk_vector2& meerkat::mk_vector2::operator*=( const double scalar_ )
{
    _x *= scalar_;
    _y *= scalar_;

    return *this;
}

const meerkat::mk_vector2 meerkat::mk_vector2::operator*( const double scalar_ ) const
{
    mk_vector2 result = *this;
    result *= scalar_;

    return result;
}

meerkat::mk_vector2& meerkat::mk_vector2::operator/=( const double scalar_ )
{
    _x /= scalar_;
    _y /= scalar_;

    return *this;
}

const meerkat::mk_vector2 meerkat::mk_vector2::operator/( const double scalar_ ) const
{
    mk_vector2 result = *this;
    result /= scalar_;

    return result;
}

double meerkat::mk_vector2::length() const
{
    return sqrt( _x*_x + _y*_y );
}

void meerkat::mk_vector2::set_length( const double length_ )
{
    double a = atan2( _y, _x );
    set( length_*cos(a), length_*sin(a) );
}

double meerkat::mk_vector2::angle() const
{
    return atan2( _y, _x );
}

void meerkat::mk_vector2::set_angle( const double angle_ )
{
    double l = length();
    set( l*cos(angle_), l*sin(angle_) );
}

void meerkat::mk_vector2::normalize()
{
    set_length( 1.0 );
}

void meerkat::mk_vector2::rotate( const double angle_ )
{
    double a = angle();
    set_angle( a + angle_ );
}
