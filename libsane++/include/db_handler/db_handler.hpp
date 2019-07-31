#ifndef SANE_DB_HANDLER_HPP
#define SANE_DB_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <type_traits>

#include <sqlite3.h>
#include <mutex>

#define DATABASE_NAME "sane.db"

// Custom SQLite3 error messages, counting downwards to not clash with official.
#define SQLITE_NEVER_RUN -1
#define SQLITE_NOT_DONE -2

static std::mutex m_mutex;
static int m_usageCounter = 0;

namespace sane {
    class DBHandler {
    public:
        explicit DBHandler(const std::string &t_dbFilename = DATABASE_NAME) {
            m_dbFilename = t_dbFilename;
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_usageCounter == 0) {
                openDB();
                m_usageCounter++;
            }
        }

        sqlite3_stmt * prepareSqlStatement(const std::string &t_sql);

//        int executeSqlStatement(const std::string &t_sql);

        int finalizePreparedSqlStatement (sqlite3_stmt *t_sqlite3PreparedStatement);

        std::string getDBFilename() {
            return m_dbFilename;
        }

        int openDB() {
            // Open the database file.
            int rc = sqlite3_open(getDBFilename().c_str(), &m_db);

            if (rc != SQLITE_OK) {
                std::cerr << "Error opening database '" << getDBFilename() << "': " << sqlite3_errmsg(m_db) << std::endl;
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

        /**
         * Runs/Executes an SQLite statement without binding any values.
         *
         * Prefixed 'run' instead of 'execute' in order to disambiguate from sqlite3_exec.
         *
         * @param t_sql An SQLite statement on string form.
         * @return      Execution status.
         */
        int runSqlStatement(const std::string &t_sql);

        ~DBHandler() {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_usageCounter == 0) {
                // Close db handle
                int rc = sqlite3_close(m_db);
                std::cout << "DB closed with status: " << rc << std::endl;
            } else {
                m_usageCounter--;
            }
        }
    private:
        std::string m_dbFilename;

static sqlite3 *m_db;


int m_lastStatus = SQLITE_NEVER_RUN;

};
} // namespace sane

#endif //SANE_DB_HANDLER_HPP
