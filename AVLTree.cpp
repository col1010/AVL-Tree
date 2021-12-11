/**
 * Name: Coleby Kauffman
 * Assignment: Project 2 AVL Tree
 * Class: CS-311 Fall 2021
 * Date: 16 November 2021
 **/


#include <iostream>
#include <random>
#include <time.h>

#define GRAPHVIZ

#ifdef GRAPHVIZ
#include<graphviz/cgraph.h>
#include<graphviz/gvc.h>
#endif


using namespace std;

template<class T>
struct node {
    T data;
    node *left, *right, *parent;
};

template<class T>
class AVLTree {
    node<T> *root = 0;

    #ifdef GRAPHVIZ
	void make_image_recur(Agraph_t*, Agnode_t*, node<T>*);
    #endif

    int balanceFactor(node<T> *n) {
        if (!n)
            return 0;
        return (heightFromNode(n->right) - heightFromNode(n->left));
    }

    int maximum(int a, int b){
	    return (a>b)? a:b;
    }

    int heightFromNode(node<T> *n) {
        if(!n)
		    return 0;
        return maximum(heightFromNode(n->left), heightFromNode(n->right)) + 1;
    }

    void printInOrder(node<T> *place) {
        if (place->left)
            printInOrder(place->left);
        cout << place->data << ", ";
        if (place->right)
            printInOrder(place->right);
    }

        void rotateLeft(node<T> *place) {
        static int count = 0;
        node<T> *oldRoot = place;
        node<T> *newRoot = place->right;

        #ifdef GRAPHVIZ
        //make_image("pre-left_rotation_#" + to_string(count));
        #endif

        if (place == root)
            root = newRoot;

        if (oldRoot->parent) {
            if (oldRoot->parent->left == oldRoot)
                oldRoot->parent->left = newRoot;
            else
                oldRoot->parent->right = newRoot;
        }

        newRoot->parent = oldRoot->parent;
        oldRoot->parent = newRoot;
        oldRoot->right = newRoot->left;
        if (oldRoot->right)
            oldRoot->right->parent = oldRoot;
        newRoot->left = oldRoot;
        // = newRoot;

        #ifdef GRAPHVIZ
        //make_image("post-left_rotation_#" + to_string(count));
        #endif

        count++;
    }

    void rotateRight(node<T> *place) {
        node<T> *oldRoot = place;
        node<T> *newRoot = place->left;

        static int count = 0;
        #ifdef GRAPHVIZ
        //make_image("pre-right_rotation_#" + to_string(count));
        #endif

        if (place == root)
            root = newRoot;

        if (oldRoot->parent) {
            if (oldRoot->parent->left == oldRoot)
                oldRoot->parent->left = newRoot;
            else
                oldRoot->parent->right = newRoot;
        }

        newRoot->parent = oldRoot->parent;
        oldRoot->parent = newRoot;
        oldRoot->left = newRoot->right;
        if (oldRoot->left)
            oldRoot->left->parent = oldRoot;
        newRoot->right = oldRoot;
        //place = newRoot;

        #ifdef GRAPHVIZ
        //make_image("post-right_rotation_#" + to_string(count));
        #endif

        count++;
    }

    void doubleRotateLeft(node<T> *place) {
        static int count = 0;
        node<T> *oldRoot = place;
        node<T> *newRoot;
        
        newRoot = place->right->left;

        #ifdef GRAPHVIZ
        //make_image("pre-double-left_rotation_#" + to_string(count));
        #endif

        if (place == root)
            root = newRoot;

        if (oldRoot->parent) {
            if (oldRoot->parent->left == oldRoot)
                oldRoot->parent->left = newRoot;
            else
                oldRoot->parent->right = newRoot;
        }
            

        newRoot->parent = oldRoot->parent;
        oldRoot->parent = newRoot;
        oldRoot->right->left = newRoot->right;
        if (oldRoot->right->left)
            oldRoot->right->left->parent = oldRoot->right;
        newRoot->right = oldRoot->right;
        oldRoot->right->parent = newRoot;
        if (newRoot->left) {
            oldRoot->right = newRoot->left;
            newRoot->left->parent = oldRoot;
        } else {
            //oldRoot->right->left = 0;
            oldRoot->right = 0;
        }
        newRoot->left = oldRoot;
        //place = newRoot;

        #ifdef GRAPHVIZ
        //make_image("post-double-left_rotation_#" + to_string(count));
        #endif

        count++;
    }

