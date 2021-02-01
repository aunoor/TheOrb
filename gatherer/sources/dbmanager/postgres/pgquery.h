#ifndef PGQUERY_H
#define PGQUERY_H

#include <libpq-fe.h>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class PgConnection;
class PgSelectQuery;

class AbstractPgQuery {
public:
    explicit AbstractPgQuery(PgConnection *connection);
    virtual ~AbstractPgQuery();
    void Prepare(const std::string& query);
    bool BindValue(const std::string& placeholder, const std::string& value);
    bool BindValue(const std::string& placeholder, int32_t value);
    bool BindValue(const std::string& placeholder, int64_t value);
    bool BindValue(const std::string& placeholder, uint32_t value);
    bool BindValue(const std::string& placeholder, uint64_t value);
    bool BindValue(const std::string& placeholder, float value);
    bool BindValue(const std::string& placeholder, bool value);
    virtual bool Exec() = 0;

    std::string ErrorString();

protected:
    PgConnection *m_connection;
    std::string m_error;
    std::string m_queryString;
    PGresult *m_pgResult;
    bool bindValue(const std::string& placeholder, const std::string& value);
};


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
        friend PgSelectQuery;
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
    friend PgSelectQuery;
};

class PgSelectQuery : public AbstractPgQuery {
public:
    explicit PgSelectQuery(PgConnection *connection);
    ~PgSelectQuery() override;
    bool Exec() override;
    int64_t RowCount();
    bool Next();
    PgRecord Record();
private:
    ///Current row
    int64_t m_currentRow;
    ///Row counts returned by select
    int64_t m_rowCnt;
};


#endif //PGQUERY_H
