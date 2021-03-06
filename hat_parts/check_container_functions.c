#pragma strict_types

#include <containers.h>

#include "../hat_def.h"

// ------------------------------------------------------- Implementation ---

void hatcheck_container(object o) {
  int capacity, value, expected;

  if(!o->query_weight())
    report(o, "Containers must be 1 or more weight.", QC_CHANNEL);

  capacity = (int) o->query_max_weight();
  if(!capacity) {
    report(o, "This container has zero capacity.", QC_CHANNEL);
  } else {
    if(!o->query_wearable())
      expected = capacity * CONTAINER_VALUE_NON_WEARABLE;
    else
      expected = capacity * CONTAINER_VALUE_WEARABLE;
    check_recommended_value(o, expected);
  }

  check_name(o, 0);
  check_short(o, TEXT_CHECK_LIMITS, ITEM_SHORT_LIMITS);
  check_long(o, TEXT_EXCEPTION_ENDING_NL | TEXT_CHECK_LIMITS, ITEM_LONG_LIMITS);
  check_identify(o, 0);
  check_set_sense(o, "search", TEXT_NOT_MANDATORY);
  check_set_sense(o, "smell", TEXT_NOT_MANDATORY);
  check_set_sense(o, "sound", TEXT_NOT_MANDATORY);
  check_set_sense(o, "taste", TEXT_NOT_MANDATORY);
  check_set_sense(o, "touch", 0);
  check_add_senses(o, 0);
  check_material(o, 1);
}
