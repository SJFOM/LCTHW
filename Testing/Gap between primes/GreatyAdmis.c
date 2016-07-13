//#include <iostream>
//#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//using namespace std;
int p[1000];
int B;
int k;
int a[1000];

// Prototype
int checkprime(int);
int check(int, int);
int sqroot(int);
int true = 1;

int main(int argc, char* argv[])
{ 
	int i;
	int b=1;
	k=725;
	p[1]=2;


	for(i=1; true ; i++)
	{
		
		if(checkprime(i)==1)
		{
			b++;
			p[b]=i;
			if(i>k)
			{  
				B=b;
				break;
			}
		}
	}
	
	a[1]=0; 
	for(i=2; i<=k; i++)
	{
		int j;
		for(j=a[i-1]+1; true; j++)
		{
			if(check(j, i)==1)
			{
				a[i]=j;
				//printf("%d", a[i]); 
				break;
			}
		}
		
	}
	
	
	int T=a[k]-a[1];
	printf("\n\nAdmissible tuple: %d, .., %d \n", a[1], a[k]);
	printf("\nWidth of our admissible tuple %d \n\n", T);

	return 0;	
}


int sqroot(int y)
{
	int i = 1;
	// for(i=1; true; i++)
	// {
	// 	if(i*i>y){return i-1;}	
	// }
	while(i*i<=y){
		i++;
	}
	return i-1;
}

int checkprime(int x)
{
	int i;

	for(i=2;i<=sqroot(x); i++)
	{
		if((x/i)*i==x){return 0;}
	}
	return 1;

}


int check(int x, int y)
{

	int i;

	for(i=1; i<=B; i++)
	{
		int c[1000];
		int f= (x % p[i]);
		c[f]=1;
		int r;

		for(r=1; r<y; r++ )
		{
			int d= (a[r] % p[i]);
			c[d]=1;	
		}

		int sum=0;

		for(r=2; r<p[i]; r++)
		{
			sum=sum+c[r];
		}
		
		if(sum==p[i]){return 0;}
	}	

	return 1;
}




