#include <iostream>
#include <memory.h>
#include <vector>
#include <algorithm>

using namespace std;

#define MAXPLATE 3

class  node
{
public:
	int value;
	char data[3][MAXPLATE];
	class node *parent;
	vector<class node*> children;

	~node(){};
	node(class node *p, char a[3][3]){
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				data[i][j] = a[i][j];
		parent = p;
	};
	int setvalue(char goal[3][3]){
		int v1 = 0, v2 = 0;
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				if (data[i][j] != goal[i][j]) v1++;
		class node *temp = this;
		for (v2 = 1; temp->parent != NULL; temp = temp->parent, v2++);
		return value = v1 + v2;
	}
	bool const matching(const char dd[3][3])const{
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				if (data[3][3] != dd[i][j]) return false;
		return true;
	};

	void addchild(vector<class node*> ch){ children = ch; }
	void addchild(class node *ch){
		children.push_back(ch);
	}
	void addchild(char ch[3][3]){
		class node *temp = new node(this, ch);
		children.push_back(temp);
	}
	void print(bool flg){
		cout << "(" << value << ")";
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 3; ++j){
				cout << (int)data[i][j];
			}
			cout << "\t";
		}
		if (flg == true){
			vector<class node*>::iterator itr = children.begin();

			cout << "<<<< children : ";
			for (; itr != children.end(); itr++)
				(*itr)->print(flg);
			cout << ("\t\t >>>> \n");
		}
	}
	bool findPlate(int *c, int l){
		for (*c = MAXPLATE - 1; (*c) >= 0; --(*c))
			if (data[l][*c] != 0) break;
		if (*c < 0){
			*c = 0;
			return false;
		}
		return true;
	}

	bool MOVE(int src, int dest){
		int c, b;
		if (src == dest) return false;
		if (findPlate(&c, src) == false) return false;
		findPlate(&b, dest);

		if (data[dest][b] != 0 && data[dest][b] < data[src][c]) return false;
		if (data[dest][b] != 0) b++;
		data[dest][b] = data[src][c];
		data[src][c] = 0;

		return true;
	}

	vector<class node*>* genchild(char goal[3][3],
		vector<class node*> openL,
		vector<class node*> closeL){
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 3; ++j){
				class node *temp = new node(this, data);
				bool ret;
				ret = temp->MOVE(i, j);
				if (ret == true){
					bool flg = true;
					vector<class node*>::iterator itr = openL.begin();
					vector<class node*>::iterator itrEnd = openL.end();
					for (; itr != itrEnd; ++itr){
						if ((*itr)->matching(temp->data) == true){
							flg = false;
							break;
						}
					}
					vector<class node*>::iterator Citr = closeL.begin();
					vector<class node*>::iterator CitrEnd = closeL.end();
					for (; flg == true && Citr != CitrEnd; ++Citr){
						if ((*Citr)->matching(temp->data) == true){
							flg = false;
							break;
						}
					}
					if (flg == true){
						temp->setvalue(goal);
						temp->print(false);
						addchild(temp);
					}
				}
			}
		}
		return &children;
	}
};

bool gtt(class node *a, class node *b){
	return a->value < b->value;
}

vector<class node*> OPEN;
vector<class node*> CLOSE;

int main(void){
	char init[3][3] = { { 3, 2, 1 }, { 0, 0, 0 }, { 0, 0, 0 } };
	char goal[3][3] = { { 0, 0, 0 }, { 3, 2, 1 }, { 0, 0, 0 } };
	class node *root = new node((class node*)NULL, init);
	vector<class node> temp;

	root->setvalue(goal);
	root->print(true);

	OPEN.push_back(root);
	for (int step = 0; OPEN.size() != 0; ++step){
		class node *itrBegin = *(OPEN.begin());
		OPEN.erase(OPEN.begin());
		CLOSE.push_back(itrBegin);
		itrBegin->print(false);
		cout << "step : " << step << "OEPN size: " << OPEN.size();
		cout << "CLOSE size: " << CLOSE.size() << endl;
		if (itrBegin->matching(goal) == true){
			cout << "Found!!\n";
			class node *temp = itrBegin;
			for (; temp != NULL; temp = temp->parent){
				temp->print(false);
				cout << endl;
			}
			break;
		}
		vector<class node*> *Achild = itrBegin->genchild(goal, OPEN, CLOSE);
		vector<class node*>::iterator Achirt = (*Achild).begin();
		for (; Achirt != Achild->end(); ++Achirt){
			OPEN.push_back(*Achirt);
		}
		//sort(OPEN.begin(), OPEN.end(), gtt);
		cout << "-----\n";
	}
}
