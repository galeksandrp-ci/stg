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
 *    Author : Boris Mikhailenko <stg34@stargazer.dp.ua>
 *    Author : Maxim Mamontov <faust@stargazer.dp.ua>
 */

#ifndef __STG_STGLIBS_SRVCONF_PARSER_GET_USER_H__
#define __STG_STGLIBS_SRVCONF_PARSER_GET_USER_H__

#include "base.h"
#include "property.h"

#include "stg/servconf_types.h"

#include <string>

namespace STG
{
namespace GET_USER
{

class PARSER: public STG::PARSER
{
public:
    typedef GET_USER::INFO INFO;

    PARSER(CALLBACK f, void * data, const std::string & encoding);
    virtual ~PARSER();
    int  ParseStart(const char * el, const char ** attr);
    void ParseEnd(const char * el);
    void Failure(const std::string & reason) { callback(false, reason, info, data); }

private:
    PROPERTY_PARSERS propertyParsers;
    CALLBACK callback;
    void * data;
    std::string encoding;
    INFO info;
    int depth;
    bool parsingAnswer;
    std::string error;

    void ParseUser(const char * el, const char ** attr);
    void ParseUserParams(const char * el, const char ** attr);
    void ParseAuthBy(const char * el, const char ** attr);
    void ParseServices(const char * el, const char ** attr);
};

} // namespace GET_USER
} // namespace STG

#endif
