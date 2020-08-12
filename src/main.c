#include<iron/full.h>
#include<iron/gl.h>
#include<microio.h>
// registers
typedef struct{
  int w, h;

}binui;

typedef enum {
  BINUI_SIZE,
  BINUI_VIEW,
  BINUI_COLOR,
  BINUI_RECT,
  BINUI_CIRCLE,
  BINUI_ID,
  BINUI_OFFSET
}binui_opcodes;

static io_reader * current_reader;
void describe(){
  // todo:
  // 3 things can happen in an element
  // 1 - it describes something - pushes onto the stack
  // 2 - it finishes something - pops from the stack
  // 3 - it moves to the next element - stack stays the same place.
  
  static int space = 0;
  space += 1;
  binui_opcodes opcode = (binui_opcodes)io_read_u8(current_reader);
  printf("%*s", space, "");
  switch(opcode){
  case BINUI_SIZE:
    {
      u16 w = io_read_u16(current_reader);
      u16 h = io_read_u16(current_reader);
      printf("Size: %i %i\n", w, h);
      describe();
    }
    break;
  case BINUI_VIEW:
    break;
  case BINUI_RECT:
    {
      printf("Model: Rectangle\n");
    }
    break;
  case BINUI_ID:
    {
      u64 len = io_read_u64_leb(current_reader);
      static char name_buf[100];
      io_read(current_reader, name_buf, len);
      name_buf[len] = 0;
      printf("ID: %s\n", name_buf);
      describe();
    }
  case BINUI_CIRCLE:
    break;
  case BINUI_COLOR:
    {
      u8 color[4];
      printf("Color: ");
      for(int i = 0; i < 4; i++){
	color[i] = io_read_u8(current_reader);
	printf("%i ", color[i]);
      }
      printf("\n");

      
      describe();
      break;
    }
  default:
    break;

  }
  space -= 1;
  
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
  io_write_u64_leb(wd, strlen(grpname));
  io_write(wd, grpname, strlen(grpname));
  
  io_write_u8(wd, BINUI_RECT);

  wd->offset = 0;
  current_reader = wd;
  describe();
  
  gl_window * w = gl_window_open(512, 512);
  gl_window_make_current(w);
  while(true){
    gl_window_poll_events();
    gl_window_swap(w);
  }
  
  return 0;
}
