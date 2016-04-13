/* meerkat vector3.
 *
 * A 3D vector class.
 *
 * Copyright (c) 2016, Enys Mones.
 */

#ifndef MEERKAT_VECTOR3_HPP
#define MEERKAT_VECTOR3_HPP

#include "math.h"
#include <vector>

namespace meerkat {

class mk_vector3 {
private:
    double _x;
    double _y;
    double _z;

public:
    /**
     * @brief mk_vector3  Empty constructor.
     */
    mk_vector3();

    /**
     * @brief mk_vector3  Complete constructor.
     * @param x_          X component.
     * @param y_          Y component.
     * @param z_          Z component.
     */
    mk_vector3( const double x_, const double y_, const double z_ );

    /**
     * @brief set_x  Sets x component.
     * @param x_     Value to set the x component to.
     */
    void set_x( const double x_ );

    /**
     * @brief set_y  Sets y component.
     * @param y_     Value to set the y component to.
     */
    void set_y( const double y_ );

    /**
     * @brief set_z  Sets z component.
     * @param z_     Value to set the z component to.
     */
    void set_z( const double z_ );

    /**
     * @brief x  Returns x component.
     * @return   X component of the vector.
     */
    double x() const;

    /**
     * @brief y  Returns y component.
     * @return   Y component of the vector.
     */
    double y() const;

    /**
     * @brief z  Returns z component.
     * @return   Z component of the vector.
     */
    double z() const;

    /**
     * @brief operator =  Assignment operator.
     * @param vector_     Vector to copy.
     * @return            Reference to the result vector.
     */
    mk_vector3& operator =( const mk_vector3 &vector_ );

    /**
     * @brief operator +=  Increment operator.
     * @param vector_      Vector increment.
     * @return             Reference to the result vector.
     */
    mk_vector3& operator +=( const mk_vector3 &vector_ );

    /**
     * @brief operator +  Addition operator.
     * @param vector_     Vector to add.
     * @return            Result of the addition.
     */
    const mk_vector3 operator +( const mk_vector3 &vector_ ) const;

    /**
     * @brief operator -=  Decrement operator.
     * @param vector_      Vector decrement.
     * @return             Reference to the result vector.
     */
    mk_vector3& operator -=( const mk_vector3 &vector_ );

    /**
     * @brief operator -  Substraction operator.
     * @param vector_     Vector to substract.
     * @return            Results of the substraction.
     */
    const mk_vector3 operator -( const mk_vector3 &vector_ ) const;

    /**
     * @brief operator *=  Scaling operator.
     * @param scalar_      Multiplier.
     * @return             Scaled vector.
     */
    mk_vector3& operator *=( const double scalar_ );

    /**
     * @brief operator *  Scalar multiplication operator.
     * @param scalar_     Multiplier.
     * @return            Result vector.
     */
    const mk_vector3 operator *( const double scalar_ ) const;

    /**
     * @brief operator /=  Scaling operator.
     * @param scalar_      Divider.
     * @return             Scaled vector.
     */
    mk_vector3& operator /=( const double scalar_ );

    /**
     * @brief operator /  Scalar division operator.
     * @param scalar_     Divider.
     * @return            Result vector.
     */
    const mk_vector3 operator /( const double scalar_ ) const;

    /**
     * @brief set  Sets all components.
     * @param x_   Value to set the x component to.
     * @param y_   Value to set the y component to.
     * @param z_   Value to set the z component to.
     */
    void set( const double x_, const double y_, const double z_ );

    /**
     * @brief r  Vector length.
     * @return   Length.
     */
    double r() const;

    /**
     * @brief rho  Distance from the z-axis.
     * @return     Distance.
     */
    double rho() const;

    /**
     * @brief phi  Angle of vector to the x-axis.
     * @return     Angle.
     */
    double phi() const;

    /**
     * @brief theta  Angle of vector to the z-axis.
     * @return       Angle.
     */
    double theta() const;

    /**
     * @brief set_r  Sets vector length.
     * @param r_     Length to set.
     */
    void set_r( const double r_ );

    /**
     * @brief set_rho  Sets distance from z-axis.
     * @param rho_     Distance to set.
     */
    void set_rho( const double rho_ );

    /**
     * @brief set_phi  Sets angle to x-axis.
     * @param phi_     Angle to set.
     */
    void set_phi( const double phi_ );

    /**
     * @brief set_theta  Sets angle to z-axis.
     * @param theta_     Angle to set.
     */
    void set_theta( const double theta_ );

    /**
     * @brief dot      Dot product.
     * @param vector_  Vector to multiply with.
     * @return         Result scalar.
     */
    double dot( const mk_vector3 &vector_ ) const;

    /**
     * @brief cross    Cross product.
     * @param vector_  Vector to multiply with.
     * @return         Result vector.
     */
    const mk_vector3 cross( const mk_vector3 &vector_ ) const;

    /**
     * @brief normalize  Normalizes vector to unit length.
     */
    void normalize();

    /**
     * @brief rotate  Rotates vector by a given angle around a given axis.
     * @param axis_   Axis of rotation.
     * @param angle_  Angle of rotation.
     */
    void rotate( const mk_vector3 &axis_, const double angle_ );
};
}

#endif // MEERKAT_VECTOR3_HPP
