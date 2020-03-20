#include "event.hpp"
#include "ball.hpp"

std::pair<int, int> CollisionEvent::get_idpair() const
{
  int aid = this->a->get_id();
  int bid = this->b->get_id();

  if (aid > bid) {
    return std::make_pair(aid, bid);
  }
  else {
    return std::make_pair(bid, aid);
  }
}

std::set<unsigned int> EventCollection::invalidate()
{
  int aid = this->previous->get_a()->get_id();
  int bid = this->previous->get_b()->get_id();
  std::set<unsigned int> rv;

  for (auto it = this->events.begin(); it != this->events.end(); it++) {
    auto kv = *it;
    auto idpair = kv.first;
    int bigger, smaller;
    bigger = idpair.first;
    smaller = idpair.second;
    if ((bigger == aid) || (bigger == bid) || (smaller == aid) || (smaller == bid)) {
      this->events.erase(it++);
      rv.insert(bigger);
      rv.insert(smaller);
    }
  }

  return rv;
}
