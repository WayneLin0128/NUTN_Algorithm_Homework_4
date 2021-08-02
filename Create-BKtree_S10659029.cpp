#include <iostream>
#include<fstream>
#include <Windows.h>
#include <vector>
#include <string>
#include<cmath>
#include<math.h>
#include <iomanip>
#include <algorithm>
using namespace std;

// maximum number of words in dict[] 
#define MAXN 200
// defines the tolerence value 
#define TOL  3
// defines maximum length of a word 
#define LEN 15

struct Node
{
	// stores the word of the current Node 
	string word;
	// links to other Node in the tree 
	int next[2 * LEN];

	// constructors 
	Node(string x) :word(x)
	{
		// initializing next[i] = 0 
		for (int i = 0; i < 2 * LEN; i++)
			next[i] = 0;
	}
	Node() {}
};

// stores the root Node 
Node RT;
// stores every Node of the tree 
Node tree[MAXN];
// index for current Node of tree 
int ptr;

vector <string> dictionary;
vector <string> test;
int determine = 0;
int dist;

int EditDistance(string X, string Y);
int levenshtein_EditDistance(string X, string Y);
int damerau_levenshtein_distance(string p_string1, string p_string2);
void add(Node& root, Node& curr);
vector <string> getSimilarWords(Node& root, string& s);

