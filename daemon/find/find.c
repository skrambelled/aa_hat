#pragma strict_types

#include "find_def.h"

#define CHUNK_SIZE 500

mapping files;
void read_data_file(int start);

void create() {
  if(is_clone()) {
    destruct(this_object());
    return;
  }
  seteuid(getuid());
  files = ([]);
  if(file_size(FIND_SAVE_FILE+".o") >= 0)
    restore_object(FIND_SAVE_FILE);
  else
    read_data_file(1);
}

void read_data_file(int start) {
  string text;

  if(text = read_file(FIND_DATA_FILE, start, CHUNK_SIZE)) {
    filter_array(explode(text, "\n"), "process_line", this_object());
    call_out("read_data_file", 2, start+CHUNK_SIZE);
  } else {
    save_object(FIND_SAVE_FILE);
  }
}

int process_line(string text) {
  string *tmp;

  tmp = explode(text, "#");
  if(sizeof(tmp) != 2)
    return 0;
  if(files[tmp[0]]) {
    if(member(files[tmp[0]], tmp[1]) == -1)
      files[tmp[0]] += ({tmp[1]});
  } else
    files[tmp[0]] = ({tmp[1]});
}

string *find(string text) {
  return files[text];
}
