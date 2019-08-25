#include <iostream>
#include <algorithm>
using namespace std;
bool por (pair < long long int, long long int > x, pair < long long int, long long int > y){
if(x.first==y.first) return x.second<y.second;
else return x.first<y.first;
}
pair < long long int , long long int > l[10000];
long long int a, b, c, m, p, po, k, ko, s, z, t, tab[100000];
int main(){
cin>>a;
for(int i=0; i<a; i++){
    cin>>l[i].first;
    l[i].second=i;
}
sort(l, l+a, por);
cin>>b;
for(int i=0; i<b; i++){
	cin>>c;
	cout<<i<<endl;
    for(int j=0; j<c; j++){
    	p=0;
		k=a-1;
        cin>>tab[j];   
    	while(p<k){
			s=(p+k)/2;
			if(l[s].first>=tab[j]) k=s;
			else p=s+1;
		}
		po=p;
		k=a-1;
    	while(p<k){
			s=(p+k+1)/2;
			if(l[s].first>tab[j]) k=s-1;
			else p=s;
		}
		k=ko;
		if(l[p].first==tab[j]){
			if(m>l[p].second){
				while(po<ko){
					s=(po+ko)/2;
					if(l[s].second<m) po=s+1;
					else ko=s;
				}
				k=ko;
			}
			if(l[po].second<m){
				t=1;
				j=c;
			}
			m=l[k].second;	
		}
		else{
			t=1;
			j=c;
		}
	}
	cout<<i<<endl;
	if(t==1) cout<<"NIE"<<endl;
	else cout<<"TAK"<<endl;
	t=0;
}
system("pause");
return 0;
}
