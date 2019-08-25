#include <iostream>
#include <cstdio>
using namespace std;
const int N = 1000000;
 int par[N+7][2]; //tab[][0] najwieksze od lewej, tab[][1] najmniejsze od prawej
int npar[N+7][2];
long long int bazar[N+7];
int main(){
    ios_base::sync_with_stdio(0);
    long long int suma,tmp1,tmp2;
    int n,j,k,m,p,x;
    j = 0;
    k = 0;
    cin >> n;
    cin >> bazar[0];
    if (bazar[0]%2 == 1){
                    npar[0][0] = bazar[0];
                    par[0][0] = -1;
    }
    else{
         npar[0][0] = -1;
         par[0][0] = bazar[0];
    }
    //obliczanie potrzebnych struktur
    for (int i=1; i<n; i++){
        cin >> x;
        bazar[i] = bazar[i-1] + x;
        if (x%2 == 1) {
                npar[i][0] = x;
                par[i][0] = par[i-1][0];
        }
        else{
                par[i][0] = x;
                npar[i][0] = npar[i-1][0];
        }
    }
    
    if ((bazar[n-1] - bazar[n-2])%2 == 1){
                    npar[n-1][1] = bazar[n-1] - bazar[n-2];
                    par[n-1][1] = -1;
    }
    else{
         npar[n-1][1] = -1;
         par[n-1][1] = bazar[n-1] - bazar[n-2];
    }
    
    for (int i=n-2; i>=1; i--){
        if ((bazar[i] - bazar[i-1])%2 == 1){
                       npar[i][1] = bazar[i] - bazar[i-1];
                       par[i][1] = par[i+1][1];
        }
        else{
             npar[i][1] = npar[i+1][1];
             par[i][1] = bazar[i] - bazar[i-1];
        }
    }
    if (bazar[0]%2 == 1){
                    npar[0][1] = bazar[0];
                    par[0][1] = par[1][1];
    }
    else{
         npar[0][1] = npar[1][1];
         par[0][1] = bazar[0];
    }
    //zapytania
    cin >> m;
    for (int t=0; t<m; t++){
        suma = -1;
        tmp1 = -1;
        tmp2 = -1;
        cin >> p;
        if (n != p) suma = bazar[n-1] - bazar[n-p-1];
        else suma = bazar[n-1];
        
        if (suma%2 == 1)  printf("%lld\n", suma);
        else if (n != p){
             if( (par[n-p][1] == -1) || (npar[n-p-1][0] == -1)) 
                 tmp1 = -1;
             else
                 tmp1 = suma - par[n-p][1] + npar[n-p-1][0];
             if( (npar[n-p][1] == -1) || (par[n-p-1][0] == -1)) 
                 tmp2 = -1;
             else
                 tmp2 = suma - npar[n-p][1] + par[n-p-1][0];
                 
             if ((tmp1 == tmp2) && (tmp1 == -1)) printf("%lld\n", tmp1);
             else if (tmp1 >= tmp2)  printf("%lld\n", tmp1);
             else  printf("%lld\n", tmp2);
        }
        else  printf("-1\n");
    }
    return 0;
}
