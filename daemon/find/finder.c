#pragma strict_types

#include "find_def.h"

#define TIME_BETWEEN_DIRS 1
#define MIN_INTERESTING 200
#define MAXFILES 10

inherit "obj/treasure";

string *stack = ({});

void create() {
  if(query_mud_port() == 2222) {
    destruct(this_object());
    return;
  }

  ::create();
  if(!is_clone())
    return;

  set_name("finder");
  set_alias("finder tool");
  set_short("The Finder Tool");
  set_long("Usage: 'traverse <path>'");
}

void init() {
  ::init();
  if(this_player() != environment())
    return;

  add_action("do_traverse", "traverse");
}

void process_directory_list(string currdir,string *thisdir, int start) {
  int n;
  string *subdirs;

  n = sizeof(thisdir);

  if(start != 0)
    write("resuming at "+start+"\n");

  if(start + MAXFILES <= n) {
    subdirs = map_array(thisdir[start..start+MAXFILES-1],
      "process_directory_entry", this_object(), currdir) - ({0});
    stack = subdirs+stack;
    call_out("process_directory_list", TIME_BETWEEN_DIRS, currdir, thisdir,
      start+MAXFILES);
  } else {
    if(start <= n-1) {
      subdirs = map_array(thisdir[start..n-1], "process_directory_entry",
        this_object(), currdir) - ({0});
      stack = subdirs+stack;
    }
    call_out("traverse_directories", TIME_BETWEEN_DIRS);
  }
}

void traverse_directories() {
  int n;
  string currdir, *thisdir;

  n = sizeof(stack);
  if(n == 0)
    return; //Done.

  currdir = stack[0];

  if(n == 1)
    stack = ({});
  else
    stack = stack[1..<1];

  write("Processing: "+currdir+"\n");
  process_directory_list(currdir, get_dir(currdir, 1), 0);
}

int process_alias(string s, string fn) {
  if(!s || !stringp(s))
    return 0;

  if(strlen(s) < 2 || s[0] == 10)
    return 0;

  if(s[0..1] == "a ")
    s = s[2..<1];
  else if(s[0..2] == "an ")
    s = s[3..<1];
  if(sizeof(explode(s," ")) > 2)
    return 0;

  write_file(FIND_DATA_FILE, s+"#"+fn+"\n");
  return 0;
}

void process_file(string s) {
  string fname, shortstring, longstring, namestring, *names, *aliases;
  object o, master;

  if(catch(fname = explode(s,"#")[0]))
    return;

  if(catch(master = find_object(fname)))
    return;

  if(master)
    if(master->query_room())
      return;

  if(catch(o = clone_object(s)))
    return; //Buggy or inaccessible file--just ignore it silently.
  if(!o)
    return; //Self-destructed--ignore it.

  if(o->query_room()) {
    catch(o->remove());
    if(o)
      catch(destruct(o));
      return; //It's a room--not interesting.
  }

  if(catch(shortstring = (string) o->short()))
    shortstring = "*";
    if(catch(longstring = (string) o->query_long()))
      longstring = "*";
      if(catch(namestring = (string) o->query_name()))
        namestring = "*";
        if(catch(aliases = (string *) o->query_alias()))
          aliases = 0;

  if(shortstring && longstring && namestring) {
    names = ({lower_case(namestring)});
    if(pointerp(aliases))
      names += aliases;
    filter_array(names, "process_alias", this_object(), s);
  }

  if(!master) { //Master object didn't already exist.
    if(catch(master = find_object(fname)))
      return;
    catch(master->remove());
    if(master)
      catch(destruct(master));
  }

  catch(o->remove());

  if(o)
    catch(destruct(o));
}

string process_directory_entry(string s, string currdir) {
  int size;

  if(s == "private" || s == "approval" || s == "qc" || s == "balance")
    return 0;

  size = file_size(currdir+s);

  if(size == -2)
    return currdir+s+"/";

  write(" -- "+s+"\n");

  if(size >= MIN_INTERESTING && s[<2..<1] == ".c")
    process_file(currdir+s);

  return 0;
}

int do_traverse(string s) {
  int size, i;

  size = file_size(s);

  if(size == -1)
    write("Directory does not exist or is not accessible.\n");
  else if(size >= 0)
    write("This is not a directory!\n");
  else {
    if(s[<1] != 47)
      s = s+"/";
    stack = explode(read_file(FIND_DIRLIST),"\n");

    i = member_array(s,stack);
    if(i == -1)
      write(s+" is not found in DIRLIST\n");
    else {
      stack = stack[i..<1];
      traverse_directories();
    }
  }

  return 1;
}
