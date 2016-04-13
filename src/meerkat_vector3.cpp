#include "meerkat_vector3.hpp"

meerkat::mk_vector3::mk_vector3()
{
    _x = 0.0;
    _y = 0.0;
    _z = 0.0;
}

meerkat::mk_vector3::mk_vector3( const double x_, const double y_, const double z_ )
{
    _x = x_;
    _y = y_;
    _z = z_;
}

double meerkat::mk_vector3::x() const
{
    return _x;
}

double meerkat::mk_vector3::y() const
{
    return _y;
}

double meerkat::mk_vector3::z() const
{
    return _z;
}

void meerkat::mk_vector3::set_x( const double x_ )
{
    _x = x_;
}

void meerkat::mk_vector3::set_y( const double y_ )
{
    _y = y_;
}

void meerkat::mk_vector3::set_z( const double z_ )
{
    _z = z_;
}

void meerkat::mk_vector3::set( const double x_, const double y_, const double z_ )
{
    _x = x_;
    _y = y_;
    _z = z_;
}

meerkat::mk_vector3& meerkat::mk_vector3::operator=( const mk_vector3 &vector_ )
{
    _x = vector_._x;
    _y = vector_._y;
    _z = vector_._z;

    return *this;
}

meerkat::mk_vector3& meerkat::mk_vector3::operator+=( const mk_vector3 &vector_ )
{
    _x += vector_._x;
    _y += vector_._y;
    _z += vector_._z;

    return *this;
}

const meerkat::mk_vector3 meerkat::mk_vector3::operator+( const mk_vector3 &vector_ ) const
{
    mk_vector3 result = *this;
    result += vector_;

    return result;
}

meerkat::mk_vector3& meerkat::mk_vector3::operator-=( const mk_vector3 &vector_ )
{
    _x -= vector_._x;
    _y -= vector_._y;
    _z -= vector_._z;

    return *this;
}

const meerkat::mk_vector3 meerkat::mk_vector3::operator-( const mk_vector3 &vector_ ) const
{
    mk_vector3 result = *this;
    result -= vector_;

    return result;
}

meerkat::mk_vector3& meerkat::mk_vector3::operator*=( const double scalar_ )
{
    _x *= scalar_;
    _y *= scalar_;
    _z *= scalar_;

    return *this;
}

const meerkat::mk_vector3 meerkat::mk_vector3::operator*( const double scalar_ ) const
{
    mk_vector3 result = *this;
    result *= scalar_;

    return result;
}

meerkat::mk_vector3& meerkat::mk_vector3::operator/=( const double scalar_ )
{
    _x /= scalar_;
    _y /= scalar_;
    _z /= scalar_;

    return *this;
}

const meerkat::mk_vector3 meerkat::mk_vector3::operator/( const double scalar_ ) const
{
    mk_vector3 result = *this;
    result /= scalar_;

    return result;
}

double meerkat::mk_vector3::r() const
{
    return sqrt( _x*_x + _y*_y + _z*_z );
}

double meerkat::mk_vector3::rho() const
{
    return sqrt( _x*_x + _y*_y );
}

double meerkat::mk_vector3::phi() const
{
    return atan2( _y, _x );
}

double meerkat::mk_vector3::theta() const
{
    double r0 = r();

    if( r0 > 0.0 )
        return acos( _z / r0 );
    else
        return 0.0;
}

void meerkat::mk_vector3::set_r( const double r_ )
{
    double p = phi();
    double t = theta();
    set( r_*sin(t)*cos(p), r_*sin(t)*sin(p), r_*cos(t) );
}

void meerkat::mk_vector3::set_rho( const double rho_ )
{
    double p = phi();
    set( rho_*cos(p), rho_*sin(p), _z );
}

void meerkat::mk_vector3::set_phi( const double phi_ )
{
    double r0 = r();
    double t = theta();
    set( r0*sin(t)*cos(phi_), r0*sin(t)*sin(phi_), r0*cos(t) );
}

void meerkat::mk_vector3::set_theta( const double theta_ )
{
    double r0 = r();
    double p = phi();
    set( r0*sin(theta_)*cos(p), r0*sin(theta_)*sin(p), r0*cos(theta_) );
}

double meerkat::mk_vector3::dot( const mk_vector3 &vector_ ) const
{
    return _x*vector_._x + _y*vector_._y + _z*vector_._z;
}

const meerkat::mk_vector3 meerkat::mk_vector3::cross( const mk_vector3 &vector_ ) const
{
    mk_vector3 result( _y*vector_._z - _z*vector_._y,
                       _z*vector_._x - _x*vector_._z,
                       _x*vector_._y - _y*vector_._x );

    return result;
}

void meerkat::mk_vector3::normalize()
{
    set_r( 1.0 );
}

void meerkat::mk_vector3::rotate( const mk_vector3 &axis_, const double angle_ )
{
    double c = cos( angle_ );
    double s = sin( angle_ );
    double cm = 1.0 - c;
    double ra = axis_.r();
    double ux = axis_._x / ra;
    double uy = axis_._y / ra;
    double uz = axis_._z / ra;

    double x = _x * ( c + ux*ux*cm )    + _y * ( ux*uy*cm - uz*s ) + _z * ( ux*uz*cm + uy*s );
    double y = _x * ( ux*uy*cm + uz*s ) + _y * ( c + uy*uy*cm )    + _z * ( uy*uz*cm - ux*s );
    double z = _x * ( ux*uz*cm - uy*s ) + _y * ( uy*uz*cm + ux*s ) + _z * ( c + uz*uz*cm );

    set( x, y, z );
}
