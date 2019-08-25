#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <algorithm>
#define FI first
#define SD second
using namespace std;
long long int tab[200007][2];
long long int D[200007];
bool V[200007];
const long long int MAX = 2147483647;

int main(){
    int n;
    long long int x,y;
    vector<vector<int> > G;
    set<pair<int,int> > pion,poziom;
    set<pair<int, int> >::iterator next;
    pair<int,int> tmp;
    priority_queue<pair<int, int>, vector<pair<int,int> >, greater<pair<int,int> > > pq;
    cin>>n;
    for(int i=1; i<=n; i++){
        D[i] = MAX;
        G.push_back(vector<int>());
        cin>>x>>y;
        tab[i][0] = x;
        tab[i][1] = y;
        tmp = make_pair(x,i);
        poziom.insert(tmp);
        tmp = make_pair(y,i);
        pion.insert(tmp); 
    }
    G.push_back(vector<int>());
    D[1] = 0;
    for(set<pair<int, int> >::iterator it=poziom.begin(); it!=(--poziom.end()); ++it){
        next = it;
        next++;
        G[(*it).SD].push_back((*next).SD);
        G[(*next).SD].push_back((*it).SD);
    }
    for(set<pair<int, int> >::iterator it=pion.begin(); it!=(--pion.end()); ++it){
        next = it;
        next++;
        for(int j=0; j<G[(*it).SD].size(); j++){
            if((*next).SD == G[(*it).SD][j]) G[(*it).SD].erase(G[(*it).SD].begin()+j);
        }
        for(int j=0; j<G[(*next).SD].size(); j++){
            if((*it).SD == G[(*next).SD][j]) G[(*next).SD].erase(G[(*next).SD].begin()+j);
        }
        G[(*it).SD].push_back((*next).SD);
        G[(*next).SD].push_back((*it).SD);
    }
    tmp = make_pair(0,1);
    pq.push(tmp);
    while(!pq.empty()){
        tmp = pq.top();
        pq.pop();
        long long int v = tmp.SD, d = tmp.FI;
        if(d <= D[v]){
        	for(vector<int>::iterator it=G[v].begin(); it !=G[v].end(); it++){
        		int v2 = (*it), cost = min(abs(tab[v2][0] - tab[v][0]),abs(tab[v2][1]-tab[v][1]));
        		if(D[v2] > D[v] + cost){
        			D[v2] = D[v] + cost;
        			tmp = make_pair(D[v2],v2);
        			pq.push(tmp);
				}
			}
        }
    }
    cout<<D[n]<<endl;
    return 0;
}
