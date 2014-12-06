// Keyglove controller source code - 3D math helper function implementations
// 2014-11-07 by Jeff Rowberg <jeff@rowberg.net>

/* ============================================
Controller code is placed under the MIT license
Copyright (c) 2014 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

/**
 * @file support_helper_3dmath.cpp
 * @brief 3D math helper function implementations
 * @author Jeff Rowberg
 * @date 2014-11-07
 *
 * Contains quaternion implementation with floats and vector implementations
 * with both floats and 16-bit signed integers. Mainly useful in any optional
 * onboard motion computations that you want to include in your customized
 * application behavior, beyond raw accel/gyro sensor readings.
 */

#include "keyglove.h"
#include "support_helper_3dmath.h"

/**
 * @brief Initializes quaternion to known "zero" state
 */
Quaternion::Quaternion() {
    w = 1.0f;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

/**
 * @brief Initializes quaternion with provided components
 * @param[in] nw W element
 * @param[in] nx X element
 * @param[in] ny Y element
 * @param[in] nz Z element
 */
Quaternion::Quaternion(float nw, float nx, float ny, float nz) {
    w = nw;
    x = nx;
    y = ny;
    z = nz;
}

/**
 * @brief Calculate the product of two quaternions Q1 * Q2
 * @param[in] q Second quaternion (Q2) in calculation
 * @return Calculated quaternion product
 */
Quaternion Quaternion::getProduct(Quaternion q) {
    // Quaternion multiplication is defined by:
    //     (Q1 * Q2).w = (w1w2 - x1x2 - y1y2 - z1z2)
    //     (Q1 * Q2).x = (w1x2 + x1w2 + y1z2 - z1y2)
    //     (Q1 * Q2).y = (w1y2 - x1z2 + y1w2 + z1x2)
    //     (Q1 * Q2).z = (w1z2 + x1y2 - y1x2 + z1w2
    return Quaternion(
        w*q.w - x*q.x - y*q.y - z*q.z,  // new w
        w*q.x + x*q.w + y*q.z - z*q.y,  // new x
        w*q.y - x*q.z + y*q.w + z*q.x,  // new y
        w*q.z + x*q.y - y*q.x + z*q.w); // new z
}

/**
 * @brief Calculate the conjugate of this quaternion
 * @return Calculated quaternion conjugate
 */
Quaternion Quaternion::getConjugate() {
    return Quaternion(w, -x, -y, -z);
}

/**
 * @brief Calculate the magnitude of this quaternion
 * @return Calculated quaternion magnitude
 */
float Quaternion::getMagnitude() {
    return sqrt(w*w + x*x + y*y + z*z);
}

/**
 * @brief Normalize this quaternion (scales so magnitude = 1.0)
 */
void Quaternion::normalize() {
    float m = getMagnitude();
    w /= m;
    x /= m;
    y /= m;
    z /= m;
}

/**
 * @brief Create normalized copy of this quaternion
 * @return Normalized quaternion
 */
Quaternion Quaternion::getNormalized() {
    Quaternion r(w, x, y, z);
    r.normalize();
    return r;
}



/**
 * @brief Initializes vector to known "zero" state
 */
VectorInt16::VectorInt16() {
    x = 0;
    y = 0;
    z = 0;
}

/**
 * @brief Initializes vector with provided components
 * @param[in] nx X element
 * @param[in] ny Y element
 * @param[in] nz Z element
 */
VectorInt16::VectorInt16(int16_t nx, int16_t ny, int16_t nz) {
    x = nx;
    y = ny;
    z = nz;
}

/**
 * @brief Calculate the magnitude of this vector
 * @return Calculated vector magnitude
 */
float VectorInt16::getMagnitude() {
    return sqrt(x*x + y*y + z*z);
}

/**
 * @brief Normalize this vector (scales so magnitude = 1.0)
 */
void VectorInt16::normalize() {
    float m = getMagnitude();
    x /= m;
    y /= m;
    z /= m;
}

/**
 * @brief Create normalized copy of this vector
 * @return Normalized vector
 */
VectorInt16 VectorInt16::getNormalized() {
    VectorInt16 r(x, y, z);
    r.normalize();
    return r;
}

/**
 * @brief Rotate this vector by a given quaternion
 * @param[in] q Quaternion to rotate vector by
 */
void VectorInt16::rotate(Quaternion *q) {
    // http://www.cprogramming.com/tutorial/3d/quaternions.html
    // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
    // http://content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation
    // ^ or: http://webcache.googleusercontent.com/search?q=cache:xgJAp3bDNhQJ:content.gpwiki.org/index.php/OpenGL:Tutorials:Using_Quaternions_to_represent_rotation&hl=en&gl=us&strip=1

    // P_out = q * P_in * conj(q)
    // - P_out is the output vector
    // - q is the orientation quaternion
    // - P_in is the input vector (a*aReal)
    // - conj(q) is the conjugate of the orientation quaternion (q=[w,x,y,z], q*=[w,-x,-y,-z])
    Quaternion p(0, x, y, z);

    // quaternion multiplication: q * p, stored back in p
    p = q -> getProduct(p);

    // quaternion multiplication: p * conj(q), stored back in p
    p = p.getProduct(q -> getConjugate());

    // p quaternion is now [0, x', y', z']
    x = p.x;
    y = p.y;
    z = p.z;
}

/**
 * @brief Create rotated copy of this vector
 * @param[in] q Quaternion to rotate vector by
 * @return Rotated vector
 */
VectorInt16 VectorInt16::getRotated(Quaternion *q) {
    VectorInt16 r(x, y, z);
    r.rotate(q);
    return r;
}



/**
 * @brief Initializes vector to known "zero" state
 */
VectorFloat::VectorFloat() {
    x = 0;
    y = 0;
    z = 0;
}

/**
 * @brief Initializes vector with provided components
 * @param[in] nx X element
 * @param[in] ny Y element
 * @param[in] nz Z element
 */
VectorFloat::VectorFloat(float nx, float ny, float nz) {
    x = nx;
    y = ny;
    z = nz;
}

/**
 * @brief Calculate the magnitude of this vector
 * @return Calculated vector magnitude
 */
float VectorFloat::getMagnitude() {
    return sqrt(x*x + y*y + z*z);
}

/**
 * @brief Normalize this vector (scales so magnitude = 1.0)
 */
void VectorFloat::normalize() {
    float m = getMagnitude();
    x /= m;
    y /= m;
    z /= m;
}

/**
 * @brief Create normalized copy of this vector
 * @return Normalized vector
 */
VectorFloat VectorFloat::getNormalized() {
    VectorFloat r(x, y, z);
    r.normalize();
    return r;
}

/**
 * @brief Rotate this vector by a given quaternion
 * @param[in] q Quaternion to rotate vector by
 */
void VectorFloat::rotate(Quaternion *q) {
    Quaternion p(0, x, y, z);

    // quaternion multiplication: q * p, stored back in p
    p = q -> getProduct(p);

    // quaternion multiplication: p * conj(q), stored back in p
    p = p.getProduct(q -> getConjugate());

    // p quaternion is now [0, x', y', z']
    x = p.x;
    y = p.y;
    z = p.z;
}

/**
 * @brief Create rotated copy of this vector
 * @param[in] q Quaternion to rotate vector by
 * @return Rotated vector
 */
VectorFloat VectorFloat::getRotated(Quaternion *q) {
    VectorFloat r(x, y, z);
    r.rotate(q);
    return r;
}
