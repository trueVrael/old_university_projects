#include <iostream>
#include <algorithm>
using namespace std;
const int M = 5000;
short int obw[M+7][3];
int dyn[2][3][M+7][3]; //rzad,wiersz,ekipy,start

int main(){
int n,k,res,tmp;
cin>>n>>k;
for(int i=0; i<n; i++) cin>>obw[i][2]>>obw[i][1]>>obw[i][0];
for(int j=0; j<3; j++)
	for(int l=0; l<3; l++){
		dyn[0][l][0][j] = 0;
		dyn[1][l][0][j] = 0;
	}
res = 0;
for(int i=0; i<3; i++){
	dyn[0][i][1][i] = max(obw[0][(i+1)%3], obw[0][(i+2)%3]);
	dyn[1][i][1][i] = max(obw[0][(i+1)%3], obw[0][(i+2)%3]);
	dyn[0][(i+1)%3][1][i] = obw[0][(i+2)%3];
	dyn[1][(i+1)%3][1][i] = obw[0][(i+2)%3];
	dyn[0][(i+2)%3][1][i] = obw[0][(i+1)%3];
	dyn[1][(i+2)%3][1][i] = obw[0][(i+1)%3];
	dyn[1][i][1][i] = max(obw[0][(i+1)%3], obw[0][(i+2)%3]);
	for(int t=2; t<=k; t++){
		dyn[0][i][t][i] = obw[0][(i+1)%3] + obw[0][(i+2)%3];
		dyn[1][i][t][i] = obw[0][(i+1)%3] + obw[0][(i+2)%3];
	}
	for(int j=1; j<n; j++){
		for(int l=1; l<=k; l++){
			// policz najwyzsze
			if(dyn[0][0][l][i] > dyn[0][0][l-1][i] + max(obw[j][1],obw[j][2]))
				tmp = dyn[0][0][l][i];
			else	tmp = dyn[0][0][l-1][i] + max(obw[j][1],obw[j][2]);
			if(l >=2){
				if (tmp < dyn[0][0][l-2][i] + obw[j][1] + obw[j][2])
					tmp = dyn[0][0][l-2][i] + obw[j][1] + obw[j][2];
			}
			if (tmp < max(dyn[0][1][l][i],dyn[0][1][l-1][i] + obw[j][2]))
				tmp = max(dyn[0][1][l][i],dyn[0][1][l-1][i] + obw[j][2]);
			if(tmp < dyn[0][2][l][i])	tmp = dyn[0][2][l][i]; 
			dyn[1][0][l][i] = tmp;
			
			// policz srodkowe
			if(dyn[0][1][l][i] > dyn[0][1][l-1][i] + max(obw[j][0],obw[j][2]))
				tmp = dyn[0][1][l][i];
			else tmp = dyn[0][1][l-1][i] + max(obw[j][0],obw[j][2]);
			if(l >=2){
				if (tmp < dyn[0][1][l-2][i] + obw[j][0] + obw[j][2])
					tmp = dyn[0][1][l-2][i] + obw[j][0] + obw[j][2];
			}
			if (tmp < max(dyn[0][0][l][i],dyn[0][0][l-1][i] + obw[j][2]))
				tmp = max(dyn[0][0][l][i],dyn[0][0][l-1][i] + obw[j][2]);
			if (tmp < max(dyn[0][2][l][i],dyn[0][2][l-1][i] + obw[j][0]))
				tmp = max(dyn[0][2][l][i],dyn[0][2][l-1][i] + obw[j][0]);
			dyn[1][1][l][i] = tmp;

			// policz najnizsze
			if(dyn[0][2][l][i] > dyn[0][2][l-1][i] + max(obw[j][1],obw[j][0]))
				tmp = dyn[0][2][l][i];
			else	tmp = dyn[0][2][l-1][i] + max(obw[j][1],obw[j][0]);
			if(l >=2){
				if (tmp < dyn[0][2][l-2][i] + obw[j][1] + obw[j][0])
					tmp = dyn[0][2][l-2][i] + obw[j][1] + obw[j][0];
			}
			if (tmp < max(dyn[0][1][l][i],dyn[0][1][l-1][i] + obw[j][0]))
				tmp = max(dyn[0][1][l][i],dyn[0][1][l-1][i] + obw[j][0]);
			if(tmp < dyn[0][0][l][i])	tmp = dyn[0][0][l][i];
			dyn[1][2][l][i] = tmp;
		}
		for(int l=0; l<=k; l++){
			dyn[0][0][l][i] = dyn[1][0][l][i];
			dyn[0][1][l][i] = dyn[1][1][l][i];
			dyn[0][2][l][i] = dyn[1][2][l][i];
		}
	}
	if(dyn[1][i][k][i] > res) res = dyn[1][i][k][i];
}
cout<<res<<endl;
system("pause");
return 0;
}
