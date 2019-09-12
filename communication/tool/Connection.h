#ifndef PGSQL_CONNECTION_H
#define PGSQL_CONNECTION_H
#include <libpq-fe.h>
#include <string>
namespace pgsql{
	class Connection{
	private:
		PGconn * m_PGconn;
	public:
		//	Terminates an open connection.
		void abort();
		//	Release this Connection object's database
		//	and pgsql resources
		//	immediately instead of waiting for the to be automatically released; 
		void close();
		//	make all changes made since previous
		//	commit/rollback permanent and releases
		//	any database locks currently held by
		//	this Connection object.
		void commit();
		//	Undoes all changes made in the current transaction and 
		//	releases any database locks currently held by this Connection object.
		void rollback();
		//	returns the value of the client info property specified by name.
		std::string getClientInfo(std::string name);
		//	retrieves whether this connection object has been closed;
		bool isClosed();
		// put this connection in read-only mode as a hint to the driver to
		//enable database optimizations.
		void setReadOnly(bool readOnly);
		//	retrieves whether this connection object is in read-only mode;
		bool isReadOnly();
		//	returns true if the connection has not been closed and is still valid.
		bool isValid(int timeout);
	};
};
#endif;