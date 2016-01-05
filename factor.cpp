#include <iostream>
#include <map>
#include <limits>
#include <cmath>

using namespace std;

long primeNumber(long n, map<long, int>& pCache){
    long s, i, p = 0;
    map<long, int>::iterator it;

    if(n < 0)   return -1;
    else if(n == 0) return 0;
    else if(n == 1) return 1;

    if(!pCache.empty()){
        for(it = pCache.begin(); it != pCache.end(); ++it){
            p = it->first;
            //cout<<it->first<<" -> "<<it->second<<endl;
            if(n % p == 0){
                //cout<<it->second<<endl;
                pCache[p] = (it->second)+1;
                return p;
            }else if(p > n){
                pCache.insert(pair<long, int>(n, 1));
                return n;
            }
        }
    }

    s = (long)(sqrt(n)+1);
    for(i = 2; i <= s; i++){
        if(n % i == 0){
            if(i > p)
                pCache.insert(pair<long, int>(i, 1));
            return i;
        }
    }
    pCache.insert(pair<long, int>(n, 1));
    return n; //itself is a prime number;
}

int main(){
    long n, primeN;
    bool valid = false;
    map<long, int> factor;

    do{
        cout<<"Please enter a number : ";
        if(cin>>n && !cin.fail()){
            valid = true;
        }else{
            cout<<"Wrong number. please try again."<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }while(!valid);

    if(n == 0 || n == 1 || n == -1){
        cout<<"("<<n<<", 1)"<<endl;
    }else{
        do{
            n /= primeNumber(n, factor);
        }while(n > 1);
        valid = false;
        for(map<long, int>::iterator it = factor.begin(); it != factor.end(); ++it){
            if(valid)   cout<<", ";
            cout<<"("<<it->first<<" ,"<<it->second<<")";
            valid = true;
        }
        cout<<'.'<<endl;
    }
}
