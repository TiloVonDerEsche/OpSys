#include "LinkedList.h"



int main(int argc, char const *argv[]) {
  struct DoublyLinkedList dll;
  struct Data data1 = {.id=1,.length=2};
  struct Data data2 = {.id=2,.length=69};
  struct Data data3 = {.id=3,.length=42};

  initializeDoublyList(&dll);


  append(&dll, data1);
  append(&dll, data2);
  append(&dll, data3);

  printListForward(&dll);

  struct Data data = {1, 1};
  pop(&dll, &data);
  printListReverse(&dll);

  printf("ID: %d, Length: %d\n",data.id,data.length);

  freeDoublyList(&dll);
  return 0;
}
