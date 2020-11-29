#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Node {
    int data;
    int height;
    Node* left;
    Node* right;
};

//Height function
int height(Node* root) {
    if ( root == NULL ) {
        return 0;
    } else {
        return root->height;
    }
}

//Check balance function
int balance(Node* root) {
    if ( root == NULL ) {
        return 0;
    } else {
        return height(root->left)-height(root->right);
    }
}

//Function to find the maximum value to the left of the node
Node* find_max_left(Node* root) {
    while ( root->right != NULL ) {
        root = root->right;
    }
    return root;
}

//Search function (used to check if node value chosen for deletion exists)
bool exists = false;
Node* search_node(Node* root, int data) {
    if ( root == NULL ) {
        exists = false;
        return root;
    } else if ( root->data == data ) {
        exists = true;
        return root;
    } else if ( root->data < data ) {
        return search_node( root->right, data );
    } else if ( root->data > data ) {
        return search_node( root->left, data );
    }
}

//New node function
Node* make_new_node(int data) {
    Node* new_node = new Node();
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 1;
    return new_node;
}

//Left rotation rebalancing function
Node* left_rotate(Node* root) {
    Node* rotato = root->right;
    root->right = rotato->left;
    rotato->left = root;

    root->height = max( height(root->left), height(root->right) ) + 1;
    rotato->height = max( height(root->right), root->height ) + 1;
    return rotato;
}

//Right rotation rebalancing function
Node* right_rotate(Node* root) {
    Node* rotato = root->left;
    root->left = rotato->right;
    rotato->right = root;

    root->height = max( height(root->left), height(root->right) ) + 1;
    rotato->height = max( height(root->right), root->height ) + 1;
    return rotato;
}

//Node insertion function
Node* insert_node(Node* root, int data) {
    if ( root == NULL ) {
        return make_new_node(data);
    } else if ( data < root->data) {
        root->left = insert_node(root->left, data);
    } else if ( data > root->data) {
        root->right = insert_node(root->right, data);
    } else {
        return root;
    }

    //Get new height and check balance
    root->height = max( height(root->left), height(root->right) ) + 1;
    int bal = balance(root);

    //Rebalance
    if ( bal > 1 && data < root->left->data ) {
        return right_rotate(root);
    } else if ( bal < -1 && data > root->right->data ) {
        return left_rotate(root);
    } else if ( bal > 1 && data > root->left->data ) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    } else if ( bal < -1 && data < root->right->data ) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

//Node deletion function
Node* delete_node(Node* root, int data) {
    if ( root == NULL ) {
        return root;
    } else if ( data < root->data ) {
        root->left = delete_node(root->left, data);
    } else if ( data > root->data ) {
        root->right = delete_node(root->right, data);
    } else {
        if ( root->left == NULL && root->right == NULL) { //No children
            delete root;
            root = NULL;
        } else if ( root->left == NULL ) { //No left child, one right child
            Node* temp = root;
            root = root->right;
            delete temp;
        } else if ( root->right == NULL ) { //No right child, one left child
            Node* temp = root;
            root = root->left;
            delete temp;
        } else {                            //Two children
            Node* temp = find_max_left(root->left);
            root->data = temp->data;
            root->left = delete_node(root->left, temp->data);
        }
    }

    if ( root == NULL )
        return root;

    //Get new height and check balance
    root->height = max( height(root->left), height(root->right) ) + 1;
    int bal = balance(root);

    //Rebalance
    if ( bal > 1 && balance(root->left) >= 0 ) {
        return right_rotate(root);
    } else if ( bal < -1 && balance(root->right) <= 0 ) {
        return left_rotate(root);
    } else if ( bal > 1 && balance(root->left) < 0 ) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    } else if ( bal < -1 && balance(root->right) > 0 ) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

//Traverse and print in-order
void in_order(Node* root) {
    if ( root == NULL ) {
      return;
    }

	in_order(root->left);
	int outputs = root->data;
	cout << outputs << " ";
	in_order(root->right);
}

//Traverse and print pre-order
void pre_order(Node* root) {
    if ( root == NULL ) {
      return;
    }

	int outputs = root->data;
	cout << outputs << " ";
	pre_order(root->left);
	pre_order(root->right);
}

//Traverse and print post-order
void post_order(Node* root) {
    if ( root == NULL ) {
      return;
    }

	post_order(root->left);
	post_order(root->right);
    int outputs = root->data;
    cout << outputs << " ";
}

//Convert string to int
int string_to_num(string s) {
    int converted_num;
    stringstream sstr(s);
    sstr >> converted_num;

    return converted_num;
}


int main() {
    int input_number;
    string temp_str = "";
    vector<string> inputs;

    Node* root = NULL;

    //While loop to read inputs. Leading A for insertion, leading D for deletion
    while( cin >> temp_str ) {
        inputs.push_back(temp_str);
        if ( temp_str.at(0) == 'A' ) {
            temp_str.erase(0, min(temp_str.find_first_not_of('A'), temp_str.size()-1));
            input_number = string_to_num(temp_str);
            root = insert_node(root, input_number);
        } else if ( temp_str.at(0) == 'D' ) {
            temp_str.erase(0, min(temp_str.find_first_not_of('D'), temp_str.size()-1));
            input_number = string_to_num(temp_str);
//            search_node(root, input_number);
//            if ( exists == true ) {
                root = delete_node(root, input_number);
//            }
        }

        //Choose traversal order based on input IN, PRE or POST
        if ( temp_str.at(0) == 'I') {
            in_order(root);
            break;
        } else if ( temp_str.at(0) == 'P' ) {
            if ( temp_str.at(1) == 'R') {
                pre_order(root);
                break;
            } else if ( temp_str.at(1) == 'O') {
                post_order(root);
                break;
            }
        }

    }

    //If tree is empty
    if (root == NULL) {
        cout << "EMPTY" << endl;
    }

    return 0;

}
