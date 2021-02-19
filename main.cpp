#include<bits/stdc++.h>
using namespace std;

#define sz(a) (int)(a).size()

int n = 2;
struct node{
	int val[2];
	node* ptr[3];
	node() : val{INT_MAX, INT_MAX}, ptr{NULL, NULL, NULL} { }
};

enum query_type{
    INSERT,
	FIND,
	COUNT,
	RANGE
};
query_type get_type(string &query){
	if(query == "INSERT") return INSERT;
	if(query == "FIND") return FIND;
	if(query == "COUNT") return COUNT;
	if(query == "RANGE") return RANGE;
}

pair<int, node*> branch_insert(node* root, int val);

// To check if the node root is leaf or not
int check_leaf(node* root)
{
	int flag = 1;
	for(int i = 0; i < n; i++)
		if(root->ptr[i]!=NULL)
			flag = 0;
	return flag;
}

void assemble_node(node* root, int val, node* ptr, vector<int>& vals, vector<node*>& ptrs)
{
	int insert = 0;
	int i;
	for(i=0;i<n && root->val[i] != INT_MAX;i++)
	{
		int flag = 0;
		if(!insert){
			if(i==0)
			{
				if(val < root->val[i]){
					insert = 1;
					flag = 1;
				}
			}
			else{
				if(val >= root->val[i-1] && val < root->val[i]){
					insert = 1;
					flag = 1;
				}
			}
		}
		if(flag)
			vals.push_back(val);

		vals.push_back(root->val[i]);
		ptrs.push_back(root->ptr[i]);

		if(flag)
			ptrs.push_back(ptr);
	}
	ptrs.push_back(root->ptr[i]);
	if(!insert)
	{
		vals.push_back(val);
		ptrs.push_back(ptr);
	}
	return;
}

// Recursice function to get to the node with value as val
node* traverse_node(node* root, int val)
{
	// base condition
	if(check_leaf(root))
		return root;

	for(int i = 0; i <= n; i++)
		if(i == n || root->val[i] >= val)
			return traverse_node(root->ptr[i], val);
}

pair<int, node*> node_overflow(node* root, vector<int>& vals, vector<node*>& ptrs)
{
	bool leaf_node = check_leaf(root);
	int node_size = sz(vals);

	node* link = root->ptr[n];

	for(int i = 0; i < node_size/2; i++)
	{
		root->val[i] = vals[i];
		root->ptr[i] = ptrs[i];
	}
	for(int i = node_size/2; i<n; i++)
	{
		root->val[i] = INT_MAX;
		root->ptr[i] = NULL;
	}
	root->ptr[n] = NULL;

	int j = node_size/2;
	node* in_node = new node;
	if(leaf_node)
		root->ptr[n] = in_node;
	else
	{
		root->ptr[j] = ptrs[j];
		j++;
	}

	if(leaf_node)
		for(int i = j; i < node_size; i++)
			in_node->val[i-j] = vals[i];	
	else{
		for(int i = j; i < node_size; i++)
		{
			in_node->val[i-j] = vals[i];
			in_node->ptr[i-j] = ptrs[i];
		}
		in_node->ptr[node_size-j] = ptrs[node_size];
	}

	if(leaf_node){
		for(int i=0;i<n;i++){
			root->ptr[i] = NULL;
			in_node->ptr[i] = NULL;
		}
		in_node->ptr[n] = link;
	}
	return make_pair(vals[node_size/2], in_node);
}

pair<int, node*> node_normal(node* root, vector<int>& vals, vector<node*>& ptrs)
{
	int size = sz(vals);
	if(check_leaf(root)){
		for(int i=0; i < size; i++)	
			root->val[i] = vals[i];
		return make_pair(INT_MAX, nullptr);
	}
	for(int i=0; i < size; i++)
	{
		root->val[i] = vals[i];
		root->ptr[i] = ptrs[i];
	}
	root->ptr[size] = ptrs[size];
	return make_pair(INT_MAX, nullptr);
}

