INCLUDES=-Iinclude

LIBNAME=librtk-inband.so
APPNAME=rtk-inband

all: $(LIBNAME) $(APPNAME)

%.o: %.c
	$(CC) -c -o $@ $(INCLUDES) $(CFLAGS) $<

OBJ=inband_if.o ioh.o

LIB_OBJ=hapd_api.o $(OBJ)
APP_OBJ=inband_cmd.o $(OBJ)


$(APPNAME):$(APP_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(LIBNAME): $(LIB_OBJ) 
	$(CC) $(CFLAGS) -shared -o $@ $^
