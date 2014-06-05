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
* Author : Maxim Mamontov <faust@stargazer.dp.ua>
*/

#ifndef PCAP_CAP_H
#define PCAP_CAP_H

#include "stg/plugin.h"
#include "stg/module_settings.h"
#include "stg/logger.h"

#include <string>
#include <vector>

#include <pcap.h>
#include <pthread.h>

class USERS;
class TARIFFS;
class ADMINS;
class TRAFFCOUNTER;
class SETTINGS;

class TRAFFCOUNTER;

struct DEV
{
    std::string device;
    std::string filterExpression;
    pcap_t * handle;
    struct bpf_program filter;
    int fd;
};

typedef std::vector<DEV> DEV_MAP;

class PCAP_CAP : public PLUGIN {
public:
    PCAP_CAP();
    virtual ~PCAP_CAP() {}

    void                SetTraffcounter(TRAFFCOUNTER * tc) { traffCnt = tc; }

    int                 Start();
    int                 Stop();
    int                 Reload() { return 0; }
    bool                IsRunning() { return isRunning; }

    int                 ParseSettings() { return 0; }
    const std::string & GetStrError() const { return errorStr; }
    std::string         GetVersion() const;
    uint16_t            GetStartPosition() const { return 40; }
    uint16_t            GetStopPosition() const { return 40; }

private:
    PCAP_CAP(const PCAP_CAP & rvalue);
    PCAP_CAP & operator=(const PCAP_CAP & rvalue);

    static void *       Run(void *);

    mutable std::string errorStr;

    pthread_t           thread;
    bool                nonstop;
    bool                isRunning;
    DEV_MAP             devices;

    TRAFFCOUNTER *      traffCnt;

    PLUGIN_LOGGER       logger;
};
//-----------------------------------------------------------------------------

#endif
