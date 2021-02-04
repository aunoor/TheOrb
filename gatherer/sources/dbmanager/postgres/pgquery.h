#ifndef PGQUERY_H
#define PGQUERY_H

#include <libpq-fe.h>
#include <ctime>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class PgConnection;
class PgQuery;

class PgRecord {
public:

    class PgField {
    public:
        std::string Value() {return m_value;};
        std::string Name() {return m_name;}
        bool IsNull() {return m_isNull;};
        bool IsValid() {return m_isValid;};
    private:
        std::string m_value;
        std::string m_name;
        bool m_isNull = true;
        bool m_isValid = false;
        PgField() = default;
        friend PgQuery;
        friend PgRecord;
    };

    PgField Field(uint32_t fieldNum);
    std::string Value(const std::string& fieldName);
    std::string Value(uint32_t fieldNum);
    bool isNull(const std::string& fieldName);
    bool isNull(uint32_t fieldNum);
    uint32_t Count();

    bool IsValid();
private:
    PgRecord() = default;
    std::vector<PgField> m_values;
    bool m_isValid = false;
    friend PgQuery;
};

class PgQuery {
public:
    explicit PgQuery(PgConnection *connection);
    ~PgQuery();

    bool Exec();
    bool Exec(const std::string &query);

    int RowCount();
    bool Next();
    PgRecord Record();

    void Prepare(const std::string& query);
    bool BindValue(const std::string& placeholder, const std::string& value);
    bool BindValue(const std::string& placeholder, int32_t value);
    bool BindValue(const std::string& placeholder, int64_t value);
    bool BindValue(const std::string& placeholder, uint32_t value);
    bool BindValue(const std::string& placeholder, uint64_t value);
    bool BindValue(const std::string& placeholder, float value);
    bool BindValue(const std::string& placeholder, bool value);
    bool BindValue(const std::string& placeholder, std::tm timeStamp);

    std::string ErrorString();

    void Clear();
private:
    ///Current row
    int64_t m_currentRow;
    ///Row counts returned by select
    int m_rowCnt;

    PgConnection *m_connection;
    std::string m_error;
    std::string m_queryString;
    PGresult *m_pgResult;
    bool bindValue(const std::string& placeholder, const std::string& value);
};


#endif //PGQUERY_H
