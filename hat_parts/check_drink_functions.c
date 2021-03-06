#pragma strict_types

#include "../hat_def.h"

// ------------------------------------------------------- Implementation ---

int is_empty(object o) { return !(int)o->value(); }

void hatcheck_drink(object o) {
  int heal;

  heal = (int) o->query_healing();
  if(heal > 45)
    report(o, "Drinks should heal 45 points or less.", BALANCE_CHANNEL);
  else if(!heal)
    report(o, "Drinks must heal for 1 or more.", BALANCE_CHANNEL);

  // TODO value in drinks are handled in /obj/drink.c and cannot be
  // controlled by the author easily
  //check_recommended_value(o, (3*heal+(heal*heal)/10)/2);

  if(!o->query_weight())
    report(o, "Drinks should weigh at least 1.", QC_CHANNEL);

  check_name(o, is_empty(o) ? TEXT_EXCEPTION_ALLOW_ARTICLE : 0);
  check_short(o, TEXT_CHECK_LIMITS, ITEM_SHORT_LIMITS);
  check_long(o, TEXT_DENY_ENDING_PUNC | TEXT_CHECK_LIMITS, ITEM_LONG_LIMITS);
  check_identify(o, TEXT_EXCEPTION_INLINE_NL);
  check_set_sense(o, "search", TEXT_NOT_MANDATORY);
  check_set_sense(o, "smell", 0);
  check_set_sense(o, "sound", TEXT_NOT_MANDATORY);
  check_set_sense(o, "taste", TEXT_EXCEPTION_INLINE_NL | TEXT_EXCEPTION_ENDING_NL);
  check_set_sense(o, "touch", TEXT_NOT_MANDATORY);
  check_add_senses(o, 0);
  // TODO investigate drink materials, cause they are funky on drinks
  //check_material(o, 0);
}
