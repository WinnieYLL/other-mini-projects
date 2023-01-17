#include <iostream>
#include <vector>
using namespace std;



template <class K, class V>

class AVLTreeNode {
  private:
    
    
  public:
    // Should have attributes named:
    AVLTreeNode* parent; // parent - pointer to parent
    AVLTreeNode* left; // left - pointer to left child
    AVLTreeNode* right; 	// right - pointer to right child

    K key; 	// key - node's key
    V value; // value - node's value
    int height; // height - node's height
    int depth;
 
    // Constructors
    AVLTreeNode(K, V);
    AVLTreeNode(K key, V value, AVLTreeNode* parent, AVLTreeNode* left, 
                AVLTreeNode* right): key(key), value(value), parent(parent), 
                left(left), right(right) { height = 0; depth = 0; };
    AVLTreeNode(const AVLTreeNode<K,V> &);
    
    int getBalance() const;
   
};

template <class K, class V>
AVLTreeNode<K,V>::AVLTreeNode(K k, V v){
  key = k;
  value = v;
  parent = NULL;
  left = NULL;
  right = NULL;
  height = 0;
  depth = 0;
}

template <class K, class V>
AVLTreeNode<K,V>::AVLTreeNode(const AVLTreeNode<K,V> &other){
  if (this == &other) return;
  
  key = other.key;
  value = other.value;
  height = other.height;
  depth = other.depth;
  parent = NULL;
  
  if (other.left) {
    left = new AVLTreeNode(*(other.left));
    left->parent = this;
  }
  else left = NULL;
  
  if (other.right){
    right = new AVLTreeNode(*(other.right));
    right->parent = this;
  }  
  else right = NULL;
}

template <class K, class V>
// AVL Tree class
class AVLTree {
private:
	// Tree attributes go here
	AVLTreeNode<K,V>* root;
  
  // UTILITY METHODS
  void ascii(AVLTreeNode<K,V>*) const;
  void updateDepth(AVLTreeNode<K,V>*, int);
  void updateDepth();
  void updateHeight(AVLTreeNode<K,V>*);
  void updateHeight();
  
 
  // HELPER METHODS
  AVLTreeNode<K,V>* insert(AVLTreeNode<K,V>*, K, V, bool&);
  V search(AVLTreeNode<K,V>*, K) const;
  void values(const AVLTreeNode<K,V>*, vector<V>&);
  void keys(const AVLTreeNode<K,V>*, vector<K>&);
  void recursiveDelete(AVLTreeNode<K,V>*);
  AVLTreeNode<K,V>* remove(AVLTreeNode<K,V>*, K, bool&);
  AVLTreeNode<K,V>* minNode(AVLTreeNode<K,V>*) const;
  unsigned int size(AVLTreeNode<K,V>* rootNode) const;
  
public:
  
  // UTILITY METHODS  
  void print() const;
  
  //Default constructor
  AVLTree(); 
  
  //Copy constructor – a constructor that creates a deep copy of its AVLTree reference parameter.
  AVLTree(const AVLTree<K,V>&);

  //Operator= – overloads the assignment operator for AVLTree objects to make a deep copy of its AVLTree reference parameter and return a reference to the calling object. The operator should behave correctly under self assignment.
  AVLTree<K,V>& operator=(const AVLTree<K,V>&);

  
  //Destructor – deletes dynamic memory allocated by the tree
  ~AVLTree();

  
  /*
  Insert – if the tree does not contain the method's first parameter which represents the key, inserts the key and value (the second parameter) and returns true; otherwise returns false without insertion; i.e. the tree does not contain duplicate keys; both both key and value should be template parameters and may be different types
  */
  bool insert(K, V);

  //Remove – removes the key and associated value from the tree where the key matches the method's parameter and returns true; if the tree does not contain the a key matching the parameter returns false.
  bool remove(K);
  
  //Search – searches the tree for the key that equals the method's single parameter, returning the corresponding value if it is found and throwing a runtime_error exception if it is not.
  V search(K) const;
  

  //Values – returns a vector that contains all of the values in the tree; the contents of the vector are in ascending key - not value - order; if the tree is empty the returned vector should also be empty
  vector<V> values();

  //Keys – returns a vector that contains all of the keys in the tree; the contents of the vector are in ascending order; if the tree is empty the returned vector should also be empty.
  vector<K> keys();
  
  //Size – returns an unsigned int equal to the number of items stored in the tree.
  
  unsigned int size() const;
  
   
  //GetRoot
	void* getRoot() const { return root; }; 
  
  
};

// UTILITY METHODS
template <class K, class V>
void AVLTree<K,V>::ascii(AVLTreeNode<K,V>* tree) const {
  if (tree == NULL) return;

  ascii(tree->right);

  for (int i = 0; i < tree->depth; i++) {
      cout << "  ";
  }
  cout << tree->key << endl;

  ascii(tree->left);
}

