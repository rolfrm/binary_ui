
typedef enum {
  BINUI_MODEL_RECT,
  BINUI_MODEL_CIRCLE,
  BINUI_MODEL_TEXT
}binui_model;
// registers
typedef struct{
  int w, h;
  const char * id;
  
}binui;

typedef enum {
  BINUI_SIZE,
  BINUI_VIEW,
  // register
  BINUI_COLOR,
  // model
  BINUI_RECT,
  // model
  BINUI_CIRCLE,
  // register
  BINUI_ID,
  BINUI_OFFSET,
  BINUI_TEXT,
  BINUI_VERIFY,
  BINUI_GROUP
}binui_opcodes;


extern io_writer * binui_stdout;

// debug, writes the binui description to the output;
void binui_describe(io_reader * reader);
void binui_iterate(io_reader * reader, void (* callback)(binui * registers, void * userdata), void * userdata); 
