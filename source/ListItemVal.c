#include "includes/ListItemVal_private.h"

/* Increments the reference counter for the object and returns
 * a pointer to the referenced object.  If referencing failed,
 * possibly due to the reference counter being corrupted,
 * NULL will be returned. */
ListItemVal* ListItemVal_ref(ListItemVal* liv)
{
	if(!liv || !liv->ref_count)return(NULL);

	++liv->ref_count;
	return(liv);
}

/* Getters */
/* Returns the value of the ListItemVal object. */
void* ListItemVal_get_value(ListItemVal* liv){return(liv->value);}
/***********/

/*******Lifecycle*******/
ListItemVal* newListItemVal(void* val, alib_free_value free_val_cb)
{
	ListItemVal* liv = (ListItemVal*)malloc(sizeof(ListItemVal));
	if(!liv)return(NULL);

	liv->value = val;
	liv->free_value_cb = free_val_cb;
	liv->ref_count = 1;

	return(liv);
}

/* Decrements the ListItemVal's reference counter and sets the pointer
 * to NULL.  If the reference counter reaches zero, the object will be
 * freed.
 *
 * If, for some reason, you must ensure the object is freed instantly without
 * waiting for each reference to call delete, call 'delListItemVal_complete()'. */
void delListItemVal(ListItemVal** liv)
{
	if(!liv || !*liv)return;

	if((*liv)->ref_count && !(--(*liv)->ref_count))
	{
		if((*liv)->free_value_cb && (*liv)->value)
			(*liv)->free_value_cb((*liv)->value);

		free(*liv);
	}

	*liv = NULL;
}
/* This will force the ListItemVal to be freed instantly and not wait
 * for the reference count to reach zero.  THIS SHOULD NOT BE USED unless
 * you are absolutely sure you won't try to use this object again from any
 * of your references.
 *
 * It is best to use 'delListItemVal()', instead, whenever you have the
 * opportunity. */
void delListItemVal_complete(ListItemVal** liv)
{
	if(!liv || !*liv)return;

	(*liv)->ref_count = 0;

	if((*liv)->free_value_cb && (*liv)->value)
		(*liv)->free_value_cb((*liv)->value);

	free(*liv);
	*liv = NULL;
}
/***********************/