template <class K, class V>
void AVLTree<K,V>::updateDepth(AVLTreeNode<K,V>* tree, int startDepth) {
  // AVLTreeNode<K,V>* curr = root;
  if (tree == NULL) return;
  tree->depth = startDepth;
  if (tree->left) updateDepth(tree->left, startDepth+1);
  if (tree->right) updateDepth(tree->right, startDepth+1);
  
  // cout << "Key: " << tree->key << " Depth: " << tree->depth << endl;
}

template <class K, class V>
void AVLTree<K,V>::updateDepth() {
  updateDepth(root, 0);
} 

template <class K, class V>
void AVLTree<K,V>::updateHeight(AVLTreeNode<K,V>* tree) {
  if (tree == NULL) return;
  
  int left_height;
  int right_height;
  
  if(tree->left) {
    updateHeight(tree->left);
    left_height = tree->left->height;
  } else {
    left_height = -1;
  }
    
  if(tree->right) {
    updateHeight(tree->right);
    right_height = tree->right->height;
  } else {
    right_height = -1;
  }
  
  tree->height = 1 + max(left_height, right_height);
  
  // cout << "At node " << tree->key << ": left_height = " << left_height << ", right_height = " << right_height << endl;
}
  
template <class K, class V>
void AVLTree<K,V>::updateHeight() {
  updateHeight(root);
}

template <class K, class V>
void AVLTree<K,V>::print() const {
  ascii(root);
  cout << endl;
} // print

// HELPER METHODS
template <class K, class V>
AVLTreeNode<K,V>* AVLTree<K,V>::insert(AVLTreeNode<K,V>* tree, K key, V value, bool& inserted){
	//Base case
	if (tree == NULL){
		 return new AVLTreeNode<K,V>(key, value, NULL, NULL, NULL);
	}

	if (key < tree->key){
		tree->left = insert(tree->left, key, value, inserted);
		tree->left->parent = tree;
	} else if (key > tree->key){
		tree->right = insert(tree->right, key, value, inserted);
		tree->right->parent = tree;
	} else{
    inserted = true;
    return tree;
  }
  
  // Update height
  updateHeight();
  
  int bf = tree->getBalance();

  //Left Left Case
  if (bf > 1 && key < tree->left->key){ return rightRotate(tree);}
  
  //Right Right Case
  if (bf < -1 && key > tree->right->key){ return leftRotate(tree);} 
  
  //Left Right Case
  if(bf > 1 && key > tree->left->key){
    tree->left = leftRotate(tree->left);
    return rightRotate(tree);
  }
  
  //Right Left Case
  if (bf < -1 && key<tree->right->key){
    tree->right = rightRotate(tree->right);
    return leftRotate(tree);
  }
	return tree;
}//insert

template <class K, class V>
V AVLTree<K,V>::search(AVLTreeNode<K,V>* rootNode, K key) const {
    if (rootNode == NULL){
      throw std::logic_error("Runtime_error exception!");
    }
    if (key == rootNode->key){
      return rootNode->value;
    } 
    else if (key < rootNode->key){
      return search(rootNode->left, key);
    } else {
      return search(rootNode->right, key);
    }
  }

template <class K, class V>  
void AVLTree<K,V>::values(const AVLTreeNode<K,V>* rootNode, vector<V>& arr){
  // vector<V> arr;
  // inorder traversal to read all the values in the tree
  if(rootNode!=NULL){
    values(rootNode->left, arr);
    arr.push_back(rootNode->value);
    values(rootNode->right, arr);
  }
}

template <class K, class V>    
void AVLTree<K,V>::keys(const AVLTreeNode<K,V>* rootNode, vector<K>& arr){
  if(rootNode!=NULL){
    keys(rootNode->left, arr);
    arr.push_back(rootNode->key);
    keys(rootNode->right, arr);
  }
}  
  
template <class K, class V> 
void AVLTree<K,V>::recursiveDelete(AVLTreeNode<K,V>* root){
  if(root!=NULL){
    if(root->left!=NULL){
      recursiveDelete(root->left);
    }
    if(root->right!=NULL){
      recursiveDelete(root->right);    
    }  
    // cout<< "deleting" << root->key << endl;
    delete root;
  }
}  

