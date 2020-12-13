#include "Hashmap.h"

Hashmap::Hashmap() {
	for (int i = 0; i < BUCKETS; i++) {
		buckets[i] = NULL;
	}
	mapSize = 0;
	return;
}
Hashmap::~Hashmap() {
	clear();
}
unsigned int Hashmap::hash(string key) const {
	unsigned char *ptr = (unsigned char *)key.c_str();
	int sum = 0;
	for (int i = 0; i < key.size(); i++) {
		sum += ptr[i];
	}
	unsigned int hash = sum%BUCKETS;
	return hash;
}
void Hashmap::insert(string key, int value) {
	if (!contains(key)){
		Node* insertNode = new Node();
		mapSize++;
		insertNode->key = key;
		insertNode->value = value;
		insertNode->prev = NULL;
		insertNode->next = NULL;
		unsigned int hashVal = hash(key);
		if (buckets[hashVal] == NULL) {
			buckets[hashVal] = insertNode;
			return;
		} else if (buckets[hashVal] != NULL){
			Node* insertAfter = buckets[hashVal];
			while(insertAfter->next != NULL) {
				insertAfter = insertAfter->next;
			}
			insertAfter->next = insertNode;
			insertNode->prev = insertAfter;
		} else {
			cout <<"error here sir" << endl;
		}
		//mapSize++;
		return;
	} else {
		Node* overWriteNode = find(key);
		if (overWriteNode->value != value) {
			overWriteNode->value = value;
		}
		return;
	}
}
bool Hashmap::contains(string key) const {
	bool isFound = false;
	unsigned int findHash = hash(key);
	for (Node* node = buckets[findHash]; node != NULL; node = node->next){
		if (node->key == key){
			return true;
		}
	}
	return false;
}
int Hashmap::get(string key) const {
	unsigned int hashVal = hash(key);
	Node* bucket = buckets[hashVal];
	if (!contains(key)) {
		//cout << "dank debugging" << endl;
		//exit(0);
		//return 5000;
		throw invalid_argument("");
		//return;
	} else {
		for (Node* node = bucket; node != NULL; node = node->next) {
			if (node->key == key) {
				return node->value;
			}
		}
	}
	/*try {

} catch (invalid_argument) {
//cout << "Error: " << error << endl;
throw invalid_argument("");
exit(0);
}*/
}
int& Hashmap::operator [](string key) {
	unsigned int hashVal = hash(key);
	if (contains(key)) {
		Node* node = buckets[hashVal];
		for (node; node!=NULL; node = node->next){
			if (node->key == key){
				return node->value;
			}
		}
	} else {
		insert(key, 0);
		return operator[](key);
	}
}
bool Hashmap::remove(string key) {
	if (contains(key)){
		unsigned int hashVal = hash(key);
		Node* removeNode = find(key);
		Node* removeNodeNext = removeNode->next;
		Node* removeNodePrev = removeNode->prev;
		if (buckets[hashVal] == removeNode) {
			buckets[hashVal] = removeNode->next;
			if (removeNodeNext != NULL) {
				buckets[hashVal] = removeNodeNext;
			} else {
				buckets[hashVal] = NULL;
			}
		} else {
			if (removeNodeNext !=NULL) {
				removeNodePrev->next = removeNodeNext;
				removeNodeNext->prev = removeNodePrev;
			} else if (removeNodeNext == NULL) {
				removeNodePrev->next = NULL;
			}
		}
		delete removeNode;
		mapSize--;
		return true;
	} else {
		return false;
	}
}
void Hashmap::clear() {
	while (mapSize > 0) {
		for (int i = 0; i < BUCKETS; i++){
			if (buckets[i] != NULL) {
				bool removed;
				for (Node* node = buckets[i]; node != NULL; node = node->next) {
					if (node !=NULL) {
						removed = remove(node->key);
					}
				}
			}
		}
	}
}
string Hashmap::toString() const {
	stringstream ss;
	for (int i = 0; i < BUCKETS; i ++){
		ss << "[" << i << "]: ";
		if (buckets[i] != NULL){
			for (Node* currNode = buckets[i]; currNode!=NULL; currNode = currNode->next) {
				if (currNode == NULL) {

				} else if (currNode != buckets[i]) {
					ss << ", " << currNode->key << " => " << currNode->value;
				} else {
					ss << currNode->key << " => " << currNode->value;
				}

			}
		}
		ss << "\n";
	}
	return ss.str();
}
int Hashmap::size() const {
	return mapSize;
}

string Hashmap::toSortedString() const {
	priority_queue<Node*, vector<Node*>, NodeCompare> nodeHeap;
	for (int i = 0; i < BUCKETS; i++) {
		if (buckets[i] != NULL) {
			for (Node* currNode = buckets[i]; currNode!=NULL; currNode = currNode->next) {
				if (currNode == NULL) {

				} else {
					nodeHeap.push(currNode);
				}

			}
		}
	}
	//print
	stringstream ss;
	while(!nodeHeap.empty()){
		ss << nodeHeap.top()->key << " => " << nodeHeap.top()->value << "\n";
		nodeHeap.pop();
	}
	return ss.str();
}
//my helper functions
Hashmap::Node* Hashmap::find(string key) {
	if (!contains(key)) {
		return NULL;
	}
	else {
		unsigned int findHash = hash(key);
		for (Node* node = buckets[findHash]; node != NULL; node = node->next){
			if (node->key == key){
				return node;
			}
		}
	}
}
