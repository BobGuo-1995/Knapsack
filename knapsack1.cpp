#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <iomanip>
#include <algorithm>


using namespace std;


typedef pair<int,int> TElement; 
class Matrix{
    vector<int> data;
    int rows;
    int columns;
    

//This is simple class for Matrix. The goal is a single memory allocating.
//We can do this as vector<vector<int>> but this class is better
public:
    Matrix(){}
    Matrix(int rows,int columns)
    {
        Resize(rows,columns);
    }
    //Memory allocating
    void Resize(int rows,int columns)
    {
        this->rows=rows;
        this->columns=columns;
        data.resize((rows+1)*(columns+1));
    }
// The Get and setter does what its name does. get the index from the matrix and set will set it with a value of the item
// value = Get(row, column) as m[row,column]
    int Get(int i, int j)
    {
        return data[i*columns+j];
    }
//m[row,column]=val
    void Set(int i,int j, int val)
    {
        data[i*columns+j]=val;
    }
};



//Global variable
int W,N;
vector <TElement> items;
Matrix m;
//vector for solution. Items in the knapsack
vector<int> kItems;

// This will solve our PartA.1) and the Findsolution function will backtrack and return the optimal solution. 
void GenMatrix(int w,int n)
{
    m.Resize(w+1,n+1);
    for(int i=1;i<=w;++i){
        for(int j=1;j<=n;++j){
         int val1=m.Get(i,j-1);
            if (i>=items[j].first)
            {
                int val2=m.Get(i-items[j].first,j-1);
                int val3=items[j].second;
                m.Set(i,j,max(val1,val2+val3));
            }
         else
             m.Set(i,j,val1);
        }
    }
}

void FindSolution(int w,int n)
{
    if(m.Get(w,n)==0)return;
    if(m.Get(w,n)==m.Get(w,n-1))
        FindSolution(w,n-1);
    else{
        FindSolution(w-items[n].first,n-1);
        kItems.push_back(n);
    }
}

//We use only 2 rows of matrix: row1 - previous, row2 - current
//thus algorithm uses only O(w) space
int GenMatrix1(int w, int n)
{
    vector<int> row1(w+1);
    vector<int> row2(w+1);
    for(int j=1;j<=n;++j){
    for(int i=1;i<=w;++i)
         if (i>=items[j].first)
             row2[i]=max(row1[i],row1[i-items[j].first]+items[j].second);
         else
             row2[i]=row1[i];
        row1.swap(row2);
    }
    return row1[w];
}

//for debug
void SaveMatrix()
{
    ofstream f2("m.txt");
    
    for(int i=0;i<=W;++i){
     for(int j=0;j<=N;++j)
     {
         f2.width(5);
         f2<<m.Get(i,j);
    }
     f2<<endl;
    }
}
void Save(const string& fname)
{
    ofstream f2(fname);
    f2<<"V "<<m.Get(W,N)<<endl;
    f2<<"i "<<kItems.size()<<endl;
     for(int el:kItems)
            f2<<el<<endl;
}
int main(int argc, char** argv)
{    
    if (argc<2){cout<<"Enter file name"<<endl;return 0;}
    string fname=argv[1];
    string fnameOutput(fname);
    fnameOutput.insert(fname.length()-4,"-output");
    ifstream f1(fname);
    string tmp;
    f1>>tmp;//W
    f1>>W;
    f1>>tmp;//n
    f1>>N;
    items.emplace_back(0,0);
    for(int i=0;i<N;++i)
    {
        int w,v;
        f1>>tmp;//w
        f1>>w;
        f1>>tmp;//v
        f1>>v;
        items.emplace_back(w,v);
    }
      
        GenMatrix(W,N);
        //SaveMatrix();
        cout<<"We save solution in the file "<<fnameOutput<<endl;
        cout<<"Part a) 1."<<endl<<"V="<<m.Get(W,N)<<endl;
        FindSolution(W,N);
        Save(fnameOutput);
        for(int el:kItems)
            cout<<el<<endl;
        cout<<"Part a) 2."<<endl<<"V="<< GenMatrix1(W,N)<<endl;
    return 0;
}