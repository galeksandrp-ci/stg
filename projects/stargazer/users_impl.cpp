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
 *    Author : Boris Mikhailenko <stg34@stargazer.dp.ua>
 */

#include <pthread.h>

#include <csignal>
#include <cassert>
#include <algorithm>
#include <utility>
#include <string>
#include <vector>

#include "stg/settings.h"
#include "stg/common.h"

#include "users_impl.h"
#include "stg_timer.h"

extern volatile time_t stgTime;

using STG::UsersImpl;

//-----------------------------------------------------------------------------
UsersImpl::UsersImpl(SettingsImpl * s, Store * st,
                    Tariffs * t, Services & svcs,
                    const Admin& sa)
    : settings(s),
      tariffs(t),
      m_services(svcs),
      store(st),
      sysAdmin(sa),
      WriteServLog(Logger::get()),
      nonstop(false),
      isRunning(false),
      handle(0)
{
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
pthread_mutex_init(&mutex, &attr);
}
//-----------------------------------------------------------------------------
UsersImpl::~UsersImpl()
{
pthread_mutex_destroy(&mutex);
}
//-----------------------------------------------------------------------------
int UsersImpl::FindByNameNonLock(const std::string & login, user_iter * user)
{
const std::map<std::string, user_iter>::const_iterator iter(loginIndex.find(login));
if (iter == loginIndex.end())
    return -1;
if (user)
    *user = iter->second;
return 0;
}
//-----------------------------------------------------------------------------
int UsersImpl::FindByNameNonLock(const std::string & login, const_user_iter * user) const
{
const std::map<std::string, user_iter>::const_iterator iter(loginIndex.find(login));
if (iter == loginIndex.end())
    return -1;
if (user)
    *user = iter->second;
return 0;
}
//-----------------------------------------------------------------------------
int UsersImpl::FindByName(const std::string & login, UserPtr * user)
{
STG_LOCKER lock(&mutex);
user_iter u;
if (FindByNameNonLock(login, &u))
    return -1;
*user = &(*u);
return 0;
}
//-----------------------------------------------------------------------------
int UsersImpl::FindByName(const std::string & login, ConstUserPtr * user) const
{
STG_LOCKER lock(&mutex);
const_user_iter u;
if (FindByNameNonLock(login, &u))
    return -1;
*user = &(*u);
return 0;
}
//-----------------------------------------------------------------------------
bool UsersImpl::Exists(const std::string & login) const
{
STG_LOCKER lock(&mutex);
const std::map<std::string, user_iter>::const_iterator iter(loginIndex.find(login));
return iter != loginIndex.end();
}
//-----------------------------------------------------------------------------
bool UsersImpl::TariffInUse(const std::string & tariffName) const
{
STG_LOCKER lock(&mutex);
std::list<UserImpl>::const_iterator iter;
iter = users.begin();
while (iter != users.end())
    {
    if (iter->GetProperties().tariffName.Get() == tariffName)
        return true;
    ++iter;
    }
return false;
}
//-----------------------------------------------------------------------------
int UsersImpl::Add(const std::string & login, const Admin * admin)
{
STG_LOCKER lock(&mutex);
const auto& priv = admin->priv();

if (!priv.userAddDel)
    {
    WriteServLog("%s tried to add user \'%s\'. Access denied.",
                 admin->logStr().c_str(), login.c_str());
    return -1;
    }

if (store->AddUser(login))
    return -1;

UserImpl u(settings, store, tariffs, &sysAdmin, this, m_services);

u.SetLogin(login);

u.SetPassiveTimeAsNewUser();

u.WriteConf();
u.WriteStat();

WriteServLog("%s User \'%s\' added.",
             admin->logStr().c_str(), login.c_str());

u.OnAdd();

users.push_front(u);

AddUserIntoIndexes(users.begin());

    {
    // Fire all "on add" notifiers
    std::set<NotifierBase<UserPtr> *>::iterator ni = onAddNotifiers.begin();
    while (ni != onAddNotifiers.end())
        {
        (*ni)->Notify(&users.front());
        ++ni;
        }
    }

    {
    // Fire all "on add" implementation notifiers
    std::set<NotifierBase<UserImplPtr> *>::iterator ni = onAddNotifiersImpl.begin();
    while (ni != onAddNotifiersImpl.end())
        {
        (*ni)->Notify(&users.front());
        ++ni;
        }
    }

return 0;
}
//-----------------------------------------------------------------------------
void UsersImpl::Del(const std::string & login, const Admin * admin)
{
const auto& priv = admin->priv();
user_iter u;

if (!priv.userAddDel)
    {
    WriteServLog("%s tried to remove user \'%s\'. Access denied.",
                 admin->logStr().c_str(), login.c_str());
    return;
    }


    {
    STG_LOCKER lock(&mutex);

    if (FindByNameNonLock(login, &u))
        {
        WriteServLog("%s tried to delete user \'%s\': not found.",
                     admin->logStr().c_str(),
                     login.c_str());
        return;
        }

    u->SetDeleted();
    }

    {
    std::set<NotifierBase<UserPtr> *>::iterator ni = onDelNotifiers.begin();
    while (ni != onDelNotifiers.end())
        {
        (*ni)->Notify(&(*u));
        ++ni;
        }
    }

    {
    std::set<NotifierBase<UserImplPtr> *>::iterator ni = onDelNotifiersImpl.begin();
    while (ni != onDelNotifiersImpl.end())
        {
        (*ni)->Notify(&(*u));
        ++ni;
        }
    }

    {
    STG_LOCKER lock(&mutex);

    u->OnDelete();

    USER_TO_DEL utd;
    utd.iter = u;
    utd.delTime = stgTime;
    usersToDelete.push_back(utd);

    DelUserFromIndexes(u);

    WriteServLog("%s User \'%s\' deleted.",
                 admin->logStr().c_str(), login.c_str());

    }
}
//-----------------------------------------------------------------------------
bool UsersImpl::Authorize(const std::string & login, uint32_t ip,
                           uint32_t enabledDirs, const Auth * auth)
{
user_iter iter;
STG_LOCKER lock(&mutex);
if (FindByNameNonLock(login, &iter))
    {
    WriteServLog("Attempt to authorize non-existant user '%s'", login.c_str());
    return false;
    }

if (FindByIPIdx(ip, iter))
    {
    if (iter->GetLogin() != login)
        {
        WriteServLog("Attempt to authorize user '%s' from ip %s which already occupied by '%s'",
                     login.c_str(), inet_ntostring(ip).c_str(),
                     iter->GetLogin().c_str());
        return false;
        }
    if (iter->Authorize(ip, enabledDirs, auth))
        return false;
    return true;
    }

if (iter->Authorize(ip, enabledDirs, auth))
    return false;

AddToIPIdx(iter);
return true;
}
//-----------------------------------------------------------------------------
bool UsersImpl::Unauthorize(const std::string & login,
                             const Auth * auth,
                             const std::string & reason)
{
user_iter iter;
STG_LOCKER lock(&mutex);
if (FindByNameNonLock(login, &iter))
    {
    WriteServLog("Attempt to unauthorize non-existant user '%s'", login.c_str());
    printfd(__FILE__, "Attempt to unauthorize non-existant user '%s'", login.c_str());
    return false;
    }

uint32_t ip = iter->GetCurrIP();

iter->Unauthorize(auth, reason);

if (!iter->GetAuthorized())
    DelFromIPIdx(ip);

return true;
}
//-----------------------------------------------------------------------------
int UsersImpl::ReadUsers()
{
std::vector<std::string> usersList;
usersList.clear();
if (store->GetUsersList(&usersList) < 0)
    {
    WriteServLog(store->GetStrError().c_str());
    return -1;
    }

user_iter ui;

unsigned errors = 0;
for (unsigned int i = 0; i < usersList.size(); i++)
    {
    UserImpl u(settings, store, tariffs, &sysAdmin, this, m_services);

    u.SetLogin(usersList[i]);
    users.push_front(u);
    ui = users.begin();

    AddUserIntoIndexes(ui);

    if (settings->GetStopOnError())
        {
        if (ui->ReadConf() < 0)
            return -1;

        if (ui->ReadStat() < 0)
            return -1;
        }
    else
        {
        if (ui->ReadConf() < 0)
            errors++;

        if (ui->ReadStat() < 0)
            errors++;
        }
    }

if (errors > 0)
    return -1;
return 0;
}
//-----------------------------------------------------------------------------
void * UsersImpl::Run(void * d)
{
sigset_t signalSet;
sigfillset(&signalSet);
pthread_sigmask(SIG_BLOCK, &signalSet, NULL);

printfd(__FILE__, "=====================| pid: %d |===================== \n", getpid());
UsersImpl * us = static_cast<UsersImpl *>(d);

struct tm t;
time_t tt = stgTime;
localtime_r(&tt, &t);

int min = t.tm_min;
int day = t.tm_mday;

printfd(__FILE__,"Day = %d Min = %d\n", day, min);

time_t touchTime = stgTime - MONITOR_TIME_DELAY_SEC;
std::string monFile = us->settings->GetMonitorDir() + "/users_r";
printfd(__FILE__, "Monitor=%d file USERS %s\n", us->settings->GetMonitoring(), monFile.c_str());

us->isRunning = true;
while (us->nonstop)
    {
    //printfd(__FILE__,"New Minute. old = %02d current = %02d\n", min, t->tm_min);
    //printfd(__FILE__,"New Day.    old = %2d current = %2d\n", day, t->tm_mday);

    for_each(us->users.begin(), us->users.end(), [](auto& user){ user.Run(); });

    tt = stgTime;
    localtime_r(&tt, &t);

    if (min != t.tm_min)
        {
        printfd(__FILE__,"Sec = %d\n", stgTime);
        printfd(__FILE__,"New Minute. old = %d current = %d\n", min, t.tm_min);
        min = t.tm_min;

        us->NewMinute(t);
        }

    if (day != t.tm_mday)
        {
        printfd(__FILE__,"Sec = %d\n", stgTime);
        printfd(__FILE__,"New Day. old = %d current = %d\n", day, t.tm_mday);
        day = t.tm_mday;
        us->NewDay(t);
        }

    if (us->settings->GetMonitoring() && (touchTime + MONITOR_TIME_DELAY_SEC <= stgTime))
        {
        //printfd(__FILE__, "Monitor=%d file TRAFFCOUNTER %s\n", tc->monitoring, monFile.c_str());
        touchTime = stgTime;
        TouchFile(monFile);
        }

    stgUsleep(100000);
    } //while (us->nonstop)

std::list<USER_TO_DEL>::iterator iter(us->usersToDelete.begin());
while (iter != us->usersToDelete.end())
    {
    iter->delTime -= 2 * userDeleteDelayTime;
    ++iter;
    }
us->RealDelUser();

us->isRunning = false;

return NULL;
}
//-----------------------------------------------------------------------------
void UsersImpl::NewMinute(const struct tm & t)
{
//Write traff, reset session traff. Fake disconnect-connect
if (t.tm_hour == 23 && t.tm_min == 59)
    {
    printfd(__FILE__,"MidnightResetSessionStat\n");
    for_each(users.begin(), users.end(), [](auto& user){ user.MidnightResetSessionStat(); });
    }

if (TimeToWriteDetailStat(t))
    {
    //printfd(__FILE__, "USER::WriteInetStat\n");
    int usersCnt = 0;

    // ����� ������ �������. � ��������� �������� USER::Run
    std::list<UserImpl>::iterator usr = users.begin();
    while (usr != users.end())
        {
        usersCnt++;
        usr->WriteDetailStat();
        ++usr;
        if (usersCnt % 10 == 0)
            for_each(users.begin(), users.end(), [](auto& user){ user.Run(); });
        }
    }

RealDelUser();
}
//-----------------------------------------------------------------------------
void UsersImpl::NewDay(const struct tm & t)
{
struct tm t1;
time_t tt = stgTime;
localtime_r(&tt, &t1);
int dayFee = settings->GetDayFee();

if (dayFee == 0)
    dayFee = DaysInCurrentMonth();

printfd(__FILE__, "DayFee = %d\n", dayFee);
printfd(__FILE__, "Today = %d DayResetTraff = %d\n", t1.tm_mday, settings->GetDayResetTraff());
printfd(__FILE__, "DayFeeIsLastDay = %d\n", settings->GetDayFeeIsLastDay());

if (!settings->GetDayFeeIsLastDay())
    {
    printfd(__FILE__, "DayResetTraff - 1 -\n");
    DayResetTraff(t1);
    //printfd(__FILE__, "DayResetTraff - 1 - 1 -\n");
    }

if (settings->GetSpreadFee())
    {
    printfd(__FILE__, "Spread DayFee\n");
    for_each(users.begin(), users.end(), [](auto& user){ user.ProcessDayFeeSpread(); });
    }
else
    {
    if (t.tm_mday == dayFee)
        {
        printfd(__FILE__, "DayFee\n");
        for_each(users.begin(), users.end(), [](auto& user){ user.ProcessDayFee(); });
        }
    }

std::for_each(users.begin(), users.end(), [](auto& user){ user.ProcessDailyFee(); });
std::for_each(users.begin(), users.end(), [](auto& user){ user.ProcessServices(); });

if (settings->GetDayFeeIsLastDay())
    {
    printfd(__FILE__, "DayResetTraff - 2 -\n");
    DayResetTraff(t1);
    }
}
//-----------------------------------------------------------------------------
void UsersImpl::DayResetTraff(const struct tm & t1)
{
int dayResetTraff = settings->GetDayResetTraff();
if (dayResetTraff == 0)
    dayResetTraff = DaysInCurrentMonth();
if (t1.tm_mday == dayResetTraff)
    {
    printfd(__FILE__, "ResetTraff\n");
    for_each(users.begin(), users.end(), [](auto& user){ user.ProcessNewMonth(); });
    //for_each(users.begin(), users.end(), mem_fun_ref(&UserImpl::SetPrepaidTraff));
    }
}
//-----------------------------------------------------------------------------
int UsersImpl::Start()
{
if (ReadUsers())
    {
    WriteServLog("USERS: Error: Cannot read users!");
    return -1;
    }

nonstop = true;
if (pthread_create(&thread, NULL, Run, this))
    {
    WriteServLog("USERS: Error: Cannot start thread!");
    return -1;
    }
return 0;
}
//-----------------------------------------------------------------------------
int UsersImpl::Stop()
{
printfd(__FILE__, "USERS::Stop()\n");

if (!isRunning)
    {
    //printfd(__FILE__, "Alredy stopped\n");
    return 0;
    }

nonstop = false;

//5 seconds to thread stops itself
struct timespec ts = {0, 200000000};
for (size_t i = 0; i < 25 * (users.size() / 50 + 1); i++)
    {
    if (!isRunning)
        break;

    nanosleep(&ts, NULL);
    }

//after 5 seconds waiting thread still running. now kill it
if (isRunning)
    {
    printfd(__FILE__, "kill USERS thread.\n");
    //TODO pthread_cancel()
    if (pthread_kill(thread, SIGINT))
        {
        //errorStr = "Cannot kill USERS thread.";
        //printfd(__FILE__, "Cannot kill USERS thread.\n");
        //return 0;
        }
    printfd(__FILE__, "USERS killed\n");
    }

printfd(__FILE__, "Before USERS::Run()\n");
for_each(users.begin(), users.end(), [](auto& user){ user.Run(); });

// 'cause bind2st accepts only constant first param
for (std::list<UserImpl>::iterator it = users.begin();
     it != users.end();
     ++it)
    it->WriteDetailStat(true);

for_each(users.begin(), users.end(), [](auto& user){ user.WriteStat(); });
//for_each(users.begin(), users.end(), mem_fun_ref(&UserImpl::WriteConf));

printfd(__FILE__, "USERS::Stop()\n");
return 0;
}
//-----------------------------------------------------------------------------
void UsersImpl::RealDelUser()
{
STG_LOCKER lock(&mutex);

printfd(__FILE__, "RealDelUser() users to del: %d\n", usersToDelete.size());

std::list<USER_TO_DEL>::iterator iter;
iter = usersToDelete.begin();
while (iter != usersToDelete.end())
    {
    printfd(__FILE__, "RealDelUser() user=%s\n", iter->iter->GetLogin().c_str());
    if (iter->delTime + userDeleteDelayTime < stgTime)
        {
        printfd(__FILE__, "RealDelUser() user=%s removed from DB\n", iter->iter->GetLogin().c_str());
        if (store->DelUser(iter->iter->GetLogin()))
            {
            WriteServLog("Error removing user \'%s\' from database.", iter->iter->GetLogin().c_str());
            }
        users.erase(iter->iter);
        usersToDelete.erase(iter++);
        }
    else
        {
        ++iter;
        }
    }
return;
}
//-----------------------------------------------------------------------------
void UsersImpl::AddToIPIdx(user_iter user)
{
printfd(__FILE__, "USERS: Add IP Idx\n");
uint32_t ip = user->GetCurrIP();
//assert(ip && "User has non-null ip");
if (!ip)
    return; // User has disconnected

STG_LOCKER lock(&mutex);

const std::map<uint32_t, user_iter>::iterator it(
        ipIndex.lower_bound(ip)
);

assert((it == ipIndex.end() || it->first != ip) && "User is not in index");

ipIndex.insert(it, std::make_pair(ip, user));
}
//-----------------------------------------------------------------------------
void UsersImpl::DelFromIPIdx(uint32_t ip)
{
printfd(__FILE__, "USERS: Del IP Idx\n");
assert(ip && "User has non-null ip");

STG_LOCKER lock(&mutex);

const std::map<uint32_t, user_iter>::iterator it(
        ipIndex.find(ip)
);

if (it == ipIndex.end())
    return;

ipIndex.erase(it);
}
//-----------------------------------------------------------------------------
bool UsersImpl::FindByIPIdx(uint32_t ip, user_iter & iter) const
{
std::map<uint32_t, user_iter>::const_iterator it(ipIndex.find(ip));
if (it == ipIndex.end())
    return false;
iter = it->second;
return true;
}
//-----------------------------------------------------------------------------
int UsersImpl::FindByIPIdx(uint32_t ip, UserPtr * usr) const
{
STG_LOCKER lock(&mutex);

user_iter iter;
if (FindByIPIdx(ip, iter))
    {
    *usr = &(*iter);
    return 0;
    }

return -1;
}
//-----------------------------------------------------------------------------
int UsersImpl::FindByIPIdx(uint32_t ip, UserImpl ** usr) const
{
STG_LOCKER lock(&mutex);

user_iter iter;
if (FindByIPIdx(ip, iter))
    {
    *usr = &(*iter);
    return 0;
    }

return -1;
}
//-----------------------------------------------------------------------------
bool UsersImpl::IsIPInIndex(uint32_t ip) const
{
STG_LOCKER lock(&mutex);

std::map<uint32_t, user_iter>::const_iterator it(ipIndex.find(ip));

return it != ipIndex.end();
}
//-----------------------------------------------------------------------------
bool UsersImpl::IsIPInUse(uint32_t ip, const std::string & login, ConstUserPtr * user) const
{
STG_LOCKER lock(&mutex);
std::list<UserImpl>::const_iterator iter;
iter = users.begin();
while (iter != users.end())
    {
    if (iter->GetLogin() != login &&
        !iter->GetProperties().ips.Get().isAnyIP() &&
        iter->GetProperties().ips.Get().find(ip))
        {
        if (user != NULL)
            *user = &(*iter);
        return true;
        }
    ++iter;
    }
return false;
}
//-----------------------------------------------------------------------------
void UsersImpl::AddNotifierUserAdd(NotifierBase<UserPtr> * n)
{
STG_LOCKER lock(&mutex);
onAddNotifiers.insert(n);
}
//-----------------------------------------------------------------------------
void UsersImpl::DelNotifierUserAdd(NotifierBase<UserPtr> * n)
{
STG_LOCKER lock(&mutex);
onAddNotifiers.erase(n);
}
//-----------------------------------------------------------------------------
void UsersImpl::AddNotifierUserDel(NotifierBase<UserPtr> * n)
{
STG_LOCKER lock(&mutex);
onDelNotifiers.insert(n);
}
//-----------------------------------------------------------------------------
void UsersImpl::DelNotifierUserDel(NotifierBase<UserPtr> * n)
{
STG_LOCKER lock(&mutex);
onDelNotifiers.erase(n);
}
//-----------------------------------------------------------------------------
void UsersImpl::AddNotifierUserAdd(NotifierBase<UserImplPtr> * n)
{
STG_LOCKER lock(&mutex);
onAddNotifiersImpl.insert(n);
}
//-----------------------------------------------------------------------------
void UsersImpl::DelNotifierUserAdd(NotifierBase<UserImplPtr> * n)
{
STG_LOCKER lock(&mutex);
onAddNotifiersImpl.erase(n);
}
//-----------------------------------------------------------------------------
void UsersImpl::AddNotifierUserDel(NotifierBase<UserImplPtr> * n)
{
STG_LOCKER lock(&mutex);
onDelNotifiersImpl.insert(n);
}
//-----------------------------------------------------------------------------
void UsersImpl::DelNotifierUserDel(NotifierBase<UserImplPtr> * n)
{
STG_LOCKER lock(&mutex);
onDelNotifiersImpl.erase(n);
}
//-----------------------------------------------------------------------------
int UsersImpl::OpenSearch()
{
STG_LOCKER lock(&mutex);
handle++;
searchDescriptors[handle] = users.begin();
return handle;
}
//-----------------------------------------------------------------------------
int UsersImpl::SearchNext(int h, UserPtr * user)
{
    UserImpl * ptr = NULL;
    if (SearchNext(h, &ptr))
        return -1;
    *user = ptr;
    return 0;
}
//-----------------------------------------------------------------------------
int UsersImpl::SearchNext(int h, UserImpl ** user)
{
STG_LOCKER lock(&mutex);

if (searchDescriptors.find(h) == searchDescriptors.end())
    {
    WriteServLog("USERS. Incorrect search handle.");
    return -1;
    }

if (searchDescriptors[h] == users.end())
    return -1;

while (searchDescriptors[h]->GetDeleted())
    {
    ++searchDescriptors[h];
    if (searchDescriptors[h] == users.end())
        {
        return -1;
        }
    }

*user = &(*searchDescriptors[h]);

++searchDescriptors[h];

return 0;
}
//-----------------------------------------------------------------------------
int UsersImpl::CloseSearch(int h)
{
STG_LOCKER lock(&mutex);
if (searchDescriptors.find(h) != searchDescriptors.end())
    {
    searchDescriptors.erase(searchDescriptors.find(h));
    return 0;
    }

WriteServLog("USERS. Incorrect search handle.");
return -1;
}
//-----------------------------------------------------------------------------
void UsersImpl::AddUserIntoIndexes(user_iter user)
{
STG_LOCKER lock(&mutex);
loginIndex.insert(make_pair(user->GetLogin(), user));
}
//-----------------------------------------------------------------------------
void UsersImpl::DelUserFromIndexes(user_iter user)
{
STG_LOCKER lock(&mutex);
loginIndex.erase(user->GetLogin());
}
//-----------------------------------------------------------------------------
bool UsersImpl::TimeToWriteDetailStat(const struct tm & t)
{
int statTime = settings->GetDetailStatWritePeriod();

switch (statTime)
    {
    case dsPeriod_1:
        if (t.tm_min == 0)
            return true;
        break;
    case dsPeriod_1_2:
        if (t.tm_min % 30 == 0)
            return true;
        break;
    case dsPeriod_1_4:
        if (t.tm_min % 15 == 0)
            return true;
        break;
    case dsPeriod_1_6:
        if (t.tm_min % 10 == 0)
            return true;
        break;
    }
return false;
}
