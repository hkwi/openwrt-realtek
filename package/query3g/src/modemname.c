#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <json/json.h>

#define MAX_MANUF_NAME 128
#define MAX_MODEL_NAME 128

#define FNAME "/usr/share/3gmodem/modem.json"

int main( int argc, char **argv )
{
	int i,j,k;
	int ret=0;
	int cnt_apn=0;
	if ( argc != 3 )
	{
		printf(" %s [Manufacturer Name] [Modem model]\n", argv[0]);
		printf("%s huawei e180\n", argv[0]);
		return -1;
	}
	
	char *str_manuf = argv[1];
	char *str_model = argv[2];
	char manuf[MAX_MANUF_NAME+1]; memset( manuf, 0, MAX_MANUF_NAME+1 );
	char model[MAX_MODEL_NAME+1]; memset( model, 0, MAX_MODEL_NAME+1 );

	//printf( "%d",  ((i=strlen(str_manuf))<MAX_MANUF_NAME) ? i : MAX_MANUF_NAME );
	memcpy( manuf, str_manuf, ((i=strlen(str_manuf))<MAX_MANUF_NAME) ? i : MAX_MANUF_NAME );
	memcpy( model, str_model, ((i=strlen(str_model))<MAX_MODEL_NAME) ? i : MAX_MODEL_NAME );
	
	struct json_object *jo = json_object_from_file( FNAME );
	if ( jo == NULL )
	{
		printf("ERR:Cannot get file\n");
		ret = 1;
		goto free_resources;
	}
	
	struct json_object *jarr = json_object_object_get( jo, "data" );
	if ( jarr == NULL )
	{
		printf("ERR:Cannot get data object\n");
		ret = 1;
		goto free_resources;
	}
	
	if ( !json_object_is_type( jarr, json_type_array ) )
	{
		printf("ERR:Object isnot array\n");
		ret = 1;
		goto free_resources;
	}
	
	for (i=0; i<json_object_array_length(jarr); i++)
	{
		json_object *obj = json_object_array_get_idx(jarr, i);
		struct json_object *jusbid =       json_object_object_get( obj, "usbid" );
		struct json_object *jmanuf =       json_object_object_get( obj, "manufacture" );
		struct json_object *jmodel =       json_object_object_get( obj, "model" );
		struct json_object *jspn =         json_object_object_get( obj, "serialportnum" );
		struct json_object *jsp =          json_object_object_get( obj, "serialport" );
		struct json_object *jcmds =        json_object_object_get( obj, "cmds" );
		struct json_object *jreset =       json_object_object_get( obj, "reset" );
		struct json_object *jarr[]={ jusbid, jmanuf, jmodel, jspn, jsp, jcmds, jreset };
		const char *smo = json_object_to_json_string( jmodel );
		const char *sma = json_object_to_json_string( jmanuf );
		if ( ( strncmp( sma+1, manuf, strlen( sma )-2 ) == 0) &&
		     ((strlen(sma+1)-1) == strlen(manuf) ) &&
		     ( strncmp( smo+1, model, strlen( smo )-2 ) == 0) &&
		     ((strlen(smo+1)-1) == strlen(model) ))
		{
			for (j=0; j<7; j++,(j<7)?printf(","):0)
			{
				if ( jarr[j] != NULL )
				{
					const char *str = json_object_to_json_string( jarr[j] );
					k=1;
					while (str[k+1]!=0x0)
					{
						putc(str[k],stdout);
						k++;
					}
				}
			}
			printf("\n");
			cnt_apn++;
		}
	}
free_resources:
	json_object_put( jo );
	
	if (cnt_apn<=0)
		ret = 1;
	return ret;
}
