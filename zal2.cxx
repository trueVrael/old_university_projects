#include <iostream>
#include <set>
#include <pqxx/pqxx> 
#include <cstdio>

using namespace std;
using namespace pqxx;

string kol[5][5];
string tab[5];
int col[5];
connection C("dbname = bd host = labdb user = tk334580 password = Poziomskilla0");
set<int> inty[3];//reszta
set<string> key[2];//klient,pracownik
void Tabele(){
	cout<<"1. Klient"<<endl;
	cout<<"2. Impreza"<<endl;
	cout<<"3. Pomieszczenie"<<endl;
	cout<<"4. Historia imprez"<<endl;
	cout<<"5. Pracownik"<<endl;
}
int Wybierz(int o){
	int x = -1;
	int it = 0;
	while((x>o) || (x<1)){
		cin>>x;
		it++;
		if(it > 1) cout<<"Podaj wartosc z poprawnego zakresu"<<endl;	
	}
	return x;
}
void Wyswietl(int w){
	nontransaction N(C);
	result R(N.exec("SELECT * from" + tab[w-1]));
	for(result::const_iterator c = R.begin(); c != R.end(); ++c){
		for(int j =0; j< col[w - 1]; ++j)
			cout<<kol[w-1][j]<<": "<<c[j]<<' ';
	cout<<endl;
	}
}
void Usun(){
	int d;
	for(int x=2; x<5; x++){
		nontransaction N(C);
		result R(N.exec("SELECT * from" + tab[x-1]));
		for(result::const_iterator c = R.begin(); c != R.end(); ++c){
			sscanf(c[0].c_str(),"%d", &d);
			inty[x-2].erase(d);
		}
	}
}
void Dodaj(){
	string nap;
	for(int i=0; i<2; i++){
		nontransaction N(C);
		result R(N.exec("SELECT * from" + tab[0+4*i]));
		for(result::const_iterator c = R.begin(); c != R.end(); ++c){
			nap = c[0].c_str();
			key[i].insert(nap);
		}
	}
}
int main()
{
   string sql;
   kol[0][0] = "Pesel";
   kol[0][1] = "Imie";
   kol[0][2] = "Nazwisko";
   kol[0][3] = "Nr_telefonu";
   kol[0][4] = "Nr_dowodu";
   kol[1][0] = "Id";
   kol[1][1] = "Data_imprezy";
   kol[1][2] = "Ile_osob";
   kol[1][3] = "Nazwisko_pracownika";
   kol[2][0] = "Id";
   kol[2][1] = "Powierzchnia";
   kol[2][2] = "Ile_osob";
   kol[3][0] = "Id";
   kol[3][1] = "Data_imprezy";
   kol[3][2] = "Ile_osob";
   kol[4][0] = "Nr_dowodu";
   kol[4][1] = "Imie";
   kol[4][2] = "Nazwisko";
   kol[4][3] = "Data_zatrudnienia";
   kol[4][4] = "Pensja";
   for(int i=1; i<100000; ++i){
	inty[0].insert(i);
	inty[1].insert(i);
	inty[2].insert(i);
   }
   int opcja = -1;
   try{
      if (C.is_open()) {
         cout << "Opened database successfully: " << C.dbname() << endl;
      } else {
         cout << "Can't open database" << endl;
         return 1;
      }
      /* Odkomentowac jesli chcemy tworzyc baze w programie */
      sql = "SET DATESTYLE TO 'European';" \
		"SET DATESTYLE TO 'SQL';";
/*		"DROP TABLE klient;" \
		"DROP TABLE impreza;" \
		"DROP TABLE pomieszczenie;" \
		"DROP TABLE pracownik;" \
		"DROP TABLE historia_imprez;" \
		"CREATE TABLE pomieszczenie(" \
		"ID	integer	primary key," \
		"Powierzchnia	INTEGER	not null," \
		"Ile_osob	INTEGER);" \
		"CREATE TABLE pracownik(" \
		"Nr_dowodu	char(9)	primary key," \
		"Imie	varchar(64) not null," \
		"Nazwisko	varchar(64)	not null," \
		"Data_zatrudnienia	date," \
		"Pensja integer);" \
		"CREATE TABLE klient(" \
		"Pesel	char(11) primary key," \
		"Imie	varchar(64) not null," \
		"Nazwisko	varchar(64) not null," \
		"Nr_telefonu	char(9) not null," \
		"Nr_dowodu char(9) not null);" \
		"CREATE TABLE impreza(" \
		"Id integer primary key," \
		"Data_imprezy date not null," \
		"Ile_osob integer not null," \
		"Nazwisko_pracownika	varchar(64) not null);" \
		"CREATE TABLE historia_imprez(" \
		"Id integer primary key," \
		"Data_imprezy date not null," \
		"Ile_osob integer not null);";*/
      work W(C);
      W.exec(sql);
      W.commit();
      tab[0] = " klient";
      tab[1] = " impreza";
      tab[2] = " pomieszczenie";
      tab[3] = " historia_imprez";
      tab[4] = " pracownik";
      col[0] = 5;
      col[1] = 4;
      col[2] = 3;
      col[3] = 3;
      col[4] = 5;
      Usun();
      Dodaj();
      while(opcja != 6){
	      sql ="";
	      int op2=-1,r1,r2;
	      cout<<"1. Wstaw dane"<<endl;
	      cout<<"2. Wyswietl tabele"<<endl;
	      cout<<"3. Usun rekord"<<endl;
	      cout<<"4. Wyszukaj"<<endl;
	      cout<<"5. Edytuj Dane"<<endl;
	      cout<<"6. Zakonc program"<<endl;
	      cin>>opcja;
	      while((opcja < 1) && (opcja > 6)){
		      cout<<"Podaj lcizbe z zakresu 1-6"<<endl;
		      cin>>opcja;
	      }
	      if(opcja == 1){
		      int s = 5;
		      string inf;
		      string cd = "'";
		      int infi,s1,s2,t;
		      sql += "INSERT INTO";
		      cout<<"Wybierz tabele do ktorej wstawisz rekord"<<endl;
		      Tabele();
		      op2 = Wybierz(s);
		      if((op2==2) || (op2==4)) cout<<"Daty prosze podawac w formacie dd/mm/yyyy"<<endl;
		      sql += tab[op2 - 1];
		      sql += "(";
		      for(int i=0; i<col[op2-1]; ++i){
			      sql+=kol[op2-1][i];
			      if(i != col[op2-1]-1)	sql+=",";
		      }
		      sql +=") VALUES(";
		      cout<<"Podaj ponizsze informacje."<<endl;
		      work W(C);
		      for(int i=0; i<col[op2-1]; ++i){
			      if((op2 != 1) && (op2 !=5) && (i==0)){
				      infi = *(inty[op2-2].begin());
				      inty[op2-2].erase(infi);
				      inf =to_string(infi);
			      }
			      else{
			     	 cout<<kol[op2-1][i]<<": "<<endl;
			     	 cin>>inf;
				 if(i==0){
					 if(op2 == 1) t = 0;
					 else t =1;
					 s1 = key[t].size();
					 key[t].insert(inf);
					 s2 = key[t].size();
					 while(s1 == s2){
						 cout<<"Ta osoba jest juz w bazie, prosze podac inne dane"<<endl;						  cin>>inf;
						 key[t].insert(inf);
						 s2 = key[t].size();
					 }
				 }
			      }
			      sql +=cd +  W.esc(inf)+ cd ;
			      if(i!=col[op2-1]-1) sql+=",";
		      }
		      sql += ");";
		     // work W(C);
		      W.exec(sql);
		      W.commit();
	      }
	      else if(opcja == 2){
		      int s = 5;
		      sql += "SELECT * from";
		      cout<<"Wybierz tabele do wyswietlenia"<<endl;
		      Tabele();
		      op2 = Wybierz(s);
		      sql += tab[op2 - 1];
		      Wyswietl(op2);
	      }
	      else if (opcja == 3){
		      string klucz;
		      string ap = "'";
		      int id;
		      int s = 5;
		      sql += "DELETE from";
		      cout<<"Wybierz tabele, z ktorej usuneisz rekord"<<endl;
		      Tabele();
		      op2 = Wybierz(s);
		      sql += tab[op2 -1];
		      Wyswietl(op2);
		      work W(C);
		      if((op2 != 5) && (op2 != 1)){
			      cout<<"Podaj ID rekordu do usuniecia"<<endl;
			      cin>>klucz;
			      sql += "  where Id = " + W.esc(klucz);
			      sscanf(klucz.c_str(), "%d", &id);
		      }
		      else if(op2 == 1){
			      cout<<"Podaj pesel klienta do usuniecia"<<endl;
			      cin>>klucz;
			      sql += "  where Pesel = " +ap+W.esc(klucz)+ap;
		      }
		      else{
			      cout<<"Podaj nr dowodu pracownika do usuneicia"<<endl;
			      cin>>klucz;
			      sql += "  where Nr_dowodu = " + ap+W.esc(klucz)+ap;
		      }
		      if((op2 !=1) && (op2 !=5)){
		     	 r1 = inty[op2-2].size();
		     	 inty[op2-2].insert(id);
		     	 r2 = inty[op2-2].size();
		      }
		      else{
			 int tmp;
			 if(op2 == 1) tmp =0;
			 else tmp = 1;
			 r1 = key[tmp].size();
			 key[tmp].insert(klucz);
			 r2 = key[tmp].size();
		      }
		      if(r1 == r2) cout<<"Usunieto 0 rekordow"<<endl;
		      else cout<<"Usunieto 1 rekord"<<endl;
		      W.exec(sql);
		      W.commit();
	      }
	      else if(opcja == 4){
		      string war;
		      int y;
		      int s = 3;
		      cout<<"1. Wyszukaj pomieszczenia, ktore zmieszcza podana lcizbe osob"<<endl;
		      cout<<"2. Wyszukaj pracownikow z pensja wieksza niz podana"<<endl;
		      cout<<"3. Imprezy odbywajace sie w podanym dniu"<<endl;
		      op2=Wybierz(s);
		      cout<<"Podaj wartosc wyszukiwania: ";
		      cin>>war;
		      nontransaction N(C);
		      if(op2 == 1){
			      sql = " SELECT * from pomieszczenie where Ile_osob >= " +N.esc(war);
			      y = 2;
		      }
		      else if(op2 == 2){
			      sql = " SELECT * from pracownik where pensja >= " +N.esc(war);
			      y = 4;
		      }
		      else if(op2 == 3){
			      string ap = "'";
			      sql = " SELECT * from impreza where Data_imprezy = "  + ap+N.esc(war)+ap ;
			      y= 1;
		      }
		     // nontransaction N(C);
		      result R(N.exec(sql));
		      for(result::const_iterator c = R.begin(); c != R.end(); ++c){
			      for(int j =0; j<col[y]; ++j)
				      cout<<kol[y][j]<<": "<<c[j]<<' ';
			       cout<<endl;
		      }
	      }
	      else if(opcja == 5){
		      string op2,inf;
		      string ap = "'";
		      cout<<"Podaj pesel klienta, dla ktorego informacje chcesz zmienic"<<endl;
		      cin>>op2;
		      work W(C);
		      sql = "UPDATE klient SET ";
		      for(int j=1; j<5; j++){
			      cout<<kol[0][j]<<": ";
			      cin>>inf;
			      sql += kol[0][j] + "=" +ap+ W.esc(inf)+ap;
			      if (j != 4) sql +=",";
		      }
		      sql += "where Pesel = " + ap + W.esc(op2) + ap;
		      W.exec(sql);
		      W.commit();
	      }
      }
      C.disconnect ();
   }catch (const std::exception &e){
      cerr << e.what() << std::endl;
      return 1;
   }

   return 0;
}
