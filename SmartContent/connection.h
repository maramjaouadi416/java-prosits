#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QtSql/QSqlDatabase>
class Connection
{
public:

    static Connection* instance();      // Access to the unique instance
    bool createConnect();               // Method to create the connection


private:
    Connection();                       // Private constructor to prevent external instantiation
    ~Connection();                      // Private destructor to control destruction
    Connection(const Connection&) = delete; // Delete the copy constructor
    Connection& operator=(const Connection&) = delete; // Delete the assignment operator

        static Connection* p_instance;      // Pointer to the unique instance

};


#endif // CONNECTION_H
