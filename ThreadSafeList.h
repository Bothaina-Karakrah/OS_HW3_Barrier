#ifndef THREAD_SAFE_LIST_H_
#define THREAD_SAFE_LIST_H_

#include <pthread.h>
#include <iostream>
#include <iomanip> // std::setw

using namespace std;

template <typename T>
class List 
{
    public:
    class Node {
    public:
        T data;
        Node *next;
        pthread_mutex_t node_m;

        Node(T data){
            this->data=data;
            this->next= NULL;
            try{
                pthread_mutex_init(&this->node_m, NULL);
            }
            catch (...){
                std::cerr << "pthread_mutex_init: failed" << std::endl;
            }
        }

        Node(){
            this->next = NULL;
            pthread_mutex_init(&this->node_m, NULL);
        }

        Node(T data, Node* nextNode){
            this->data = data;
            this->next = nextNode;
            pthread_mutex_init(&node_m, NULL);
        }

        ~Node(){
            pthread_mutex_destroy(&this->node_m);
        }

    };
        /**
         * Constructor
         */
        List() { //TODO: add your implementation
            head = new Node();
            if (!head) {
                cerr << "List: failed" << endl;
                exit(-1);
            }

            this->list_len = 0;
            pthread_mutex_init(&list_m, NULL);
            pthread_mutex_init(&isSorted_m, NULL);
        }

        void listDistroy(Node* head){
            if(!head)
                return;
            listDistroy(head->next);
            free(head);
        }

        /**
         * Destructor
         */
        ~List(){ //TODO: add your implementation
            listDistroy(this->head);
            pthread_mutex_destroy(&this->list_m);
            pthread_mutex_destroy(&this->isSorted_m);
             }

         ////added for test////

/// for testing only  // TODO: add this func to "ThreadSafeList.h" and make adjustments before the test, don't forget to remove before submit
    bool isSorted(){
        pthread_mutex_lock(&isSorted_m);
        if(!head) {
            pthread_mutex_unlock(&isSorted_m);
            return true;
        }else{
            pthread_mutex_lock(&head->node_m);
            pthread_mutex_unlock(&isSorted_m);
        }
        Node* prev = head;
        Node* curr = head->next;
        while(curr) {
            pthread_mutex_lock(&curr->node_m);
            if(prev->data >= curr->data) {
                pthread_mutex_unlock(&curr->node_m);
                pthread_mutex_unlock(&prev->node_m);
                return false;
            }
            pthread_mutex_unlock(&prev->node_m);
            prev = curr;
            curr = curr->next;
        }
        pthread_mutex_unlock(&prev->node_m);
        return true;
    }
         ////////////


        /**
         * Insert new node to list while keeping the list ordered in an ascending order
         * If there is already a node has the same data as @param data then return false (without adding it again)
         * @param data the new data to be added to the list
         * @return true if a new node was added and false otherwise
         */
        bool insert(const T& data) {
            try {

                pthread_mutex_lock(&head->node_m);
                Node *prev = head;
                Node *curr = head->next;
                //lock it if it's not null
                if (curr != NULL) {
                    pthread_mutex_lock(&curr->node_m);
                }

                //find the place
                while (curr != NULL && curr->data < data) {
                    pthread_mutex_unlock(&prev->node_m);
                    prev = curr;
                    curr = curr->next;
                    if (curr != NULL) {
                        pthread_mutex_lock(&curr->node_m);
                    }
                }

                //insert first
                if (curr == NULL) {
                    Node *new_node = new Node(data);
                    if (!new_node){
                        cerr << "insert: failed" << endl;
                        exit(-1);
                    }
                    prev->next = new_node;
                    new_node->next = NULL;
                    pthread_mutex_lock(&list_m);
                    list_len++;
                    pthread_mutex_unlock(&list_m);
                    __insert_test_hook();
                    pthread_mutex_unlock(&prev->node_m);
                    return true;
                }

                //if data exist
                if (curr->data == data) {
                    pthread_mutex_unlock(&prev->node_m);
                    pthread_mutex_unlock(&curr->node_m);
                    return false;
                }

                //insert
                Node *new_node = new Node(data);
                if (!new_node){
                    cerr << "insert: failed" << endl;
                    exit(-1);
                }
                prev->next = new_node;
                new_node->next = curr;
                pthread_mutex_lock(&list_m);
                list_len++;
                pthread_mutex_unlock(&list_m);
                __insert_test_hook();
                pthread_mutex_unlock(&prev->node_m);
                pthread_mutex_unlock(&curr->node_m);
                return true;
            }
            catch (...){
                cerr << "insert: failed" << endl;
                exit(-1);
            }

        }

        /**
         * Remove the node that its data equals to @param value
         * @param value the data to lookup a node that has the same data to be removed
         * @return true if a matched node was found and removed and false otherwise
         */
        bool remove(const T& value) {
            try {
                pthread_mutex_lock(&head->node_m);
                Node *prev = head;
                Node *curr = prev->next;

                if (curr != NULL) {
                    pthread_mutex_lock(&curr->node_m);
                }

                while (curr != NULL && curr->data < value) {
                    pthread_mutex_unlock(&prev->node_m);
                    prev = curr;
                    curr = curr->next;
                    if (curr != NULL) {
                        pthread_mutex_lock(&curr->node_m);
                    }
                }

                //we did not find the data
                if (curr == NULL) {
                    pthread_mutex_unlock(&prev->node_m);
                    return false;
                }

                //data exist, delete it
                if (curr->data == value) {
                    prev->next = curr->next;
                    pthread_mutex_lock(&list_m);
                    list_len--;
                    pthread_mutex_unlock(&list_m);
                    __remove_test_hook();
                    pthread_mutex_unlock(&prev->node_m);
                    pthread_mutex_unlock(&curr->node_m);
                    return true;
                }

                pthread_mutex_unlock(&prev->node_m);
                pthread_mutex_unlock(&curr->node_m);
                return false;
            }
            catch (...){
                cerr << "remove: failed" << endl;
                exit(-1);
            }

        }

        /**
         * Returns the current size of the list
         * @return current size of the list
         */
        unsigned int getSize() {
            pthread_mutex_lock(&list_m);
            unsigned int size = this->list_len;
            pthread_mutex_unlock(&list_m);
            return size;
        }

		// Don't remove
        void print() {
          Node* temp = head->next;
          if (temp == NULL)
          {
            cout << "";
          }
          else if (temp->next == NULL)
          {
            cout << temp->data;
          }
          else
          {
            while (temp != NULL)
            {
              cout << right << setw(3) << temp->data;
              temp = temp->next;
              cout << " ";
            }
          }
          cout << endl;
        }

		// Don't remove
        virtual void __insert_test_hook() {}
		// Don't remove
        virtual void __remove_test_hook() {}

    private:
        Node* head;
        unsigned int list_len;
        pthread_mutex_t  list_m;
        pthread_mutex_t  isSorted_m;
};

#endif //THREAD_SAFE_LIST_H_
