// Sudoku
// UVa ID: 1309
// Verdict: Accepted
// Submission Date: 2019-08-14
// UVa Run Time: 0.240S
//
// 版权所有（C）2019，邱秋。metaphysis # yeah dot net
//
// 在program1.cpp的基础上优化，参考：https://www.cnblogs.com/grenet/p/3163550.html。

#include <bits/stdc++.h>

using namespace std;

const int MAXV = 100000;

struct node { int L, R, U, D, tag, ch, rh; } DL[1000010];
int S[2048], O[2048], head, idx, n;
char grid[20][20];

void cover(int c)
{
	DL[DL[c].R].L = DL[c].L;
    DL[DL[c].L].R = DL[c].R;
	for (int i = DL[c].D; i != c; i = DL[i].D)
		for (int j = DL[i].R; j != i; j = DL[j].R)
		{
			DL[DL[j].D].U = DL[j].U;
			DL[DL[j].U].D = DL[j].D;
			S[DL[j].ch]--;
		}
}

void uncover(int c)
{
	for (int i = DL[c].D; i != c; i = DL[i].D)
		for (int j = DL[i].L; j != i; j = DL[j].L)
		{
			DL[DL[j].D].U = j;
			DL[DL[j].U].D = j;
			S[DL[j].ch]++;
		}
	DL[DL[c].R].L = DL[DL[c].L].R = c;
}

void render(int k)
{
	int cache[512];
	for (int i = 0; i < k; i++)
		cache[DL[O[i]].tag] = DL[O[i]].rh;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		    if (grid[i][j] == '-') cout << (char)(cache[i * 16 + j] + 'A');
		    else cout << grid[i][j];
		cout << '\n';
	}
}

bool dfs(int k)
{
	if (DL[head].R == head)	{ render(k); return true; }
	int c;
	for (int i = DL[head].R, tmp = MAXV; i != head; i = DL[i].R)
		if (S[i] < tmp)
			tmp = S[i], c = i;
	cover(c);
	for (int i = DL[c].D; i != c; i = DL[i].D)
	{
		O[k] = i;
		for (int j = DL[i].R; j != i; j = DL[j].R) cover(DL[j].ch);
		if (dfs(k + 1)) return true;
		for (int j = DL[i].L; j != i; j = DL[j].L) uncover(DL[j].ch);
	}
	uncover(c);
	return false;
}

int addNode(int U, int D, int L, int R)
{
	DL[idx].U = U, DL[idx].D = D, DL[idx].L = L, DL[idx].R = R;
	DL[U].D = DL[D].U = DL[L].R = DL[R].L = idx;
	return idx++;
}

void addRow(int tot, int cln[], int rh, int tag)
{
	for (int i = 0, c, row = 0, next; i < tot; i++)
	{
		c = cln[i];
		DL[idx].ch = c, DL[idx].tag = tag, S[c]++;
		if (!row)
		{
			row = addNode(DL[DL[c].ch].U, DL[c].ch, idx, idx);
			DL[row].rh = rh;
		}
		else
		{
			next = addNode(DL[DL[c].ch].U, DL[c].ch, DL[row].L, row);
			DL[next].rh = rh;
		}
	}
}

void initialize(int m)
{
	idx = 0;
	head = addNode(0, 0, 0, 0);
	for (int i = 1; i <= m; i++)
	{
		addNode(i, i, DL[head].L, head);
		DL[i].ch = i, S[i] = 0;
	}
}

int main(int argc, char *argv[])
{
    cin.tie(0), cout.tie(0), ios::sync_with_stdio(false);

	int cln[8], cases = 0, marked[2048];
	char letter;

	while (cin >> letter)
	{
	    cin.putback(letter);
		n = 16;
		initialize(4 * n * n);
		memset(marked, 0, sizeof(marked));
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
			{
			    cin >> grid[i][j];
				if (grid[i][j] != '-')
				{
					int c = grid[i][j] - 'A';
					marked[i * n + j + 1] = 1;
					marked[n * n + i * n + c + 1] = 1;
					marked[2 * n * n + j * n + c + 1] = 1;
					marked[3 * n * n + (i / 4 * 4 + j / 4) * n + c + 1] = 1;
				}
			}
		for (int i = 0; i < n; i++)
		    for (int j = 0; j < n; j++)
		        if (grid[i][j] == '-')
		            for (int c = 0; c < n; c++)
					{
						int tot = 0;
						cln[tot++] = i * n + j + 1;
						cln[tot++] = n * n + i * n + c + 1;
						cln[tot++] = 2 * n * n + j * n + c + 1;
						cln[tot++] = 3 * n * n + (i / 4 * 4 + j / 4) * n + c + 1;
						int unmarked = 1;
						for (int k = 0; k < tot; k++)
						    if (marked[cln[k]])
						    {
						        unmarked = 0;
						        break;
						    }
				        if (unmarked) addRow(tot, cln, c, i * n + j);
					}
		for (int i = 0; i <= 4 * n * n; i++)
		    if (marked[i])
		        cover(i);
		if (cases++) cout << '\n';
		dfs(0);
	}
	return 0;
}
