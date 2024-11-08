#include "LinkedList.h"



int main(int argc, char const *argv[]) {
  struct DoublyLinkedList dll;
  struct Data data1 = {.id=1,.length=2};
  struct Data data2 = {.id=2,.length=69};
  struct Data data3 = {.id=3,.length=42};

  struct Data data0 = {.id=0,.length=0};

  initializeDoublyList(&dll);


  append(&dll, data1);
  append(&dll, data2);
  append(&dll, data3);

  printListForward(&dll);puts("");
  pop(&dll, &data0);
  printListForward(&dll);puts("");

  printf("We popped node with: ID: %d, Length: %d\n",data0.id,data0.length);

  freeDoublyList(&dll);
  return 0;
}
