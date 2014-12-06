// Keyglove controller source code - 3D math helper function declarations
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
 * @file support_helper_3dmath.h
 * @brief 3D math helper function declarations
 * @author Jeff Rowberg
 * @date 2014-11-07
 */

#ifndef _SUPPORT_HELPER_3DMATH_H_
#define _SUPPORT_HELPER_3DMATH_H_

/**
 * @brief Provides float-based quaternion representation and manipulation
 */
class Quaternion {
    public:
        float w; ///< Vector rotation component of quaternion
        float x; ///< X-axis rotation component of quaternion
        float y; ///< Y-axis rotation component of quaternion
        float z; ///< Z-axis rotation component of quaternion

        Quaternion();
        Quaternion(float nw, float nx, float ny, float nz);

        Quaternion getProduct(Quaternion q);
        Quaternion getConjugate();
        float getMagnitude();
        void normalize();
        Quaternion getNormalized();
};

/**
 * @brief Provides int16_t-based vector representation and manipulation
 */
class VectorInt16 {
    public:
        int16_t x; ///< X-axis component of vector
        int16_t y; ///< Y-axis component of vector
        int16_t z; ///< Z-axis component of vector

        VectorInt16();
        VectorInt16(int16_t nx, int16_t ny, int16_t nz);

        float getMagnitude();
        void normalize();
        VectorInt16 getNormalized();
        void rotate(Quaternion *q);
        VectorInt16 getRotated(Quaternion *q);
};

/**
 * @brief Provides float-based vector representation and manipulation
 */
class VectorFloat {
    public:
        float x; ///< X-axis component of vector
        float y; ///< Y-axis component of vector
        float z; ///< Z-axis component of vector

        VectorFloat();
        VectorFloat(float nx, float ny, float nz);

        float getMagnitude();
        void normalize();
        VectorFloat getNormalized();
        void rotate(Quaternion *q);
        VectorFloat getRotated(Quaternion *q);
};

#endif /* _SUPPORT_HELPER_3DMATH_H_ */