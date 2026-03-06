/**
* @file
* @author  Huang Wen <Email:ww1383@163.com, QQ:214890094, WeChat:w18926268887>
* @version 1.0
*
* @section LICENSE
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* @section DESCRIPTION
*
* This file is ...
*/
#ifndef _QS_JSON_H_
#define _QS_JSON_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "qs.h"

/* qs_json Types: */
#define qs_json_False 0
#define qs_json_True 1
#define qs_json_NULL 2
#define qs_json_Number 3
#define qs_json_String 4
#define qs_json_Array 5
#define qs_json_Object 6
	
#define qs_json_IsReference 256
#define qs_json_StringIsConst 512

/* The qs_json structure: */
typedef struct _qs_json {
	struct _qs_json *next,*prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
	struct _qs_json *child;		/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

	int type;					/* The type of the item, as above. */

	char *valuestring;			/* The item's string, if type==qs_json_String */
	int valueint;				/* The item's number, if type==qs_json_Number */
	double valuedouble;			/* The item's number, if type==qs_json_Number */

	char *string;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} qs_json;

typedef struct qs_json_Hooks {
      void *(*malloc_fn)(size_t sz);
      void (*free_fn)(void *ptr);
} qs_json_Hooks;

/* Supply malloc, realloc and free functions to qs_json */
extern void qs_json_InitHooks(qs_json_Hooks* hooks);


/* Supply a block of JSON, and this returns a qs_json object you can interrogate. Call qs_json_Delete when finished. */
extern qs_json *qs_json_Parse(const char *value);
/* Render a qs_json entity to text for transfer/storage. Free the char* when finished. */
extern char  *qs_json_Print(qs_json *item);
/* Render a qs_json entity to text for transfer/storage without any formatting. Free the char* when finished. */
extern char  *qs_json_PrintUnformatted(qs_json *item);
/* Render a qs_json entity to text using a buffered strategy. prebuffer is a guess at the final size. guessing well reduces reallocation. fmt=0 gives unformatted, =1 gives formatted */
extern char *qs_json_PrintBuffered(qs_json *item,int prebuffer,int fmt);
/* Delete a qs_json entity and all subentities. */
extern void   qs_json_Delete(qs_json *c);

/* Returns the number of items in an array (or object). */
extern int	  qs_json_GetArraySize(qs_json *array);
/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
extern qs_json *qs_json_GetArrayItem(qs_json *array,int item);
/* Get item "string" from object. Case insensitive. */
extern qs_json *qs_json_GetObjectItem(qs_json *object,const char *string);

/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when qs_json_Parse() returns 0. 0 when qs_json_Parse() succeeds. */
extern const char *qs_json_GetErrorPtr(void);
	
/* These calls create a qs_json item of the appropriate type. */
extern qs_json *qs_json_CreateNull(void);
extern qs_json *qs_json_CreateTrue(void);
extern qs_json *qs_json_CreateFalse(void);
extern qs_json *qs_json_CreateBool(int b);
extern qs_json *qs_json_CreateNumber(double num);
extern qs_json *qs_json_CreateString(const char *string);
extern qs_json *qs_json_CreateArray(void);
extern qs_json *qs_json_CreateObject(void);

/* These utilities create an Array of count items. */
extern qs_json *qs_json_CreateIntArray(const int *numbers,int count);
extern qs_json *qs_json_CreateFloatArray(const float *numbers,int count);
extern qs_json *qs_json_CreateDoubleArray(const double *numbers,int count);
extern qs_json *qs_json_CreateStringArray(const char **strings,int count);

/* Append item to the specified array/object. */
extern void qs_json_AddItemToArray(qs_json *array, qs_json *item);
extern void	qs_json_AddItemToObject(qs_json *object,const char *string,qs_json *item);
extern void	qs_json_AddItemToObjectCS(qs_json *object,const char *string,qs_json *item);	/* Use this when string is definitely const (i.e. a literal, or as good as), and will definitely survive the qs_json object */
/* Append reference to item to the specified array/object. Use this when you want to add an existing qs_json to a new qs_json, but don't want to corrupt your existing qs_json. */
extern void qs_json_AddItemReferenceToArray(qs_json *array, qs_json *item);
extern void	qs_json_AddItemReferenceToObject(qs_json *object,const char *string,qs_json *item);

/* Remove/Detatch items from Arrays/Objects. */
extern qs_json *qs_json_DetachItemFromArray(qs_json *array,int which);
extern void   qs_json_DeleteItemFromArray(qs_json *array,int which);
extern qs_json *qs_json_DetachItemFromObject(qs_json *object,const char *string);
extern void   qs_json_DeleteItemFromObject(qs_json *object,const char *string);
	
/* Update array items. */
extern void qs_json_InsertItemInArray(qs_json *array,int which,qs_json *newitem);	/* Shifts pre-existing items to the right. */
extern void qs_json_ReplaceItemInArray(qs_json *array,int which,qs_json *newitem);
extern void qs_json_ReplaceItemInObject(qs_json *object,const char *string,qs_json *newitem);

/* Duplicate a qs_json item */
extern qs_json *qs_json_Duplicate(qs_json *item,int recurse);
/* Duplicate will create a new, identical qs_json item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */

/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
extern qs_json *qs_json_ParseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated);

extern void qs_json_Minify(char *json);

/* Macros for creating things quickly. */
#define qs_json_AddNullToObject(object,name)		qs_json_AddItemToObject(object, name, qs_json_CreateNull())
#define qs_json_AddTrueToObject(object,name)		qs_json_AddItemToObject(object, name, qs_json_CreateTrue())
#define qs_json_AddFalseToObject(object,name)		qs_json_AddItemToObject(object, name, qs_json_CreateFalse())
#define qs_json_AddBoolToObject(object,name,b)		qs_json_AddItemToObject(object, name, qs_json_CreateBool(b))
#define qs_json_AddNumberToObject(object,name,n)	qs_json_AddItemToObject(object, name, qs_json_CreateNumber(n))
#define qs_json_AddStringToObject(object,name,s)	qs_json_AddItemToObject(object, name, qs_json_CreateString(s))

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define qs_json_SetIntValue(object,val)			((object)?(object)->valueint=(object)->valuedouble=(val):(val))
#define qs_json_SetNumberValue(object,val)		((object)?(object)->valueint=(object)->valuedouble=(val):(val))

#ifdef __cplusplus
}
#endif

#endif
