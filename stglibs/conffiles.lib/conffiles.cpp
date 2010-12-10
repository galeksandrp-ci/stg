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
 *    Date: 27.10.2002
 */

/*
 *    Author : Boris Mikhailenko <stg34@ua.fm>
 */

 /*
 $Revision: 1.5 $
 $Date: 2009/10/22 11:40:22 $
 */

//---------------------------------------------------------------------------
#include <cerrno>
#include <cstring>
#include <cstdlib>

#include <fstream>

#include "conffiles.h"
#include "common.h"

using namespace std;

//---------------------------------------------------------------------------
bool StringCaseCmp(const string & str1, const string & str2)
{
return (strcasecmp(str1.c_str(), str2.c_str()) < 0);
}
//---------------------------------------------------------------------------
CONFIGFILE::CONFIGFILE(const string & fn, bool nook)
    : param_val(StringCaseCmp),
      fileName(fn),
      error(0)
{
ifstream f(fileName.c_str());

if (!f)
    {
    if (!nook)
        error = -1;
    return;
    }

string line;
while (getline(f, line))
    {
    size_t pos = line.find('#');
    if (pos != string::npos)
        line.resize(pos);

    if (line.find_first_not_of(" \t\r") == string::npos)
        continue;

    pos = line.find_first_of('=');
    if (pos == string::npos)
        {
        error = -1;
        return;
        }

    string parameter = line.substr(0, pos);
    string value = line.substr(pos + 1);
    param_val[parameter] = value;
    }
}
//---------------------------------------------------------------------------
CONFIGFILE::~CONFIGFILE()
{
}
//---------------------------------------------------------------------------
const string & CONFIGFILE::GetFileName() const
{
return fileName;
}
//---------------------------------------------------------------------------
int CONFIGFILE::Error()
{
int e = error;
error = 0;
return e;
}
//---------------------------------------------------------------------------
int CONFIGFILE::Flush()
{
ofstream f(fileName.c_str());
if (!f.is_open())
    {
    error = EIO;
    return EIO;
    }

map<string, string>::const_iterator it = param_val.begin();
while (it != param_val.end())
    {
    f << it->first << "=" << it->second << "\n";
    it++;
    }

f.close();

return 0;
}
/*//---------------------------------------------------------------------------
int CONFIGFILE::ReadString(const string & param, char * str, int * maxLen, const char * defaultVal) const
{
it = param_val.find(param);
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    strncpy(str, param_val[param].c_str(), *maxLen);
    *maxLen = param_val[param].size();
    return 0;
    }

strncpy(str, defaultVal, *maxLen);
*maxLen = strlen(defaultVal);
return -1;
}*/
//---------------------------------------------------------------------------
int CONFIGFILE::ReadString(const string & param, string * val, const string & defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    *val = it->second;
    return 0;
    }

*val = defaultVal;
return -1;
}
/*//---------------------------------------------------------------------------
int CONFIGFILE::WriteString(const string & param, const char * val)
{
WriteString(param, string(val));
return 0;
}*/
//---------------------------------------------------------------------------
int CONFIGFILE::WriteString(const string & param, const string &val)
{
param_val[param] = val;
Flush();
return 0;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadTime(const string & param, time_t * val, time_t defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));

if (it != param_val.end())
    {
    char *res;
    *val = strtol(it->second.c_str(), &res, 10);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadInt(const string & param, int * val, int defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    char *res;
    *val = strtol(it->second.c_str(), &res, 10);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadUInt(const string & param, unsigned int * val, unsigned int defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    char *res;
    *val = strtoul(it->second.c_str(), &res, 10);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadLongInt(const string & param, long int * val, long int defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    char *res;
    *val = strtol(it->second.c_str(), &res, 10);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadULongInt(const string & param, unsigned long int * val, unsigned long int defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    char *res;
    *val = strtoul(it->second.c_str(), &res, 10);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadLongLongInt(const string & param, int64_t * val, int64_t defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    char *res;
    *val = strtoll(it->second.c_str(), &res, 10);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadULongLongInt(const string & param, uint64_t * val, uint64_t defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    char *res;
    *val = strtoull(it->second.c_str(), &res, 10);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadShortInt(const string & param, short int * val, short int defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    char *res;
    *val = (short)strtol(it->second.c_str(), &res, 10);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadUShortInt(const string & param, unsigned short int * val, unsigned short int defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    char *res;
    *val = (short)strtoul(it->second.c_str(), &res, 10);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::WriteInt(const string & param, int64_t val)
{
string s;
//sprintf(s, "%lld", val);
x2str(val, s);
param_val[param] = s;
Flush();
return 0;
}
//---------------------------------------------------------------------------
int CONFIGFILE::ReadDouble(const string & param, double * val, double defaultVal) const
{
const map<string, string>::const_iterator it(param_val.find(param));
// ����� ������ ����������

if (it != param_val.end())
    {
    // ���-�� �����
    char *res;
    *val = strtod(it->second.c_str(), &res);
    if (*res != 0)
        {
        *val = defaultVal; //Error!
        return EINVAL;
        }
    return 0;
    }

*val = defaultVal;
return -1;
}
//---------------------------------------------------------------------------
int CONFIGFILE::WriteDouble(const string & param, double val)
{
char s[30];
sprintf(s, "%f", val);
param_val[param] = s;
Flush();
return 0;
}
//---------------------------------------------------------------------------
