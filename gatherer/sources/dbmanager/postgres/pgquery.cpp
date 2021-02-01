#include "pgquery.h"
#include "pgconnection.h"

//--------------------------------------------------------------------------------------------------------------------//

AbstractPgQuery::AbstractPgQuery(PgConnection *connection) {
    m_connection = connection;
    m_pgResult = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------//

AbstractPgQuery::~AbstractPgQuery() {
    if (m_pgResult) {
        PQclear(m_pgResult);
    }
}

//--------------------------------------------------------------------------------------------------------------------//

std::string AbstractPgQuery::ErrorString() {
    return m_error;
}

//--------------------------------------------------------------------------------------------------------------------//

PgSelectQuery::PgSelectQuery(PgConnection *connection) : AbstractPgQuery (connection) {
    m_currentRow = -1;
    m_rowCnt = 0;
}

//--------------------------------------------------------------------------------------------------------------------//

PgSelectQuery::~PgSelectQuery() = default;

//--------------------------------------------------------------------------------------------------------------------//

void AbstractPgQuery::Prepare(const std::string& query) {
    m_queryString = query;
}

//--------------------------------------------------------------------------------------------------------------------//

bool AbstractPgQuery::BindValue(const std::string& placeholder, const std::string& value) {
    auto pos = m_queryString.find(placeholder);
    if (pos == -1) return false;
    m_queryString.replace(pos, placeholder.length(), value);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

bool AbstractPgQuery::BindValue(const std::string& placeholder, int value) {
    auto pos = m_queryString.find(placeholder);
    if (pos == -1) return false;
    std::string tmpValue = std::to_string(value);
    m_queryString.replace(pos, placeholder.length(), tmpValue);
    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgSelectQuery::Exec() {
    bool res = false;
    m_currentRow = -1;
    m_pgResult = PQexec(m_connection->GetPGconn(), m_queryString.c_str());
    do {
        if (!m_pgResult) break;
        auto result = PQresultStatus(m_pgResult);
        auto resultStatus = PQresultStatus(m_pgResult);
        if ( (resultStatus != PGRES_TUPLES_OK) && (resultStatus != PGRES_COMMAND_OK)) {
            m_error = PQresultErrorMessage(m_pgResult);
            break;
        }
        m_currentRow = 0;
        m_rowCnt = PQntuples(m_pgResult);
        res = true;
    } while(false);
    return res;
}

//--------------------------------------------------------------------------------------------------------------------//

int64_t PgSelectQuery::RowCount() {
    return m_rowCnt;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgSelectQuery::Next() {
    if (m_rowCnt == 0) return false;
    if (m_currentRow == m_rowCnt) return false;
    m_currentRow++;
    return true;
}

//--------------------------------------------------------------------------------------------------------------------//

PgRecord PgSelectQuery::Record() {
    if (m_currentRow < 0 || m_currentRow > m_rowCnt) return PgRecord();
    PgRecord record;
    if (PQnfields(m_pgResult) > 0) {
        record.m_values.reserve(PQnfields(m_pgResult));
        for (int i = 0; i < PQnfields(m_pgResult); i++) {
            PgRecord::PgField field;
            field.m_name = PQfname(m_pgResult, i);
            field.m_isNull = PQgetisnull(m_pgResult, m_currentRow, i);
            if (!field.m_isNull) {
                std::string fldValue = PQgetvalue(m_pgResult, m_currentRow, i);
            }
            field.m_isValid = true;
            record.m_values.push_back(field);
        }
    }
    return record;
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgRecord::IsValid() {
    return m_isValid;
}

//--------------------------------------------------------------------------------------------------------------------//

PgRecord::PgField PgRecord::Field(uint32_t fieldNum) {
    if (fieldNum > Count()-1) return PgRecord::PgField();
    return m_values.at(fieldNum);
}

//--------------------------------------------------------------------------------------------------------------------//

uint32_t PgRecord::Count() {
    return m_values.size();
}

//--------------------------------------------------------------------------------------------------------------------//

bool PgRecord::isNull(const std::string &fieldName) {
    for (auto &item : m_values) {
        if (item.Name() == fieldName) {
            return item.IsNull();
        }
    }
    return true;
}

bool PgRecord::isNull(uint32_t fieldNum) {
    if (fieldNum > Count()-1) return true;
    return m_values.at(fieldNum).IsNull();
}

//--------------------------------------------------------------------------------------------------------------------//

std::string PgRecord::Value(uint32_t fieldNum) {
    if (fieldNum > Count()-1) return std::string();
    return m_values.at(fieldNum).Value();
}

std::string PgRecord::Value(const std::string& fieldName) {
    for (auto &item : m_values) {
        if (item.Name() == fieldName) {
            return item.Value();
        }
    }
    return std::string();
}

//--------------------------------------------------------------------------------------------------------------------//

