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

