#include<iostream>
using namespace std;

class ADTstack
{
	private:
		int stack[10];
		int topstack;
	public:
		ADTstack()
		{
			topstack = -1;
		}
		
		int empty()
		{
			if(topstack==-1)
				return 1;
			else
				return 0;
		} 
		int full()
		{
			if(topstack==9)
				return 1;
			else
				return 0;
		}
		void push(int num)
		{
			if (!full())
			{
				topstack++;
				stack[topstack] = num;
			}
			else
			{
				cout<<"Stack is Full"<<endl;
			}
		}
		int pop()
		{
			int num;
			if(!empty())
			{
				num = stack[topstack];
				topstack--;
				return num;
			}
			else
			{
				cout<<"Stack is Empty"<<endl;
				return 0;
			}
		}
};
void main()
{
	ADTstack st;
	st.push(23);
	st.push(46);
	st.push(37);
	cout<<st.pop()<<endl;
	cout<<st.pop()<<endl;
	cout<<st.pop()<<endl;
	cout<<st.pop()<<endl;
}