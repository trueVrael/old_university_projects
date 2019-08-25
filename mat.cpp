#include <iostream>
using namespace std;
 
int main(){
        char znak,s;
        int min = 1000001;
        int akt = 0;
        int i, last,d;
        i=1;
        last=1;
        cin>>s;
        while(cin >> znak){
                  i++;
                  if((znak != s) && (znak !='*')){
                           if(s == '*'){
                                 s = znak;
                                 last = i;
                           }
                           else{
                                s = znak;
                                d = i - last;
                                last = i;
                                if (d < min) min = d;
                           } 
                  }
                  else if (znak == s) last = i;
        }
        if (min == 1000001) min = i;
        cout<< i - min +1<<endl;
        return 0;
}
