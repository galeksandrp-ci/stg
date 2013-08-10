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

#ifndef __STG_STGLIBS_SRVCONF_PARSER_CHG_USER_H__
#define __STG_STGLIBS_SRVCONF_PARSER_CHG_USER_H__

#include "parser.h"

class PARSER_CHG_USER: public PARSER
{
public:
    typedef int (* CALLBACK)(const char * asnwer, void * data);

    PARSER_CHG_USER();
    int  ParseStart(const char * el, const char ** attr);
    void ParseEnd(const char * el);
    void SetCallback(CALLBACK f, void * data);
private:
    CALLBACK callback;
    void * data;
    int depth;

    void ParseAnswer(const char * el, const char ** attr);
};

#endif
