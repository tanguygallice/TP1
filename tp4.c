#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned int_width(int i)
{
	unsigned count=0;
	long int j=i;
	if(j==0)
		count+=1;
	if(j<0)
		count+=1;
	while(j)
	{
		count+=1;
		j=j/10;
	}
	return count;
}


unsigned ints_width(const int* tab, unsigned count)
{
	int max = 0;
	for (unsigned i = 1; i < count; i++)
	{
		if (int_width(tab[i]) > int_width(tab[max]))
			max = i;
	}
	return int_width(tab[max]);
}

void print_int_array(FILE *out, const int *tab, unsigned count)
{
	unsigned max = ints_width(tab, count);
	unsigned columns = 0;
	unsigned spaces_first = log10(count-1);
	unsigned i = 0;
	fprintf(out, "%*s[%d]", spaces_first, "", i);
	columns += spaces_first + 3;
	for (; i < count; i++)
	{
		if ((columns + 1 + max) <= 80)
		{
			fprintf(out, " ");
			columns += 1;
			fprintf(out, "%*d", max, tab[i]);
			columns += max;
		}
		else
		{
			fprintf(out, "\n");
			columns = 0;
			fprintf(out,"%*s[%d] ", spaces_first - int_width(i) + 1, "", i);
			columns += spaces_first + 3 + int_width(i);
			fprintf(out, "%*d", max, tab[i]);
			columns += max;
		}
	}
	fprintf(out, "\n");
}

void insert_sort(int* tab, unsigned count)
{
	for (unsigned i = 1; i < count; i++)
	{
		int key = tab[i];
		int j = i - 1;

		while (j >= 0 && tab[j] > key)
		{
			tab[j + 1] = tab[j];
			j--;
		}
		tab[j + 1] = key;
	}
}


void insert_sort_cmp(int* tab, unsigned count, int (*cmp)(int a, int b))
{
	for (unsigned i = 1; i < count; i++)
	{
		int key = tab[i];
		int j = i - 1;

		while (j >= 0 && (*cmp)(tab[j], key) == 1)
		{
			tab[j + 1] = tab[j];
			j--;
		}
		tab[j + 1] = key;
	}
}

int decreasing(int a, int b)
{
	if (a > b)
		return -1;
	return a < b;

}




unsigned linear_search(const int* tab, unsigned count, int val,
		int (*cmp)(int a, int b))
{
	for (unsigned i = 0; i < count; i++)
	{
		if ((cmp)(tab[i], val) >= 0)
			return i;
	}
	return count;
}


unsigned binary_search(int* tab, unsigned count, int val, 
		int (*cmp)(int a, int b))
{
	unsigned b = 0;
	unsigned e = count;
	unsigned m = 0;

	if (count == 0)
		return 0;
	while (b < e)
	{

		m = b+ (e-b)/2;
		int comp = cmp(val,tab[m]);
		if (val==tab[m])
			return m;
		else
		{
			if (comp == -1)
				e = m;
			else
				b = m + 1;
		}
	}
	return b;
}

void heapify(int *tab, unsigned pos,unsigned n)
{
	int left = 2 * pos + 1;
	int right = 2 * pos + 2;
	int max = 0;
	int swtch;
	int pos1=pos;
	int n1=n;
	if (left < n1 && tab[left] > tab[pos1])
		max = left;
	else
		max = pos1;
	if (right < n1 && tab[right] > tab[max])
		max = right;
	if (max != pos1)
	{
		swtch =tab[pos1];
		tab[pos1] = tab[max];
		tab[max] = swtch;
		heapify(tab,max,n1);
	}
}

void make_heap(int *tab,unsigned n)
{
	for (int i = (n / 2) - 1 ; i >= 0 ;i--)
		heapify(tab ,i ,n);	
}

bool check_heap(int* heap,unsigned n)
{
	int father = 0;
	for (int i=n-1; i>=0;i--)
	{
		father = (i-1)/2;
		if (heap[father] < heap[i])
			return false;	
	}
	return true;
}

void heap_sort(int* tab,unsigned n)
{
	make_heap(tab,n);
	int swit=0;
	for(unsigned i = n - 1; i > 0 ;i--)
	{
		swit=tab[i];
		tab[i]=tab[0];
		tab[0]=swit;
		heapify(tab,0,i);
	}

}

unsigned partition_cmp(int *tab,unsigned b,unsigned e,unsigned p,int (*cmp)(int a,int b))
{
	int max = tab[p];
	unsigned i = b - 1;
	unsigned j = e;
	while(1)
	{
		do
		{
			i++;
		} while (cmp(tab[i], max) < 0);
		do
		{
			j--;
		} while (cmp(tab[j], max) > 0);
		if (i < j)
		{
			int w = tab[j];
			tab[j]=tab[i];
			tab[i]=w;
		}
		else
			return i + (b==i);
	}
	return p;
}

unsigned cmp_count;

int increasing(int a, int b)
{
	++cmp_count;
	if (a < b)
		return -1;
	return a > b;

}

#define my_test(val) \
	cmp_count = 0; \
	printf("binary_search(a, %u, %d, increasing) = %u\n", \
			asize, (val), binary_search(a, asize, (val), increasing)); \
			printf("\twith %u comparisons\n", cmp_count);

int main(void)
{
	for (unsigned p = 0; p <= 10; p += 5)
	{
		int tab[11] = { 2, 8, 42, 9, 13, 5, 3, 23, 40, -3, 55 };
		printf("p = %u  =>  m = %u\n", p, partition_cmp(tab, 0, 11, p, increasing));
		print_int_array(stdout, tab, 11);
	}
	return 0;
}
