#include "PgBase.h"
//PgBase初始化
PgBase::PgBase(){
	m_PGconn = nullptr;
	m_PGresult = nullptr;
}
char * PgBase::Columns(char * fieldName){
	int column_number = PG_PQfnumber(fieldName);
	
	if (column_number < 0){
		return "";
	}
	if (row_number < 0){
		return "";
	}
	return PG_PQgetvalue(row_number, column_number);
}
int PgBase::PG_PQfnumber(const char *column_name){

	int column_number = PQfnumber(m_PGresult, column_name);

	return column_number;
}
char * PgBase::PG_PQgetvalue(int row_number,int column_number){
	if (m_PGresult == nullptr){
		return NULL;
	}
	return PQgetvalue(m_PGresult, row_number, column_number);
}
int PgBase::PG_PQntuples(){
	if (m_PGresult == nullptr){
		return NULL;
	}
	PQntuples(m_PGresult);
}
int PgBase::PG_PQnfields(){
	if (m_PGresult == nullptr){
		return NULL;
	}
	PQnfields(m_PGresult);
}
char * PgBase::PG_PQresultErrorMessage(){
	if (m_PGresult == nullptr){
		return NULL;
	}
	return PQresultErrorMessage(m_PGresult);	
}
int PgBase::PG_PQexecuteStatus(){
	int msg = -2;

	if (PGRES_COMMAND_OK == PQresultStatus(m_PGresult))
	{
		msg = 0;
	}
	return msg;
}
int PgBase::PG_PQqueryStatus(){
	int msg = -2;

	if (PGRES_TUPLES_OK == PQresultStatus(m_PGresult))
	{
		msg = 0;
	}
	return msg;
}
//	Transaction
//	提交事务
int PgBase::PG_Transaction(int action){
	if (m_PGconn == nullptr){
		return -1;
	}
	switch (action)
	{
		case TSACT_BEIGIN:
		{
			char _sql[30];

			sprintf(_sql, "begin");

			m_PGresult = PQexec(m_PGconn, _sql);

		}
			break;
		case TSACT_COMMIT:
		{
			char _sql[30];

			sprintf(_sql, "commit");

			m_PGresult = PQexec(m_PGconn, _sql);

		}
			break;
		case TSACT_ROLLBACK:
		{
			char _sql[30];

			sprintf(_sql, "rollback");

			m_PGresult = PQexec(m_PGconn, _sql);

		}
			break;
		default:
			break;
	}

	return PG_PQexecuteStatus();
	
}
//	Makes a new connection to the database server.
//	建立与数据库服务器的新连接
int PgBase::PG_Connect(char *conninfo){
	
	if (conninfo == NULL || strlen(conninfo) < 1){
		
		return -2;
	
	}
	m_PGconn = PQconnectdb(conninfo);

	if (PQstatus(m_PGconn) == CONNECTION_BAD)
	{
		return -1;
	}

	return 0;
}
//	Sets the client encoding.
//	设置数据库字符集
int PgBase::PG_SetEncoding(char * encoding){
	if (m_PGconn == nullptr){
		return -2;
	}

	int ret=PQsetClientEncoding(m_PGconn, encoding);

	return ret;
	
}
int PgBase::PG_PQprepared(char *stmtName, char *query, int nParams, const Oid * paramTypes){

	if (m_PGconn == nullptr || stmtName == nullptr){
		return -1;
	}
	m_PGresult = PQprepare(m_PGconn, stmtName, query, nParams, paramTypes);

}
//	Submits a command to the server and waits for the result.
//	向服务器提交命令并等待结果。
int PgBase::PG_PQexecute(char * command){
	if (m_PGconn == nullptr || command == nullptr || strlen(command)<1){
		return -1;
	}
	m_PGresult = PQexec(m_PGconn, command);
	//Execute
	return PG_PQexecuteStatus();
}
int PgBase::PG_PQexecPreparedExecute(const char * stmtName, int nParams, const char * const * paramValues){
	if (m_PGconn == nullptr || stmtName == nullptr){
		return -1;
	}
	m_PGresult = PQexecPrepared(m_PGconn, stmtName, nParams, paramValues, nullptr, nullptr, 0);
	//Execute
	return PG_PQexecuteStatus();
}
int PgBase::PG_PQexecPreparedQuery(const char * stmtName, int nParams, const char * const * paramValues){
	if (m_PGconn == nullptr || stmtName == nullptr){
		return -1;
	}
	m_PGresult = PQexecPrepared(m_PGconn, stmtName, nParams, paramValues, nullptr, nullptr, 0);
	//Query
	return PG_PQqueryStatus();
}
//	Submits a Query to the server and waits for the result.
//	向服务器提交查询并等待结果。
int PgBase::PG_PQquery(char * query){
	if (m_PGconn == nullptr || query == nullptr || strlen(query)<1){
		return -1;
	}
	m_PGresult = PQexec(m_PGconn, query);
	//Query
	return PG_PQqueryStatus();
}
PgBase::~PgBase(){
	if (m_PGresult != NULL){
		
		PQclear(m_PGresult);
		m_PGresult = NULL;

	}
	if (m_PGconn != NULL){

		PQfinish(m_PGconn);
		m_PGconn = NULL;

	}
}