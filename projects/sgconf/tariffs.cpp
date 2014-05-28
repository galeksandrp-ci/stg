#include "tariffs.h"

#include "api_action.h"
#include "options.h"
#include "config.h"

#include "stg/servconf.h"
#include "stg/servconf_types.h"
#include "stg/tariff_conf.h"
#include "stg/os_int.h"

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cassert>

namespace
{

std::string Indent(size_t level, bool dash = false)
{
if (level == 0)
    return "";
return dash ? std::string(level * 4 - 2, ' ') + "- " : std::string(level * 4, ' ');
}

std::string PeriodToString(TARIFF::PERIOD period)
{
switch (period)
    {
    case TARIFF::DAY:
        return "daily";
    case TARIFF::MONTH:
        return "monthly";
    }
return "unknown";
}

std::string TraffTypeToString(int traffType)
{
switch (traffType)
    {
    case TRAFF_UP:
        return "upload";
    case TRAFF_DOWN:
        return "download";
    case TRAFF_UP_DOWN:
        return "upload + download";
    case TRAFF_MAX:
        return "max(upload, download)";
    }
return "unknown";
}

std::string TimeToString(int h, int m)
{
std::ostringstream stream;
stream << (h < 10 ? "0" : "") << h << ":"
       << (m < 10 ? "0" : "") << m;
return stream.str();
}

void PrintDirPriceData(size_t dir, const DIRPRICE_DATA & data, size_t level)
{
std::string night = TimeToString(data.hNight, data.mNight);
std::string day = TimeToString(data.hDay, data.mDay);
std::cout << Indent(level, true) << "dir: " << dir << "\n"
          << Indent(level)       << "'" << night << "' - '" << day << "': " << data.priceDayA << "/" << data.priceDayB << "\n"
          << Indent(level)       << "'" << day << "' - '" << night << "': " << data.priceNightA << "/" << data.priceNightB << "\n"
          << Indent(level)       << "threshold: " << data.threshold << "\n"
          << Indent(level)       << "single price: " << (data.singlePrice ? "yes" : "no") << "\n"
          << Indent(level)       << "discount: " << (data.noDiscount ? "no" : "yes") << "\n"; // Attention!
}

void PrintTariffConf(const TARIFF_CONF & conf, size_t level)
{
std::cout << Indent(level, true) << "name: " << conf.name << "\n"
          << Indent(level)       << "fee: " << conf.fee << "\n"
          << Indent(level)       << "free mb: " << conf.free << "\n"
          << Indent(level)       << "passive cost: " << conf.passiveCost << "\n"
          << Indent(level)       << "traff type: " << TraffTypeToString(conf.traffType) << "\n"
          << Indent(level)       << "period: " << PeriodToString(conf.period) << "\n";
}

void PrintTariff(const STG::GET_TARIFF::INFO & info, size_t level = 0)
{
PrintTariffConf(info.tariffConf, level);
std::cout << Indent(level) << "dir prices:\n";
for (size_t i = 0; i < info.dirPrice.size(); ++i)
    PrintDirPriceData(i, info.dirPrice[i], level + 1);
}

std::vector<SGCONF::API_ACTION::PARAM> GetTariffParams()
{
std::vector<SGCONF::API_ACTION::PARAM> params;
params.push_back({"fee", "<fee>", "\t\ttariff fee"});
params.push_back({"free", "<free mb>", "\tprepaid traff"});
params.push_back({"passive-cost", "<cost>", "\tpassive cost"});
params.push_back({"traff-type", "<type>", "\ttraff type (up, dow, up+down, max)"});
params.push_back({"period", "<period>", "\ttarification period (daily, monthly)"});
params.push_back({"times", "<hh:mm-hh:mm, ...>", "coma-separated day time-spans for each direction"});
params.push_back({"day-prices", "<price/price, ...>", "coma-separated day prices for each direction"});
params.push_back({"night-prices", "<price/price, ...>", "coma-separated day prices for each direction"});
params.push_back({"thresholds", "<threshold, ...>", "coma-separated thresholds for each direction"});
return params;
}

void SimpleCallback(bool result,
                    const std::string & reason,
                    void * /*data*/)
{
if (!result)
    {
    std::cerr << "Operation failed. Reason: '" << reason << "'." << std::endl;
    return;
    }
std::cout << "Success.\n";
}

void GetTariffsCallback(bool result,
                        const std::string & reason,
                        const std::vector<STG::GET_TARIFF::INFO> & info,
                        void * /*data*/)
{
if (!result)
    {
    std::cerr << "Failed to get tariff list. Reason: '" << reason << "'." << std::endl;
    return;
    }
std::cout << "Tariffs:\n";
for (size_t i = 0; i < info.size(); ++i)
    PrintTariff(info[i], 1);
}

void GetTariffCallback(bool result,
                       const std::string & reason,
                       const std::vector<STG::GET_TARIFF::INFO> & info,
                       void * data)
{
assert(data != NULL && "Expecting pointer to std::string with the tariff's name.");
const std::string & name = *static_cast<const std::string *>(data);
if (!result)
    {
    std::cerr << "Failed to get tariff. Reason: '" << reason << "'." << std::endl;
    return;
    }
for (size_t i = 0; i < info.size(); ++i)
    if (info[i].tariffConf.name == name)
        PrintTariff(info[i]);
}

bool GetTariffsFunction(const SGCONF::CONFIG & config,
                        const std::string & /*arg*/,
                        const std::map<std::string, std::string> & /*options*/)
{
STG::SERVCONF proto(config.server.data(),
                    config.port.data(),
                    config.userName.data(),
                    config.userPass.data());
return proto.GetTariffs(GetTariffsCallback, NULL) == STG::st_ok;
}

bool GetTariffFunction(const SGCONF::CONFIG & config,
                       const std::string & arg,
                       const std::map<std::string, std::string> & /*options*/)
{
STG::SERVCONF proto(config.server.data(),
                    config.port.data(),
                    config.userName.data(),
                    config.userPass.data());
// STG currently doesn't support <GetTariff name="..."/>.
// So get a list of tariffs and filter it. 'data' param holds a pointer to 'name'.
std::string name(arg);
return proto.GetTariffs(GetTariffCallback, &name) == STG::st_ok;
}

bool DelTariffFunction(const SGCONF::CONFIG & config,
                       const std::string & arg,
                       const std::map<std::string, std::string> & /*options*/)
{
STG::SERVCONF proto(config.server.data(),
                    config.port.data(),
                    config.userName.data(),
                    config.userPass.data());
return proto.DelTariff(arg, SimpleCallback, NULL) == STG::st_ok;
}

bool AddTariffFunction(const SGCONF::CONFIG & config,
                       const std::string & arg,
                       const std::map<std::string, std::string> & /*options*/)
{
// TODO
std::cerr << "Unimplemented.\n";
return false;
}

bool ChgTariffFunction(const SGCONF::CONFIG & config,
                       const std::string & arg,
                       const std::map<std::string, std::string> & options)
{
// TODO
std::cerr << "Unimplemented.\n";
return false;
}

} // namespace anonymous

void SGCONF::AppendTariffsOptionBlock(COMMANDS & commands, OPTION_BLOCKS & blocks)
{
std::vector<API_ACTION::PARAM> params(GetTariffParams());
blocks.Add("Tariff management options")
      .Add("get-tariffs", SGCONF::MakeAPIAction(commands, GetTariffsFunction), "\tget tariff list")
      .Add("get-tariff", SGCONF::MakeAPIAction(commands, "<name>", GetTariffFunction), "get tariff")
      .Add("add-tariff", SGCONF::MakeAPIAction(commands, "<name>", params, AddTariffFunction), "add tariff")
      .Add("del-tariff", SGCONF::MakeAPIAction(commands, "<name>", DelTariffFunction), "del tariff")
      .Add("chg-tariff", SGCONF::MakeAPIAction(commands, "<name>", params, ChgTariffFunction), "change tariff");
}