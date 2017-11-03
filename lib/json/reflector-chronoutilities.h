#ifndef REFLECTIVE_RAPIDJSON_JSON_REFLECTOR_CHRONO_UTILITIES_H
#define REFLECTIVE_RAPIDJSON_JSON_REFLECTOR_CHRONO_UTILITIES_H

/*!
 * \file reflector-chronoutilities.h
 * \brief Contains functions for (de)serializing objects from the chrono utilities provided by the
 *        C++ utilities library.
 * \remarks This file demonstrates implementing custom (de)serialization for specific types.
 */

#include "./reflector.h"

#include <c++utilities/chrono/datetime.h>
#include <c++utilities/chrono/timespan.h>
#include <c++utilities/conversion/conversionexception.h>

namespace ReflectiveRapidJSON {
namespace JsonReflector {

// define functions to "push" values to a RapidJSON array or object

template <>
inline void push<ChronoUtilities::DateTime>(
    const ChronoUtilities::DateTime &reflectable, RAPIDJSON_NAMESPACE::Value::Array &value, RAPIDJSON_NAMESPACE::Document::AllocatorType &allocator)
{
    const std::string str(reflectable.toIsoString());
    value.PushBack(RAPIDJSON_NAMESPACE::GenericValue<RAPIDJSON_NAMESPACE::UTF8<char>>(str.data(), str.size(), allocator), allocator);
}

template <>
inline void push<ChronoUtilities::DateTime>(const ChronoUtilities::DateTime &reflectable, const char *name, RAPIDJSON_NAMESPACE::Value::Object &value,
    RAPIDJSON_NAMESPACE::Document::AllocatorType &allocator)
{
    const std::string str(reflectable.toIsoString());
    value.AddMember(RAPIDJSON_NAMESPACE::StringRef(name),
        RAPIDJSON_NAMESPACE::GenericValue<RAPIDJSON_NAMESPACE::UTF8<char>>(str.data(), str.size(), allocator), allocator);
}

template <>
inline void push<ChronoUtilities::TimeSpan>(
    const ChronoUtilities::TimeSpan &reflectable, RAPIDJSON_NAMESPACE::Value::Array &value, RAPIDJSON_NAMESPACE::Document::AllocatorType &allocator)
{
    const std::string str(reflectable.toString());
    value.PushBack(RAPIDJSON_NAMESPACE::GenericValue<RAPIDJSON_NAMESPACE::UTF8<char>>(str.data(), str.size(), allocator), allocator);
}

template <>
inline void push<ChronoUtilities::TimeSpan>(const ChronoUtilities::TimeSpan &reflectable, const char *name, RAPIDJSON_NAMESPACE::Value::Object &value,
    RAPIDJSON_NAMESPACE::Document::AllocatorType &allocator)
{
    const std::string str(reflectable.toString());
    value.AddMember(RAPIDJSON_NAMESPACE::StringRef(name),
        RAPIDJSON_NAMESPACE::GenericValue<RAPIDJSON_NAMESPACE::UTF8<char>>(str.data(), str.size(), allocator), allocator);
}

// define functions to "pull" values from a RapidJSON array or object

template <>
inline void pull<ChronoUtilities::DateTime>(ChronoUtilities::DateTime &reflectable,
    const RAPIDJSON_NAMESPACE::GenericValue<RAPIDJSON_NAMESPACE::UTF8<char>> &value, JsonDeserializationErrors *errors)
{
    std::string asString;
    pull(asString, value, errors);
    try {
        reflectable = ChronoUtilities::DateTime::fromIsoStringGmt(asString.data());
    } catch (const ConversionUtilities::ConversionException &) {
        if (errors) {
            errors->reportConversionError(JsonType::String);
        }
    }
}

template <>
inline void pull<ChronoUtilities::TimeSpan>(ChronoUtilities::TimeSpan &reflectable,
    const RAPIDJSON_NAMESPACE::GenericValue<RAPIDJSON_NAMESPACE::UTF8<char>> &value, JsonDeserializationErrors *errors)
{
    std::string asString;
    pull(asString, value, errors);
    try {
        reflectable = ChronoUtilities::TimeSpan::fromString(asString.data());
    } catch (const ConversionUtilities::ConversionException &) {
        if (errors) {
            errors->reportConversionError(JsonType::String);
        }
    }
}

} // namespace JsonReflector
} // namespace ReflectiveRapidJSON

#endif // REFLECTIVE_RAPIDJSON_JSON_REFLECTOR_CHRONO_UTILITIES_H
