#include<iostream>
using namespace std;
void main() {
	int table[200], target, first, last, mid, i, found;
	//setting up the array
	for(i - 0; i < 200; i++) { 
		table[i] = 2*i;
	}
	first = 0;
	last = 199;
	found = 0;
	//input
	cout<<"Input the target:";
	cin>>target;
	//binary search 
	while ((found==0) && (first <= last)) {
		mid = (first + last)/2;
		if(target == table[mid]){
			found = 1;
		} else {
			if(target < table[mid]) {
				last = mid - 1;
			} else{
				first = mid + 1;
			}
		}
		if(found) {
			cout<<"\nPosition = "<<mid;
		}else {
			cout<<"\nTarget not found";
		}
	}
}