template <class K, class V>   
AVLTreeNode<K,V>* AVLTree<K,V>::remove(AVLTreeNode<K,V>* rootNode, K key, bool& removed){
  //search the node
  //delete the node
  //check if AVLTree and rebalance if needed
  if (rootNode == NULL){
    return NULL;
    
  } else if (key > rootNode->key){
    // cout << "recurse right" << endl;
    rootNode->right = remove(rootNode->right, key, removed);
    
  } else if (key < rootNode->key){
    // cout << "recurse left" << endl;
    rootNode->left = remove(rootNode->left, key, removed);
    
  } else if (key == rootNode->key) {
    
    removed = true;
    // leaf, delete this one
    if (rootNode->left == NULL && rootNode->right == NULL) {
      return NULL;
    }
    
    
    if (rootNode->left == NULL) {
      AVLTreeNode<K,V>* temp = rootNode->right;
      delete rootNode;
      return temp;  // this will be returned to the previous level
    }
    
    if (rootNode->right == NULL) {
      AVLTreeNode<K,V>* temp = rootNode->left;
      delete rootNode;
      return temp; // this will be returned to the previous level
    }
    
    // two children (since above ifs did not return)
    // get successor (samllest in the right subtree)
    AVLTreeNode<K, V>* temp = minNode(rootNode->right);
    
    //copy the inorder successor's data to this node
    rootNode->key = temp->key;
    
    //delete the inorder successor
    rootNode->right = remove(rootNode->right, temp->key, removed);
  }
  
  if (rootNode==NULL) return NULL;
  
  updateHeight();
    
  int bf = rootNode->getBalance();
  
  //Left Left Case
  if (bf > 1 && rootNode->left->getBalance() >= 0){ return rightRotate(rootNode);}
  
  //Right Right Case
  if (bf < -1 && rootNode->right->getBalance() <= 0){ return leftRotate(rootNode);} 
  
  //Left Right Case
  if(bf > 1 && rootNode->left->getBalance() < 0){
    rootNode->left = leftRotate(rootNode->left);
    return rightRotate(rootNode);
  }
  
  //Right Left Case
  if (bf < -1 && rootNode->right->getBalance() > 0){
    rootNode->right = rightRotate(rootNode->right);
    return leftRotate(rootNode);
  }
	return rootNode;
}  

template <class K, class V>
AVLTreeNode<K,V>* AVLTree<K,V>::minNode(AVLTreeNode<K,V>* rootNode) const {
	//inorder successor = smallest in the right subtree
  if(rootNode->left == NULL){
    return rootNode;
  }
  return minNode(rootNode->left);
}

template <class K, class V> 
unsigned int AVLTree<K,V>::size(AVLTreeNode<K,V>* rootNode) const {
    if (rootNode == NULL) return 0;
    return 1 + size(rootNode->left) + size(rootNode->right);
}
 
// REQUIRED METHODS
template <class K, class V>
AVLTree<K,V>::AVLTree() {
  root = NULL;
}

template <class K, class V>
AVLTree<K,V>::AVLTree(const AVLTree<K,V> &other){
  if (this!= &other){
    if (other.root == NULL) this->root = NULL;
    else this->root = new AVLTreeNode<K,V>(*other.root); // this is recursive!
  }
}

template <class K, class V>
AVLTree<K,V>& AVLTree<K,V>::operator=(const AVLTree<K,V>& rht ){
  if (this != &rht) {
    if(rht.root) recursiveDelete(this->root);
    if (rht.root == NULL) this->root = NULL;
    else this->root = new AVLTreeNode<K,V>(*rht.root);
  }  
  return *this;
}//lht = rht

template <class K, class V>
AVLTree<K,V>::~AVLTree(){
  recursiveDelete(root);
}

template <class K, class V>
bool AVLTree<K,V>::insert(K key, V value){
  bool inserted = false;
  root = insert(root, key, value, inserted);
  updateDepth();
  updateHeight();
  return inserted;
}

template <class K, class V>
V AVLTree<K,V>::search(K key) const {
  return search(root, key);
}

template <class K, class V>
int AVLTreeNode<K,V>::getBalance() const {
  int left_height;
  int right_height;
  
  if(left) left_height = left->height;
  else     left_height = -1;

  if(right) right_height = right->height;
  else      right_height = -1;  

  return left_height - right_height;
}

template <class K, class V>  
vector<V> AVLTree<K,V>::values() {
  vector<V> arr;
  arr.clear();
  values(root, arr);
  return arr;
} 

template <class K, class V>  
vector<K> AVLTree<K,V>::keys() {
  vector<K> arr;
  arr.clear();
  keys(root, arr);
  return arr;
}

template <class K, class V> 
bool AVLTree<K,V>::remove(K key){
  bool rotated = false;
  root = remove(root, key, rotated);
  updateDepth();
  updateHeight();
  return rotated;
}

template <class K, class V> 
unsigned int AVLTree<K,V>::size() const {
  return size(root);
}

template <class K, class V>
AVLTreeNode<K,V>* rightRotate(AVLTreeNode<K,V>* tree){
  AVLTreeNode<K,V>* T1 = tree->left;
  AVLTreeNode<K,V>* T2 = T1->right;
  
  //Perform rotation
  T1->right = tree;
  tree->left = T2;
  return T1;
}

template <class K, class V>
AVLTreeNode<K,V>* leftRotate(AVLTreeNode<K,V>* tree){
  AVLTreeNode<K,V>* T1 = tree->right;
  AVLTreeNode<K,V>* T2 = T1->left;
  
  //Perform rotation
  T1->left = tree;
  tree->right = T2;
  return T1;
}

int max(int a, int b){
  if (a>=b) return a;
  else {
    return b;
  }
}