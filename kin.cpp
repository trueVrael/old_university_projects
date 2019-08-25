#include <iostream>
using namespace std;
long long int stare[20001];
long long int nowe[20001];
long long int tab[20001];
long long int tree[65536+2];
int main(){
    long long int n,k,size,c,sum,v,res,x,y;
    cin>>n>>k;
    size = 1;
    while (size < n) size *= 2;
    for (int i=0; i<= size*2; i++) tree[i] = 0;
    for(int i=0; i<n; i++){
            cin>>tab[i];
            stare[i]=1;
    }
    c = 2;
    while(c <= k){
        for(int i=0; i<n; i++){
                sum=0;
                x = size + tab[i];
                y = size + n;
                sum = tree[x]%1000000000;
                if(x != y) sum = (sum+tree[y])%1000000000;
                while (x/2 != y/2){
                      if (x%2 == 0) sum = (sum + tree[x+1])%1000000000;
                      if (y%2 == 1) sum = (sum + tree[y-1])%1000000000;
                      x /= 2;
                      y /= 2;
                }
                nowe[i] = sum%1000000000;
                v = size + tab[i];
                tree[v] = (tree[v] + stare[i])%1000000000;
                while (v != 1) {
                      v /= 2;
                      tree[v] = (tree[2*v] + tree[2*v+1])%1000000000;
                }
        }    
        for(int i=0; i<n; i++) stare[i] = nowe[i];
        for (int i=0; i<= size*2; i++) tree[i] = 0;
        c++;
    }
    res = 0;
    for(int i=0; i<n; i++) res = (res + nowe[i])%1000000000;
    cout<<res<<endl;
    return 0;
}
