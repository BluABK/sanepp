#ifndef SANE_DB_HANDLER_HPP
#define SANE_DB_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <type_traits>

#include <sqlite3.h>
#include <mutex>

// Custom SQLite3 error messages, counting downwards to not clash with official.
#define SQLITE_NEVER_RUN -1
#define SQLITE_NOT_DONE -2

static std::mutex m_mutex;
static int m_usageCounter;

namespace sane {
    class DBHandler {
    public:
        DBHandler() {
            m_usageCounter++;
            std::lock_guard<std::mutex> guard(m_mutex);
            openDB();
        }

        sqlite3_stmt * prepareSqlStatement(const std::string &t_sql);

        int finalizePreparedSqlStatement (int t_rcStatusCode, sqlite3_stmt *t_sqlite3PreparedStatement);

        std::string getDBFilename() {
            return m_dbFilename;
        }

        sqlite3 * getDB();

        int openDB() {
            // Open the database file.
            int rc = sqlite3_open(getDBFilename().c_str(), &m_db);

            if (rc != SQLITE_OK) {
                std::cerr << "Error opening database '" << getDBFilename() << "': " << sqlite3_errmsg(getDB()) << std::endl;
                sqlite3_close(m_db);
                updateStatus(SQLITE_CANTOPEN);
                return SQLITE_CANTOPEN;
            }
            return rc;
        }

        void updateStatus(int newStatus) {
            m_lastStatus = newStatus;
        }

        int lastStatus() {
            return m_lastStatus;
        }

        ~DBHandler() {
            // Close db handle
            sqlite3_close(m_db);

            m_usageCounter--;
        }
    private:
        std::string m_dbFilename = "sane.db";

        sqlite3 *m_db = nullptr;


        int m_lastStatus = SQLITE_NEVER_RUN;
    };
} // namespace sane

#endif //SANE_DB_HANDLER_HPP
