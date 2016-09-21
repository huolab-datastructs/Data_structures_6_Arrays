#include   "../../C/UTILITY.H"
typedef int Index;
const int max_list = 7; //  small value for testing purposes

template <class List_entry>
class Node{
public:
   List_entry entry;
   Index next;
};
 
template <class List_entry>
class List {
public:
//  Methods of the list ADT
   List();
   int size() const;
   bool full() const;
   bool empty() const;
   void clear();
   void traverse(void (*visit)(List_entry &));
   Error_code retrieve(int position, List_entry &x) const;
   Error_code replace(int position, const List_entry &x);
   Error_code remove(int position, List_entry &x);
   Error_code insert(int position, const List_entry &x);

protected:

//  Data members
   Node<List_entry> workspace[max_list];
   Index available, last_used, head;
   int count;

//  Auxiliary member functions
   Index new_node();
   void delete_node(Index n);
   int current_position(Index n) const;
   Index set_position(int position) const;
};


