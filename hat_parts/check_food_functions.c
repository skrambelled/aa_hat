#pragma strict_types

#include "../hat_def.h"

// ------------------------------------------------------- Implementation ---

void hatcheck_food(object o) {
  int heal;

  heal = (int) o->query_healing();
  if(heal > 40)
    report(o, "Food should heal 40 points or less.", BALANCE_CHANNEL);
  else
    if(!heal)
      report(o, "Food must heal for 1 or more.", BALANCE_CHANNEL);
  check_recommended_value(o, 4*heal+(heal*heal)/10);
  if(!o->query_weight())
    report(o, "Food should weigh at least 1.", QC_CHANNEL);

  check_name(o, 0);
  check_short(o, TEXT_CHECK_LIMITS, ITEM_SHORT_LIMITS);
  check_long(o, TEXT_CHECK_LIMITS, ITEM_LONG_LIMITS);
  check_identify(o, 0);
  check_set_sense(o, "search", TEXT_NOT_MANDATORY);
  check_set_sense(o, "smell", 0);
  check_set_sense(o, "sound", TEXT_NOT_MANDATORY);
  check_set_sense(o, "taste", TEXT_EXCEPTION_ENDING_NL);
  check_set_sense(o, "touch", 0);
  check_add_senses(o, 0);
  check_material(o, 1);
}
