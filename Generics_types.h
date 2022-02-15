/**
 * The Generics_types headers defines basic types shared by Generics Classes
 *
 * @file Generic_types.h
 * @author Boris Burdin
 * @date 20220215 - One way plateform : check for active collision
 *                  Collisison struct add isActive attribut
 * @date 20220116 - Add Contact Listener
 *                  add enum ContactState
 * @date 20220107 - Forward declaration of all Generics classes
 * @date 20220106 - Add body multiconfig
 *                  Add typedef typeBodyconfigID
 * @date 20211214 - Edge collider : consider restitution vector
 *                  Add operator^ to Vect2d, Add method pi
 * @date 20211128 - Algo for Min collision detection
 *                  Add method norm to Vect2d
 * @date 20211123 - Add SpacePartitionBody BodyType
 * @date 20210529 - Generic collision between two colliders
 *                  Add struct Collision
 * @date 20210501 - Collider class > struct
 *                  Remove ColliderProperties
 *                - Add AABB struct
 * @date 20210424 - Add SpacePartitionColliderProperties
 * @date 20210423 - Add vect2d struct
 * @date 20200605 - Initial Release
 */

#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>

namespace Generics
{
    ///Forward declarations
    class SpacePartitionGrid;
    class SpacePartitionBody;
    class SpacePartitionBodyTemplate;
    class SpacePartitionCollider;
    class SpacePartitionWorld;
    class SpacePartitionContactListener;
    class SpacePartitionContact;
    
    //StateMachine

	typedef std::string typeStateID;
	typedef std::string typeTransitionID;

    //SpacePartition

    //comparison of two floats
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
        float norm() const
        {
            return (sqrt(x*x + y*y));
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
    inline float operator^(const Vect2d& a, const Vect2d& b) {
        return a.x * b.y - b.x * a.y;
    }

    //pi expression
    inline const double pi() { return std::atan(1) * 4; };

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
        bool intersect(const AABB& other) const
        {
            return (abs(position.x - other.position.x) * 2 <= (width + other.width)) &&
                (abs(position.y - other.position.y) * 2 <= (height + other.height));
        }
    };

    ////SpacePartitionCollider
    
    enum class ColliderType
    {
        BOX,
        EDGE
    };
    typedef std::string typeColliderTag;

    struct Collision
    {
        bool isTouching = false;
        Vect2d response = { 0, 0 };
        bool isActive = true;
    };

    ////SpacePartitionBody

    // Bodies can have several configs for colliders association
    typedef std::string typeBodyconfigID;

    enum class BodyType
    {
        STATIC,
        DYNAMIC
    };

    ////SpacePartitionContactListener

    enum class ContactState
    {
        PENDING,
        NEW,
        UPDATED
    };
}