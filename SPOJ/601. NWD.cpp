#include<iostream>
using namespace std;

int nwd(int a,int b){
	while(a!=b){
		if(a>b)a-=b;
		else b-=a;	
	}
	return a;
}

int main(){
	int t,a,b;
	cin>>t;
	while(t--){
		cin>>a>>b;
		cout<<nwd(a,b)<<endl;
	}
	return 0;
}
