#include<iostream>
using namespace std;
struct node{
	int data;
	node *next;
};
void main() {
	clrscr();
	node *head, *temp, *prev, *ins;
	int i, x, y, z;
	//creating first node
	head = new node;
	cout<<"data ";
	cin>>x;
	head->data = x;
	head->next = NULL;
	temp = head;
	for(i = 0; i < 4; i++) {
		temp->next = new node;
		temp = temp->next;
		cout<<"data ";
		cin>>x;
		temp->data = x;
		temp->next = NULL;
	}
	cout<<"Enter data to be deleted: ";
	cin>>y;
	//deleting a node
	temp = head;
	while((temp != NULL) && (temp->data != y)) {
		prev = temp;
		temp = temp->next;
	}
	if(temp != NULL) {
		prev->next = temp->next;
		delete temp;
	}
	//inserting a node
	temp = head;
	while((temp != NULL) && (temp->data != z)) {
		prev = temp;
		temp = temp->next;
	};
	if(temp != NULL) {
		ins = new node;
		ins->data = y;
		ins->next = temp;
		prev->next = ins;
	}
	//printing out the linked list
	cout<<"The list elements are: "<<endl;
	temp = head;
	while(temp != NULL) {
		cout<<"temp->data"<<endl;
		temp = temp->next;
	}
}