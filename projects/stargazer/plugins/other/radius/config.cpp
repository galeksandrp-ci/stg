/*
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 *    Author : Maxim Mamontov <faust@stargazer.dp.ua>
 */

#include "config.h"

#include "stg/common.h"

#include <vector>
#include <stdexcept>

#include <strings.h> // strncasecmp

using STG::Config;

namespace
{

struct ParserError : public std::runtime_error
{
    ParserError(size_t pos, const std::string& message)
        : runtime_error("Parsing error at position " + x2str(pos) + ". " + message),
          position(pos),
          error(message)
    {}
    virtual ~ParserError() throw() {}

    size_t position;
    std::string error;
};

size_t skipSpaces(const std::string& value, size_t start)
{
    while (start < value.length() && std::isspace(value[start]))
        ++start;
    return start;
}

size_t checkChar(const std::string& value, size_t start, char ch)
{
    if (start >= value.length())
        throw ParserError(start, "Unexpected end of string. Expected '" + std::string(1, ch) + "'.");
    if (value[start] != ch)
        throw ParserError(start, "Expected '" + std::string(1, ch) + "', got '" + std::string(1, value[start]) + "'.");
    return start + 1;
}

std::pair<size_t, std::string> readString(const std::string& value, size_t start)
{
    std::string dest;
    while (start < value.length() && !std::isspace(value[start]))
        dest.push_back(value[start++]);
    if (dest.empty()) {
        if (start == value.length())
            throw ParserError(start, "Unexpected end of string. Expected string.");
        else
            throw ParserError(start, "Unexpected whitespace. Expected string.");
    }
    return std::make_pair(start, dest);
}

Config::Pairs toPairs(const std::vector<std::string>& values)
{
    if (values.empty())
        return Config::Pairs();
    std::string value(values[0]);
    Config::Pairs res;
    size_t start = 0;
    while (start < value.size()) {
        Config::Pair pair;
        start = skipSpaces(value, start);
        size_t pairStart = start;
        start = checkChar(value, start, '(');
        std::pair<size_t, std::string> key = readString(value, start);
        start = key.first;
        pair.first = key.second;
        start = skipSpaces(value, start);
        start = checkChar(value, start, ',');
        start = skipSpaces(value, start);
        std::pair<size_t, std::string> val = readString(value, start);
        start = key.first;
        pair.second = val.second;
        start = skipSpaces(value, start);
        start = checkChar(value, start, ')');
        if (res.find(pair.first) != res.end())
            throw ParserError(pairStart, "Duplicate field.");
        res.insert(pair);
    }
    return res;
}

bool toBool(const std::vector<std::string>& values)
{
    if (values.empty())
        return false;
    std::string value(values[0]);
    return strncasecmp(value.c_str(), "yes", 3) == 0;
}

std::string toString(const std::vector<std::string>& values)
{
    if (values.empty())
        return "";
    return values[0];
}

template <typename T>
T toInt(const std::vector<std::string>& values)
{
    if (values.empty())
        return 0;
    T res = 0;
    if (str2x(values[0], res) == 0)
        return res;
    return 0;
}

Config::Pairs parseVector(const std::string& paramName, const MODULE_SETTINGS& params)
{
    for (size_t i = 0; i < params.moduleParams.size(); ++i)
        if (params.moduleParams[i].param == paramName)
            return toPairs(params.moduleParams[i].value);
    return Config::Pairs();
}

bool parseBool(const std::string& paramName, const MODULE_SETTINGS& params)
{
    for (size_t i = 0; i < params.moduleParams.size(); ++i)
        if (params.moduleParams[i].param == paramName)
            return toBool(params.moduleParams[i].value);
    return false;
}

std::string parseString(const std::string& paramName, const MODULE_SETTINGS& params)
{
    for (size_t i = 0; i < params.moduleParams.size(); ++i)
        if (params.moduleParams[i].param == paramName)
            return toString(params.moduleParams[i].value);
    return "";
}

template <typename T>
T parseInt(const std::string& paramName, const MODULE_SETTINGS& params)
{
    for (size_t i = 0; i < params.moduleParams.size(); ++i)
        if (params.moduleParams[i].param == paramName)
            return toInt<T>(params.moduleParams[i].value);
    return 0;
}

} // namespace anonymous

Config::Config(const MODULE_SETTINGS& settings)
    : match(parseVector("match", settings)),
      modify(parseVector("modify", settings)),
      reply(parseVector("reply", settings)),
      verbose(parseBool("verbose", settings)),
      bindAddress(parseString("bind_address", settings)),
      portStr(parseString("port", settings)),
      port(parseInt<uint16_t>("port", settings)),
      key(parseString("key", settings))
{
}