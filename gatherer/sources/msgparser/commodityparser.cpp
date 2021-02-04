#include <iomanip>
#include <sstream>

#include "commodityparser.h"
#include "common/schemadefs.h"


bool CommodityParser::Parse(rapidjson::Value &message, MarketData &marketData) {

//    if (!message.HasMember(headerName)) {
//        //TODO: log
//        return false;
//    }

    marketData.MarketId = message["marketId"].GetUint64();
    marketData.StationName = message["stationName"].GetString();
    marketData.SystemName = message["systemName"].GetString();

    std::string ts = message["timestamp"].GetString();
    std::istringstream ss(ts);
    //"2021-01-22T12:43:45Z"
    ss >> std::get_time(&marketData.Timestamp, "%Y-%m-%dT%H:%M:%S");


//    auto message = message[messageName].GetObject();
//    if (!message.HasMember(commoditiesName)) {
//        //TODO: log
//        return false;
//    }

    if (!message[commoditiesName].IsArray()) {
        //TODO: log
        return false;
    }

    auto commodities = message[commoditiesName].GetArray();
    for (auto &item : commodities) {
        Commodity commodity;
        commodity.BuyPrice = item["buyPrice"].GetUint();
        commodity.Demand = item["demand"].GetUint();
        if (item["demandBracket"].IsUint()) {
            commodity.DemandBracket = item["demandBracket"].GetUint();
        }
        commodity.MeanPrice = item["meanPrice"].GetUint();
        commodity.Name = item["name"].GetString();
        commodity.SellPrice = item["sellPrice"].GetUint();
        commodity.Stock = item["stock"].GetUint();
        if (item["stockBracket"].IsUint()) {
            commodity.StockBracket = item["stockBracket"].GetUint();
        }
        marketData.Commodities.push_back(commodity);
    }

    return true;
}
