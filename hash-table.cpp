#include <iostream>
#include <cstring>

using namespace std;


const unsigned scale = 225;

const unsigned M = 3907;

template <class T>
class Set {
    private:
        T * A[M];

        int hashKey(string key) const;

        unsigned count; 

    public:
 
        Set() { 
            memset(A, 0, sizeof(T *)*M); 
            count = 0;
        }

        int insert(T * x);

        T * search(string key) const;
};

//HELPER FUNCTIONS:
template <class T>
int Set<T>::hashKey(string key) const {
  int hashed_key = 0;
  int hashed_digit;
  
  for (int j = 0; j < key.length(); j++){

    // process each character
    int keyc = key[j];
    if      (keyc <= '9' && keyc >= '0') { hashed_digit = keyc-48; } //0 - 9
    else if (keyc <= 'z' && keyc >= 'a') { hashed_digit = keyc-87; } //a - z
    else if (keyc <= 'Z' && keyc >= 'A') { hashed_digit = keyc-29; } // A - Z
    else if (keyc == '_')                { hashed_digit = 62; } //_
    else                                 { return -1; }
    
    // update hashed key
    hashed_key = ( (hashed_key*64)%M + hashed_digit*225)%M; 
  }
  return hashed_key;
}


template <class T>
int Set<T>::insert(T * x) {
      // to check if the A[M] is full
      if(count == M){return -1;}
      
      //hashed key
      int hashed_key = hashKey(x->key);
      int k = hashed_key; // just in case we need to use the original hashed key
      int i = 0;
      //For prime m > 2, most choices of c1 and c2 will make h(k,i) distinct for i in [0, (m−1)/2]. Such choices include c1 = c2 = 1/2, c1 = c2 = 1, and c1 = 0, c2 = 1. However, there are only m/2 distinct probes for a given element, requiring other techniques to guarantee that insertions will succeed when the load factor exceeds 1/2. -- From Wikipedia
      
      //So we probably just need to probe about (M-1)/2 times --> to be safe let's do M times 
      
      while(i<=M){ 
          if(A[k] == NULL) {
            // cout << "INSERTED, NULL CASE" << endl;
            A[k] = x;
            count++;
            return k;
          }
          
          if (A[k]->key == x->key){
            // cout << "INSERTED, REPLACE CASE" << endl;
            A[k] = x;
            return k;            
          }
          
          // else
          i++;
          k = hashed_key + i*i;
          k %= M;
      }
        // case 1: if (A[hashed key] == NULL) set to x;
        // case 2: if (A[hashed key]->key == x->key) replace with x;
        // case 3: else {apply quadratic probing sequence to find the next (NULL/x->key)} (A[k] != NULL)&&(A[k]->key != x->key)
    return -1;
}



template <class T>
T * Set<T>::search(string key) const {
  // search the key in the A[M] using its hashed key
    // if (A[hashed key] != key) {find the next index by applying quadratic probing sequence to the hashed key && compare the key with the index's key again && keep going until quadratic hashed key == pow(M,2)}
  if (count == 0){return NULL;}
  
  // hashed key
  int hashed_key = hashKey(key);
  int k = hashed_key; // just in case we need to use the original hashed key
  int i = 0;
  while(i<=M){  
    if(A[k] == NULL) {
      // cout << "End the search function in ROUND " << i << endl;
      return NULL;
    }
    if (A[k]->key == key){
      // cout << "found it in ROUND " << i << endl;
      return A[k];            
    }
    else
    i++;
    k = hashed_key + i*i;
    k %= M;
  }
  return NULL;
}
  




