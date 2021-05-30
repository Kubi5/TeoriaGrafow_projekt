#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;
fstream is("matrix.txt");
int MAX = 100000;

class Element{
public:
   Element *next;
    int data;
};

class Graph{
private:
    Element *start;
    Element *end;
public:
    friend class Element;

    int **floats;
    int *predecessors;
    int *CFP;
    int residual_cap;
    int node_amount;
    int start_index;
    int end_index;
    int x,y;   //nr wierzchołków połączonych krawędzią
    bool exit;

    int max_path;
    int **final_matrix;

    Graph();
    ~Graph();
    int front();
    bool empty() {return !start;}
    void push(int val);
    void del();
    void algo();
    istream& read(istream &is);
    ostream&  write(ostream &os);

};

Graph::Graph(){
    start = end = nullptr;
}

Graph::~Graph(){
    while(start != nullptr){
        del();
    }
}

istream& Graph::read(istream &is){
    string s,w;
    char trash;
    getline(is, s);
    replace(s.begin(), s.end(), ',', ' ');
        for(int i=0;i<s.length();i++){
            if(s[i] == ']' || s[i] == '['){
              node_amount++;
            }
        }
        node_amount = node_amount/2;
        floats = new int* [node_amount];
        for(int i=0;i<node_amount;i++){
            floats[i] = new int [node_amount];
        }

        int k=0;
        for(int i=0; i<node_amount; i++){
            int j=0;
            if(s[k] == '['){
                k++;
                while(s[k] != ']'){
                    if(s[k] != ' '){
                        floats[i][j] = s[k]-48;
                        j++;
                    }
                    k++;
                }
            }
            k++;
        }


 return is;

}

ostream& Graph::write(ostream& os) {
    for (int i = 0; i < node_amount; i++) {
        for (int j = 0; j < node_amount; j++) {
            if(floats[i][j]) os<<i<<"->"<<j<<" "<<final_matrix[i][j]<<":"<<floats[i][j]<<endl;
        }

    }
        os << endl;
        os << "MAX dla tego kanalu to: " << max_path<<endl;
        //cout<<endl<<final_matrix[0][0];
        return os;

}

void Graph::algo(){
    read(is);
    start_index = 0;
    end_index = 3;
    max_path = 0;
    final_matrix = new int* [node_amount];
    for(int i=0; i<node_amount; i++){
        final_matrix[i] = new int [node_amount];
    }
    predecessors = new int [node_amount];
    CFP = new int [node_amount];
    for(int i=0; i<node_amount; i++){
        for(int j=0; j<node_amount; j++){
            final_matrix[i][j] = 0;
        }
    }
    while(true) {
        for (int i = 0; i < node_amount; i++) {
            predecessors[i] = -1;
        }
        predecessors[start_index] = -2;
        CFP[start_index] = MAX;

        while (empty() == 0) {
            del();
        }
        push(start_index);

        exit = false;

        while (empty() == 0) {
            x = front();

            del();
            for (y = 0; y < node_amount; y++) {
                residual_cap = floats[x][y] - final_matrix[x][y];
                if (residual_cap && (predecessors[y] == -1)) {
                    predecessors[y] = x;
                    CFP[y] = CFP[x] > residual_cap ? residual_cap : CFP[x];
                    if (y == end_index) {
                        max_path += CFP[end_index];
                        int i = y;
                        while (i != start_index) {
                            x = predecessors[i];
                            final_matrix[x][i] += CFP[end_index];
                            final_matrix[i][x] -= CFP[end_index];
                            i = x;
                        }
                        exit = true;
                        break;
                    }
                    push(y);
                }

            }
            if (exit) break;
        }
        if(!exit) break;
    }

    write(cout);
}



int Graph::front(){
    if(start) return start-> data;
    else return 101;
}

void Graph::push(int val){
    Element *n = new Element;
    n->next = nullptr;
    n->data = val;
    if(end) end->next = n;
    else start =n;
    end = n;
}

void Graph::del(){
    if(start){
        Element *n = start;
        start = start->next;
        if( !end ) end = nullptr;
        delete n;
    }
}


int main(){
    Graph g;
    g.algo();


}