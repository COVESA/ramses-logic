//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include "ramses-logic/APIExport.h"
#include "ramses-logic/EPropertyType.h"

#include <cstddef>
#include <optional>
#include <memory>
#include <string>

namespace rlogic::internal
{
    class PropertyImpl;
}

namespace rlogic
{
    /**
    * Represents a generic property slot of the #rlogic::LogicNode and its derived classes.
    * Properties can have primitive types (string, integer, etc.) or complex types (structs, arrays).
    * Complex types can have "children", i.e. nested properties: named fields (structs), or indexed
    * fields (arrays).
    */
    class Property
    {
    public:
        /**
        * Returns the type of this Property
        *
        * @return the type of this Property
        */
        RLOGIC_API EPropertyType getType() const;

        /**
        * Returns the name of this Property. Note that not all properties have a name -
        * for example an array element does not have a name. In that case the result
        * will be an empty string view. Struct fields always have a non-empty name.
        *
        * @return the name of this Property
        */
        RLOGIC_API std::string_view getName() const;

        /**
        * Returns the amount of available child (nested) properties. If the Property
        * is of type #rlogic::EPropertyType::Struct, the returned number will correspond
        * to the number of named properties of the struct. If the Property is of type
        * #rlogic::EPropertyType::Array, the method will return the array size. For all
        * other property types #getChildCount returns zero.
        *
        * @return the number of nested properties.
        */
        RLOGIC_API size_t getChildCount() const;

        /**
        * Returns the child property with the given \p index. \p index must be < #getChildCount().
        *
        * This method can be used to get nested properties of structs and arrays. For primitive types this will
        * always return nullptr.
        *
        * Note that array indexing in #getChild follows C++ conventions and **not** Lua conventions! Inside the Lua scripts, you can
        * and must use Lua conventions when indexing arrays (start at 1, end at N) while in C++ you must use [0, N-1].
        *
        * Struct properties can also be retrieved by index. The ordering is **not** guaranteed to match the order of
        * declaration inside Lua scripts (Lua explicitly warns to not rely on ordering of named table entries!).
        * However, once a script is created, the index will not change, i.e. it is permitted that user code caches
        * the property index for faster future access.
        *
        * @return the child with the given index, or nullptr if the property is primitive or the index is out of range
        */
        RLOGIC_API const Property* getChild(size_t index) const;

        /**
        * @copydoc getChild(size_t index) const
        */
        RLOGIC_API Property* getChild(size_t index);

        /**
        * Searches for a child with the given name. Only properties of type #rlogic::EPropertyType::Struct can return a child by name.
        * In case of a primitive property or array this method will return nullptr.
        *
        * Note that this method may be slower than #getChild(size_t index) as it must do a string-based search.
        *
        * @return the child with the given name, or nullptr if property is not of type #rlogic::EPropertyType::Struct
        */
        RLOGIC_API Property* getChild(std::string_view name);

        /**
        * @copydoc getChild(std::string_view name)
        */
        RLOGIC_API const Property* getChild(std::string_view name) const;

        /**
        * Returns the value of this property. The supported template types are defined by
        * #rlogic::IsPrimitiveProperty where IsPrimitiveProperty<T>::value == true for a type T.
        *
        * Attention! We recommend always specifying the template argument T explicitly, and don't rely on the compiler's
        * type deduction! If T is not one of the supported types, a linker error will be issued!
        *
        * If the template type and the internal type do not match, std::nullopt will be returned.
        *
        * @return the value of this Property as std::optional or std::nullopt if T does not match.
        */
        template <typename T> RLOGIC_API std::optional<T> get() const;

        /**
        * Sets the value of this Property. Same rules apply to template parameter T as in #get()
        *
        * Attention! We recommend always specifying the template argument T explicitly, and don't rely on the compiler's
        * type deduction! If T is not one of the supported types, a linker error will be issued!
        *
        * @param value the value to set for this Property
        * @return true if setting the \p value was successful, false otherwise.
        */
        template <typename T> RLOGIC_API bool set(T value);

        /**
        * Constructor of Property. User is not supposed to call this - properties are created by other factory classes
        *
        * @param impl implementation details of the property
        */
        explicit Property(std::unique_ptr<internal::PropertyImpl> impl) noexcept;

        /**
        * Destructor of Property. User is not supposed to call this - properties are destroyed by other factory classes
        */
        ~Property() noexcept;

        /**
        * Copy Constructor of Property is deleted because properties are not supposed to be copied
        * @param other property to copy from
        */
        Property(const Property& other) = delete;

        /**
        * Move Constructor of Property is deleted because properties are not supposed to be moved
        * @param other property to move from
        */
        Property(Property&& other) = delete;

        /**
        * Assignment operator of Property is deleted because properties are not supposed to be copied
        * @param other property to assign from
        */
        Property& operator=(const Property& other) = delete;

        /**
        * Move assignment operator of Property is deleted because properties are not supposed to be moved
        * @param other property to move from
        */
        Property& operator=(Property&& other) = delete;

        /**
        * Implementation details of the Property class
        */
        std::unique_ptr<internal::PropertyImpl> m_impl;

    };
}
