/* meerkat vector2.
 *
 * A 2D vector class.
 *
 * Copyright (c) 2016, Enys Mones.
 */

#ifndef MEERKAT_VECTOR2_HPP
#define MEERKAT_VECTOR2_HPP

#include "math.h"
#include <vector>

namespace meerkat {

class mk_vector2 {
private:
    double _x;
    double _y;

public:
    /**
     * @brief mk_vector2  Empty constructor.
     */
    mk_vector2();

    /**
     * @brief mk_vector2  Complete constructor.
     * @param x_          X component.
     * @param y_          Y component.
     */
    mk_vector2( const double x_, const double y_ );

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
     * @brief set_x  Sets the x component.
     * @param x_     Value to set the x component to.
     */
    void set_x( const double x_ );

    /**
     * @brief set_y  Sets the y component.
     * @param y_     Value to set the y component to.
     */
    void set_y( const double y_ );

    /**
     * @brief set  Sets both components of the vector.
     * @param x_   Value of the x component.
     * @param y_   Value of the y component.
     */
    void set( const double x_, const double y_ );

    /**
     * @brief operator =  Assignment operator.
     * @param vector_     Vector to copy.
     * @return            Reference to the result vector.
     */
    mk_vector2& operator =( const mk_vector2 &vector_ );

    /**
     * @brief operator +=  Increment operator.
     * @param vector_      Vector increment.
     * @return             Reference to the result vector.
     */
    mk_vector2& operator +=( const mk_vector2 &vector_ );

    /**
     * @brief operator +  Addition operator.
     * @param vector_     Vector to add.
     * @return            Result of the addition.
     */
    const mk_vector2 operator +( const mk_vector2 &vector_ ) const;

    /**
     * @brief operator -=  Decrement operator.
     * @param vector_      Vector decrement.
     * @return             Reference to the result vector.
     */
    mk_vector2& operator -=( const mk_vector2 &vector_ );

    /**
     * @brief operator -  Substraction operator.
     * @param vector_     Vector to substract.
     * @return            Results of the substraction.
     */
    const mk_vector2 operator -( const mk_vector2 &vector_ ) const;

    /**
     * @brief operator *  Scalar product.
     * @param vector_     Vector to multiply with.
     * @return            Scalar product.
     */
    double operator *( const mk_vector2 &vector_ ) const;

    /**
     * @brief operator *=  Scaling operator.
     * @param scalar_      Multiplier.
     * @return             Scaled vector.
     */
    mk_vector2& operator *=( const double scalar_ );

    /**
     * @brief operator *  Scalar multiplication operator.
     * @param scalar_     Multiplier.
     * @return            Result vector.
     */
    const mk_vector2 operator *( const double scalar_ ) const;

    /**
     * @brief operator /=  Scaling operator.
     * @param scalar_      Divider.
     * @return             Scaled vector.
     */
    mk_vector2& operator /=( const double scalar_ );

    /**
     * @brief operator /  Scalar division operator.
     * @param scalar_     Divider.
     * @return            Result vector.
     */
    const mk_vector2 operator /( const double scalar_ ) const;

    /**
     * @brief length  Euclidean length of the vector.
     * @return        Length.
     */
    double length() const;

    /**
     * @brief angle  Angle of the vector to the X axis.
     * @return       Angle in radian.
     */
    double angle() const;

    /**
     * @brief set_length  Sets vector length.
     * @param length_     Length to set.
     */
    void set_length( const double length_ );

    /**
     * @brief set_angle  Sets vector angle.
     * @param angle_     Angle to set.
     */
    void set_angle( const double angle_ );

    /**
     * @brief normalize  Normalizes vector to unit length.
     */
    void normalize();

    /**
     * @brief rotate  Rotates vector by a given angle.
     * @param angle_  Angle of rotation.
     */
    void rotate( const double angle_ );
};
}

#endif // MEERKAT_VECTOR2_HPP
