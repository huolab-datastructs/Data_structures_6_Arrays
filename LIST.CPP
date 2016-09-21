#include "LIST.H"
template <class List_entry>
List<List_entry>::List()
/* 
Post: The List is initialized to be empty.
*/
{
   head = -1;
   count = 0;
   available = -1;
   last_used = -1;
}
 
template <class List_entry>
void List<List_entry>::clear()
/* 
Post: The List is cleared.
*/
{
   Index p, q;

   for (p = head; p != -1; p = q) {
      q = workspace[p].next;
      delete_node(p);
   }
   count = 0;
   head = -1;
}
 
template <class List_entry>
int List<List_entry>::size() const
/* 
Post: The function returns the number of entries in the List.
*/
{
   return count;
}
 
template <class List_entry>
bool List<List_entry>::empty() const
/* 
Post: The function returns true or false according as the List is empty or not.
*/
{
   return count <= 0;
}
 
template <class List_entry>
bool List<List_entry>::full() const
/* 
Post: The function returns true or false according as the List is full or not.
*/
{
   return count >= max_list;
}
 
template <class List_entry>
void List<List_entry>::traverse(void (*visit)(List_entry &))
/* 
Post: The action specified by function *visit has been performed on every
entry of the List, beginning at position 0 and doing each in turn.
*/
{
   for (Index n = head; n != -1; n = workspace[n].next)
      (*visit)(workspace[n].entry);
}
 
template <class List_entry>
Error_code List<List_entry>::insert(int position, const List_entry &x)
/* 
Post: If the List is not full and 0 <= position <= n,
where n is the number of entries in the List,
the function succeeds:
Any entry formerly at
position and all later entries have their
position numbers increased by 1 and
x is inserted at position of the List.
 
Else:
the function fails with a diagnostic error code.
*/
{
   Index new_index, previous, following;

   if (position < 0 || position > count) return Error_code::range_error;

   if (position > 0) {
      previous = set_position(position - 1);
      following = workspace[previous].next;
   }
   else following = head;
   if ((new_index = new_node()) == -1) return overflow;
   workspace[new_index].entry = x;
   workspace[new_index].next = following;

   if (position == 0)
      head = new_index;
   else
      workspace[previous].next = new_index;

   count++;
   return success;
}
 
template <class List_entry>
Error_code List<List_entry>::retrieve(int position, List_entry &x) const
/* 
Post: If the List is not full and 0 <= position < n,
where n is the number of entries in the List,
the function succeeds:
The entry in position is copied to x.
Otherwise the function fails with an error code of range_error.
*/
{
   Index current;
   if (position < 0 || position >= count) return range_error;
   current = set_position(position);
   x = workspace[current].entry;
   return success;
}
 
template <class List_entry>
Error_code List<List_entry>::replace(int position, const List_entry &x)
/* 
Post: If 0 <= position < n,
where n is the number of entries in the List,
the function succeeds:
The entry in position is replaced by x,
all other entries remain unchanged.
Otherwise the function fails with an error code of range_error.
*/
{
   Index current;
   if (position < 0 || position >= count) return range_error;
   current = set_position(position);
   workspace[current].entry = x;
   return success;
}
 
template <class List_entry>
Error_code List<List_entry>::remove(int position, List_entry &x)
/* 
Post: If 0 <= position < n,
where n is the number of entries in the List,
the function succeeds:
The entry in position is removed
from the List, and the entries in all later positions
have their position numbers decreased by 1.
The parameter x records a copy of
the entry formerly in position.
Otherwise the function fails with a diagnostic error code.
*/
{
   Index prior, current;
   if (count == 0) return underflow;
   if (position < 0 || position >= count) return range_error;
   if (position > 0) {
      prior = set_position(position - 1);
      current = workspace[prior].next;
      workspace[prior].next = workspace[current].next;
   }

   else {
      current = head;
      head = workspace[head].next;
   }
   x = workspace[current].entry;
   delete_node(current);
   count--;
   return success;
}
 
template <class List_entry>
Index List<List_entry>::new_node()
/* 
Post: The Index of the first available
Node in workspace is returned;
the data members available, last_used, and
workspace are updated as necessary.  If the
work-space is already full, -1 is returned.
*/
{
   Index new_index;

   if (available != -1) {
      new_index = available;
      available = workspace[available].next;
   } else if (last_used < max_list - 1) {
      new_index = ++last_used;
   } else return -1;
   workspace[new_index].next = -1;
   return new_index;
}
 
template <class List_entry>
int List<List_entry>::current_position(Index n) const
/* 
 
Post:
Returns the position number of the Node stored at Index n,
or -1 if there no such Node.
 
*/

{
   int i = 0;
   for (Index m = head; m != -1; m = workspace[m].next, i++)
      if (m == n) return i; //  position number of Node at Index n
   return -1;
}
 
template <class List_entry>
Index List<List_entry>::set_position(int position) const
/*   
Pre:   position is a valid position in the List;
      0 <= position < count.
Post: Returns the Index of the Node in position of the List.
 */

{
   Index n = head;
   for (int i = 0; i < position; i++, n = workspace[n].next);
   return n;
}
 
template <class List_entry>
void List<List_entry>::delete_node(Index old_index)
/* 
Pre:  The List has a Node stored at Index old_index.
Post:
The List Index old_index is pushed onto the linked stack
of available space; available, last_used, and workspace are
updated as necessary.
*/
{
   Index previous;
   if (old_index == head) head = workspace[old_index].next;

   else {
      previous = set_position(current_position(old_index) - 1);
      workspace[previous].next = workspace[old_index].next;
   }

   workspace[old_index].next = available;
   available = old_index;
}
