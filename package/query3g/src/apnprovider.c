#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <json/json.h>

#define FNAME "/usr/share/3gmodem/apn.json"

int main( int argc, char **argv )
{
	int ret=0;
	int i,j,k;
	int cnt_apn=0;
	if ( argc != 2 )
	{
		printf(" %s [APN_Provider_5_symbols]\n", argv[0]);
		printf("%s 12345\n", argv[0]);
		return -1;
	} else if ( strlen(argv[1]) != 5 )
	{
		printf("ERR: Only 5 number apn supported\n");
		return -1;
	}
	
	char *apnnumber = argv[1];
	char mcc[4];
	char mnc[3];
	memcpy( mcc, apnnumber, 3 );
	mcc[3] = 0;
	memcpy( mnc, apnnumber+3 , 2 );
	mnc[2] = 0;
	
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
		struct json_object *jmcc = json_object_object_get( obj, "mcc" );
		struct json_object *jmnc = json_object_object_get( obj, "mnc" );
		struct json_object *jname = json_object_object_get( obj, "name" );
		struct json_object *jfullname = json_object_object_get( obj, "fullname" );
		struct json_object *jstatus = json_object_object_get( obj, "status" );
		struct json_object *jgsmband = json_object_object_get( obj, "gsmband" );
		struct json_object *jarr[] = { jmcc, jmnc, jname, jfullname, jstatus, jgsmband };
		if ( (jmcc != NULL) && (jmnc != NULL) && ( (jname != NULL) || (jfullname != NULL) ) )
		{
			const char *tmp_mcc = json_object_to_json_string( jmcc );
			const char *tmp_mnc = json_object_to_json_string( jmnc );
			if ( strncmp( tmp_mcc+1, mcc, 3 ) != 0 )
			{
				continue;
			}
			if ( strncmp( tmp_mnc+1, mnc, 2 ) != 0 )
			{
				continue;
			}
			for (j=0; j<6; j++,(j<6)?printf(","):0)
			{
				if ( jarr[j] != NULL )
				{
					const char *str = json_object_to_json_string( jarr[j] );
					k=1;
					while (str[k+1]!=0x0)
					{
						if (str[k] != '\\')
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