int main() {
	ifstream fin("Dic1.txt");
	string temp;
	int size, calculate = 1;
	bool word_in_dict =false;
	ptr = 0;
	while (!fin.eof())
	{
		fin >> temp;
		dictionary.push_back(temp);
	}
	size = dictionary.size();

	/*****************************傳統 Edit Distance****************************/
	cout  << "***************************傳統 Edit Distance********************************" << endl;
	determine = 0;
	fstream fwrite("BKtreeDic1.txt");

	for (int i = 0; i < size; i++)
	{
		Node tmp = Node(dictionary[i]);
		add(RT, tmp);
	}

	cout << "Root: " << RT.word << ": ";
	fwrite << RT.word << " ";
	for (int i = 0; i < 2 * LEN; i++)
	{
		cout << RT.next[i] << " ";
		fwrite << RT.next[i] << " ";
	}
	fwrite << endl;
	cout << endl;

	for (int i = 1; i < size; i++) {
		cout << tree[i].word << ": ";
		fwrite << tree[i].word << " ";
		for (int j = 0; j < 2 * LEN; j++)
		{
			cout << tree[i].next[j] << " ";
			fwrite << tree[i].next[j] << " ";
		}
		cout << endl;
		fwrite << endl;
	}
	
	/**********************清除tree******************************/
	ptr = 0;
	RT.word = "";
	for (int i = 0; i < 2 * LEN; i++)
	{
		RT.next[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		tree[i].word = "";
		for (int j = 0; j < 2 * LEN; j++)
		{
			tree[i].next[j] = 0;
		}
	}


	/*******************Levenshtein Edit Distance***********************/
	determine = 1;
	cout << endl << "***************************Levenshtein Edit Distance********************************" << endl;
	fstream fwrite_len("BKtreeDic2.txt");

	for (int i = 0; i < size; i++)
	{
		Node tmp = Node(dictionary[i]);
		add(RT, tmp);
	}

	cout << "Root: " << RT.word << ": ";
	fwrite_len << RT.word << " ";
	for (int i = 0; i < 2 * LEN; i++)
	{
		cout << RT.next[i] << " ";
		fwrite_len << RT.next[i] << " ";
	}
	fwrite_len << endl;
	cout << endl;

	for (int i = 1; i < size; i++) {
		cout << tree[i].word << ": ";
		fwrite_len << tree[i].word << " ";
		for (int j = 0; j < 2 * LEN; j++)
		{
			cout << tree[i].next[j] << " ";
			fwrite_len << tree[i].next[j] << " ";
		}
		cout << endl;
		fwrite_len << endl;
	}

	/**********************清除tree******************************/
	ptr = 0;
	RT.word = "";
	for (int i = 0; i < 2 * LEN; i++)
	{
		RT.next[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		tree[i].word = "";
		for (int j = 0; j < 2 * LEN; j++)
		{
			tree[i].next[j] = 0;
		}
	}


	/****************Damerau–Levenshtein Distance**********************/
	determine = 2;
	cout << endl << "***************************Damerau–Levenshtein Distance********************************" << endl;
	fstream fwrite_DL("BKtreeDic3.txt");

	for (int i = 0; i < size; i++)
	{
		Node tmp = Node(dictionary[i]);
		add(RT, tmp);
	}

	cout << "Root: " << RT.word << ": ";
	fwrite_DL << RT.word << " ";
	for (int i = 0; i < 2 * LEN; i++)
	{
		cout << RT.next[i] << " ";
		fwrite_DL << RT.next[i] << " ";
	}
	fwrite_DL << endl;
	cout << endl;

	for (int i = 1; i < size; i++) {
		cout << tree[i].word << ": ";
		fwrite_DL << tree[i].word << " ";
		for (int j = 0; j < 2 * LEN; j++)
		{
			cout << tree[i].next[j] << " ";
			fwrite_DL << tree[i].next[j] << " ";
		}
		cout << endl;
		fwrite_DL << endl;
	}

	system("pause");
	return 0;
}

int EditDistance(string X, string Y) {
	int n = X.length();
	int m = Y.length();
	int Deletion, Insertion, Substitution;

	int **D;

	//創一個二微陣列D[i + 1][j + 1]
	D = new int* [n + 1];
	for (int i = 0; i < n + 1; i++)
	{
		D[i] = new int[m + 1];
	}

	//初始化第一行和第一列
	for (int i = 0; i < n + 1; i++)
	{
		D[i][0] = i;
	}
	for (int i = 0; i < m + 1; i++)
	{
		D[0][i] = i;
	}

	//比對並計算Distance
	for (int i = 1; i < n + 1; i++)
	{
		for (int j = 1; j < m + 1; j++)
		{
			Deletion = D[i - 1][j] + 1;
			Insertion = D[i][j - 1] + 1;

			if (X[i-1] == Y[j-1]) //兩個字相同
			{
				Substitution = D[i - 1][j - 1];
			}
			else //兩個字不同，substitution設為1 
			{
				Substitution = D[i - 1][j - 1] + 1;
			}

			//取最小
			D[i][j] = min(Deletion, min(Insertion, Substitution));
		}
	}

	return D[n][m];
}

int levenshtein_EditDistance(string X, string Y) {
	int n = X.length();
	int m = Y.length();
	int Deletion, Insertion, Substitution;

	int** D;

	//創一個二微陣列D[i + 1][j + 1]
	D = new int* [n + 1];
	for (int i = 0; i < n + 1; i++)
	{
		D[i] = new int[m + 1];
	}

	//初始化第一行和第一列
	for (int i = 0; i < n + 1; i++)
	{
		D[i][0] = i;
	}
	for (int i = 0; i < m + 1; i++)
	{
		D[0][i] = i;
	}

	//比對並計算Distance
	for (int i = 1; i < n + 1; i++)
	{
		for (int j = 1; j < m + 1; j++)
		{
			Deletion = D[i - 1][j] + 1;
			Insertion = D[i][j - 1] + 1;

			if (X[i - 1] == Y[j - 1]) //兩個字相同
			{
				Substitution = D[i - 1][j - 1];
			}
			else //兩個字不同，substitution設為1 
			{
				Substitution = D[i - 1][j - 1] + 2;
			}

			//取最小
			D[i][j] = min(Deletion, min(Insertion, Substitution));
		}
	}

	return D[n][m];
}

int damerau_levenshtein_distance(string X, string Y)
{
	int n = X.length();
	int m = Y.length();
	int Deletion, Insertion, Substitution;
	int** D;
	//創一個二微陣列D[i + 1][j + 1]
	D = new int* [n + 1];
	for (int i = 0; i < n + 1; i++)
	{
		D[i] = new int[m + 1];
	}

	//初始化第一行和第一列
	for (int i = 0; i < n + 1; i++)
	{
		D[i][0] = i;
	}
	for (int i = 0; i < m + 1; i++)
	{
		D[0][i] = i;
	}

	//比對並計算Distance
	for (int i = 1; i < n + 1; i++)
	{
		for (int j = 1; j < m + 1; j++)
		{
			Deletion = D[i - 1][j] + 1;
			Insertion = D[i][j - 1] + 1;

			if (X[i - 1] == Y[j - 1]) //兩個字相同
			{
				Substitution = D[i - 1][j - 1];
			}
			else //兩個字不同，substitution設為1 
			{
				Substitution = D[i - 1][j - 1] + 1;
			}

			//取最小
			D[i][j] = min(Deletion, min(Insertion, Substitution));

			if ((i > 1) && (j > 1) && (X[i] == Y[j - 1]) && (X[i - 1] == Y[j]))
			{
				D[i][j] = min(D[i][j], D[i - 2][j - 2] + 1);
			}
		}
	}
	return D[n][m];
}

// adds curr Node to the tree 
void add(Node& root, Node& curr)
{
	if (root.word == "")
	{
		// if it is the first Node 
		// then make it the root Node 
		root = curr;
		return;
	}

	// get its editDist from the Root Node 
	if (determine == 0){
		dist = EditDistance(curr.word, root.word);
	}
	else if (determine == 1){
		dist = levenshtein_EditDistance(curr.word, root.word);
	}
	else if (determine == 2){
		dist = damerau_levenshtein_distance(curr.word, root.word);
	}

	if (tree[root.next[dist]].word == "")
	{
		/* if no Node exists at this dist from root
		 * make it child of root Node*/

		 // incrementing the pointer for curr Node 
		ptr++;

		// adding curr Node to the tree 
		tree[ptr] = curr;
		// curr as child of root Node 
		root.next[dist] = ptr;
	}
	else
	{
		// recursively find the parent for curr Node 
		add(tree[root.next[dist]], curr);
	}
}

vector <string> getSimilarWords(Node& root, string& s)
{
	vector < string > ret;
	if (root.word == "")
		return ret;

	// calculating editdistance of s from root 
	if (determine == 0)	{
		dist = EditDistance(root.word, s);
	}
	else if (determine == 1)	{
		dist = levenshtein_EditDistance(root.word, s);
	}
	else if (determine == 2)	{
		dist = damerau_levenshtein_distance(root.word, s);
	}

	// if dist is less than tolerance value 
	// add it to similar words 
	if (dist <= TOL) ret.push_back(root.word);

	// iterate over the string havinng tolerane 
	// in range (dist-TOL , dist+TOL) 
	int start = dist - TOL;
	if (start < 0)
		start = 1;

	while (start < dist + TOL)
	{
		vector <string> tmp = getSimilarWords(tree[root.next[start]], s);
		for (auto i : tmp)
			ret.push_back(i);
		start++;
	}
	return ret;
}