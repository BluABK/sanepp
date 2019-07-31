#include <iostream>
#include <list>
#include <vector>
#include <sqlite3.h>
#include <functional>
#include <type_traits>

#include <db_handler/db_handler.hpp>
namespace sane {
    sqlite3 *DBHandler::m_db = nullptr;

    /**
    * Prepare an sqlStatement
    *
    *                              Suggested step-method:
    *                                  while ((rc = sqlite3_step(t_sqlite3PreparedStatment)) == SQLITE_ROW) { //code }
    *
    * @param t_dbName              Filename of database.
    * @param t_sql                 An SQL statement.
    * @return                      List: [rc, sqlite3PreparedStatement]. if rc is not SQLITE_OK then you should abort.
    */
    sqlite3_stmt * DBHandler::prepareSqlStatement(const std::string &t_sql) {
        sqlite3_stmt *sqlite3PreparedStatement = nullptr;

        int rc;

        rc = sqlite3_prepare_v2(m_db, t_sql.c_str(), -1, &sqlite3PreparedStatement, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error preparing SQLite3 statement: " << sqlite3_errmsg(m_db) << std::endl;
            sqlite3_close(m_db);
            updateStatus(rc);
            return sqlite3PreparedStatement;
        }
        updateStatus(SQLITE_OK);

        //  Optional, but will most likely increase performance.
        rc = sqlite3_exec(m_db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
        updateStatus(rc);

        // Return the prepared statement to be stepped through and run custom code on.
        return sqlite3PreparedStatement;
    }

//    int DBHandler::executeSqlStatement(const std::string &t_sql) { // FIXME: Bring back for use on unbound stmts?
//        sqlite3_exec(m_db, t_sql, );
//        return 0;
//    }

    int DBHandler::finalizePreparedSqlStatement (sqlite3_stmt *t_sqlite3PreparedStatement) {
        // FIXME: Check triggers false positive "not an error" rc status.
//        if (t_rcStatusCode != SQLITE_DONE) {
//            std::cerr << "Error sqlite3_step ended but does not have status SQLITE_DONE: " << sqlite3_errmsg(m_db) <<
//                      std::endl;
//            updateStatus(SQLITE_NOT_DONE);
//            return t_rcStatusCode;
//        }
        char * zErrMsg;
        int rc = sqlite3_exec(m_db, "END TRANSACTION", nullptr, nullptr, &zErrMsg );   //  End the transaction.

        if (rc != SQLITE_OK) {
            std::cerr << zErrMsg << std::endl;
            return rc;
        }

        updateStatus(rc);

        // Finalize the prepared statement object.
        sqlite3_finalize(t_sqlite3PreparedStatement);

        updateStatus(SQLITE_OK);
        return SQLITE_OK;
    }
    
    /**
     * Runs/Executes an SQLite statement without binding any values.
     *
     * Prefixed 'run' instead of 'execute' in order to disambiguate from sqlite3_exec.
     *
     * @param t_sql An SQLite statement on string form.
     * @return      Execution status.
     */
    int DBHandler::runSqlStatement(const std::string &t_sql) {
        // Setup
        sqlite3_stmt *preparedStatement = nullptr;
        int rc = SQLITE_NEVER_RUN;
        
        // 1/3: Create a prepared statement
        preparedStatement = prepareSqlStatement(t_sql);
        if (lastStatus() != SQLITE_OK) {
            std::cerr << "create_table(" << getDBFilename() << ", " << t_sql << 
            ") ERROR: returned non-zero status: " << lastStatus()  << std::endl;
            return lastStatus();
        }

        // 2/3: Step through, and do nothing because this is a CREATE statement with no VALUE bindings.
        while ((rc = sqlite3_step(preparedStatement)) == SQLITE_ROW) {}

        // 3/3: Finalize prepared statement
        finalizePreparedSqlStatement(preparedStatement);

        return SQLITE_OK;
    }
} // namespace sane