#include <iostream>
using namespace std;
short int stan[2097152+2];//0 czarny, 1 bialy, 2 mieszany
int pod[2097152+2];
int drzewo[2097152+2];
int n,size,a,b;
void dfs(int x, int y, int v, int c, int akt){//zakres,malowanie
	int s,l,p;
	if(akt==2) akt = stan[v];
	if(v < size){
		l = v*2;
		p = v*2 +1;
		if(akt != 2){
            stan[l] = akt;
            stan[p] = akt;
			drzewo[l] = pod[l] * stan[l]; 
			drzewo[p] = pod[p] * stan[p];
		}
		stan[v] = 2;
//cout<<l<<" "<<p<<" "<<akt<<endl;
	}
	if((x >= a) && (y <= b) && ((a!=x) && (b!=y)) || (x==y)){
		stan[v] = c;
		drzewo[v] = pod[v] * c;
//cout<<"fsafsa "<<v<<endl;
		return;
	} 
	s = (x+y)/2;
	if(a <= s) dfs(x, s,2*v,c,akt);
	if(b > s) dfs(s+1,y,(2*v)+1,c,akt);

}
int main(){
    int k,x1,y1,x2,y2,l,p,s,v1,v2,kol;
    char c;
    cin>>n>>k;
    size = 1;
    while (size < n) size *= 2;
    for(int i=0; i<n; i++) pod[size + i] = 1;
    for(int i=size-1; i>0; i--) pod[i] = pod[i*2] + pod[i*2+1];
 //   for(int i=1; i<size +n; i++) cout<<pod[i]<<endl;
    for(int i=0; i<2097153; i++){
            drzewo[i]=0;
            stan[i]=0;
    }
    for(int i=0; i<k; i++){
            cin>>a>>b>>c;
            l = size+a-1;
            p = size+b-1;
	    if(c == 'C') kol = 0;
	    else kol = 1;
	    dfs(1,size,1,kol,stan[1]);
             //       for(int i=1; i<size +n; i++) cout<<i<<": "<<drzewo[i]<<endl;
          //napraw
          v1 = l;
          v2 = p;
          while (v1 != 1){
                      v1 /= 2;
                      v2 /= 2;
                      drzewo[v1] = (drzewo[2*v1] + drzewo[2*v1+1]);
                      drzewo[v2] = (drzewo[2*v2] + drzewo[2*v2+1]);
        //              cout<<v1<<" "<<v2<<" "<<drzewo[v1]<<" "<<drzewo[v2]<<endl;
          }
      //    for(int i=1; i<size +n; i++) cout<<i<<": "<<drzewo[i]<<endl;
        cout<<drzewo[1]<<endl;
    }
    system("pause");
    return 0;
}
