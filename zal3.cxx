#include <iostream>
#include <pqxx/pqxx> 
using namespace std;
using namespace pqxx;

int main(){
connection Conn("dbname=bd host=labdb");
work Xaction(Conn, "DemoTransaction");
Xaction.exec("DELETE FROM emp");

result R = Xaction.exec("SELECT * FROM emp");
for (result::const_iterator i = R.begin(); i != R.end(); ++i)
{
		// Moşemy dosta�? si�? do danych tak:
		cout<<i[0].c_str();
		// albo tak
		cout<<i["id"].c_str();
		// jeşeli chcemy dosta�? co�? innego niş napis, to robimy inaczej:
		int id;
		if (!i[0].to(id))
		{
			/* je�?li by�? null, to metoda to() zwraca false */
			return ojejku!;
		}
}

Xaction.commit();
return 0;
}