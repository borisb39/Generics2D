/**
 * The Generics_types headers defines basic types shared by Generics Classes
 *
 * @file Generic_types.h
 * @author Boris Burdin
 * @date 20200605 - Initial Release
 * @date 20210423 - Add vect2d struct
 * @date 20210424 - Add SpacePartitionColliderProperties
 * @date 20210501 - Add AABB struct
 */

#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <map>

namespace Generics
{
    //StateMachine

	typedef std::string typeStateID;
	typedef std::string typeTransitionID;

    //SpacePartition

    //comparison of to float
    inline bool isEqual(float f1, float f2)
    {
        static constexpr auto epsilon = 1.0e-05f;
        if (abs(f1 - f2) <= epsilon)
            return true;
        return abs(f1 - f2) <= epsilon * fmax(abs(f1), abs(f2));
    }

    ////2d vector with basic arithmetic operations
    struct Vect2d {
        float x; 
        float y;
        bool operator==(const Vect2d&  other) const {
            return isEqual(x, other.x) && isEqual(y, other.y);
        }
        Vect2d& operator+=(const Vect2d& other) {
            x += other.x; y += other.y;
            return *this;
        }
        Vect2d& operator-=(const Vect2d& other) {
            x -= other.x; y -= other.y;
            return *this;
        }
        template<typename T>
        Vect2d& operator*=(const T& val) {
            x *= val; y *= val;
            return *this;
        }
        template<typename T>
        Vect2d& operator/=(const T& val) {
            x /= val; y /= val;
            return *this;
        }
    };
    inline Vect2d operator+(const Vect2d&  a, const Vect2d& b) {
        Vect2d c(a);
        return c += b;
    }
    inline Vect2d operator-(const Vect2d&  a, const Vect2d& b) {
        Vect2d c(a);
        return c -= b;
    }
    template<typename T>
    inline Vect2d operator*(const Vect2d&  a, const T& val) {
        Vect2d c(a);
        return c *= val;
    }
    template<typename T>
    inline Vect2d operator/(const Vect2d&  a, const T& val) {
        Vect2d c(a);
        return c /= val;
    }

    ////SpacePartition Axis Aligned Bounding Box
    struct AABB
    {
        Vect2d position{0, 0};
        float width = 0;
        float height = 0;
        bool operator==(const AABB& other) const {
            return (position == other.position)
                && isEqual(width, other.width)
            && isEqual(height, other.height);
        }
        float top() const
        {
            return position.y + height / 2;
        }
        float bottom() const
        {
            return position.y - height / 2;
        }
        float right() const
        {
            return position.x + width / 2;
        }
        float left() const
        {
            return position.x - width / 2;
        }
    };

    ////SpacePartitionCollider

    enum class ColliderType
    {
        BOX,
        CIRCLE,
        EDGE
    };
    typedef std::string typeColliderTag;
    struct ColliderProperties
    {
        ColliderType type = ColliderType::BOX;
        typeColliderTag tag = "None";
        bool isEnabled = false;
        bool isSensor = false;
        Vect2d position{ 0, 0 };
        float boxWidth = 0;
        float boxHeight = 0;
        float radius = 0;
        Vect2d vertice0{ 0, 0 };
        Vect2d vertice1{ 0, 0 };
    };
}