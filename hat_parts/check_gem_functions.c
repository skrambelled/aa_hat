#pragma strict_types

#include "../hat_def.h"

// ------------------------------------------------------- Implementation ---

void hatcheck_gem(object o) {
  int is_base_gem;
  string path;
  
  path = explode(file_name(o), "#")[0];
  is_base_gem = path == "/obj/gem";
  if(!is_base_gem)
    report(o, "This is a custom gem and will need QC approval, due to the Keepink project.", QC_CHANNEL);
  else if((int)o->query_gem_flaw() == 1 && (int)o->query_gem_size() == 1 && (int)o->query_gem_val() == 1)
    report(o, "This gem has low stats, is this intentional? Expected: 3-30.", QC_CHANNEL);

  check_name(o, is_base_gem ? TEXT_EXCEPTION_NAME_IN_ALIAS : 0);
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
