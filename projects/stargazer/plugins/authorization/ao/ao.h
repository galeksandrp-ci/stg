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
 */

/*
 $Revision: 1.21 $
 $Date: 2010/09/10 06:38:26 $
 $Author: faust $
*/

#ifndef AO_H
#define AO_H

#include <pthread.h>

#include <string>
#include <list>

#include "auth.h"
#include "store.h"
#include "notifer.h"
#include "user_ips.h"
#include "user.h"

extern "C" PLUGIN * GetPlugin();

class AUTH_AO;
class USERS;
//-----------------------------------------------------------------------------
template <typename varParamType>
class CHG_BEFORE_NOTIFIER: public PROPERTY_NOTIFIER_BASE<varParamType> {
public:
                CHG_BEFORE_NOTIFIER(AUTH_AO & a, USER_PTR u) : user(u), auth(a) {}
    void        Notify(const varParamType & oldValue, const varParamType & newValue);
    USER_PTR      GetUser() const { return user; }

private:
    USER_PTR      user;
    const       AUTH_AO & auth;
};
//-----------------------------------------------------------------------------
template <typename varParamType>
class CHG_AFTER_NOTIFIER: public PROPERTY_NOTIFIER_BASE<varParamType> {
public:
                CHG_AFTER_NOTIFIER(AUTH_AO & a, USER_PTR u) : user(u), auth(a) {}
    void        Notify(const varParamType & oldValue, const varParamType & newValue);
    USER_PTR      GetUser() const { return user; }

private:
    USER_PTR          user;
    const AUTH_AO & auth;
};
//-----------------------------------------------------------------------------
class AUTH_AO_SETTINGS {
public:
    const std::string & GetStrError() const { static std::string s; return s; }
    int ParseSettings(const MODULE_SETTINGS &) { return 0; }
};
//-----------------------------------------------------------------------------
class AUTH_AO :public AUTH {
public:
    AUTH_AO();
    virtual ~AUTH_AO(){};

    void                SetUsers(USERS * u);
    void                SetTariffs(TARIFFS *) {}
    void                SetAdmins(ADMINS *) {}
    void                SetTraffcounter(TRAFFCOUNTER *) {}
    void                SetStore(STORE *) {}
    void                SetStgSettings(const SETTINGS *) {}

    int                 Start();
    int                 Stop();
    int                 Reload() { return 0; }
    bool                IsRunning();
    void                SetSettings(const MODULE_SETTINGS & s);
    int                 ParseSettings();
    const std::string & GetStrError() const;
    const std::string   GetVersion() const;
    uint16_t            GetStartPosition() const;
    uint16_t            GetStopPosition() const;

    void                AddUser(USER_PTR u);
    void                DelUser(USER_PTR u);

    void                UpdateUserAuthorization(USER_PTR u) const;
    void                Unauthorize(USER_PTR u) const;

    int                 SendMessage(const STG_MSG & msg, uint32_t ip) const;

private:
    void                GetUsers();
    void                SetUserNotifiers(USER_PTR u);
    void                UnSetUserNotifiers(USER_PTR u);

    mutable std::string errorStr;
    AUTH_AO_SETTINGS    aoSettings;
    USERS *             users;
    std::list<USER_PTR> usersList;
    bool                isRunning;
    MODULE_SETTINGS     settings;

    /*
    �� ������ ������������� ����������� ����������� ����� ��� ���������
    ��������� ��� ����������:
    - alwaysOnline
    - ips
    */

    list<CHG_BEFORE_NOTIFIER<int> >         BeforeChgAONotifierList;
    list<CHG_AFTER_NOTIFIER<int> >          AfterChgAONotifierList;

    list<CHG_BEFORE_NOTIFIER<USER_IPS> >    BeforeChgIPNotifierList;
    list<CHG_AFTER_NOTIFIER<USER_IPS> >     AfterChgIPNotifierList;

    class ADD_USER_NONIFIER: public NOTIFIER_BASE<USER_PTR> {
    public:
        ADD_USER_NONIFIER(AUTH_AO & a) : auth(a) {}
        virtual ~ADD_USER_NONIFIER() {}

        void Notify(const USER_PTR & user)
            {
            auth.AddUser(user);
            }

    private:
        AUTH_AO & auth;
    } onAddUserNotifier;

    class DEL_USER_NONIFIER: public NOTIFIER_BASE<USER_PTR> {
    public:
        DEL_USER_NONIFIER(AUTH_AO & a) : auth(a) {}
        virtual ~DEL_USER_NONIFIER() {}

        void Notify(const USER_PTR & user)
            {
            auth.DelUser(user);
            }

    private:
        AUTH_AO & auth;
    } onDelUserNotifier;

};
//-----------------------------------------------------------------------------

#endif
