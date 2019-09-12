#include <PgBase.h>
#include <iostream>
#include <ctime>
int main(){
	PgBase base;
	clock_t startTime, endTime;
	
	base.PG_Connect("host=10.210.103.229 dbname=qimwnf_test user=qimwnf_test password=qimwnf_test");
	base.PG_SetEncoding("GBK");
	for (int j = 0; j < 10; j++){

	
	startTime = clock();
	for (int i = 0; i < 1000; i++)
	{
		//select * from tt_repair_part a,tt_result_repair b where a.request_no=b.request_no and a.location_cd=b.location_cd and a.prepared_parts_number > a.final_parts_number and cast(b.complete_flg as integer) <>3 and cast(b.complete_flg as integer) <>5
		base.PG_PQquery("select");
	}
	endTime = clock();
	std::cout << "" << (double)(endTime - startTime) / CLOCKS_PER_SEC <<',';

	Oid params[] = {1042};
	const char * params_data[] = { "22" };
	startTime = clock();
	base.PG_PQprepared("1", "select * from tt_repiar_part where request_no =$1", 1, params);
	for (int i = 0; i < 1000; i++){
		base.PG_PQexecPreparedQuery("1", 1, params_data);
	}
	endTime = clock();
	std::cout << "" << (double)(endTime - startTime) / CLOCKS_PER_SEC <<   std::endl;
	}
	int x = 0;
	std::cin >> x;
	std::cout<< base.PG_PQresultErrorMessage();
	
	base.PG_Transaction(TSACT_COMMIT);
	return 0;
}