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
	int distance;
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
int determine = 0;

vector <string> dictionary;
vector <string> test;
vector <string> fixing;

int EditDistance(string X, string Y);
int levenshtein_EditDistance(string X, string Y);
int damerau_levenshtein_distance(string p_string1, string p_string2);
vector <string> getSimilarWords(Node& root, string& s);


int main() {
	fstream testfile("test1.txt");
	string temp;
	Node TreeTemp;
	int size;
	ptr = 1;
	int wrong_word = 0;
	bool wrong = false;
	int fix_word = 0;


	//文章
	while (!testfile.eof())
	{
		testfile >> temp;
		test.push_back(temp);
	}
	testfile.close();

	for (int i = 0; i < test.size(); i++)
	{
		fixing.push_back(test[i]);
	}

	cout << "容忍值為: " << TOL << endl;

	/*****************************傳統 Edit Distance****************************/
	determine = 0;
	cout << "***************************傳統 Edit Distance********************************" << endl;
	fstream fin("BKtreeDic1.txt");
	//Root
	fin >> RT.word;
	for (int i = 0; i < 2 * LEN; i++)
	{
		fin >> RT.next[i];
	}

	//Node
	while (!fin.eof())
	{
		fin >> tree[ptr].word;
		for (int i = 0; i < 2 * LEN; i++)
		{
			fin >> tree[ptr].next[i];
		}	
		ptr++;
	}
	size = ptr - 1;

	//比對文章
	fstream textFixed("test1_fixed.txt");
	bool word_in_dict = false;

	for (int i = 0; i < test.size(); i++)
	{
		for (int k = 0; k < size; k++)
		{
			if (test[i] == tree[k].word)
			{
				wrong = true;
			}
		}
		if (!wrong)
		{
			wrong_word++;
		}
		wrong = false;

		vector < string > match = getSimilarWords(RT, test[i]);
		if (match.size() > 1)
		{
			word_in_dict = false;
			for (int j = 0; j < match.size(); j++)
			{
				if (match[j] == test[i])
				{
					word_in_dict = true;
				}
			}

			//修改
			if (!word_in_dict)
			{
				cout << test[i] << "相似: ";
				for (int j = 0; j < match.size(); j++)
					cout << match[j] << " ";
				cout << endl << "	" << fixing[i] << "改成" << match[0] << endl;
				fixing[i] = match[0];
				fix_word++;
			}
		}
		textFixed << fixing[i] << endl;
		match.clear();
	}
	cout << endl << test.size() << "個字中，改錯了" << fix_word << "個字" << "錯誤率為" << double(fix_word) / test.size() * 100 << "%" << endl;
	fin.close();
	textFixed.close();
	fixing.clear();

	/**********************清除tree******************************/
	ptr = 1;
	RT.word = "";
	for (int i = 0; i < 2 * LEN; i++)
	{
		RT.next[i] = 0;
	}
	for (int i = 0; i < size + 1; i++)
	{
		tree[i].word = "";
		for (int j = 0; j < 2 * LEN; j++)
		{
			tree[i].next[j] = 0;
		}
	}

	for (int i = 0; i < test.size(); i++)
	{
		fixing.push_back(test[i]);
	}

	/*******************Levenshtein Edit Distance***********************/
	determine = 1;
	cout << endl << "***************************Levenshtein Edit Distance********************************" << endl;
	fstream fin_Len("BKtreeDic2.txt");
	//Root
	fin_Len >> RT.word;
	for (int i = 0; i < 2 * LEN; i++)
	{
		fin_Len >> RT.next[i];
	}

	//Node
	while (!fin_Len.eof())
	{
		fin_Len >> tree[ptr].word;
		for (int i = 0; i < 2 * LEN; i++)
		{
			fin_Len >> tree[ptr].next[i];
		}
		ptr++;

	}

	size = ptr - 1;
	fix_word = 0;
	//比對文章
	fstream textFixed_Len("test1_fixed_Len.txt");
	word_in_dict = false;

	for (int i = 0; i < test.size(); i++)
	{
		vector < string > match = getSimilarWords(RT, test[i]);
		word_in_dict = false;
		if (match.size() > 1)
		{
			for (int j = 0; j < match.size(); j++)
			{
				if (match[j] == test[i])
				{
					word_in_dict = true;
				}
			}

			//修改
			if (!word_in_dict)
			{
				cout << test[i] << "相似: ";
				for (int j = 0; j < match.size(); j++)
					cout << match[j] << " ";
				cout << endl << "	" << test[i] << "改成" << match[0] << endl;
				fixing[i] = match[0];
				fix_word++;
			}
		}
		textFixed_Len << fixing[i] << endl;
		match.clear();
	}
	cout << endl << test.size() << "個字中，錯了" << fix_word << "個字" << endl << "錯誤率為" << double(fix_word) / test.size() * 100 << "%" << endl;
	fin_Len.close();
	textFixed_Len.close();
	
		/**********************清除tree******************************/
	ptr = 1;
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
	fstream fin_DL("BKtreeDic3.txt");
	//Root
	fin_DL >> RT.word;
	for (int i = 0; i < 2 * LEN; i++)
	{
		fin_DL >> RT.next[i];
	}

	//Node
	while (!fin_DL.eof())
	{
		fin_DL >> tree[ptr].word;
		for (int i = 0; i < 2 * LEN; i++)
		{
			fin_DL >> tree[ptr].next[i];
		}
		ptr++;

	}

	size = ptr - 1;
	fix_word = 0;
	//比對文章
	fstream textFixed_DL("test1_fixed_DL.txt");
	word_in_dict = true;

	for (int i = 0; i < test.size(); i++)
	{
		vector < string > match = getSimilarWords(RT, test[i]);
		if (match.size() > 1)
		{
			word_in_dict = false;
			for (int j = 0; j < match.size(); j++)
			{
				if (match[j] == test[i])
				{
					word_in_dict = true;
				}
			}

			//修改
			if (!word_in_dict)
			{
				cout << test[i] << "相似: ";
				for (int j = 0; j < match.size(); j++)
					cout << match[j] << " ";
				cout << endl << "	" << test[i] << "改成" << match[0] << endl;
				test[i] = match[0];
				fix_word++;
			}
		}
		textFixed_DL << test[i] << endl;
	}
	cout << endl << test.size() << "個字中，錯了" << fix_word << "個字" << endl << "錯誤率為" << double(fix_word) / test.size() * 100 << "%" << endl << endl;


	system("pause");
	return 0;
}

int EditDistance(string X, string Y) {
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
			else //兩個字不同，substitution設為2 
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

vector <string> getSimilarWords(Node& root, string& s)
{
	vector < string > ret;
	if (root.word == "")
		return ret;

	// calculating editdistance of s from root 
	int dist;

	if (determine == 0)
	{
		dist = EditDistance(root.word, s);
	}
	else if (determine == 1) 
	{
		dist = levenshtein_EditDistance(root.word, s);
	}
	else if (determine == 2) 
	{
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