
#include "eventbuf.h"
#include <stdio.h>

struct eventbuf *buf;

int main(int argc, char* argv[]) {
  if (argc != 5) printf("Use: # prod, # cons, # events, # outstanding");

  buf = eventbuf_create();

  int num_producers   = atoi(argv[1]);
  int num_consumers   = atoi(argv[2]);
  int num_events      = atoi(argv[3]);
  int num_outstanding = atoi(argv[4]);
}