pair<int, node*> insertion_trace(node* root, int val)
{
	if(check_leaf(root))
		return make_pair(val, nullptr);
	if(val < root->val[0])
		return branch_insert(root->ptr[0], val);
	for(int i = 1; i < n; i++)
		if(val >= root->val[i-1] && val < root->val[i])
			return branch_insert(root->ptr[i], val);
	if(val >= root->val[n-1])
		return branch_insert(root->ptr[n], val);
}

pair<int, node*> branch_insert(node* root, int val)
{
	vector<node*> ptrs;
	vector<int> vals;

	pair<int, node*> insert_element = insertion_trace(root, val);
	int insert_value = insert_element.first;

	if(insert_value >= INT_MAX)
		return insert_element;
	
	assemble_node(root, insert_element.first, insert_element.second, vals, ptrs);
	int node_size = sz(vals);

	if(node_size > n)
		return node_overflow(root, vals, ptrs);
	return node_normal(root, vals, ptrs);
}

node* insert(node*& root, int val)
{
	pair<int, node*> insert_element = branch_insert(root, val);
	int insert_value = insert_element.first;
	node* child = insert_element.second;

	if(insert_value >= INT_MAX)
		return root;

	// for a new root
	node* new_root = new node;
	new_root->ptr[0] = root;
	new_root->ptr[1] = child;
	new_root->val[0] = insert_value;
	return new_root;
}

int count(node*& root, int val)
{
	if(root == NULL)
		return 0;

	node* start_node = traverse_node(root, val);

	int count = 0;
	while(start_node!=NULL && start_node->val[0] <= val)
	{
		for(int i = 0; i < n; i++){
			if(start_node->val[i] == val)
				count++;
		}
		start_node = start_node->ptr[n];
	}
	return count;
}

bool find(node*& root, int val)
{
	if(root == NULL)
		return false;

	for(int i = 0; i <= n; i++)
	{
		if(i!=n && root->val[i] == val)
			return true;
		else if(i == n || root->val[i] > val)
			return find(root->ptr[i], val);
	}
}

int range(node*& root, int start, int end)
{
	if(root == NULL)
		return 0;
		
	node* start_node = traverse_node(root, start);

	int count = 0;
	while(start_node != NULL && start_node->val[0] <= end)
	{
		for(int i = 0;i<n; i++){
			if(start_node->val[i] >= start && start_node->val[i] <= end)
				count++;
		}
		start_node = start_node->ptr[n];
	}
	return count;
}

// courtesy: Geeks for Geeks
vector<string> tokenizer(string query)
{
	vector<string> tokens;
	stringstream check1(query);
    string intermediate;
	while(getline(check1, intermediate, ' '))
        tokens.push_back(intermediate);
    return tokens;
}

void process_query(node*& root, string query_string)
{
	vector<string> query = tokenizer(query_string);
	if(sz(query) == 0)
	{
		cout << "Query Empty\n";
		return;
	}
	switch(get_type(query[0])) {
		case INSERT:
		{
			if(sz(query) != 2){
				cout << "INCORRECT NUMBER OF ARGUMENTS\n";
				return;
			}
			int X = stoi(query[1]);
			root = insert(root, X);
			break;
		}
		case FIND:
		{
			if(sz(query) != 2){
				cout << "INCORRECT NUMBER OF ARGUMENTS\n";
				return;
			}
			int X = stoi(query[1]);
			bool found = find(root, X);
			found? cout << "YES\n": cout << "NO\n";
			break;
		}
		case COUNT:
		{
			if(sz(query) != 2){
				cout << "INCORRECT NUMBER OF ARGUMENTS\n";
				return;
			}
			int X = stoi(query[1]);
			cout << count(root, X) << "\n";
			break;
		}
		case RANGE:
		{
			if(sz(query) != 3){
				cout << "INCORRECT NUMBER OF ARGUMENTS\n";
				return;
			}
			int X = stoi(query[1]), Y = stoi(query[2]);
			cout << range(root, X, Y) << "\n";
			break;
		}
	}
}

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		cout << "Incorrect Usage. Format is ./a.out <filename>\n";
		return 0;
	}

	string filename = argv[1];
	ifstream in;
	in.open(filename);
	if(!in)
	{
		cout << "File Open Error\n";
		return 0;
	}

	node *root = new node;
	string query;
	while(getline(in, query))
		process_query(root, query);
	in.close();

	return 0;
}