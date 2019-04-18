int fibo(int n){
	int i,first,last,temp;
	first=0;
	last=1;
	for(i=0;i<n;i=i+1){
		temp=last;
		last = first+last;
		first =temp;
	}
	return last;
}
int main() {
	int a;
	a=fibo(6);
	print(a);
	exit;
}

