#ifndef PG_BASE_H
#define PG_BASE_H
#include <libpq-fe.h>
#include <string>
using namespace std;
enum transaction_args_id{
	TSACT_BEIGIN,
	TSACT_COMMIT,
	TSACT_ROLLBACK,
};
class ResultSet{
	PGresult * m_PGresult;
};
class PgBase{
private:
	PGconn * m_PGconn;
	PGresult * m_PGresult;
	int PG_PQqueryStatus();
	int PG_PQexecuteStatus();
	//	Execute a prepared statement
	//	@stmtName: use PQprepared to define a statementName;
	//	@nParams: the count of args in statement
	//	@paramValues: the values of args
	//	@paramLengths: Specifies the actual data lengths of binary-format parameters. 
	//	It is ignored for null parameters and text-format parameters. 
	//	The array pointer can be null when there are no binary parameters.
	//	@paramFormats: Specifies whether parameters are text (put a zero in the array entry for the corresponding parameter) 
	//	or binary (put a one in the array entry for the corresponding parameter). 
	//	If the array pointer is null then all parameters are presumed to be text strings.
	//	@resultFormats:Specify zero to obtain results in text format, 
	//	or one to obtain results in binary format. 
	//	(There is not currently a provision to obtain different result columns in different formats,
	//	although that is possible in the underlying protocol.
	int PG_PQexecPrepared(const char * stmtName, int nParams, const char * const * paramValues,
		const int * paramLengths, const int * paramFormats, int resultFormat);
	//	Returns a single field value of one row of a PGresult. 
	char * PG_PQgetvalue(int row_number,int col_number);
	//	Returns the column number associated with the given column name.
	//	-1 is returned if the given name does not match any column
	int PG_PQfnumber(const char *column_name);
public:
	//初始化函数
	PgBase();
	~PgBase();
	//	Makes a new connection to the database server.
	//	建立与数据库服务器的新连接
	int PG_Connect(char *conninfo);
	//	Sets the client encoding.
	//	设置数据库字符集
	int PG_SetEncoding(char * encoding);
	//	Submits a command to the server and waits for the result.
	//	向服务器提交命令并等待结果。
	int PG_PQexecute(char * command);
	//	Submits a Query to the server and waits for the result.
	//	向服务器提交查询并等待结果。
	int PG_PQquery(char * query);
	//	Transaction
	//	@param action TSACT_BEIGIN,TSACT_COMMIT,TSACT_ROLLBACK
	//	提交事务
	int PG_Transaction(int action);
	//	Returns the error message associated with the command,
	//	or an empty string if there was no error.
	char * PG_PQresultErrorMessage();
	//	Create a prepared statement
	//	@stmtName = anyName
	//	@query = "SELECT * FROM A WHERE A.args=$1"
	//	@nParams = the count of '$1,$2....'
	//	@paramTypes paramTypes[] specifies, by OID, the data types to be assigned to the parameter symbols.
	//	if you don't know Oid You can use 'select oid,* from pg_type;' to know it.
	//	different versions of oid may be not the same.
	int PG_PQprepared(char *stmtName, char *query, int nParams, const Oid * paramTypes);
	//	Execute a prepared Query statement
	//	@stmtName: use PQprepared to define a statementName;
	//	@nParams: the count of args in statement
	//	@paramValues: the values of args
	int PG_PQexecPreparedQuery(const char * stmtName, int nParams, const char * const * paramValues);
	//	Execute a prepared Command statement
	//	@stmtName: use PQprepared to define a statementName;
	//	@nParams: the count of args in statement
	//	@paramValues: the values of args
	int PG_PQexecPreparedExecute(const char * stmtName, int nParams, const char * const * paramValues);
	//Returns the number of rows (tuples) in the query result. 
	int PG_PQntuples();
	//Returns the number of columns(fields) in each row of the query result.
	int PG_PQnfields();

	char * Columns(char * fieldName);
protected:
	
	
};
#endif