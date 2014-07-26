#include "bdd.h"
#include <string.h>
#include "../src/data/queue.c"

Feature("Queue test")

Case("Create and destroy a queue") {
  Given("I created a queue with size of 1000 bytes")
    Queue *q = queue_create(1000);
  When("I destroy the queue")
    queue_destroy(q);
  Then("should not throw exception")
    assert(true);
}

Case("Enqueue and dequeue") {
  Given("I created a queue with size of 1000 bytes")
    Queue *q = queue_create(1000);
  When("I enqueue a string")
    char str[] = "I am a string";
    queue_enqueue(q, str, strlen(str) + 1);
  And("I dequeue the string from the queue")
    char buf[100];
    int size = sizeof(buf);
    queue_dequeue(q, buf, &size);
  Then("the string should be the same")
    assert(strcmp(str, buf) == 0);
  And("returned size should be equal to the original size")
    assert(size == strlen(str) + 1); 

  queue_destroy(q);
}

Case("Enqueue and dequeue multiple items") {
  int i;
  Given("I created a queue with size of 1000 bytes")
    Queue *q = queue_create(1000);
  When("I enqueue 10 integer from 1 to 10 in order")
    for(i = 1; i <= 10; i++) {
      queue_enqueue(q, &i, sizeof(i));
    }
  And("I dequeue 10 items")
    int items[10];
    int size = sizeof(int);
    for(i = 0; i < 10; i++) {
      queue_dequeue(q, items + i, &size);
    }
  Then("The items should range from 1 to 10 in order")
    bool inorder = true;
    for(i = 1; i <= 10; i++) {
      if (items[i - 1] != i)
        inorder = false;
    }
    assert(inorder);
  And("I cannot dequeue any more items")
    assert(queue_dequeue(q, i, &size) == false);

  queue_destroy(q);
}

Case("Enqueue and dequeue with wrapped memory") {
  Given("I created a queue with size of 20 bytes")
    Queue *q = queue_create(20);
  When("I enqueue a string with 14 bytes")
    char str[] = "I am a string";
    queue_enqueue(q, str, strlen(str) + 1);
  And("I dequeue the string from the queue")
    char buf[100];
    int size = sizeof(buf);
    queue_dequeue(q, buf, &size);
  And("I enqueue the same string")
    queue_enqueue(q, str, strlen(str) + 1);
  And("I dequeue the string")
    queue_dequeue(q, buf, &size);
  Then("the string should be the same")
    assert(strcmp(str, buf) == 0);
  And("returned size should be equal to the original size")
    assert(size == strlen(str) + 1); 

  queue_destroy(q);
}

Case("Enqueue large message") {
  Given("I created a queue with size of 20 bytes")
    Queue *q = queue_create(20);
  When("I enqueue a string with 100 bytes")
    char str[100];
    bool result = queue_enqueue(q, str, sizeof(str));
  Then("it should fail")
    assert(!result);

  queue_destroy(q);
}

End
