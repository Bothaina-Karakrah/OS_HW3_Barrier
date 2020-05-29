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
        /**
         * Constructor
         */
        List() { //TODO: add your implementation
             }

        /**
         * Destructor
         */
        ~List(){ //TODO: add your implementation
             }

        class Node {
         public:
          T data;
          Node *next;
          // TODO: Add your methods and data members
          pthread_mutex_t m;

            Node(T data){
                this->data=data;
                this->next= nullptr;
                try{
                    pthread_mutex_init(&this->m, NULL);
                }
                catch (...){
                    std::cerr << "pthread_mutex_init: failed" << std::endl;
                    exit(-1);
                }
            }
            Node(){
                this->next = NULL;
                pthread_mutex_init(&this->m, NULL);
            }
            ~Node(){
                pthread_mutex_destroy(&this->m);
            }
            void lock(){
                pthread_mutex_lock(&this->m);
            }
            void unlock(){
                pthread_mutex_unlock(&this->m);
            }

        };

        /**
         * Insert new node to list while keeping the list ordered in an ascending order
         * If there is already a node has the same data as @param data then return false (without adding it again)
         * @param data the new data to be added to the list
         * @return true if a new node was added and false otherwise
         */
        bool insert(const T& data) {
			//TODO: add your implementation
        }

        /**
         * Remove the node that its data equals to @param value
         * @param value the data to lookup a node that has the same data to be removed
         * @return true if a matched node was found and removed and false otherwise
         */
        bool remove(const T& value) {
			//TODO: add your implementation
        }

        /**
         * Returns the current size of the list
         * @return current size of the list
         */
        unsigned int getSize() {
			//TODO: add your implementation
            return this->len;
        }

		// Don't remove
        void print() {
          Node* temp = head;
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
    // TODO: Add your own methods and data members
    int len;
    pthread_mutex_t list_mutex;
};

#endif //THREAD_SAFE_LIST_H_
