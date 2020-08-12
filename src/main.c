#include<iron/full.h>
#include<iron/gl.h>
#include<microio.h>
#include<binui.h>
static io_reader * current_reader;
io_writer * binui_stdout;
static void describe(){
  // todo:
  // 3 things can happen in an element
  // 1 - it describes something - pushes onto the stack
  // 2 - it finishes something - pops from the stack
  // 3 - it moves to the next element - stack stays the same place.
  
  static int space = 0;
  space += 1;
  binui_opcodes opcode = (binui_opcodes)io_read_u8(current_reader);
  io_write_f(binui_stdout, "%*s", space, "");
  switch(opcode){
  case BINUI_SIZE:
    {
      u16 w = io_read_u16(current_reader);
      u16 h = io_read_u16(current_reader);
      io_write_f(binui_stdout, "Size: %i %i\n", w, h);
      describe();
    }
    break;
  case BINUI_VIEW:
    break;
  case BINUI_RECT:
    {
      io_write_f(binui_stdout, "Model: Rectangle\n");
    }
    break;
  case BINUI_ID:
    {
      u32 len = 0;
      char * name_buf = io_read_strn(current_reader, &len);
      io_write_f(binui_stdout, "ID: %.*s\n", len, name_buf);
      free(name_buf);
      describe();
    }
  case BINUI_CIRCLE:
    break;
  case BINUI_COLOR:
    {
      u8 color[4];
      io_write_f(binui_stdout, "Color: ");
      for(int i = 0; i < 4; i++){
	color[i] = io_read_u8(current_reader);
	io_write_f(binui_stdout, "%i ", color[i]);
      }
      io_write_f(binui_stdout, "\n");

      
      describe();
      break;
    }
  default:
    break;

  }
  space -= 1;  
}

static io_writer _binui_stdout;


static void write_stdout(void * data, size_t count, void * userdata){
  UNUSED(userdata);
  fwrite(data, count, 1, stdout);
}

void binui_describe(io_reader * reader){
  if(binui_stdout == NULL){
    binui_stdout = &_binui_stdout;
    binui_stdout->f =  write_stdout;
  }
  
  io_reader * prev_reader = current_reader;
  current_reader = reader;
  describe();
  current_reader = prev_reader;
}

void binui_iterate(io_reader * reader, void (* callback)(binui * registers, void * userdata), void * userdata){

}

int main(int argc, char ** argv){
  io_writer _wd = {0};
  io_writer * wd = &_wd;
  io_write_u8(wd, BINUI_SIZE);
  // 500x500
  io_write_u16(wd, 500);
  io_write_u16(wd, 500);
  io_write_u8(wd, BINUI_COLOR);
  // rgba
  io_write_u8(wd, 255);
  io_write_u8(wd, 0);
  io_write_u8(wd, 0);
  io_write_u8(wd, 255);
  io_write_u8(wd, BINUI_ID);
  const char * grpname = "hello world";
  io_write_strn(wd, grpname);
  
  io_write_u8(wd, BINUI_RECT);

  io_reset(wd);
  binui_describe(wd);
  
  gl_window * w = gl_window_open(512, 512);
  gl_window_make_current(w);
  while(true){
    gl_window_poll_events();
    gl_window_swap(w);
  }
  
  return 0;
}
