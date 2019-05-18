#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <iomanip>
#include <algorithm>

using namespace std;
typedef pair<int, int> TElement;
struct TResult {
	int v, k, item;
	vector<int> middleRow;
};
int W, N, V;
int k;
vector <TElement> items;

//set for solution. Items in the knapsack
//we use set here because we need sort solution and get only unique items
set<int> kItems;

//Function for Part b) 2.i
//this function finds v and k
//We use only 2 rows of matrix: row1 - previous, row2 - current
//thus algorithm uses only O(w) space
pair<int, int> GenMiddleK(int w2, int n)
{
	int k = 0;
	int start = 0;
	int middle = (start + n) / 2;
	vector<int> row1(W + 1);
	vector<int> row2(W + 1);
	vector<int> capacity1(W + 1), capacity2(W + 1);
	for (int j = start + 1; j <= n; ++j) {
		for (int i = 1; i <= W; ++i) {
			int val1 = row1[i];
			if (i >= items[j].first) {
				int val2 = row1[i - items[j].first];
				//for last half items we compute capacity vector
				if (j > middle)
					capacity2[i] = val1 < val2 + items[j].second ? capacity1[i - items[j].first] + items[j].first : capacity1[i];
				row2[i] = max(val1, val2 + items[j].second);
			}
			else
				row2[i] = val1;
		}
		row1.swap(row2);
		capacity1.swap(capacity2);
	}
	int v = row1[w2];
	k = w2 - capacity1[w2];
	return make_pair(v, k);
}

//Finctions for Part b) 2.ii
//iteration counter for proof time O(N*W)
int iterations = 0;
TResult GenMatrix1(int w1, int w2, int start, int n, vector<int> row1)
{
	int k = 0;
	TResult result;
	int middle = (start + n) / 2;
	vector<int> row2(W + 1);
	vector<int> capacity1(W + 1), capacity2(W + 1);
	for (int j = start + 1; j <= n; ++j) {
		for (int i = w1 + 1; i <= w2; ++i) {
			++iterations;//for proof time = O(N*W)
			int val1 = row1[i];
			if (i >= items[j].first) {
				int val2 = row1[i - items[j].first];
				if (j > middle) {
					capacity2[i] = val1 < val2 + items[j].second ?
						capacity1[i - items[j].first] + items[j].first :
						capacity1[i];
				}
				row2[i] = max(val1, val2 + items[j].second);
			}
			else
				row2[i] = val1;

		}
		if (j == middle)result.middleRow = row2;
		row1.swap(row2);
		capacity1.swap(capacity2);
	}
	//check and find solution item
	int item = row1[w2] == row2[w2] ? 0 : n;
	result.item = item;
	//compute new k
	result.k = w2 - capacity1[w2];
	//get value for new k but we don't use it
	result.v = row1[w2];
	row1.swap(row2);
	return result;
}
//Find solution and save it in kItems
void FindPath(int w1, int n1, int w2, int n2, vector<int>* rowStart)
{
	auto r = GenMatrix1(w1, w2, n1, n2, *rowStart);
	//If we found solution item we save it
	if (r.item > 0)
		kItems.insert(r.item);
	//Stop recursion
	if (n2 - n1 == 1)
		return;

	int middle = (n1 + n2) / 2;

	FindPath(w1, n1, r.k, middle, rowStart);
	//Stop recursion
	if (n2 - middle == 1)return;
	FindPath(r.k - 1, middle, w2, n2, &r.middleRow);
}

void Save(const string& fname)
{
	ofstream f2(fname);
	f2 << "V " << V << endl;
	f2 << "i " << kItems.size() << endl;
	for (int el : kItems)
		f2 << el << endl;
}

int main(int argc, char** argv)
{
	if (argc < 2) { cout << "Enter file name" << endl; return 0; }
	string fname = argv[1];
	string fnameOutput(fname);
	fnameOutput.insert(fname.length() - 4, "-output");
	ifstream f1(fname);
	string tmp;
	f1 >> tmp;//W
	f1 >> W;
	f1 >> tmp;//n
	f1 >> N;
	items.emplace_back(0, 0);
	for (int i = 0; i < N; ++i)
	{
		int w, v;
		f1 >> tmp;//w
		f1 >> w;
		f1 >> tmp;//v
		f1 >> v;
		items.emplace_back(w, v);
	}
	auto result = GenMiddleK(W, N);
	V = result.first;
	cout << "We save solution in the file " << fnameOutput << endl;
	cout << "Part b) 2i." << endl << "V=" << V << "  k=" << result.second << endl;
	cout << "Part b) 2ii." << endl;
	cout << "Find solution" << endl;
	//first row of our virtual matrix 
	vector<int> row(W + 1);
	FindPath(0, 0, W, N, &row);
	cout << "iterations/(N*W)=" << 1.0*iterations/(N * W)<< endl;
	//Save solution into the file 
	Save(fnameOutput);
	//Show solution
	for (int el : kItems)
		cout << el << endl;

	return 0;
}