    void doubleRotateRight(node<T> *place) {
        static int count = 0;
        node<T> *oldRoot = place;
        node<T> *newRoot;
        
        newRoot = place->left->right;

        #ifdef GRAPHVIZ
        //make_image("pre-double-right_rotation_#" + to_string(count));
        #endif

        if (place == root)
            root = newRoot;

        if (oldRoot->parent) {
            if (oldRoot->parent->left == oldRoot)
                oldRoot->parent->left = newRoot;
            else
                oldRoot->parent->right = newRoot;
        }

        newRoot->parent = oldRoot->parent;
        oldRoot->parent = newRoot;
        oldRoot->left->right = newRoot->left;
        if (oldRoot->left->right)
            oldRoot->left->right->parent = oldRoot->left;
        newRoot->left = oldRoot->left;
        oldRoot->left->parent = newRoot;
        if (newRoot->right) {
            oldRoot->left = newRoot->right;
            newRoot->right->parent = oldRoot;
        } else {
            //oldRoot->left->right = 0;
            oldRoot->left = 0;
        }
        newRoot->right = oldRoot;
        //place = newRoot;

        #ifdef GRAPHVIZ
        //make_image("post-double-right_rotation_#" + to_string(count));
        #endif

        count++;
    }

    void rebalance(node<T> *place) {
        
        int bf;

        while (place) {

            bf = balanceFactor(place);
            if (bf > 1) {
                if (place->right->right && balanceFactor(place->right) > -1) {
                    //cout << " Rotating left at node " << place->data << "\n";
                    rotateLeft(place);
                }
                else {
                    //cout << " Double rotate left at node " << place->data << "\n";
                    
                    doubleRotateLeft(place);
                }
            }
            if (bf < -1) {
                if (place->left->left && balanceFactor(place->left) < 1) {
                    //cout << " Rotating right at node " << place->data << "\n";
                    rotateRight(place);
                } else {
                    //cout << " Double rotate right at node " << place->data << "\n";
                    doubleRotateRight(place);
                }
                
            }
            place = place->parent;
        }
    }
    void deallocate_tree(node<T>* place){
	if(place->left)
		deallocate_tree(place->left);
	if(place->right)
		deallocate_tree(place->right);
	delete place;
    }


    public:

    #ifdef GRAPHVIZ
	void make_image(string);
    #endif

    void insert(T data) {
        cout << " Inserting " << data << "\n";
        node<T> *new_node = new node<T>;
        new_node->data = data;
        new_node->left = new_node->right = new_node->parent = 0;

        if (!root) {
            root = new_node;
            return;
        }
        node<T> *place = root;
        while(place){
            new_node->parent = place;
            if(data < place->data)
                place = place->left;
            else if(data > place->data)
                place = place->right;
            else {
                cout << " Duplicate node detected. Node not entered.\n";
                delete new_node;
                return;
            }
        }
        if(new_node->parent->data < data)
            new_node->parent->right = new_node;
        else
            new_node->parent->left = new_node;

        rebalance(new_node->parent);
        
    }

    void remove(T data) {

        if (!root) {
            cout << "\n Nothing in the tree to remove.\n";
            return;
        }

        if (!root->right && !root->left) {
            root = 0;
            return;
        }
        
        node<T> *place = root;
        node<T> *parent = 0;


        while (place) {
            if (place->data == data) {

                if (place->parent)
                    parent = place->parent;

                if (!place->left && !place->right) {
                    if (parent->data > place->data)
                        parent->left = 0;
                    else
                        parent->right = 0;
                    delete place;

                    if (parent)
                        rebalance(parent);

                    return;
                }

                if (place->left) {
                    node<T> *largest = place->left;

                    if (largest->right) {
                        while (largest->right) {
                            largest = largest->right;
                        }
                        if (largest->left)
                            largest->parent->right = largest->left;
                        else
                            largest->parent->right = 0;
                    }

                    place->data = largest->data;
                    if (largest == place->left) {
                        largest->parent->left = largest->left;
                        if (largest->left)
                            largest->left->parent = largest->parent;
                    } else if (largest->left) {
                        largest->parent->right = largest->left;
                        if (largest->left)
                            largest->left->parent = largest->parent;
                    } else {
                        largest->parent->right = 0;
                    }
                    delete largest;
                    rebalance(place);
                    return;

                } else { // this runs if the node to delete only has a right subtree
                    place->data = place->right->data;
                    delete place->right;
                    place->right = 0;
                    rebalance(place);
                    return;
                }

            }
            if (place->data > data)
                place = place->left;
            else
                place = place->right;
        }
        cout << "\n Node not found.\n";

    }

    void print() {
        if (!root) {
            cout << "[]";
            return;
        }
        else {
            cout << "[";
            printInOrder(root);
        }
        cout << "\b\b]";
        
    }
    
