#ifndef __SNMP_AGENT_H__
#define __SNMP_AGENT_H__

#include <pthread.h>

#include <string>
#include <map>

#include "asn1/SMUX-PDUs.h"
#include "asn1/ObjectSyntax.h"

#include "stg/os_int.h"
#include "stg/plugin.h"
#include "stg/module_settings.h"
#include "stg/users.h"
#include "stg/tariffs.h"

#include "sensors.h"

extern "C" PLUGIN * GetPlugin();

class USER;
class SETTINGS;
class SNMP_AGENT;

typedef bool (SNMP_AGENT::*SMUXPacketHandler)(const SMUX_PDUs_t * pdus);
typedef bool (SNMP_AGENT::*PDUsHandler)(const PDUs_t * pdus);
typedef std::map<SMUX_PDUs_PR, SMUXPacketHandler> SMUXHandlers;
typedef std::map<PDUs_PR, PDUsHandler> PDUsHandlers;
//-----------------------------------------------------------------------------
class SNMP_AGENT_SETTINGS {
public:
    SNMP_AGENT_SETTINGS();
    virtual ~SNMP_AGENT_SETTINGS() {}
    const std::string & GetStrError() const { return errorStr; }
    int ParseSettings(const MODULE_SETTINGS & s);

    uint32_t GetIP() const { return ip; }
    uint16_t GetPort() const { return port; }
    const std::string GetPassword() const { return password; }

private:
    mutable std::string errorStr;

    uint32_t ip;
    uint16_t port;
    std::string password;
};
//-----------------------------------------------------------------------------
class SNMP_AGENT : public PLUGIN {
public:
    SNMP_AGENT();
    virtual ~SNMP_AGENT();

    void SetUsers(USERS * u) { users = u; }
    void SetTariffs(TARIFFS * t) { tariffs = t; }
    void SetAdmins(ADMINS *) {}
    void SetTraffcounter(TRAFFCOUNTER *) {}
    void SetStore(STORE *) {}
    void SetStgSettings(const SETTINGS *) {}
    void SetSettings(const MODULE_SETTINGS & s) { settings = s; }
    int ParseSettings();

    int Start();
    int Stop();
    int Reload() { return 0; }
    bool IsRunning() { return running && !stopped; }

    const std::string & GetStrError() const { return errorStr; }
    const std::string GetVersion() const { return "Stg SNMP Agent 1.0"; }
    uint16_t GetStartPosition() const { return 100; }
    uint16_t GetStopPosition() const { return 100; }

private:
    static void * Runner(void * d);
    void Run();
    bool PrepareNet();

    bool DispatchPDUs(const SMUX_PDUs_t * pdus);

    bool CloseHandler(const SMUX_PDUs_t * pdus);
    bool RegisterResponseHandler(const SMUX_PDUs_t * pdus);
    bool PDUsHandler(const SMUX_PDUs_t * pdus);
    bool CommitOrRollbackHandler(const SMUX_PDUs_t * pdus);

    bool GetRequestHandler(const PDUs_t * pdus);
    bool GetNextRequestHandler(const PDUs_t * pdus);
    bool SetRequestHandler(const PDUs_t * pdus);

    USERS * users;
    TARIFFS * tariffs;

    mutable std::string errorStr;
    SNMP_AGENT_SETTINGS snmpAgentSettings;
    MODULE_SETTINGS settings;

    pthread_t thread;
    pthread_mutex_t mutex;
    bool running;
    bool stopped;

    int sock;

    SMUXHandlers smuxHandlers;
    PDUsHandlers pdusHandlers;
    Sensors sensors;
};
//-----------------------------------------------------------------------------

extern "C" PLUGIN * GetPlugin();

#endif
