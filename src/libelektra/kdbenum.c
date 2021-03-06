/*This is a dummy file do document the enums which have
 * no name in the header file. They are duplicated here
 * to document them. */

/**
 * Allows keyNew() to determine which information comes next.
 *
 * @ingroup key
 * @see keyNew()
 */
enum keyswitch_t
{
	KEY_NAME=1,		/*!< Flag for the key name */
	KEY_VALUE=1<<1,		/*!< Flag for the key data */
	KEY_OWNER=1<<2,		/*!< Flag for the key user domain */
	KEY_COMMENT=1<<3,	/*!< Flag for the key comment */
	KEY_BINARY=1<<4,	/*!< Flag if the key is binary */
	KEY_UID=1<<5,		/*!< Flag for the key UID @deprecated do not use */
	KEY_GID=1<<6,		/*!< Flag for the key GID @deprecated do not use  */
	KEY_MODE=1<<7,		/*!< Flag for the key permissions @deprecated do not use  */
	KEY_ATIME=1<<8,		/*!< Flag for the key access time @deprecated do not use  */
	KEY_MTIME=1<<9,		/*!< Flag for the key change time @deprecated do not use  */
	KEY_CTIME=1<<10,	/*!< Flag for the key status change time @deprecated do not use  */
	KEY_SIZE=1<<11,		/*!< Flag for maximum size to limit value */
	KEY_DIR=1<<14,		/*!< Flag for the key directories @deprecated do not use */
	KEY_META=1<<15,		/*!< Flag for meta data*/
	KEY_END=0		/*!< Used as a parameter terminator to keyNew() */
};

/**
 * End of a list of keys.
 *
 * Use this macro to define the end of a variable-length list
 * of keys.
 *
 * @def KS_END
 * @see ksNew() and ksVNew()
 */
#define KS_END ((Key*)0)

/**
 * Options to change the default behavior of
 * ksLookup() functions.
 *
 * These options can be ORed. That is the |-Operator in C.
 *
 * @ingroup keyset
 * @see kdbGet(), kdbSet()
 */
enum option_t
{

/**
 * No Option set.
 *
 * @see ksLookup()
 */
	KDB_O_NONE=0,
/**
 * Delete parentKey key in ksLookup().
 *
 * @see ksLookup()
 */
	KDB_O_DEL=1,
/** Pop Parent out of keyset key in ksLookup().
 *
 * @see ksPop().
 */
	KDB_O_POP=1<<1,
/**
 * Feature not available
 *
 * TODO: remove for 1.0
 */
	KDB_O_NODIR=1<<2,
/**
 * Feature not available
 *
 * TODO: remove for 1.0
 */
	KDB_O_DIRONLY=1<<3,
/**
 * Feature not available
 *
 * TODO: remove for 1.0
 */
	KDB_O_NOREMOVE=1<<6,
/**
 * Feature not available
 *
 * TODO: remove for 1.0
 */
	KDB_O_REMOVEONLY=1<<7,
/**
 * Feature not available
 *
 * TODO: remove for 1.0
 */
	KDB_O_INACTIVE=1<<8,
/**
 * Feature not available
 *
 * TODO: remove for 1.0
 */
	KDB_O_SYNC=1<<9,
/**
 * Feature not available
 *
 * TODO: remove for 1.0
 */
	KDB_O_SORT=1<<10,
/**
 * Feature not available
 *
 * TODO: remove for 1.0
 */
	KDB_O_NORECURSIVE=1<<11,
/** Ignore case.
 *
 * do not use without KDB_O_NOALL
 *
 * TODO: remove for 1.0
 *
 * @see ksLookup()
 * */
	KDB_O_NOCASE=1<<12,
/** Search with owner.
 *
 * The owner concept is deprecated, do not use.
 *
 * TODO: remove for 1.0
 *
 * @see ksLookup()
 * */
	KDB_O_WITHOWNER=1<<13,
/** Linear search from start -> cursor to cursor -> end.
 *
 * TODO: remove for 1.0
 *
 * @see ksLookup()
 * */
	KDB_O_NOALL=1<<14
};