    // not fetch, but fetch would break certain types of trees
    void replace(T data, T data2){

        if (!contains(data)) {
            cout << "\n Node not found in tree. Modification failed.\n";
            return;
        }
        if (contains(data2)) {
            cout << "\n The value you entered to replace a node with is a duplicate. Modification failed.\n";
            return;
        }
        remove(data);
        cout << "\n";
        insert(data2);
    }

    bool contains(T data) {
        node<T> *place = root;
	    while(place){
            if(place->data == data) {
                return true;
            }
            if(place->data > data)
                place = place->left;
            else
                place = place->right;
        }
        return false;
    }

    ~AVLTree() { deallocate_tree(root); } 
};

#ifdef GRAPHVIZ
/* To build this program remember to add -lcgraph and -lgvc to your compile line
	If there's an error about missing header files, maybe libgraphviz-dev isn't installed:
	sudo apt install libgraphviz-dev
	*/
template<class T>
void AVLTree<T>::make_image_recur(Agraph_t *g, Agnode_t *parent, node<T> *place){
	Agnode_t *us = agnode(g, (char*)to_string(place->data).c_str(), 1); // problem later because a T isn't a char*
	if(parent)
		Agedge_t *e = agedge(g, parent, us, (char*)"", 1);
	if(place->left)
		make_image_recur(g, us, place->left);
	if(place->right)
		make_image_recur(g, us, place->right);
}

template<class T>
void AVLTree<T>::make_image(string name){
	Agraph_t *g;
	g = agopen((char*)"G", Agdirected, 0);
    agattr(g, AGRAPH, (char*) "rankdir", (char*) "TB");
    agset(g, (char*) "rankdir", (char*) "BT");
	Agsym_t *color_attr = agattr(g, AGNODE, (char*)"color", (char*)"black");

	// Go through the tree, and add all our nodes!
	// Add our own node
	// If there is a left subtree, add the left subtree
	// If there is a right subtree, add the right subtree
	make_image_recur(g, 0, root);

	GVC_t* gvc = gvContext();
	gvLayout(gvc, g, "dot");
	agwrite(g, stdout);
	gvRenderFilename(gvc, g, "png", (name + ".png").c_str());
	gvFreeLayout(gvc, g);
	agclose(g);
}

#endif

int main() {

    srand(time(NULL));

    AVLTree<int> test_tree;

    int count = 0;

    test_tree.insert(9);
    test_tree.insert(13);
    test_tree.insert(16);
    test_tree.insert(7);
    test_tree.insert(5);
    test_tree.insert(19);
    test_tree.insert(18);
    test_tree.insert(2);
    test_tree.insert(4);
    test_tree.insert(1);
    test_tree.insert(8);

    int choice = 1, num, num2;

    cout << "\n The current tree is as follows: ";
    test_tree.print();
    cout << "\n\n";


    while (choice > 0 && choice < 7) {

        #ifdef GRAPHVIZ
        //test_tree.make_image("tree_" + to_string(count++));
        #endif

        cout << "\n What would you like to do with the tree?\n\n";
        cout << " 1: Insert a node\n";
        cout << " 2: Remove a node\n";
        cout << " 3: Modify a node\n";
        cout << " 4: Check if a node exists\n";
        cout << " 5: Print the tree in order\n";
        cout << " 6: Add 10 random numbers between 0 and 500\n";
        cout << " 7: Exit program, export to png, and deallocate tree\n";

        cin >> choice;

        switch (choice) {
            case 1: 
                cout << "\n What would you like to insert?\n";
                cin >> num;
                cout << "\n";
                test_tree.insert(num);
                break;
            
            case 2:
                cout << "\n What would you like to remove?\n";
                cin >> num;
                test_tree.remove(num);
                break;
            
            case 3:
                cout << "\n What would you like to modify?\n";
                cin >> num;
                cout << "\n What would you like to replace it with?\n";
                cin >> num2;
                test_tree.replace(num, num2);
                break;
            
            case 4:
                cout << "\n What node would you like to search for?\n";
                cin >> num;
                if (test_tree.contains(num))
                    cout << "\n The tree contains " << num << "\n";
                else
                    cout << "\n The tree does not contain " << num << "\n";
                break;

            case 5: 
                cout << "\n";
                test_tree.print();
                cout << "\n";
                break;

            case 6:
                for (int i = 0; i < 10; i++)
                    test_tree.insert(rand() % 500);
        }
    }


    #ifdef GRAPHVIZ
    test_tree.make_image("tree");
    #endif
    return 0;
}