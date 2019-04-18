int fact(int n){
	int i,b;
	b=1;
	for(i=n;i>0;i=i-1){
		b=b*i;
	}
	return b;
}
int main() {
	int a;
	a=fact(6);
	print(a);
	exit;
}

