#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <minmax.h>
#include <string.h>
#include <Windows.h>
#include <sys/stat.h>
struct tree {
	char letter;
	int frequency;
	tree*left, *right;
};
struct node {
	tree*data;
	node*next;
};
struct linked {
	char data;
	linked*next;
};
linked*top = NULL;
void makelist(char data)
{
	if (!top)
	{
		top = (linked*)malloc(sizeof(linked));
		top->data = data;
		top->next = NULL;
	}
	else
	{
		linked *temp = top;
		while (temp->next)
			temp = temp->next;
		temp->next = (linked*)malloc(sizeof(linked));
		temp->next->data = data;
		temp->next->next = NULL;
	}
}
void makelinkedlist(char arr[])
{
	for (int i = 0; i<strlen(arr); i++)
		makelist(arr[i]);
}
node* push(node*top, tree*data)
{
	if (!top)
	{
		top = (node*)malloc(sizeof(node));
		top->data = data;
		top->next = NULL;
	}
	else
	{
		node*temp = top;
		top = (node*)malloc(sizeof(node));
		top->data = data;
		top->next = temp;
	}
	return top;
}
tree*maketree(tree*root, char s)
{
	if (!root)
	{
		root = (tree*)malloc(sizeof(tree));
		root->letter = s;
		root->frequency = 1;
		root->left = root->right = NULL;
	}
	else
	{
		if (s>root->letter)
			root->right = maketree(root->right, s);
		else if (s<root->letter)
			root->left = maketree(root->left, s);
		else
			root->frequency++;
	}
	return root;
}
tree*maketreenode(tree*r, char s, int freq)
{
	r = (tree*)malloc(sizeof(tree));
	r->frequency = freq;
	r->left = r->right = NULL;
	r->letter = s;
	return r;
}
void inorder(tree*root, node**top)
{
	if (root != NULL)
	{
		inorder(root->left, top);
		*top = push(*top, maketreenode(root, root->letter, root->frequency));
		inorder(root->right, top);
	}
}
int isEmpty(node*top)
{
	if (!top)
		return 1;
	return 0;
}
node* pop(node*top, tree**r)
{
	*r = top->data;
	node*tmp = top;
	top = top->next;
	free(tmp);
	return top;
}
tree*dequeue(node**top)
{
	node*temp = NULL;
	tree*tmp;
	int min;
	min = (*top)->data->frequency;
	while (!isEmpty(*top))
	{
		*top = pop(*top, &tmp);
		if (tmp->frequency<min)
			min = tmp->frequency;
		temp = push(temp, tmp);
	}
	tree*r = NULL;
	int flag = 0;
	while (!isEmpty(temp))
	{
		temp = pop(temp, &tmp);
		if (tmp->frequency == min&&flag == 0)
		{
			++flag;
			r = tmp;
		}
		else
			*top = push(*top, tmp);
	}
	return r;
}
int oneleft(node*top)
{
	int size = 0;
	while (top != NULL)
	{
		size++;
		if(size>1)
			break;
		top = top->next;
	}
	if (size == 1)
		return 1;
	return 0;
}
tree*makesubtree(tree*a, tree*b)
{
	tree*r = NULL;
	r = maketree(r,252);
	r->frequency = a->frequency + b->frequency;
	r->right = b;
	r->left = a;
	return r;
}
tree*huffman(tree*r)
{
	node*top = NULL;
	inorder(r, &top);
	while (!oneleft(top))
	{
		tree*a = NULL, *b = NULL, *c = NULL;
		a = dequeue(&top);
		b = dequeue(&top);
		c = makesubtree(a, b);
		top = push(top, c);
	}
	top = pop(top, &r);
	return r;
}
int isLeaf(tree*r)
{
	if(r->right==NULL&&r->left==NULL)
		return 1;
	return 0;
}
void convert(tree*r, char s, char arr[], int top)
{
	if (r->left)
	{
		arr[top] = '0';
		convert(r->left, s, arr, top + 1);
	}
	if (r->right)
	{
		arr[top] = '1';
		convert(r->right, s, arr, top + 1);
	}
	if (r->letter == s)
	{
		arr[top] = '\0';
		makelinkedlist(arr);
	}
}
int maxdepth(tree*r)
{
	if (!r)
		return 0;
	return max(maxdepth(r->left), maxdepth(r->right));
}
void compress(tree*r, char c)
{
	char*p = (char*)malloc((sizeof(char)*maxdepth(r)) + 1);
	convert(r, c, p, 0);
}
void put(FILE*w)
{
	char x[9], y;
	int i = 0;
	while (top != NULL)
	{
		x[i] = top->data;
		i++;
		if (i == 8)
		{
			x[8] = '\0';
			y = strtol(x, NULL, 2);
			fprintf(w, "%c", y);
			i = 0;
		}
		top = top->next;
	}
	if (i<7)
	{
		for (int j = i; j<8; j++)
			x[j] = '0';
		x[8] = '\0';
		y = strtol(x, NULL, 2);
		fprintf(w, "%c", y);
	}
}
void puttree(tree*root, FILE*w)
{
	if (root != NULL)
	{
		puttree(root->left, w);
		puttree(root->right, w);
		fprintf(w, "%c", root->letter);
	}
}
tree*buildtree(tree*r)
{
	node*stack = NULL;
	linked*temp = top;
	tree*a, *b, *c;
	while (temp!= NULL)
	{
		c = NULL;
		if (temp->data == -4)
		{
			stack = pop(stack, &a);
			stack = pop(stack, &b);
			c = maketree(c, 252);
			c->left = b;
			c->right = a;
			stack = push(stack, c);
		}
		else
		{
			c = maketree(c, temp->data);
			c->left = c->right = NULL;
			stack = push(stack, c);
		}
		temp = temp->next;
	}
	stack = pop(stack, &r);
	return r;
}
void deletelist()
{
	linked*next;
	while (top != NULL)
	{
		next = top->next;
		free(top);
		top = next;
	}
}
void printlist()
{
	linked*list = top;
	while (list != NULL)
	{
		printf("%c", list->data);
		list = list->next;
	}
}
void decompress(tree*root, FILE*w)
{
	linked*temp = top;
	tree*tmp = root;
	while (temp!= NULL)
	{
		if (isLeaf(tmp))
		{
			if(tmp->letter==-3)
			{
				return ;
			}
			fprintf(w, "%c", tmp->letter);
			tmp = root;
		}
		if (temp->data == '1')
			tmp = tmp->right;
		if (temp->data == '0')
			tmp = tmp->left;
		temp = temp->next;
	}
}
void postorder(tree*r)
{
	if(!r)
		return;
	postorder(r->left);
	postorder(r->right);
	printf("%c",r->letter);
}
int main(void)
{
	int n;
	tree*root = NULL;
start:
	system("cls");
	printf("1-Compress a file.\n2-Decompress a file.");
	scanf("%d", &n);
	if (n == 1)
	{
		char filename[MAX_PATH];//got this from internet
		OPENFILENAME ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
		ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select a File";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn))
			printf("You chose the file : %s\n", filename);
		FILE*r, *w;
		char x;
		r = fopen(filename, "rb");
		if (r == NULL)
		{
			printf("FILE NOT FOUND");
			getch();
			exit(1);
		}
		while (!feof(r))
		{
			fscanf(r, "%c", &x);
			root = maketree(root, x);
		}
		root=maketree(root,253);
		rewind(r);
		root = huffman(root);
		w = fopen("Compressed.txt", "wb");	//Make sure you are using binary files
		while (!feof(r))
		{
			fscanf(r, "%c", &x);
			compress(root, x);
		}
		compress(root,253);
		puttree(root,w);
		fprintf(w,"%c",254);
		struct stat st;
		stat(filename,&st);
		int size=st.st_size;
		fprintf(r,"%d",size%8);
		put(w);
		fclose(r);
		fclose(w);
		getch();
		return 0;
	}
	else
	{
		char filename[MAX_PATH];//got this from internet
		OPENFILENAME ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
		ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select a File";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileNameA(&ofn))
			printf("You chose the file : %s\n", filename);
		FILE*decompressed;
		decompressed = fopen("decompressed.txt", "wb");
		char x;
		int i = 0;
		FILE*r = fopen(filename, "rb");
		fscanf(r,"%c",&x);
		while(x!=-2)
		{
			makelist(x);
			fscanf(r,"%c",&x);
		}
		int bit;
		fscanf(r,"%d",&bit);
		tree*getr=NULL;
		getr=buildtree(getr);
		struct stat st;
		stat("Compressed.txt",&st);
		int size=st.st_size;
		int coun=0;
		while (!feof(r))
		{
			++coun;
			fscanf(r, "%c", &x);
			if(coun==size)
			{
				for(int i=7;i>=abs(bit-7);i++)
					x&(1 << i) ? makelist('1') : makelist('0');
			}
			else for(int i = 7; i >= 0; i--)
				x&(1 << i) ? makelist('1') : makelist('0');
		}
		decompress(getr, decompressed);
		fclose(decompressed);	//make sure you are closing the decompressed file to flush data to the output
	}
	getch();
}